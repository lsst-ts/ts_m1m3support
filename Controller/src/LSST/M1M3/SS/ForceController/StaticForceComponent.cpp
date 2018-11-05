/*
 * StaticForceComponent.cpp
 *
 *  Created on: Jul 9, 2018
 *      Author: ccontaxis
 */

#include <StaticForceComponent.h>
#include <SAL_MTM1M3C.h>
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

	this->forceActuatorState->timestamp = this->publisher->getTimestamp();
	this->forceActuatorState->staticForcesApplied = this->enabled;
	this->publisher->tryLogForceActuatorState();
}

void StaticForceComponent::postUpdateActions() {
	Log.Trace("StaticForceController: postUpdateActions()");

	bool notInRange = false;
	bool rejectionRequired = false;
	this->appliedStaticForces->timestamp = this->publisher->getTimestamp();
	this->rejectedStaticForces->timestamp = this->appliedStaticForces->timestamp;
	for(int zIndex = 0; zIndex < 156; ++zIndex) {
		int xIndex = this->forceActuatorApplicationSettings->ZIndexToXIndex[zIndex];
		int yIndex = this->forceActuatorApplicationSettings->ZIndexToYIndex[zIndex];

		this->forceSetpointWarning->staticForceWarning[zIndex] = false;

		if (xIndex != -1) {
			float xLowFault = this->forceActuatorSettings->StaticLimitXTable[xIndex].LowFault;
			float xHighFault = this->forceActuatorSettings->StaticLimitXTable[xIndex].HighFault;
			this->rejectedStaticForces->xForces[xIndex] = this->xCurrent[xIndex];
			notInRange = !Range::InRangeAndCoerce(xLowFault, xHighFault, this->rejectedStaticForces->xForces[xIndex], this->appliedStaticForces->xForces + xIndex);
			this->forceSetpointWarning->staticForceWarning[zIndex] = this->forceSetpointWarning->staticForceWarning[zIndex] || notInRange;
		}

		if (yIndex != -1) {
			float yLowFault = this->forceActuatorSettings->StaticLimitYTable[yIndex].LowFault;
			float yHighFault = this->forceActuatorSettings->StaticLimitYTable[yIndex].HighFault;
			this->rejectedStaticForces->yForces[yIndex] = this->yCurrent[yIndex];
			notInRange = !Range::InRangeAndCoerce(yLowFault, yHighFault, this->rejectedStaticForces->yForces[yIndex], this->appliedStaticForces->yForces + yIndex);
			this->forceSetpointWarning->staticForceWarning[zIndex] = this->forceSetpointWarning->staticForceWarning[zIndex] || notInRange;
		}

		float zLowFault = this->forceActuatorSettings->StaticLimitZTable[zIndex].LowFault;
		float zHighFault = this->forceActuatorSettings->StaticLimitZTable[zIndex].HighFault;
		this->rejectedStaticForces->zForces[zIndex] = this->zCurrent[zIndex];
		notInRange = !Range::InRangeAndCoerce(zLowFault, zHighFault, this->rejectedStaticForces->zForces[zIndex], this->appliedStaticForces->zForces + zIndex);
		this->forceSetpointWarning->staticForceWarning[zIndex] = this->forceSetpointWarning->staticForceWarning[zIndex] || notInRange;
		rejectionRequired = rejectionRequired || this->forceSetpointWarning->staticForceWarning[zIndex];
	}

	ForcesAndMoments fm = ForceConverter::calculateForcesAndMoments(this->forceActuatorApplicationSettings, this->forceActuatorSettings, this->appliedStaticForces->xForces, this->appliedStaticForces->yForces, this->appliedStaticForces->zForces);
	this->appliedStaticForces->fX = fm.Fx;
	this->appliedStaticForces->fY = fm.Fy;
	this->appliedStaticForces->fZ = fm.Fz;
	this->appliedStaticForces->mX = fm.Mx;
	this->appliedStaticForces->mY = fm.My;
	this->appliedStaticForces->mZ = fm.Mz;
	this->appliedStaticForces->forceMagnitude = fm.ForceMagnitude;

	fm = ForceConverter::calculateForcesAndMoments(this->forceActuatorApplicationSettings, this->forceActuatorSettings, this->rejectedStaticForces->xForces, this->rejectedStaticForces->yForces, this->rejectedStaticForces->zForces);
	this->rejectedStaticForces->fX = fm.Fx;
	this->rejectedStaticForces->fY = fm.Fy;
	this->rejectedStaticForces->fZ = fm.Fz;
	this->rejectedStaticForces->mX = fm.Mx;
	this->rejectedStaticForces->mY = fm.My;
	this->rejectedStaticForces->mZ = fm.Mz;
	this->rejectedStaticForces->forceMagnitude = fm.ForceMagnitude;

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
