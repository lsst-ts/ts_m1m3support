/*
 * FinalForceComponent.cpp
 *
 *  Created on: Jul 10, 2018
 *      Author: ccontaxis
 */

#include <FinalForceComponent.h>
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

FinalForceComponent::FinalForceComponent(M1M3SSPublisher* publisher, SafetyController* safetyController, ForceActuatorApplicationSettings* forceActuatorApplicationSettings, ForceActuatorSettings* forceActuatorSettings) {
    this->name = "Final";

    this->publisher = publisher;
    this->safetyController = safetyController;
    this->forceActuatorApplicationSettings = forceActuatorApplicationSettings;
    this->forceActuatorSettings = forceActuatorSettings;
    this->forceActuatorState = this->publisher->getEventForceActuatorState();
    this->forceActuatorWarning = this->publisher->getEventForceActuatorWarning();
    this->appliedForces = this->publisher->getEventAppliedForces();
    this->rejectedForces = this->publisher->getEventRejectedForces();
    this->maxRateOfChange = this->forceActuatorSettings->FinalComponentSettings.MaxRateOfChange;
    this->nearZeroValue = this->forceActuatorSettings->FinalComponentSettings.NearZeroValue;

    this->appliedAberrationForces = this->publisher->getEventAppliedAberrationForces();
    this->appliedAccelerationForces = this->publisher->getEventAppliedAccelerationForces();
    this->appliedActiveOpticForces = this->publisher->getEventAppliedActiveOpticForces();
    this->appliedAzimuthForces = this->publisher->getEventAppliedAzimuthForces();
    this->appliedBalanceForces = this->publisher->getEventAppliedBalanceForces();
    this->appliedElevationForces = this->publisher->getEventAppliedElevationForces();
    this->appliedOffsetForces = this->publisher->getEventAppliedOffsetForces();
    this->appliedStaticForces = this->publisher->getEventAppliedStaticForces();
    this->appliedThermalForces = this->publisher->getEventAppliedThermalForces();
    this->appliedVelocityForces = this->publisher->getEventAppliedVelocityForces();

    this->enable();
}

