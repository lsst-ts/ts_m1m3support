#include <ThermalForceComponent.h>
#include <M1M3SSPublisher.h>
#include <SafetyController.h>
#include <ForceActuatorApplicationSettings.h>
#include <ForceActuatorSettings.h>
#include <Range.h>
#include <ForcesAndMoments.h>
#include <ForceConverter.h>
#include <DistributedForces.h>
#include <spdlog/spdlog.h>

namespace LSST {
namespace M1M3 {
namespace SS {

ThermalForceComponent::ThermalForceComponent(
        M1M3SSPublisher* publisher, SafetyController* safetyController,
        ForceActuatorApplicationSettings* forceActuatorApplicationSettings,
        ForceActuatorSettings* forceActuatorSettings) {
    this->name = "Thermal";

    this->publisher = publisher;
    this->safetyController = safetyController;
    this->forceActuatorApplicationSettings = forceActuatorApplicationSettings;
    this->forceActuatorSettings = forceActuatorSettings;
    this->forceActuatorState = this->publisher->getEventForceActuatorState();
    this->forceSetpointWarning = this->publisher->getEventForceSetpointWarning();
    this->appliedThermalForces = this->publisher->getEventAppliedThermalForces();
    this->rejectedThermalForces = this->publisher->getEventRejectedThermalForces();
    this->maxRateOfChange = this->forceActuatorSettings->ThermalComponentSettings.MaxRateOfChange;
    this->nearZeroValue = this->forceActuatorSettings->ThermalComponentSettings.NearZeroValue;
}

void ThermalForceComponent::applyThermalForces(float* x, float* y, float* z) {
    spdlog::trace("ThermalForceComponent: applyThermalForces()");
    if (!this->enabled) {
        spdlog::error("ThermalForceComponent: applyThermalForces() called when the component is not applied");
        return;
    }
    if (this->disabling) {
        spdlog::warn("ThermalForceComponent: applyThermalForces() called when the component is disabling");
        return;
    }
    for (int i = 0; i < 156; ++i) {
        if (i < 12) {
            this->xTarget[i] = x[i];
        }

        if (i < 100) {
            this->yTarget[i] = y[i];
        }

        this->zTarget[i] = z[i];
    }
}

void ThermalForceComponent::applyThermalForcesByMirrorTemperature(float temperature) {
    spdlog::trace("ThermalForceComponent: applyThermalForcesByMirrorForces({:.1f})", temperature);
    DistributedForces forces =
            ForceConverter::calculateForceFromTemperature(this->forceActuatorSettings, temperature);
    float xForces[12];
    float yForces[100];
    float zForces[156];
    for (int zIndex = 0; zIndex < FA_COUNT; ++zIndex) {
        int xIndex = this->forceActuatorApplicationSettings->ZIndexToXIndex[zIndex];
        int yIndex = this->forceActuatorApplicationSettings->ZIndexToYIndex[zIndex];

        if (xIndex != -1) {
            xForces[xIndex] = forces.XForces[zIndex];
        }
        if (yIndex != -1) {
            yForces[yIndex] = forces.YForces[zIndex];
        }
        zForces[zIndex] = forces.ZForces[zIndex];
    }
    this->applyThermalForces(xForces, yForces, zForces);
}

void ThermalForceComponent::postEnableDisableActions() {
    spdlog::debug("ThermalForceComponent: postEnableDisableActions()");

    this->forceActuatorState->timestamp = this->publisher->getTimestamp();
    this->forceActuatorState->thermalForcesApplied = this->enabled;
    this->publisher->tryLogForceActuatorState();
}

void ThermalForceComponent::postUpdateActions() {
    spdlog::trace("ThermalForceController: postUpdateActions()");

    bool notInRange = false;
    bool rejectionRequired = false;
    this->appliedThermalForces->timestamp = this->publisher->getTimestamp();
    this->rejectedThermalForces->timestamp = this->appliedThermalForces->timestamp;
    for (int zIndex = 0; zIndex < 156; ++zIndex) {
        int xIndex = this->forceActuatorApplicationSettings->ZIndexToXIndex[zIndex];
        int yIndex = this->forceActuatorApplicationSettings->ZIndexToYIndex[zIndex];

        this->forceSetpointWarning->thermalForceWarning[zIndex] = false;

        if (xIndex != -1) {
            float xLowFault = this->forceActuatorSettings->ThermalLimitXTable[xIndex].LowFault;
            float xHighFault = this->forceActuatorSettings->ThermalLimitXTable[xIndex].HighFault;
            this->rejectedThermalForces->xForces[xIndex] = this->xCurrent[xIndex];
            notInRange = !Range::InRangeAndCoerce(xLowFault, xHighFault,
                                                  this->rejectedThermalForces->xForces[xIndex],
                                                  this->appliedThermalForces->xForces + xIndex);
            this->forceSetpointWarning->thermalForceWarning[zIndex] =
                    this->forceSetpointWarning->thermalForceWarning[zIndex] || notInRange;
        }

        if (yIndex != -1) {
            float yLowFault = this->forceActuatorSettings->ThermalLimitYTable[yIndex].LowFault;
            float yHighFault = this->forceActuatorSettings->ThermalLimitYTable[yIndex].HighFault;
            this->rejectedThermalForces->yForces[yIndex] = this->yCurrent[yIndex];
            notInRange = !Range::InRangeAndCoerce(yLowFault, yHighFault,
                                                  this->rejectedThermalForces->yForces[yIndex],
                                                  this->appliedThermalForces->yForces + yIndex);
            this->forceSetpointWarning->thermalForceWarning[zIndex] =
                    this->forceSetpointWarning->thermalForceWarning[zIndex] || notInRange;
        }

        float zLowFault = this->forceActuatorSettings->ThermalLimitZTable[zIndex].LowFault;
        float zHighFault = this->forceActuatorSettings->ThermalLimitZTable[zIndex].HighFault;
        this->rejectedThermalForces->zForces[zIndex] = this->zCurrent[zIndex];
        notInRange =
                !Range::InRangeAndCoerce(zLowFault, zHighFault, this->rejectedThermalForces->zForces[zIndex],
                                         this->appliedThermalForces->zForces + zIndex);
        this->forceSetpointWarning->thermalForceWarning[zIndex] =
                this->forceSetpointWarning->thermalForceWarning[zIndex] || notInRange;
        rejectionRequired = rejectionRequired || this->forceSetpointWarning->thermalForceWarning[zIndex];
    }

    ForcesAndMoments fm = ForceConverter::calculateForcesAndMoments(
            this->forceActuatorApplicationSettings, this->forceActuatorSettings,
            this->appliedThermalForces->xForces, this->appliedThermalForces->yForces,
            this->appliedThermalForces->zForces);
    this->appliedThermalForces->fx = fm.Fx;
    this->appliedThermalForces->fy = fm.Fy;
    this->appliedThermalForces->fz = fm.Fz;
    this->appliedThermalForces->mx = fm.Mx;
    this->appliedThermalForces->my = fm.My;
    this->appliedThermalForces->mz = fm.Mz;
    this->appliedThermalForces->forceMagnitude = fm.ForceMagnitude;

    fm = ForceConverter::calculateForcesAndMoments(
            this->forceActuatorApplicationSettings, this->forceActuatorSettings,
            this->rejectedThermalForces->xForces, this->rejectedThermalForces->yForces,
            this->rejectedThermalForces->zForces);
    this->rejectedThermalForces->fx = fm.Fx;
    this->rejectedThermalForces->fy = fm.Fy;
    this->rejectedThermalForces->fz = fm.Fz;
    this->rejectedThermalForces->mx = fm.Mx;
    this->rejectedThermalForces->my = fm.My;
    this->rejectedThermalForces->mz = fm.Mz;
    this->rejectedThermalForces->forceMagnitude = fm.ForceMagnitude;

    this->safetyController->forceControllerNotifyThermalForceClipping(rejectionRequired);

    this->publisher->tryLogForceSetpointWarning();
    if (rejectionRequired) {
        this->publisher->logRejectedThermalForces();
    }
    this->publisher->logAppliedThermalForces();
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
