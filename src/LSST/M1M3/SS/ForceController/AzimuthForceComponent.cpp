#include <AzimuthForceComponent.h>
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

AzimuthForceComponent::AzimuthForceComponent(
        M1M3SSPublisher* publisher, SafetyController* safetyController,
        ForceActuatorApplicationSettings* forceActuatorApplicationSettings,
        ForceActuatorSettings* forceActuatorSettings) {
    this->name = "Azimuth";

    this->publisher = publisher;
    this->safetyController = safetyController;
    this->forceActuatorApplicationSettings = forceActuatorApplicationSettings;
    this->forceActuatorSettings = forceActuatorSettings;
    this->forceActuatorState = this->publisher->getEventForceActuatorState();
    this->forceSetpointWarning = this->publisher->getEventForceSetpointWarning();
    this->appliedAzimuthForces = this->publisher->getEventAppliedAzimuthForces();
    this->rejectedAzimuthForces = this->publisher->getEventRejectedAzimuthForces();
    this->maxRateOfChange = this->forceActuatorSettings->AzimuthComponentSettings.MaxRateOfChange;
    this->nearZeroValue = this->forceActuatorSettings->AzimuthComponentSettings.NearZeroValue;
}

void AzimuthForceComponent::applyAzimuthForces(float* x, float* y, float* z) {
    spdlog::trace("AzimuthForceComponent: applyAzimuthForces()");
    if (!this->enabled) {
        spdlog::error("AzimuthForceComponent: applyAzimuthForces() called when the component is not applied");
        return;
    }
    if (this->disabling) {
        spdlog::warn("AzimuthForceComponent: applyAzimuthForces() called when the component is disabling");
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

void AzimuthForceComponent::applyAzimuthForcesByAzimuthAngle(float azimuthAngle) {
    spdlog::trace("AzimuthForceComponent: applyAzimuthForcesByMirrorForces({:.1f})", azimuthAngle);
    DistributedForces forces =
            ForceConverter::calculateForceFromAzimuthAngle(this->forceActuatorSettings, azimuthAngle);
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
    this->applyAzimuthForces(xForces, yForces, zForces);
}

void AzimuthForceComponent::postEnableDisableActions() {
    spdlog::debug("AzimuthForceComponent: postEnableDisableActions()");

    this->forceActuatorState->timestamp = this->publisher->getTimestamp();
    this->forceActuatorState->azimuthForcesApplied = this->enabled;
    this->publisher->tryLogForceActuatorState();
}

void AzimuthForceComponent::postUpdateActions() {
    spdlog::trace("AzimuthForceController: postUpdateActions()");

    bool notInRange = false;
    bool rejectionRequired = false;
    this->appliedAzimuthForces->timestamp = this->publisher->getTimestamp();
    this->rejectedAzimuthForces->timestamp = this->appliedAzimuthForces->timestamp;
    for (int zIndex = 0; zIndex < 156; ++zIndex) {
        int xIndex = this->forceActuatorApplicationSettings->ZIndexToXIndex[zIndex];
        int yIndex = this->forceActuatorApplicationSettings->ZIndexToYIndex[zIndex];

        this->forceSetpointWarning->azimuthForceWarning[zIndex] = false;

        if (xIndex != -1) {
            float xLowFault = this->forceActuatorSettings->AzimuthLimitXTable[xIndex].LowFault;
            float xHighFault = this->forceActuatorSettings->AzimuthLimitXTable[xIndex].HighFault;
            this->rejectedAzimuthForces->xForces[xIndex] = this->xCurrent[xIndex];
            notInRange = !Range::InRangeAndCoerce(xLowFault, xHighFault,
                                                  this->rejectedAzimuthForces->xForces[xIndex],
                                                  this->appliedAzimuthForces->xForces + xIndex);
            this->forceSetpointWarning->azimuthForceWarning[zIndex] =
                    this->forceSetpointWarning->azimuthForceWarning[zIndex] || notInRange;
        }

        if (yIndex != -1) {
            float yLowFault = this->forceActuatorSettings->AzimuthLimitYTable[yIndex].LowFault;
            float yHighFault = this->forceActuatorSettings->AzimuthLimitYTable[yIndex].HighFault;
            this->rejectedAzimuthForces->yForces[yIndex] = this->yCurrent[yIndex];
            notInRange = !Range::InRangeAndCoerce(yLowFault, yHighFault,
                                                  this->rejectedAzimuthForces->yForces[yIndex],
                                                  this->appliedAzimuthForces->yForces + yIndex);
            this->forceSetpointWarning->azimuthForceWarning[zIndex] =
                    this->forceSetpointWarning->azimuthForceWarning[zIndex] || notInRange;
        }

        float zLowFault = this->forceActuatorSettings->AzimuthLimitZTable[zIndex].LowFault;
        float zHighFault = this->forceActuatorSettings->AzimuthLimitZTable[zIndex].HighFault;
        this->rejectedAzimuthForces->zForces[zIndex] = this->zCurrent[zIndex];
        notInRange =
                !Range::InRangeAndCoerce(zLowFault, zHighFault, this->rejectedAzimuthForces->zForces[zIndex],
                                         this->appliedAzimuthForces->zForces + zIndex);
        this->forceSetpointWarning->azimuthForceWarning[zIndex] =
                this->forceSetpointWarning->azimuthForceWarning[zIndex] || notInRange;
        rejectionRequired = rejectionRequired || this->forceSetpointWarning->azimuthForceWarning[zIndex];
    }

    ForcesAndMoments fm = ForceConverter::calculateForcesAndMoments(
            this->forceActuatorApplicationSettings, this->forceActuatorSettings,
            this->appliedAzimuthForces->xForces, this->appliedAzimuthForces->yForces,
            this->appliedAzimuthForces->zForces);
    this->appliedAzimuthForces->fx = fm.Fx;
    this->appliedAzimuthForces->fy = fm.Fy;
    this->appliedAzimuthForces->fz = fm.Fz;
    this->appliedAzimuthForces->mx = fm.Mx;
    this->appliedAzimuthForces->my = fm.My;
    this->appliedAzimuthForces->mz = fm.Mz;
    this->appliedAzimuthForces->forceMagnitude = fm.ForceMagnitude;

    fm = ForceConverter::calculateForcesAndMoments(
            this->forceActuatorApplicationSettings, this->forceActuatorSettings,
            this->rejectedAzimuthForces->xForces, this->rejectedAzimuthForces->yForces,
            this->rejectedAzimuthForces->zForces);
    this->rejectedAzimuthForces->fx = fm.Fx;
    this->rejectedAzimuthForces->fy = fm.Fy;
    this->rejectedAzimuthForces->fz = fm.Fz;
    this->rejectedAzimuthForces->mx = fm.Mx;
    this->rejectedAzimuthForces->my = fm.My;
    this->rejectedAzimuthForces->mz = fm.Mz;
    this->rejectedAzimuthForces->forceMagnitude = fm.ForceMagnitude;

    this->safetyController->forceControllerNotifyAzimuthForceClipping(rejectionRequired);

    this->publisher->tryLogForceSetpointWarning();
    if (rejectionRequired) {
        this->publisher->logRejectedAzimuthForces();
    }
    this->publisher->logAppliedAzimuthForces();
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
