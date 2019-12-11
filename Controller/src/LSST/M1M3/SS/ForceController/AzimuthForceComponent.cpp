/*
 * AzimuthForceComponent.cpp
 *
 *  Created on: Jul 9, 2018
 *      Author: ccontaxis
 */

#include <AzimuthForceComponent.h>
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

AzimuthForceComponent::AzimuthForceComponent(M1M3SSPublisher* publisher, SafetyController* safetyController, ForceActuatorApplicationSettings* forceActuatorApplicationSettings, ForceActuatorSettings* forceActuatorSettings) {
    this->name = "Azimuth";

    this->publisher = publisher;
    this->safetyController = safetyController;
    this->forceActuatorApplicationSettings = forceActuatorApplicationSettings;
    this->forceActuatorSettings = forceActuatorSettings;
    this->forceActuatorState = this->publisher->getEventForceActuatorState();
    this->forceActuatorWarning = this->publisher->getEventForceActuatorWarning();
    this->appliedAzimuthForces = this->publisher->getEventAppliedAzimuthForces();
    this->rejectedAzimuthForces = this->publisher->getEventRejectedAzimuthForces();
    this->maxRateOfChange = this->forceActuatorSettings->AzimuthComponentSettings.MaxRateOfChange;
    this->nearZeroValue = this->forceActuatorSettings->AzimuthComponentSettings.NearZeroValue;
}

void AzimuthForceComponent::applyAzimuthForces(float* x, float* y, float* z) {
    Log.Trace("AzimuthForceComponent: applyAzimuthForces()");
    if (!this->enabled) {
        Log.Error("AzimuthForceComponent: applyAzimuthForces() called when the component is not applied");
        return;
    }
    if (this->disabling) {
        Log.Warn("AzimuthForceComponent: applyAzimuthForces() called when the component is disabling");
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
    Log.Trace("AzimuthForceComponent: applyAzimuthForcesByMirrorForces(%0.1f)", azimuthAngle);
    DistributedForces forces = ForceConverter::calculateForceFromAzimuthAngle(this->forceActuatorSettings, azimuthAngle);
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
    Log.Debug("AzimuthForceComponent: postEnableDisableActions()");

    this->forceActuatorState->azimuthForcesApplied = this->enabled;
    this->publisher->tryLogForceActuatorState();
}

void AzimuthForceComponent::postUpdateActions() {
    Log.Trace("AzimuthForceController: postUpdateActions()");

    bool rejectionRequired = false;
    for (int zIndex = 0; zIndex < 156; ++zIndex) {
        int xIndex = this->forceActuatorApplicationSettings->ZIndexToXIndex[zIndex];
        int yIndex = this->forceActuatorApplicationSettings->ZIndexToYIndex[zIndex];
        bool clipping = false;

        if (xIndex != -1) {
            float xLowFault = this->forceActuatorSettings->AzimuthLimitXTable[xIndex].LowFault;
            float xHighFault = this->forceActuatorSettings->AzimuthLimitXTable[xIndex].HighFault;
            this->rejectedAzimuthForces->xForces[xIndex] = this->xCurrent[xIndex];
            clipping = clipping || !Range::InRangeAndCoerce(xLowFault, xHighFault, this->rejectedAzimuthForces->xForces[xIndex], this->appliedAzimuthForces->xForces + xIndex);
        }

        if (yIndex != -1) {
            float yLowFault = this->forceActuatorSettings->AzimuthLimitYTable[yIndex].LowFault;
            float yHighFault = this->forceActuatorSettings->AzimuthLimitYTable[yIndex].HighFault;
            this->rejectedAzimuthForces->yForces[yIndex] = this->yCurrent[yIndex];
            clipping = clipping || !Range::InRangeAndCoerce(yLowFault, yHighFault, this->rejectedAzimuthForces->yForces[yIndex], this->appliedAzimuthForces->yForces + yIndex);
        }

        float zLowFault = this->forceActuatorSettings->AzimuthLimitZTable[zIndex].LowFault;
        float zHighFault = this->forceActuatorSettings->AzimuthLimitZTable[zIndex].HighFault;
        this->rejectedAzimuthForces->zForces[zIndex] = this->zCurrent[zIndex];
        clipping = clipping || !Range::InRangeAndCoerce(zLowFault, zHighFault, this->rejectedAzimuthForces->zForces[zIndex], this->appliedAzimuthForces->zForces + zIndex);
        BitHelper::set(this->forceActuatorWarning->forceActuatorFlags + zIndex, ForceActuatorFlags::ForceSetpointAzimuthForceClipped, clipping);
        rejectionRequired = rejectionRequired || clipping;
    }

    ForcesAndMoments fm = ForceConverter::calculateForcesAndMoments(this->forceActuatorApplicationSettings, this->forceActuatorSettings, this->appliedAzimuthForces->xForces, this->appliedAzimuthForces->yForces, this->appliedAzimuthForces->zForces);
    this->appliedAzimuthForces->fX = fm.Fx;
    this->appliedAzimuthForces->fY = fm.Fy;
    this->appliedAzimuthForces->fZ = fm.Fz;
    this->appliedAzimuthForces->mX = fm.Mx;
    this->appliedAzimuthForces->mY = fm.My;
    this->appliedAzimuthForces->mZ = fm.Mz;
    this->appliedAzimuthForces->forceMagnitude = fm.ForceMagnitude;

    fm = ForceConverter::calculateForcesAndMoments(this->forceActuatorApplicationSettings, this->forceActuatorSettings, this->rejectedAzimuthForces->xForces, this->rejectedAzimuthForces->yForces, this->rejectedAzimuthForces->zForces);
    this->rejectedAzimuthForces->fX = fm.Fx;
    this->rejectedAzimuthForces->fY = fm.Fy;
    this->rejectedAzimuthForces->fZ = fm.Fz;
    this->rejectedAzimuthForces->mX = fm.Mx;
    this->rejectedAzimuthForces->mY = fm.My;
    this->rejectedAzimuthForces->mZ = fm.Mz;
    this->rejectedAzimuthForces->forceMagnitude = fm.ForceMagnitude;

    this->safetyController->forceControllerNotifyAzimuthForceClipping(rejectionRequired);

    this->publisher->tryLogForceActuatorWarning();
    if (rejectionRequired) {
        this->publisher->logRejectedAzimuthForces();
    }
    this->publisher->logAppliedAzimuthForces();
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
