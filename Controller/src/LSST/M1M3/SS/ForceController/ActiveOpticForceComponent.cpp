/*
 * ActiveOpticForceComponent.cpp
 *
 *  Created on: Jul 9, 2018
 *      Author: ccontaxis
 */

#include <ActiveOpticForceComponent.h>
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

ActiveOpticForceComponent::ActiveOpticForceComponent(M1M3SSPublisher* publisher, SafetyController* safetyController, ForceActuatorApplicationSettings* forceActuatorApplicationSettings, ForceActuatorSettings* forceActuatorSettings) {
    this->name = "ActiveOptic";

    this->publisher = publisher;
    this->safetyController = safetyController;
    this->forceActuatorApplicationSettings = forceActuatorApplicationSettings;
    this->forceActuatorSettings = forceActuatorSettings;
    this->forceActuatorState = this->publisher->getEventForceActuatorState();
    this->forceActuatorWarning = this->publisher->getEventForceActuatorWarning();
    this->appliedActiveOpticForces = this->publisher->getEventAppliedActiveOpticForces();
    this->rejectedActiveOpticForces = this->publisher->getEventRejectedActiveOpticForces();
    this->maxRateOfChange = this->forceActuatorSettings->ActiveOpticComponentSettings.MaxRateOfChange;
    this->nearZeroValue = this->forceActuatorSettings->ActiveOpticComponentSettings.NearZeroValue;
}

void ActiveOpticForceComponent::applyActiveOpticForces(float* z) {
    Log.Debug("ActiveOpticForceComponent: applyActiveOpticForces()");
    if (!this->enabled) {
        Log.Error("ActiveOpticForceComponent: applyActiveOpticForces() called when the component is not applied");
        return;
    }
    if (this->disabling) {
        Log.Warn("ActiveOpticForceComponent: applyActiveOpticForces() called when the component is disabling");
        this->enable();
    }
    for (int i = 0; i < 156; ++i) {
        this->zTarget[i] = z[i];
    }
}

void ActiveOpticForceComponent::applyActiveOpticForcesByBendingModes(float* coefficients) {
    Log.Debug("ActiveOpticForceComponent: applyActiveOpticForcesByBendingModes()");
    DistributedForces forces = ForceConverter::calculateForceFromBendingModes(this->forceActuatorSettings, coefficients);
    this->applyActiveOpticForces(forces.ZForces);
}

void ActiveOpticForceComponent::postEnableDisableActions() {
    Log.Debug("ActiveOpticForceComponent: postEnableDisableActions()");

    this->forceActuatorState->activeOpticForcesApplied = this->enabled;
    this->publisher->tryLogForceActuatorState();
}

void ActiveOpticForceComponent::postUpdateActions() {
    Log.Trace("ActiveOpticForceController: postUpdateActions()");

    bool rejectionRequired = false;
    for (int zIndex = 0; zIndex < 156; ++zIndex) {
        float zLowFault = this->forceActuatorSettings->ActiveOpticLimitZTable[zIndex].LowFault;
        float zHighFault = this->forceActuatorSettings->ActiveOpticLimitZTable[zIndex].HighFault;
        bool clipping = false;

        this->rejectedActiveOpticForces->zForces[zIndex] = this->zCurrent[zIndex];
        clipping = clipping || !Range::InRangeAndCoerce(zLowFault, zHighFault, this->rejectedActiveOpticForces->zForces[zIndex], this->appliedActiveOpticForces->zForces + zIndex);
        BitHelper::set(this->forceActuatorWarning->forceActuatorFlags + zIndex, ForceActuatorFlags::ForceSetpointActiveOpticForceClipped, clipping);
        rejectionRequired = rejectionRequired || clipping;
    }

    ForcesAndMoments fm = ForceConverter::calculateForcesAndMoments(this->forceActuatorApplicationSettings, this->forceActuatorSettings, this->appliedActiveOpticForces->zForces);
    this->appliedActiveOpticForces->fZ = fm.Fz;
    this->appliedActiveOpticForces->mX = fm.Mx;
    this->appliedActiveOpticForces->mY = fm.My;

    fm = ForceConverter::calculateForcesAndMoments(this->forceActuatorApplicationSettings, this->forceActuatorSettings, this->rejectedActiveOpticForces->zForces);
    this->rejectedActiveOpticForces->fZ = fm.Fz;
    this->rejectedActiveOpticForces->mX = fm.Mx;
    this->rejectedActiveOpticForces->mY = fm.My;

    BitHelper::set(&this->forceActuatorWarning->globalWarningFlags, GlobalActuatorFlags::ForceSetpointNetActiveOpticForceWarning,
                   !Range::InRange(-this->forceActuatorSettings->NetActiveOpticForceTolerance, this->forceActuatorSettings->NetActiveOpticForceTolerance, this->appliedActiveOpticForces->fZ) ||
                           !Range::InRange(-this->forceActuatorSettings->NetActiveOpticForceTolerance, this->forceActuatorSettings->NetActiveOpticForceTolerance, this->appliedActiveOpticForces->mX) ||
                           !Range::InRange(-this->forceActuatorSettings->NetActiveOpticForceTolerance, this->forceActuatorSettings->NetActiveOpticForceTolerance, this->appliedActiveOpticForces->mY));

    this->safetyController->forceControllerNotifyActiveOpticForceClipping(rejectionRequired);
    this->safetyController->forceControllerNotifyActiveOpticNetForceCheck(BitHelper::get(this->forceActuatorWarning->globalWarningFlags, GlobalActuatorFlags::ForceSetpointNetActiveOpticForceWarning));

    this->publisher->tryLogForceActuatorWarning();
    if (rejectionRequired) {
        this->publisher->logRejectedActiveOpticForces();
    }
    this->publisher->logAppliedActiveOpticForces();
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
