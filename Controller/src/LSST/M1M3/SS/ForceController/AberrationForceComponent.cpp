/*
 * AberrationForceComponent.cpp
 *
 *  Created on: Jul 9, 2018
 *      Author: ccontaxis
 */

#include <AberrationForceComponent.h>
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

AberrationForceComponent::AberrationForceComponent(M1M3SSPublisher* publisher, SafetyController* safetyController, ForceActuatorApplicationSettings* forceActuatorApplicationSettings, ForceActuatorSettings* forceActuatorSettings) {
	this->name = "Aberration";

	this->publisher = publisher;
	this->safetyController = safetyController;
	this->forceActuatorApplicationSettings = forceActuatorApplicationSettings;
	this->forceActuatorSettings = forceActuatorSettings;
	this->forceActuatorState = this->publisher->getEventForceActuatorState();
	this->forceSetpointWarning = this->publisher->getEventForceSetpointWarning();
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
	for(int i = 0; i < 156; ++i) {
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

	this->forceActuatorState->Timestamp = this->publisher->getTimestamp();
	this->forceActuatorState->AberrationForcesApplied = this->enabled;
	this->publisher->tryLogForceActuatorState();
}

void AberrationForceComponent::postUpdateActions() {
	Log.Trace("AberrationForceController: postUpdateActions()");

	bool notInRange = false;
	bool rejectionRequired = false;
	this->appliedAberrationForces->Timestamp = this->publisher->getTimestamp();
	this->rejectedAberrationForces->Timestamp = this->appliedAberrationForces->Timestamp;
	for(int zIndex = 0; zIndex < 156; ++zIndex) {
		float zLowFault = this->forceActuatorSettings->AberrationLimitZTable[zIndex].LowFault;
		float zHighFault = this->forceActuatorSettings->AberrationLimitZTable[zIndex].HighFault;

		this->forceSetpointWarning->AberrationForceWarning[zIndex] = false;

		this->rejectedAberrationForces->ZForces[zIndex] = this->zCurrent[zIndex];
		notInRange = !Range::InRangeAndCoerce(zLowFault, zHighFault, this->rejectedAberrationForces->ZForces[zIndex], this->appliedAberrationForces->ZForces + zIndex);
		this->forceSetpointWarning->AberrationForceWarning[zIndex] = this->forceSetpointWarning->AberrationForceWarning[zIndex] || notInRange;

		rejectionRequired = rejectionRequired || this->forceSetpointWarning->AberrationForceWarning[zIndex];
	}

	ForcesAndMoments fm = ForceConverter::calculateForcesAndMoments(this->forceActuatorApplicationSettings, this->forceActuatorSettings, this->appliedAberrationForces->ZForces);
	this->appliedAberrationForces->Fz = fm.Fz;
	this->appliedAberrationForces->Mx = fm.Mx;
	this->appliedAberrationForces->My = fm.My;

	fm = ForceConverter::calculateForcesAndMoments(this->forceActuatorApplicationSettings, this->forceActuatorSettings, this->rejectedAberrationForces->ZForces);
	this->rejectedAberrationForces->Fz = fm.Fz;
	this->rejectedAberrationForces->Mx = fm.Mx;
	this->rejectedAberrationForces->My = fm.My;

	this->forceSetpointWarning->AberrationNetForceWarning =
			!Range::InRange(-this->forceActuatorSettings->NetAberrationForceTolerance, this->forceActuatorSettings->NetAberrationForceTolerance, this->appliedAberrationForces->Fz) ||
			!Range::InRange(-this->forceActuatorSettings->NetAberrationForceTolerance, this->forceActuatorSettings->NetAberrationForceTolerance, this->appliedAberrationForces->Mx) ||
			!Range::InRange(-this->forceActuatorSettings->NetAberrationForceTolerance, this->forceActuatorSettings->NetAberrationForceTolerance, this->appliedAberrationForces->My);

	this->safetyController->forceControllerNotifyAberrationForceClipping(rejectionRequired);
	this->safetyController->forceControllerNotifyAberrationNetForceCheck(this->forceSetpointWarning->AberrationNetForceWarning);

	this->publisher->tryLogForceSetpointWarning();
	if (rejectionRequired) {
		this->publisher->logRejectedAberrationForces();
	}
	this->publisher->logAppliedAberrationForces();
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