void FinalForceComponent::applyForces(float* x, float* y, float* z) {
    Log.Trace("FinalForceComponent: applyForces()");
    if (!this->isEnabled()) {
        this->enable();
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

void FinalForceComponent::applyForcesByComponents() {
    Log.Trace("FinalForceComponent: applyForcesByComponents()");
    if (!this->isEnabled()) {
        this->enable();
    }
    for (int i = 0; i < 156; ++i) {
        if (i < 12) {
            this->xTarget[i] =
                    (this->appliedAccelerationForces->xForces[i] +
                     this->appliedAzimuthForces->xForces[i] +
                     this->appliedBalanceForces->xForces[i] +
                     this->appliedElevationForces->xForces[i] +
                     this->appliedOffsetForces->xForces[i] +
                     this->appliedStaticForces->xForces[i] +
                     this->appliedThermalForces->xForces[i] +
                     this->appliedVelocityForces->xForces[i]);
        }

        if (i < 100) {
            this->yTarget[i] =
                    (this->appliedAccelerationForces->yForces[i] +
                     this->appliedAzimuthForces->yForces[i] +
                     this->appliedBalanceForces->yForces[i] +
                     this->appliedElevationForces->yForces[i] +
                     this->appliedOffsetForces->yForces[i] +
                     this->appliedStaticForces->yForces[i] +
                     this->appliedThermalForces->yForces[i] +
                     this->appliedVelocityForces->yForces[i]);
        }

        this->zTarget[i] =
                (this->appliedAberrationForces->zForces[i] +
                 this->appliedAccelerationForces->zForces[i] +
                 this->appliedActiveOpticForces->zForces[i] +
                 this->appliedAzimuthForces->zForces[i] +
                 this->appliedBalanceForces->zForces[i] +
                 this->appliedElevationForces->zForces[i] +
                 this->appliedOffsetForces->zForces[i] +
                 this->appliedStaticForces->zForces[i] +
                 this->appliedThermalForces->zForces[i] +
                 this->appliedVelocityForces->zForces[i]);
    }
}

void FinalForceComponent::postEnableDisableActions() {
    Log.Trace("FinalForceComponent: postEnableDisableActions()");
}

void FinalForceComponent::postUpdateActions() {
    Log.Trace("FinalForceController: postUpdateActions()");

    bool rejectionRequired = false;
    for (int zIndex = 0; zIndex < 156; ++zIndex) {
        int xIndex = this->forceActuatorApplicationSettings->ZIndexToXIndex[zIndex];
        int yIndex = this->forceActuatorApplicationSettings->ZIndexToYIndex[zIndex];
        bool clipping = false;

        if (xIndex != -1) {
            float xLowFault = this->forceActuatorSettings->ForceLimitXTable[xIndex].LowFault;
            float xHighFault = this->forceActuatorSettings->ForceLimitXTable[xIndex].HighFault;
            this->rejectedForces->xForces[xIndex] = this->xCurrent[xIndex];
            clipping = clipping || !Range::InRangeAndCoerce(xLowFault, xHighFault, this->rejectedForces->xForces[xIndex], this->appliedForces->xForces + xIndex);
        }

        if (yIndex != -1) {
            float yLowFault = this->forceActuatorSettings->ForceLimitYTable[yIndex].LowFault;
            float yHighFault = this->forceActuatorSettings->ForceLimitYTable[yIndex].HighFault;
            this->rejectedForces->yForces[yIndex] = this->yCurrent[yIndex];
            clipping = clipping || !Range::InRangeAndCoerce(yLowFault, yHighFault, this->rejectedForces->yForces[yIndex], this->appliedForces->yForces + yIndex);
        }

        float zLowFault = this->forceActuatorSettings->ForceLimitZTable[zIndex].LowFault;
        float zHighFault = this->forceActuatorSettings->ForceLimitZTable[zIndex].HighFault;
        this->rejectedForces->zForces[zIndex] = this->zCurrent[zIndex];
        clipping = clipping || !Range::InRangeAndCoerce(zLowFault, zHighFault, this->rejectedForces->zForces[zIndex], this->appliedForces->zForces + zIndex);
        BitHelper::set(this->forceActuatorWarning->forceActuatorFlags + zIndex, ForceActuatorFlags::ForceSetpointFinalForceClipped, clipping);
        rejectionRequired = rejectionRequired || clipping;
    }

    ForcesAndMoments fm = ForceConverter::calculateForcesAndMoments(this->forceActuatorApplicationSettings, this->forceActuatorSettings, this->appliedForces->xForces, this->appliedForces->yForces, this->appliedForces->zForces);
    this->appliedForces->fX = fm.Fx;
    this->appliedForces->fY = fm.Fy;
    this->appliedForces->fZ = fm.Fz;
    this->appliedForces->mX = fm.Mx;
    this->appliedForces->mY = fm.My;
    this->appliedForces->mZ = fm.Mz;
    this->appliedForces->forceMagnitude = fm.ForceMagnitude;

    fm = ForceConverter::calculateForcesAndMoments(this->forceActuatorApplicationSettings, this->forceActuatorSettings, this->rejectedForces->xForces, this->rejectedForces->yForces, this->rejectedForces->zForces);
    this->rejectedForces->fX = fm.Fx;
    this->rejectedForces->fY = fm.Fy;
    this->rejectedForces->fZ = fm.Fz;
    this->rejectedForces->mX = fm.Mx;
    this->rejectedForces->mY = fm.My;
    this->rejectedForces->mZ = fm.Mz;
    this->rejectedForces->forceMagnitude = fm.ForceMagnitude;

    this->safetyController->forceControllerNotifyForceClipping(rejectionRequired);

    this->publisher->tryLogForceActuatorWarning();
    if (rejectionRequired) {
        this->publisher->logRejectedForces();
    }
    this->publisher->logAppliedForces();
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
