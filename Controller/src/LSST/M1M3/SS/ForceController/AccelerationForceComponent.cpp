/*
 * AccelerationForceComponent.cpp
 *
 *  Created on: Jul 9, 2018
 *      Author: ccontaxis
 */

#include <AccelerationForceComponent.h>
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

AccelerationForceComponent::AccelerationForceComponent(M1M3SSPublisher* publisher, SafetyController* safetyController, ForceActuatorApplicationSettings* forceActuatorApplicationSettings, ForceActuatorSettings* forceActuatorSettings) {
    this->name = "Acceleration";

    this->publisher = publisher;
    this->safetyController = safetyController;
    this->forceActuatorApplicationSettings = forceActuatorApplicationSettings;
    this->forceActuatorSettings = forceActuatorSettings;
    this->forceActuatorState = this->publisher->getEventForceActuatorState();
    this->forceActuatorWarning = this->publisher->getEventForceActuatorWarning();
    this->appliedAccelerationForces = this->publisher->getEventAppliedAccelerationForces();
    this->rejectedAccelerationForces = this->publisher->getEventRejectedAccelerationForces();
    this->maxRateOfChange = this->forceActuatorSettings->AccelerationComponentSettings.MaxRateOfChange;
    this->nearZeroValue = this->forceActuatorSettings->AccelerationComponentSettings.NearZeroValue;
}

void AccelerationForceComponent::applyAccelerationForces(float* x, float* y, float* z) {
    Log.Trace("AccelerationForceComponent: applyAccelerationForces()");
    if (!this->enabled) {
        Log.Error("AccelerationForceComponent: applyAccelerationForces() called when the component is not applied");
        return;
    }
    if (this->disabling) {
        Log.Warn("AccelerationForceComponent: applyAccelerationForces() called when the component is disabling");
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

void AccelerationForceComponent::applyAccelerationForcesByAngularAccelerations(float angularAccelerationX, float angularAccelerationY, float angularAccelerationZ) {
    Log.Trace("AccelerationForceComponent: applyAccelerationForcesByAngularAccelerations(%0.1f, %0.1f, %0.1f)", angularAccelerationX, angularAccelerationY, angularAccelerationZ);
    DistributedForces forces = ForceConverter::calculateForceFromAngularAcceleration(this->forceActuatorSettings, angularAccelerationX, angularAccelerationY, angularAccelerationZ);
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
    this->applyAccelerationForces(xForces, yForces, zForces);
}

void AccelerationForceComponent::postEnableDisableActions() {
    Log.Debug("AccelerationForceComponent: postEnableDisableActions()");

    this->forceActuatorState->accelerationForcesApplied = this->enabled;
    this->publisher->tryLogForceActuatorState();
}

void AccelerationForceComponent::postUpdateActions() {
    Log.Trace("AccelerationForceController: postUpdateActions()");

    bool rejectionRequired = false;
    for (int zIndex = 0; zIndex < 156; ++zIndex) {
        int xIndex = this->forceActuatorApplicationSettings->ZIndexToXIndex[zIndex];
        int yIndex = this->forceActuatorApplicationSettings->ZIndexToYIndex[zIndex];
        bool clipping = false;

        if (xIndex != -1) {
            float xLowFault = this->forceActuatorSettings->AccelerationLimitXTable[xIndex].LowFault;
            float xHighFault = this->forceActuatorSettings->AccelerationLimitXTable[xIndex].HighFault;
            this->rejectedAccelerationForces->xForces[xIndex] = this->xCurrent[xIndex];
            clipping = clipping || !Range::InRangeAndCoerce(xLowFault, xHighFault, this->rejectedAccelerationForces->xForces[xIndex], this->appliedAccelerationForces->xForces + xIndex);
        }

        if (yIndex != -1) {
            float yLowFault = this->forceActuatorSettings->AccelerationLimitYTable[yIndex].LowFault;
            float yHighFault = this->forceActuatorSettings->AccelerationLimitYTable[yIndex].HighFault;
            this->rejectedAccelerationForces->yForces[yIndex] = this->yCurrent[yIndex];
            clipping = clipping || !Range::InRangeAndCoerce(yLowFault, yHighFault, this->rejectedAccelerationForces->yForces[yIndex], this->appliedAccelerationForces->yForces + yIndex);
        }

        float zLowFault = this->forceActuatorSettings->AccelerationLimitZTable[zIndex].LowFault;
        float zHighFault = this->forceActuatorSettings->AccelerationLimitZTable[zIndex].HighFault;
        this->rejectedAccelerationForces->zForces[zIndex] = this->zCurrent[zIndex];
        clipping = clipping || !Range::InRangeAndCoerce(zLowFault, zHighFault, this->rejectedAccelerationForces->zForces[zIndex], this->appliedAccelerationForces->zForces + zIndex);
        BitHelper::set(this->forceActuatorWarning->forceActuatorFlags + zIndex, ForceActuatorFlags::ForceSetpointAccelerationForceClipped, clipping);

        rejectionRequired = rejectionRequired || clipping;
    }

    ForcesAndMoments fm = ForceConverter::calculateForcesAndMoments(this->forceActuatorApplicationSettings, this->forceActuatorSettings, this->appliedAccelerationForces->xForces, this->appliedAccelerationForces->yForces, this->appliedAccelerationForces->zForces);
    this->appliedAccelerationForces->fX = fm.Fx;
    this->appliedAccelerationForces->fY = fm.Fy;
    this->appliedAccelerationForces->fZ = fm.Fz;
    this->appliedAccelerationForces->mX = fm.Mx;
    this->appliedAccelerationForces->mY = fm.My;
    this->appliedAccelerationForces->mZ = fm.Mz;
    this->appliedAccelerationForces->forceMagnitude = fm.ForceMagnitude;

    fm = ForceConverter::calculateForcesAndMoments(this->forceActuatorApplicationSettings, this->forceActuatorSettings, this->rejectedAccelerationForces->xForces, this->rejectedAccelerationForces->yForces, this->rejectedAccelerationForces->zForces);
    this->rejectedAccelerationForces->fX = fm.Fx;
    this->rejectedAccelerationForces->fY = fm.Fy;
    this->rejectedAccelerationForces->fZ = fm.Fz;
    this->rejectedAccelerationForces->mX = fm.Mx;
    this->rejectedAccelerationForces->mY = fm.My;
    this->rejectedAccelerationForces->mZ = fm.Mz;
    this->rejectedAccelerationForces->forceMagnitude = fm.ForceMagnitude;

    this->safetyController->forceControllerNotifyAccelerationForceClipping(rejectionRequired);

    this->publisher->tryLogForceActuatorWarning();
    if (rejectionRequired) {
        this->publisher->logRejectedAccelerationForces();
    }
    this->publisher->logAppliedAccelerationForces();
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
