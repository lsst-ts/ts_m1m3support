/*
 * ThermalForceComponent.cpp
 *
 *  Created on: Jul 9, 2018
 *      Author: ccontaxis
 */

#include <ThermalForceComponent.h>
#include <SAL_MTM1M3C.h>
#include <M1M3SSPublisher.h>
#include <SafetyController.h>
#include <ForceActuatorApplicationSettings.h>
#include <ForceActuatorSettings.h>
#include <Range.h>
#include <ForcesAndMoments.h>
#include <ForceConverter.h>
#include <DistributedForces.h>
#include <SALEnumerations.h>
#include <BitHelper.h>
#include <Log.h>

namespace LSST {
namespace M1M3 {
namespace SS {

ThermalForceComponent::ThermalForceComponent(M1M3SSPublisher* publisher, SafetyController* safetyController, ForceActuatorApplicationSettings* forceActuatorApplicationSettings, ForceActuatorSettings* forceActuatorSettings) {
    this->name = "Thermal";

    this->publisher = publisher;
    this->safetyController = safetyController;
    this->forceActuatorApplicationSettings = forceActuatorApplicationSettings;
    this->forceActuatorSettings = forceActuatorSettings;
    this->forceActuatorState = this->publisher->getEventForceActuatorState();
    this->forceActuatorWarning = this->publisher->getEventForceActuatorWarning();
    this->appliedThermalForces = this->publisher->getEventAppliedThermalForces();
    this->rejectedThermalForces = this->publisher->getEventRejectedThermalForces();
    this->maxRateOfChange = this->forceActuatorSettings->ThermalComponentSettings.MaxRateOfChange;
    this->nearZeroValue = this->forceActuatorSettings->ThermalComponentSettings.NearZeroValue;
}

void ThermalForceComponent::applyThermalForces(float* x, float* y, float* z) {
    Log.Trace("ThermalForceComponent: applyThermalForces()");
    if (!this->enabled) {
        Log.Error("ThermalForceComponent: applyThermalForces() called when the component is not applied");
        return;
    }
    if (this->disabling) {
        Log.Warn("ThermalForceComponent: applyThermalForces() called when the component is disabling");
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
    Log.Trace("ThermalForceComponent: applyThermalForcesByMirrorForces(%0.1f)", temperature);
    DistributedForces forces = ForceConverter::calculateForceFromTemperature(this->forceActuatorSettings, temperature);
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
    Log.Debug("ThermalForceComponent: postEnableDisableActions()");

    this->forceActuatorState->thermalForcesApplied = this->enabled;
    this->publisher->tryLogForceActuatorState();
}

void ThermalForceComponent::postUpdateActions() {
    Log.Trace("ThermalForceController: postUpdateActions()");

    bool rejectionRequired = false;
    for (int zIndex = 0; zIndex < 156; ++zIndex) {
        int xIndex = this->forceActuatorApplicationSettings->ZIndexToXIndex[zIndex];
        int yIndex = this->forceActuatorApplicationSettings->ZIndexToYIndex[zIndex];
        bool clipping = false;

        if (xIndex != -1) {
            float xLowFault = this->forceActuatorSettings->ThermalLimitXTable[xIndex].LowFault;
            float xHighFault = this->forceActuatorSettings->ThermalLimitXTable[xIndex].HighFault;
            this->rejectedThermalForces->xForces[xIndex] = this->xCurrent[xIndex];
            clipping = clipping || !Range::InRangeAndCoerce(xLowFault, xHighFault, this->rejectedThermalForces->xForces[xIndex], this->appliedThermalForces->xForces + xIndex);
        }

        if (yIndex != -1) {
            float yLowFault = this->forceActuatorSettings->ThermalLimitYTable[yIndex].LowFault;
            float yHighFault = this->forceActuatorSettings->ThermalLimitYTable[yIndex].HighFault;
            this->rejectedThermalForces->yForces[yIndex] = this->yCurrent[yIndex];
            clipping = clipping || !Range::InRangeAndCoerce(yLowFault, yHighFault, this->rejectedThermalForces->yForces[yIndex], this->appliedThermalForces->yForces + yIndex);
        }

        float zLowFault = this->forceActuatorSettings->ThermalLimitZTable[zIndex].LowFault;
        float zHighFault = this->forceActuatorSettings->ThermalLimitZTable[zIndex].HighFault;
        this->rejectedThermalForces->zForces[zIndex] = this->zCurrent[zIndex];
        clipping = clipping || !Range::InRangeAndCoerce(zLowFault, zHighFault, this->rejectedThermalForces->zForces[zIndex], this->appliedThermalForces->zForces + zIndex);
        BitHelper::set(this->forceActuatorWarning->forceActuatorFlags + zIndex, ForceActuatorFlags::ForceSetpointThermalForceClipped, clipping);
        rejectionRequired = rejectionRequired || clipping;
    }

    ForcesAndMoments fm = ForceConverter::calculateForcesAndMoments(this->forceActuatorApplicationSettings, this->forceActuatorSettings, this->appliedThermalForces->xForces, this->appliedThermalForces->yForces, this->appliedThermalForces->zForces);
    this->appliedThermalForces->fX = fm.Fx;
    this->appliedThermalForces->fY = fm.Fy;
    this->appliedThermalForces->fZ = fm.Fz;
    this->appliedThermalForces->mX = fm.Mx;
    this->appliedThermalForces->mY = fm.My;
    this->appliedThermalForces->mZ = fm.Mz;
    this->appliedThermalForces->forceMagnitude = fm.ForceMagnitude;

    fm = ForceConverter::calculateForcesAndMoments(this->forceActuatorApplicationSettings, this->forceActuatorSettings, this->rejectedThermalForces->xForces, this->rejectedThermalForces->yForces, this->rejectedThermalForces->zForces);
    this->rejectedThermalForces->fX = fm.Fx;
    this->rejectedThermalForces->fY = fm.Fy;
    this->rejectedThermalForces->fZ = fm.Fz;
    this->rejectedThermalForces->mX = fm.Mx;
    this->rejectedThermalForces->mY = fm.My;
    this->rejectedThermalForces->mZ = fm.Mz;
    this->rejectedThermalForces->forceMagnitude = fm.ForceMagnitude;

    this->safetyController->forceControllerNotifyThermalForceClipping(rejectionRequired);

    this->publisher->tryLogForceActuatorWarning();
    if (rejectionRequired) {
        this->publisher->logRejectedThermalForces();
    }
    this->publisher->logAppliedThermalForces();
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
