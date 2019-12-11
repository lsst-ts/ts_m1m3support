/*
 * AberrationForceComponent.cpp
 *
 *  Created on: Jul 9, 2018
 *      Author: ccontaxis
 */

#include <AberrationForceComponent.h>
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

AberrationForceComponent::AberrationForceComponent(M1M3SSPublisher* publisher, SafetyController* safetyController, ForceActuatorApplicationSettings* forceActuatorApplicationSettings, ForceActuatorSettings* forceActuatorSettings) {
    this->name = "Aberration";

    this->publisher = publisher;
    this->safetyController = safetyController;
    this->forceActuatorApplicationSettings = forceActuatorApplicationSettings;
    this->forceActuatorSettings = forceActuatorSettings;
    this->forceActuatorState = this->publisher->getEventForceActuatorState();
    this->forceActuatorWarning = this->publisher->getEventForceActuatorWarning();
    this->appliedAberrationForces = this->publisher->getEventAppliedAberrationForces();
    this->rejectedAberrationForces = this->publisher->getEventRejectedAberrationForces();
    this->maxRateOfChange = this->forceActuatorSettings->AberrationComponentSettings.MaxRateOfChange;
    this->nearZeroValue = this->forceActuatorSettings->AberrationComponentSettings.NearZeroValue;
}

void AberrationForceComponent::applyAberrationForces(float* z) {
    Log.Debug("AberrationForceComponent: applyAberrationForces()");
    if (!this->enabled) {
        Log.Error("AberrationForceComponent: applyAberrationForces() called when the component is not applied");
        return;
    }
    if (this->disabling) {
        Log.Warn("AberrationForceComponent: applyAberrationForces() called when the component is disabling");
        this->enable();
    }
    for (int i = 0; i < 156; ++i) {
        this->zTarget[i] = z[i];
    }
}

void AberrationForceComponent::applyAberrationForcesByBendingModes(float* coefficients) {
    Log.Debug("AberrationForceComponent: applyAberrationForcesByBendingModes()");
    DistributedForces forces = ForceConverter::calculateForceFromBendingModes(this->forceActuatorSettings, coefficients);
    this->applyAberrationForces(forces.ZForces);
}

void AberrationForceComponent::postEnableDisableActions() {
    Log.Debug("AberrationForceComponent: postEnableDisableActions()");

    this->forceActuatorState->aberrationForcesApplied = this->enabled;
    this->publisher->tryLogForceActuatorState();
}

void AberrationForceComponent::postUpdateActions() {
    Log.Trace("AberrationForceController: postUpdateActions()");

    bool rejectionRequired = false;
    for (int zIndex = 0; zIndex < 156; ++zIndex) {
        float zLowFault = this->forceActuatorSettings->AberrationLimitZTable[zIndex].LowFault;
        float zHighFault = this->forceActuatorSettings->AberrationLimitZTable[zIndex].HighFault;
        bool clipping = false;

        this->forceActuatorWarning->forceActuatorFlags[zIndex] = false;

        this->rejectedAberrationForces->zForces[zIndex] = this->zCurrent[zIndex];
        clipping = clipping || !Range::InRangeAndCoerce(zLowFault, zHighFault, this->rejectedAberrationForces->zForces[zIndex], this->appliedAberrationForces->zForces + zIndex);
        BitHelper::set(this->forceActuatorWarning->forceActuatorFlags + zIndex, ForceActuatorFlags::ForceSetpointAberrationForceClipped, clipping);

        rejectionRequired = rejectionRequired || clipping;
    }

    ForcesAndMoments fm = ForceConverter::calculateForcesAndMoments(this->forceActuatorApplicationSettings, this->forceActuatorSettings, this->appliedAberrationForces->zForces);
    this->appliedAberrationForces->fZ = fm.Fz;
    this->appliedAberrationForces->mX = fm.Mx;
    this->appliedAberrationForces->mY = fm.My;

    fm = ForceConverter::calculateForcesAndMoments(this->forceActuatorApplicationSettings, this->forceActuatorSettings, this->rejectedAberrationForces->zForces);
    this->rejectedAberrationForces->fZ = fm.Fz;
    this->rejectedAberrationForces->mX = fm.Mx;
    this->rejectedAberrationForces->mY = fm.My;

    BitHelper::set(&this->forceActuatorWarning->globalWarningFlags, GlobalActuatorFlags::ForceSetpointNetAberrationForceWarning,
                   !Range::InRange(-this->forceActuatorSettings->NetAberrationForceTolerance, this->forceActuatorSettings->NetAberrationForceTolerance, this->appliedAberrationForces->fZ) ||
                     !Range::InRange(-this->forceActuatorSettings->NetAberrationForceTolerance, this->forceActuatorSettings->NetAberrationForceTolerance, this->appliedAberrationForces->mX) ||
                     !Range::InRange(-this->forceActuatorSettings->NetAberrationForceTolerance, this->forceActuatorSettings->NetAberrationForceTolerance, this->appliedAberrationForces->mY));

    this->safetyController->forceControllerNotifyAberrationForceClipping(rejectionRequired);
    this->safetyController->forceControllerNotifyAberrationNetForceCheck(BitHelper::get(this->forceActuatorWarning->globalWarningFlags, GlobalActuatorFlags::ForceSetpointNetAberrationForceWarning));

    this->publisher->tryLogForceActuatorWarning();
    if (rejectionRequired) {
        this->publisher->logRejectedAberrationForces();
    }
    this->publisher->logAppliedAberrationForces();
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
