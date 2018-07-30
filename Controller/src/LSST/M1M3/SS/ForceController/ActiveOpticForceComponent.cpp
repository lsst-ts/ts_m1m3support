/*
 * ActiveOpticForceComponent.cpp
 *
 *  Created on: Jul 9, 2018
 *      Author: ccontaxis
 */

#include <ActiveOpticForceComponent.h>
#include <M1M3SSPublisher.h>
#include <SafetyController.h>
#include <ForceActuatorApplicationSettings.h>
#include <ForceActuatorSettings.h>
#include <Range.h>
#include <ForcesAndMoments.h>
#include <ForceConverter.h>
#include <DistributedForces.h>
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
	this->forceSetpointWarning = this->publisher->getEventForceSetpointWarning();
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
	for(int i = 0; i < 156; ++i) {
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

	this->forceActuatorState->Timestamp = this->publisher->getTimestamp();
	this->forceActuatorState->ActiveOpticForcesApplied = this->enabled;
	this->publisher->tryLogForceActuatorState();
}

void ActiveOpticForceComponent::postUpdateActions() {
	Log.Trace("ActiveOpticForceController: postUpdateActions()");

	bool notInRange = false;
	bool rejectionRequired = false;
	this->appliedActiveOpticForces->Timestamp = this->publisher->getTimestamp();
	this->rejectedActiveOpticForces->Timestamp = this->appliedActiveOpticForces->Timestamp;
	for(int zIndex = 0; zIndex < 156; ++zIndex) {
		float zLowFault = this->forceActuatorSettings->ActiveOpticLimitZTable[zIndex].LowFault;
		float zHighFault = this->forceActuatorSettings->ActiveOpticLimitZTable[zIndex].HighFault;

		this->forceSetpointWarning->ActiveOpticForceWarning[zIndex] = false;

		this->rejectedActiveOpticForces->ZForces[zIndex] = this->zCurrent[zIndex];
		notInRange = !Range::InRangeAndCoerce(zLowFault, zHighFault, this->rejectedActiveOpticForces->ZForces[zIndex], this->appliedActiveOpticForces->ZForces + zIndex);
		this->forceSetpointWarning->ActiveOpticForceWarning[zIndex] = this->forceSetpointWarning->ActiveOpticForceWarning[zIndex] || notInRange;
		rejectionRequired = rejectionRequired || this->forceSetpointWarning->ActiveOpticForceWarning[zIndex];
	}

	ForcesAndMoments fm = ForceConverter::calculateForcesAndMoments(this->forceActuatorApplicationSettings, this->forceActuatorSettings, this->appliedActiveOpticForces->ZForces);
	this->appliedActiveOpticForces->Fz = fm.Fz;
	this->appliedActiveOpticForces->Mx = fm.Mx;
	this->appliedActiveOpticForces->My = fm.My;

	fm = ForceConverter::calculateForcesAndMoments(this->forceActuatorApplicationSettings, this->forceActuatorSettings, this->rejectedActiveOpticForces->ZForces);
	this->rejectedActiveOpticForces->Fz = fm.Fz;
	this->rejectedActiveOpticForces->Mx = fm.Mx;
	this->rejectedActiveOpticForces->My = fm.My;

	this->forceSetpointWarning->ActiveOpticNetForceWarning =
			!Range::InRange(-this->forceActuatorSettings->NetActiveOpticForceTolerance, this->forceActuatorSettings->NetActiveOpticForceTolerance, this->appliedActiveOpticForces->Fz) ||
			!Range::InRange(-this->forceActuatorSettings->NetActiveOpticForceTolerance, this->forceActuatorSettings->NetActiveOpticForceTolerance, this->appliedActiveOpticForces->Mx) ||
			!Range::InRange(-this->forceActuatorSettings->NetActiveOpticForceTolerance, this->forceActuatorSettings->NetActiveOpticForceTolerance, this->appliedActiveOpticForces->My);

	this->safetyController->forceControllerNotifyActiveOpticForceClipping(rejectionRequired);
	this->safetyController->forceControllerNotifyActiveOpticNetForceCheck(this->forceSetpointWarning->ActiveOpticNetForceWarning);

	this->publisher->tryLogForceSetpointWarning();
	if (rejectionRequired) {
		this->publisher->logRejectedActiveOpticForces();
	}
	this->publisher->logAppliedActiveOpticForces();
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
