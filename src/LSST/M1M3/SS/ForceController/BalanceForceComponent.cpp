#include <BalanceForceComponent.h>
#include <M1M3SSPublisher.h>
#include <SafetyController.h>
#include <ForceActuatorApplicationSettings.h>
#include <ForceActuatorSettings.h>
#include <PIDSettings.h>
#include <Range.h>
#include <ForcesAndMoments.h>
#include <ForceConverter.h>
#include <DistributedForces.h>
#include <spdlog/spdlog.h>

namespace LSST {
namespace M1M3 {
namespace SS {

BalanceForceComponent::BalanceForceComponent(
        M1M3SSPublisher* publisher, SafetyController* safetyController,
        ForceActuatorApplicationSettings* forceActuatorApplicationSettings,
        ForceActuatorSettings* forceActuatorSettings, PIDSettings* pidSettings)
        : fx(0, pidSettings->Fx, publisher),
          fy(1, pidSettings->Fy, publisher),
          fz(2, pidSettings->Fz, publisher),
          mx(3, pidSettings->Mx, publisher),
          my(4, pidSettings->My, publisher),
          mz(5, pidSettings->Mz, publisher) {
    this->name = "Balance";

    this->publisher = publisher;
    this->safetyController = safetyController;
    this->forceActuatorApplicationSettings = forceActuatorApplicationSettings;
    this->forceActuatorSettings = forceActuatorSettings;
    this->pidSettings = pidSettings;
    this->forceActuatorState = this->publisher->getEventForceActuatorState();
    this->forceSetpointWarning = this->publisher->getEventForceSetpointWarning();
    this->appliedBalanceForces = this->publisher->getEventAppliedBalanceForces();
    this->rejectedBalanceForces = this->publisher->getEventRejectedBalanceForces();
    this->maxRateOfChange = this->forceActuatorSettings->BalanceComponentSettings.MaxRateOfChange;
    this->nearZeroValue = this->forceActuatorSettings->BalanceComponentSettings.NearZeroValue;
}

void BalanceForceComponent::applyBalanceForces(float* x, float* y, float* z) {
    spdlog::trace("BalanceForceComponent: applyBalanceForces()");
    if (!this->enabled) {
        spdlog::error("BalanceForceComponent: applyBalanceForces() called when the component is not applied");
        return;
    }
    if (this->disabling) {
        spdlog::warn("BalanceForceComponent: applyBalanceForces() called when the component is disabling");
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

void BalanceForceComponent::applyBalanceForcesByMirrorForces(float xForce, float yForce, float zForce,
                                                             float xMoment, float yMoment, float zMoment) {
    spdlog::trace(
            "BalanceForceComponent: applyBalanceForcesByMirrorForces({:.1f}, {:.1f}, {:.1f}, {:.1f}, {:.1f}, "
            "{:.1f})",
            xForce, yForce, zForce, xMoment, yMoment, zMoment);
    float fx = this->fx.process(0, xForce);
    float fy = this->fy.process(0, yForce);
    float fz = this->fz.process(0, zForce);
    float mx = this->mx.process(0, xMoment);
    float my = this->my.process(0, yMoment);
    float mz = this->mz.process(0, zMoment);
    // Note: Publishing from any PID will publish ALL PID data
    this->fx.publishTelemetry();
    DistributedForces forces =
            ForceConverter::calculateForceDistribution(this->forceActuatorSettings, fx, fy, fz, mx, my, mz);
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
    this->applyBalanceForces(xForces, yForces, zForces);
}

void BalanceForceComponent::updatePID(int id, PIDParameters parameters) {
    spdlog::debug("BalanceForceComponent: updatePID()");
    PID* pid = this->idToPID(id);
    if (pid != 0) {
        pid->updateParameters(parameters);
    }
}

void BalanceForceComponent::resetPID(int id) {
    spdlog::debug("BalanceForceComponent: resetPID()");
    PID* pid = this->idToPID(id);
    if (pid != 0) {
        pid->restoreInitialParameters();
    }
}

void BalanceForceComponent::resetPIDs() {
    spdlog::debug("BalanceForceComponent: resetPIDs()");
    this->fx.restoreInitialParameters();
    this->fy.restoreInitialParameters();
    this->fz.restoreInitialParameters();
    this->mx.restoreInitialParameters();
    this->my.restoreInitialParameters();
    this->mz.restoreInitialParameters();
}

void BalanceForceComponent::postEnableDisableActions() {
    spdlog::debug("BalanceForceComponent: postEnableDisableActions()");

    if (this->enabled) {
        this->resetPIDs();
    }

    this->forceActuatorState->timestamp = this->publisher->getTimestamp();
    this->forceActuatorState->balanceForcesApplied = this->enabled;
    this->publisher->tryLogForceActuatorState();
}

void BalanceForceComponent::postUpdateActions() {
    spdlog::trace("BalanceForceController: postUpdateActions()");

    bool notInRange = false;
    bool rejectionRequired = false;
    this->appliedBalanceForces->timestamp = this->publisher->getTimestamp();
    this->rejectedBalanceForces->timestamp = this->appliedBalanceForces->timestamp;
    for (int zIndex = 0; zIndex < 156; ++zIndex) {
        int xIndex = this->forceActuatorApplicationSettings->ZIndexToXIndex[zIndex];
        int yIndex = this->forceActuatorApplicationSettings->ZIndexToYIndex[zIndex];

        this->forceSetpointWarning->balanceForceWarning[zIndex] = false;

        if (xIndex != -1) {
            float xLowFault = this->forceActuatorSettings->BalanceLimitXTable[xIndex].LowFault;
            float xHighFault = this->forceActuatorSettings->BalanceLimitXTable[xIndex].HighFault;
            this->rejectedBalanceForces->xForces[xIndex] = this->xCurrent[xIndex];
            notInRange = !Range::InRangeAndCoerce(xLowFault, xHighFault,
                                                  this->rejectedBalanceForces->xForces[xIndex],
                                                  this->appliedBalanceForces->xForces + xIndex);
            this->forceSetpointWarning->balanceForceWarning[zIndex] =
                    this->forceSetpointWarning->balanceForceWarning[zIndex] || notInRange;
        }

        if (yIndex != -1) {
            float yLowFault = this->forceActuatorSettings->BalanceLimitYTable[yIndex].LowFault;
            float yHighFault = this->forceActuatorSettings->BalanceLimitYTable[yIndex].HighFault;
            this->rejectedBalanceForces->yForces[yIndex] = this->yCurrent[yIndex];
            notInRange = !Range::InRangeAndCoerce(yLowFault, yHighFault,
                                                  this->rejectedBalanceForces->yForces[yIndex],
                                                  this->appliedBalanceForces->yForces + yIndex);
            this->forceSetpointWarning->balanceForceWarning[zIndex] =
                    this->forceSetpointWarning->balanceForceWarning[zIndex] || notInRange;
        }

        float zLowFault = this->forceActuatorSettings->BalanceLimitZTable[zIndex].LowFault;
        float zHighFault = this->forceActuatorSettings->BalanceLimitZTable[zIndex].HighFault;
        this->rejectedBalanceForces->zForces[zIndex] = this->zCurrent[zIndex];
        notInRange =
                !Range::InRangeAndCoerce(zLowFault, zHighFault, this->rejectedBalanceForces->zForces[zIndex],
                                         this->appliedBalanceForces->zForces + zIndex);
        this->forceSetpointWarning->balanceForceWarning[zIndex] =
                this->forceSetpointWarning->balanceForceWarning[zIndex] || notInRange;
        rejectionRequired = rejectionRequired || this->forceSetpointWarning->balanceForceWarning[zIndex];
    }

    ForcesAndMoments fm = ForceConverter::calculateForcesAndMoments(
            this->forceActuatorApplicationSettings, this->forceActuatorSettings,
            this->appliedBalanceForces->xForces, this->appliedBalanceForces->yForces,
            this->appliedBalanceForces->zForces);
    this->appliedBalanceForces->fx = fm.Fx;
    this->appliedBalanceForces->fy = fm.Fy;
    this->appliedBalanceForces->fz = fm.Fz;
    this->appliedBalanceForces->mx = fm.Mx;
    this->appliedBalanceForces->my = fm.My;
    this->appliedBalanceForces->mz = fm.Mz;
    this->appliedBalanceForces->forceMagnitude = fm.ForceMagnitude;

    fm = ForceConverter::calculateForcesAndMoments(
            this->forceActuatorApplicationSettings, this->forceActuatorSettings,
            this->rejectedBalanceForces->xForces, this->rejectedBalanceForces->yForces,
            this->rejectedBalanceForces->zForces);
    this->rejectedBalanceForces->fx = fm.Fx;
    this->rejectedBalanceForces->fy = fm.Fy;
    this->rejectedBalanceForces->fz = fm.Fz;
    this->rejectedBalanceForces->mx = fm.Mx;
    this->rejectedBalanceForces->my = fm.My;
    this->rejectedBalanceForces->mz = fm.Mz;
    this->rejectedBalanceForces->forceMagnitude = fm.ForceMagnitude;

    this->safetyController->forceControllerNotifyBalanceForceClipping(rejectionRequired);

    this->publisher->tryLogForceSetpointWarning();
    if (rejectionRequired) {
        this->publisher->logRejectedBalanceForces();
    }
    this->publisher->logAppliedBalanceForces();
}

PID* BalanceForceComponent::idToPID(int id) {
    switch (id) {
        case 1:
            return &this->fx;
        case 2:
            return &this->fy;
        case 3:
            return &this->fz;
        case 4:
            return &this->mx;
        case 5:
            return &this->my;
        case 6:
            return &this->mz;
        default:
            return 0;
    }
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
