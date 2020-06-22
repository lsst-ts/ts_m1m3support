#include <AccelerationForceComponent.h>
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

AccelerationForceComponent::AccelerationForceComponent(
        M1M3SSPublisher* publisher, SafetyController* safetyController,
        ForceActuatorApplicationSettings* forceActuatorApplicationSettings,
        ForceActuatorSettings* forceActuatorSettings) {
    this->name = "Acceleration";

    this->publisher = publisher;
    this->safetyController = safetyController;
    this->forceActuatorApplicationSettings = forceActuatorApplicationSettings;
    this->forceActuatorSettings = forceActuatorSettings;
    this->forceActuatorState = this->publisher->getEventForceActuatorState();
    this->forceSetpointWarning = this->publisher->getEventForceSetpointWarning();
    this->appliedAccelerationForces = this->publisher->getEventAppliedAccelerationForces();
    this->rejectedAccelerationForces = this->publisher->getEventRejectedAccelerationForces();
    this->maxRateOfChange = this->forceActuatorSettings->AccelerationComponentSettings.MaxRateOfChange;
    this->nearZeroValue = this->forceActuatorSettings->AccelerationComponentSettings.NearZeroValue;
}

void AccelerationForceComponent::applyAccelerationForces(float* x, float* y, float* z) {
    spdlog::trace("AccelerationForceComponent: applyAccelerationForces()");
    if (!this->enabled) {
        spdlog::error(
                "AccelerationForceComponent: applyAccelerationForces() called when the component is not "
                "applied");
        return;
    }
    if (this->disabling) {
        spdlog::warn(
                "AccelerationForceComponent: applyAccelerationForces() called when the component is "
                "disabling");
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

void AccelerationForceComponent::applyAccelerationForcesByAngularAccelerations(float angularAccelerationX,
                                                                               float angularAccelerationY,
                                                                               float angularAccelerationZ) {
    spdlog::trace(
            "AccelerationForceComponent: applyAccelerationForcesByAngularAccelerations(P:.1f}, {.1f}, {.1f})",
            angularAccelerationX, angularAccelerationY, angularAccelerationZ);
    DistributedForces forces = ForceConverter::calculateForceFromAngularAcceleration(
            this->forceActuatorSettings, angularAccelerationX, angularAccelerationY, angularAccelerationZ);
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
    spdlog::debug("AccelerationForceComponent: postEnableDisableActions()");

    this->forceActuatorState->timestamp = this->publisher->getTimestamp();
    this->forceActuatorState->accelerationForcesApplied = this->enabled;
    this->publisher->tryLogForceActuatorState();
}

void AccelerationForceComponent::postUpdateActions() {
    spdlog::trace("AccelerationForceController: postUpdateActions()");

    bool notInRange = false;
    bool rejectionRequired = false;
    this->appliedAccelerationForces->timestamp = this->publisher->getTimestamp();
    this->rejectedAccelerationForces->timestamp = this->appliedAccelerationForces->timestamp;
    for (int zIndex = 0; zIndex < 156; ++zIndex) {
        int xIndex = this->forceActuatorApplicationSettings->ZIndexToXIndex[zIndex];
        int yIndex = this->forceActuatorApplicationSettings->ZIndexToYIndex[zIndex];

        this->forceSetpointWarning->accelerationForceWarning[zIndex] = false;

        if (xIndex != -1) {
            float xLowFault = this->forceActuatorSettings->AccelerationLimitXTable[xIndex].LowFault;
            float xHighFault = this->forceActuatorSettings->AccelerationLimitXTable[xIndex].HighFault;
            this->rejectedAccelerationForces->xForces[xIndex] = this->xCurrent[xIndex];
            notInRange = !Range::InRangeAndCoerce(xLowFault, xHighFault,
                                                  this->rejectedAccelerationForces->xForces[xIndex],
                                                  this->appliedAccelerationForces->xForces + xIndex);
            this->forceSetpointWarning->accelerationForceWarning[zIndex] =
                    this->forceSetpointWarning->accelerationForceWarning[zIndex] || notInRange;
        }

        if (yIndex != -1) {
            float yLowFault = this->forceActuatorSettings->AccelerationLimitYTable[yIndex].LowFault;
            float yHighFault = this->forceActuatorSettings->AccelerationLimitYTable[yIndex].HighFault;
            this->rejectedAccelerationForces->yForces[yIndex] = this->yCurrent[yIndex];
            notInRange = !Range::InRangeAndCoerce(yLowFault, yHighFault,
                                                  this->rejectedAccelerationForces->yForces[yIndex],
                                                  this->appliedAccelerationForces->yForces + yIndex);
            this->forceSetpointWarning->accelerationForceWarning[zIndex] =
                    this->forceSetpointWarning->accelerationForceWarning[zIndex] || notInRange;
        }

        float zLowFault = this->forceActuatorSettings->AccelerationLimitZTable[zIndex].LowFault;
        float zHighFault = this->forceActuatorSettings->AccelerationLimitZTable[zIndex].HighFault;
        this->rejectedAccelerationForces->zForces[zIndex] = this->zCurrent[zIndex];
        notInRange = !Range::InRangeAndCoerce(zLowFault, zHighFault,
                                              this->rejectedAccelerationForces->zForces[zIndex],
                                              this->appliedAccelerationForces->zForces + zIndex);
        this->forceSetpointWarning->accelerationForceWarning[zIndex] =
                this->forceSetpointWarning->accelerationForceWarning[zIndex] || notInRange;
        rejectionRequired = rejectionRequired || this->forceSetpointWarning->accelerationForceWarning[zIndex];
    }

    ForcesAndMoments fm = ForceConverter::calculateForcesAndMoments(
            this->forceActuatorApplicationSettings, this->forceActuatorSettings,
            this->appliedAccelerationForces->xForces, this->appliedAccelerationForces->yForces,
            this->appliedAccelerationForces->zForces);
    this->appliedAccelerationForces->fx = fm.Fx;
    this->appliedAccelerationForces->fy = fm.Fy;
    this->appliedAccelerationForces->fz = fm.Fz;
    this->appliedAccelerationForces->mx = fm.Mx;
    this->appliedAccelerationForces->my = fm.My;
    this->appliedAccelerationForces->mz = fm.Mz;
    this->appliedAccelerationForces->forceMagnitude = fm.ForceMagnitude;

    fm = ForceConverter::calculateForcesAndMoments(
            this->forceActuatorApplicationSettings, this->forceActuatorSettings,
            this->rejectedAccelerationForces->xForces, this->rejectedAccelerationForces->yForces,
            this->rejectedAccelerationForces->zForces);
    this->rejectedAccelerationForces->fx = fm.Fx;
    this->rejectedAccelerationForces->fy = fm.Fy;
    this->rejectedAccelerationForces->fz = fm.Fz;
    this->rejectedAccelerationForces->mx = fm.Mx;
    this->rejectedAccelerationForces->my = fm.My;
    this->rejectedAccelerationForces->mz = fm.Mz;
    this->rejectedAccelerationForces->forceMagnitude = fm.ForceMagnitude;

    this->safetyController->forceControllerNotifyAccelerationForceClipping(rejectionRequired);

    this->publisher->tryLogForceSetpointWarning();
    if (rejectionRequired) {
        this->publisher->logRejectedAccelerationForces();
    }
    this->publisher->logAppliedAccelerationForces();
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
