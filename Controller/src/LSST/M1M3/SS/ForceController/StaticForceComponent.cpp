/*
 * StaticForceComponent.cpp
 *
 *  Created on: Jul 9, 2018
 *      Author: ccontaxis
 */

#include <StaticForceComponent.h>
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

StaticForceComponent::StaticForceComponent(M1M3SSPublisher* publisher, SafetyController* safetyController, ForceActuatorApplicationSettings* forceActuatorApplicationSettings, ForceActuatorSettings* forceActuatorSettings) {
	this->name = "Static";

	this->publisher = publisher;
	this->safetyController = safetyController;
	this->forceActuatorApplicationSettings = forceActuatorApplicationSettings;
	this->forceActuatorSettings = forceActuatorSettings;
	this->forceActuatorState = this->publisher->getEventForceActuatorState();
	this->forceSetpointWarning = this->publisher->getEventForceSetpointWarning();
	this->appliedStaticForces = this->publisher->getEventAppliedStaticForces();
	this->rejectedStaticForces = this->publisher->getEventRejectedStaticForces();
	this->maxRateOfChange = this->forceActuatorSettings->StaticComponentSettings.MaxRateOfChange;
	this->nearZeroValue = this->forceActuatorSettings->StaticComponentSettings.NearZeroValue;
}

void StaticForceComponent::applyStaticForces(std::vector<float>* x, std::vector<float>* y, std::vector<float>* z) {
	Log.Debug("StaticForceComponent: applyStaticForces()");
	if (!this->enabled) {
		Log.Error("StaticForceComponent: applyStaticForces() called when the component is not applied");
		return;
	}
	if (this->disabling) {
		Log.Warn("StaticForceComponent: applyStaticForces() called when the component is disabling");
		return;
	}
	for(int i = 0; i < 156; ++i) {
		if (i < 12) {
			this->xTarget[i] = (*x)[i];
		}

		if (i < 100) {
			this->yTarget[i] = (*y)[i];
		}

		this->zTarget[i] = (*z)[i];
	}
}

void StaticForceComponent::postEnableDisableActions() {
	Log.Debug("StaticForceComponent: postEnableDisableActions()");

	this->forceActuatorState->Timestamp = this->publisher->getTimestamp();
	this->forceActuatorState->StaticForcesApplied = this->enabled;
	this->publisher->tryLogForceActuatorState();
}

void StaticForceComponent::postUpdateActions() {
	Log.Trace("StaticForceController: postUpdateActions()");

	bool notInRange = false;
	bool rejectionRequired = false;
	this->appliedStaticForces->Timestamp = this->publisher->getTimestamp();
	this->rejectedStaticForces->Timestamp = this->appliedStaticForces->Timestamp;
	for(int zIndex = 0; zIndex < 156; ++zIndex) {
		int xIndex = this->forceActuatorApplicationSettings->ZIndexToXIndex[zIndex];
		int yIndex = this->forceActuatorApplicationSettings->ZIndexToYIndex[zIndex];

		this->forceSetpointWarning->StaticForceWarning[zIndex] = false;

		if (xIndex != -1) {
			float xLowFault = this->forceActuatorSettings->StaticLimitXTable[xIndex].LowFault;
			float xHighFault = this->forceActuatorSettings->StaticLimitXTable[xIndex].HighFault;
			this->rejectedStaticForces->XForces[xIndex] = this->xCurrent[xIndex];
			notInRange = !Range::InRangeAndCoerce(xLowFault, xHighFault, this->rejectedStaticForces->XForces[xIndex], this->appliedStaticForces->XForces + xIndex);
			this->forceSetpointWarning->StaticForceWarning[zIndex] = this->forceSetpointWarning->StaticForceWarning[zIndex] || notInRange;
		}

		if (yIndex != -1) {
			float yLowFault = this->forceActuatorSettings->StaticLimitYTable[yIndex].LowFault;
			float yHighFault = this->forceActuatorSettings->StaticLimitYTable[yIndex].HighFault;
			this->rejectedStaticForces->YForces[yIndex] = this->yCurrent[yIndex];
			notInRange = !Range::InRangeAndCoerce(yLowFault, yHighFault, this->rejectedStaticForces->YForces[yIndex], this->appliedStaticForces->YForces + yIndex);
			this->forceSetpointWarning->StaticForceWarning[zIndex] = this->forceSetpointWarning->StaticForceWarning[zIndex] || notInRange;
		}

		float zLowFault = this->forceActuatorSettings->StaticLimitZTable[zIndex].LowFault;
		float zHighFault = this->forceActuatorSettings->StaticLimitZTable[zIndex].HighFault;
		this->rejectedStaticForces->ZForces[zIndex] = this->zCurrent[zIndex];
		notInRange = !Range::InRangeAndCoerce(zLowFault, zHighFault, this->rejectedStaticForces->ZForces[zIndex], this->appliedStaticForces->ZForces + zIndex);
		this->forceSetpointWarning->StaticForceWarning[zIndex] = this->forceSetpointWarning->StaticForceWarning[zIndex] || notInRange;
		rejectionRequired = rejectionRequired || this->forceSetpointWarning->StaticForceWarning[zIndex];
	}

	ForcesAndMoments fm = ForceConverter::calculateForcesAndMoments(this->forceActuatorApplicationSettings, this->forceActuatorSettings, this->appliedStaticForces->XForces, this->appliedStaticForces->YForces, this->appliedStaticForces->ZForces);
	this->appliedStaticForces->Fx = fm.Fx;
	this->appliedStaticForces->Fy = fm.Fy;
	this->appliedStaticForces->Fz = fm.Fz;
	this->appliedStaticForces->Mx = fm.Mx;
	this->appliedStaticForces->My = fm.My;
	this->appliedStaticForces->Mz = fm.Mz;
	this->appliedStaticForces->ForceMagnitude = fm.ForceMagnitude;

	fm = ForceConverter::calculateForcesAndMoments(this->forceActuatorApplicationSettings, this->forceActuatorSettings, this->rejectedStaticForces->XForces, this->rejectedStaticForces->YForces, this->rejectedStaticForces->ZForces);
	this->rejectedStaticForces->Fx = fm.Fx;
	this->rejectedStaticForces->Fy = fm.Fy;
	this->rejectedStaticForces->Fz = fm.Fz;
	this->rejectedStaticForces->Mx = fm.Mx;
	this->rejectedStaticForces->My = fm.My;
	this->rejectedStaticForces->Mz = fm.Mz;
	this->rejectedStaticForces->ForceMagnitude = fm.ForceMagnitude;

	this->safetyController->forceControllerNotifyStaticForceClipping(rejectionRequired);

	this->publisher->tryLogForceSetpointWarning();
	if (rejectionRequired) {
		this->publisher->logRejectedStaticForces();
	}
	this->publisher->logAppliedStaticForces();
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
