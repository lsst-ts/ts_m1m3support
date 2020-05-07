#include <ActiveOpticForceComponent.h>
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

ActiveOpticForceComponent::ActiveOpticForceComponent(
        M1M3SSPublisher* publisher, SafetyController* safetyController,
        ForceActuatorApplicationSettings* forceActuatorApplicationSettings,
        ForceActuatorSettings* forceActuatorSettings) {
    this->name = "ActiveOptic";

    this->publisher = publisher;
    this->safetyController = safetyController;
    this->forceActuatorApplicationSettings = forceActuatorApplicationSettings;
    this->forceActuatorSettings = forceActuatorSettings;
    this->forceActuatorState = this->publisher->getEventForceActuatorState();
    this->forceSetpointWarning = this->publisher->getEventForceSetpointWarning();
    this->appliedActiveOpticForces = this->publisher->getEventAppliedActiveOpticForces();
    this->rejectedActiveOpticForces = this->publisher->getEventRejectedActiveOpticForces();
    this->maxRateOfChange = this->forceActuatorSettings->ActiveOpticComponentSettings.MaxRateOfChange;
    this->nearZeroValue = this->forceActuatorSettings->ActiveOpticComponentSettings.NearZeroValue;
}

void ActiveOpticForceComponent::applyActiveOpticForces(float* z) {
    spdlog::debug("ActiveOpticForceComponent: applyActiveOpticForces()");
    if (!this->enabled) {
        spdlog::error(
                "ActiveOpticForceComponent: applyActiveOpticForces() called when the component is not "
                "applied");
        return;
    }
    if (this->disabling) {
        spdlog::warn(
                "ActiveOpticForceComponent: applyActiveOpticForces() called when the component is disabling");
        this->enable();
    }
    for (int i = 0; i < 156; ++i) {
        this->zTarget[i] = z[i];
    }
}

void ActiveOpticForceComponent::applyActiveOpticForcesByBendingModes(float* coefficients) {
    spdlog::debug("ActiveOpticForceComponent: applyActiveOpticForcesByBendingModes()");
    DistributedForces forces =
            ForceConverter::calculateForceFromBendingModes(this->forceActuatorSettings, coefficients);
    this->applyActiveOpticForces(forces.ZForces);
}

void ActiveOpticForceComponent::postEnableDisableActions() {
    spdlog::debug("ActiveOpticForceComponent: postEnableDisableActions()");

    this->forceActuatorState->timestamp = this->publisher->getTimestamp();
    this->forceActuatorState->activeOpticForcesApplied = this->enabled;
    this->publisher->tryLogForceActuatorState();
}

void ActiveOpticForceComponent::postUpdateActions() {
    spdlog::trace("ActiveOpticForceController: postUpdateActions()");

    bool notInRange = false;
    bool rejectionRequired = false;
    this->appliedActiveOpticForces->timestamp = this->publisher->getTimestamp();
    this->rejectedActiveOpticForces->timestamp = this->appliedActiveOpticForces->timestamp;
    for (int zIndex = 0; zIndex < 156; ++zIndex) {
        float zLowFault = this->forceActuatorSettings->ActiveOpticLimitZTable[zIndex].LowFault;
        float zHighFault = this->forceActuatorSettings->ActiveOpticLimitZTable[zIndex].HighFault;

        this->forceSetpointWarning->activeOpticForceWarning[zIndex] = false;

        this->rejectedActiveOpticForces->zForces[zIndex] = this->zCurrent[zIndex];
        notInRange = !Range::InRangeAndCoerce(zLowFault, zHighFault,
                                              this->rejectedActiveOpticForces->zForces[zIndex],
                                              this->appliedActiveOpticForces->zForces + zIndex);
        this->forceSetpointWarning->activeOpticForceWarning[zIndex] =
                this->forceSetpointWarning->activeOpticForceWarning[zIndex] || notInRange;
        rejectionRequired = rejectionRequired || this->forceSetpointWarning->activeOpticForceWarning[zIndex];
    }

    ForcesAndMoments fm = ForceConverter::calculateForcesAndMoments(this->forceActuatorApplicationSettings,
                                                                    this->forceActuatorSettings,
                                                                    this->appliedActiveOpticForces->zForces);
    this->appliedActiveOpticForces->fz = fm.Fz;
    this->appliedActiveOpticForces->mx = fm.Mx;
    this->appliedActiveOpticForces->my = fm.My;

    fm = ForceConverter::calculateForcesAndMoments(this->forceActuatorApplicationSettings,
                                                   this->forceActuatorSettings,
                                                   this->rejectedActiveOpticForces->zForces);
    this->rejectedActiveOpticForces->fz = fm.Fz;
    this->rejectedActiveOpticForces->mx = fm.Mx;
    this->rejectedActiveOpticForces->my = fm.My;

    this->forceSetpointWarning->activeOpticNetForceWarning =
            !Range::InRange(-this->forceActuatorSettings->NetActiveOpticForceTolerance,
                            this->forceActuatorSettings->NetActiveOpticForceTolerance,
                            this->appliedActiveOpticForces->fz) ||
            !Range::InRange(-this->forceActuatorSettings->NetActiveOpticForceTolerance,
                            this->forceActuatorSettings->NetActiveOpticForceTolerance,
                            this->appliedActiveOpticForces->mx) ||
            !Range::InRange(-this->forceActuatorSettings->NetActiveOpticForceTolerance,
                            this->forceActuatorSettings->NetActiveOpticForceTolerance,
                            this->appliedActiveOpticForces->my);

    this->safetyController->forceControllerNotifyActiveOpticForceClipping(rejectionRequired);
    this->safetyController->forceControllerNotifyActiveOpticNetForceCheck(
            this->forceSetpointWarning->activeOpticNetForceWarning);

    this->publisher->tryLogForceSetpointWarning();
    if (rejectionRequired) {
        this->publisher->logRejectedActiveOpticForces();
    }
    this->publisher->logAppliedActiveOpticForces();
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
