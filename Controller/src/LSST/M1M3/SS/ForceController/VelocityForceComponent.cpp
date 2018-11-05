/*
 * VelocityForceComponent.cpp
 *
 *  Created on: Jul 9, 2018
 *      Author: ccontaxis
 */

#include <VelocityForceComponent.h>
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

VelocityForceComponent::VelocityForceComponent(M1M3SSPublisher* publisher, SafetyController* safetyController, ForceActuatorApplicationSettings* forceActuatorApplicationSettings, ForceActuatorSettings* forceActuatorSettings) {
	this->name = "Velocity";

	this->publisher = publisher;
	this->safetyController = safetyController;
	this->forceActuatorApplicationSettings = forceActuatorApplicationSettings;
	this->forceActuatorSettings = forceActuatorSettings;
	this->forceActuatorState = this->publisher->getEventForceActuatorState();
	this->forceSetpointWarning = this->publisher->getEventForceSetpointWarning();
	this->appliedVelocityForces = this->publisher->getEventAppliedVelocityForces();
	this->rejectedVelocityForces = this->publisher->getEventRejectedVelocityForces();
	this->maxRateOfChange = this->forceActuatorSettings->VelocityComponentSettings.MaxRateOfChange;
	this->nearZeroValue = this->forceActuatorSettings->VelocityComponentSettings.NearZeroValue;
}

void VelocityForceComponent::applyVelocityForces(float* x, float* y, float* z) {
	Log.Trace("VelocityForceComponent: applyVelocityForces()");
	if (!this->enabled) {
		Log.Error("VelocityForceComponent: applyVelocityForces() called when the component is not applied");
		return;
	}
	if (this->disabling) {
		Log.Warn("VelocityForceComponent: applyVelocityForces() called when the component is disabling");
		return;
	}
	for(int i = 0; i < 156; ++i) {
		if (i < 12) {
			this->xTarget[i] = x[i];
		}

		if (i < 100) {
			this->yTarget[i] = y[i];
		}

		this->zTarget[i] = z[i];
	}
}

void VelocityForceComponent::applyVelocityForcesByAngularVelocity(float angularVelocityX, float angularVelocityY, float angularVelocityZ) {
	Log.Trace("VelocityForceComponent: applyVelocityForcesByMirrorForces(%0.1f, %0.1f, %0.1f)", angularVelocityX, angularVelocityY, angularVelocityZ);
	DistributedForces forces = ForceConverter::calculateForceFromAngularVelocity(this->forceActuatorSettings, angularVelocityX, angularVelocityY, angularVelocityZ);
	float xForces[12];
	float yForces[100];
	float zForces[156];
	for(int zIndex = 0; zIndex < FA_COUNT; ++zIndex) {
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
	this->applyVelocityForces(xForces, yForces, zForces);
}

void VelocityForceComponent::postEnableDisableActions() {
	Log.Debug("VelocityForceComponent: postEnableDisableActions()");

	this->forceActuatorState->timestamp = this->publisher->getTimestamp();
	this->forceActuatorState->velocityForcesApplied = this->enabled;
	this->publisher->tryLogForceActuatorState();
}

void VelocityForceComponent::postUpdateActions() {
	Log.Trace("VelocityForceController: postUpdateActions()");

	bool notInRange = false;
	bool rejectionRequired = false;
	this->appliedVelocityForces->timestamp = this->publisher->getTimestamp();
	this->rejectedVelocityForces->timestamp = this->appliedVelocityForces->timestamp;
	for(int zIndex = 0; zIndex < 156; ++zIndex) {
		int xIndex = this->forceActuatorApplicationSettings->ZIndexToXIndex[zIndex];
		int yIndex = this->forceActuatorApplicationSettings->ZIndexToYIndex[zIndex];

		this->forceSetpointWarning->velocityForceWarning[zIndex] = false;

		if (xIndex != -1) {
			float xLowFault = this->forceActuatorSettings->VelocityLimitXTable[xIndex].LowFault;
			float xHighFault = this->forceActuatorSettings->VelocityLimitXTable[xIndex].HighFault;
			this->rejectedVelocityForces->xForces[xIndex] = this->xCurrent[xIndex];
			notInRange = !Range::InRangeAndCoerce(xLowFault, xHighFault, this->rejectedVelocityForces->xForces[xIndex], this->appliedVelocityForces->xForces + xIndex);
			this->forceSetpointWarning->velocityForceWarning[zIndex] = this->forceSetpointWarning->velocityForceWarning[zIndex] || notInRange;
		}

		if (yIndex != -1) {
			float yLowFault = this->forceActuatorSettings->VelocityLimitYTable[yIndex].LowFault;
			float yHighFault = this->forceActuatorSettings->VelocityLimitYTable[yIndex].HighFault;
			this->rejectedVelocityForces->yForces[yIndex] = this->yCurrent[yIndex];
			notInRange = !Range::InRangeAndCoerce(yLowFault, yHighFault, this->rejectedVelocityForces->yForces[yIndex], this->appliedVelocityForces->yForces + yIndex);
			this->forceSetpointWarning->velocityForceWarning[zIndex] = this->forceSetpointWarning->velocityForceWarning[zIndex] || notInRange;
		}

		float zLowFault = this->forceActuatorSettings->VelocityLimitZTable[zIndex].LowFault;
		float zHighFault = this->forceActuatorSettings->VelocityLimitZTable[zIndex].HighFault;
		this->rejectedVelocityForces->zForces[zIndex] = this->zCurrent[zIndex];
		notInRange = !Range::InRangeAndCoerce(zLowFault, zHighFault, this->rejectedVelocityForces->zForces[zIndex], this->appliedVelocityForces->zForces + zIndex);
		this->forceSetpointWarning->velocityForceWarning[zIndex] = this->forceSetpointWarning->velocityForceWarning[zIndex] || notInRange;
		rejectionRequired = rejectionRequired || this->forceSetpointWarning->velocityForceWarning[zIndex];
	}

	ForcesAndMoments fm = ForceConverter::calculateForcesAndMoments(this->forceActuatorApplicationSettings, this->forceActuatorSettings, this->appliedVelocityForces->xForces, this->appliedVelocityForces->yForces, this->appliedVelocityForces->zForces);
	this->appliedVelocityForces->fX = fm.Fx;
	this->appliedVelocityForces->fY = fm.Fy;
	this->appliedVelocityForces->fZ = fm.Fz;
	this->appliedVelocityForces->mX = fm.Mx;
	this->appliedVelocityForces->mY = fm.My;
	this->appliedVelocityForces->mZ = fm.Mz;
	this->appliedVelocityForces->forceMagnitude = fm.ForceMagnitude;

	fm = ForceConverter::calculateForcesAndMoments(this->forceActuatorApplicationSettings, this->forceActuatorSettings, this->rejectedVelocityForces->xForces, this->rejectedVelocityForces->yForces, this->rejectedVelocityForces->zForces);
	this->rejectedVelocityForces->fX = fm.Fx;
	this->rejectedVelocityForces->fY = fm.Fy;
	this->rejectedVelocityForces->fZ = fm.Fz;
	this->rejectedVelocityForces->mX = fm.Mx;
	this->rejectedVelocityForces->mY = fm.My;
	this->rejectedVelocityForces->mZ = fm.Mz;
	this->rejectedVelocityForces->forceMagnitude = fm.ForceMagnitude;

	this->safetyController->forceControllerNotifyVelocityForceClipping(rejectionRequired);

	this->publisher->tryLogForceSetpointWarning();
	if (rejectionRequired) {
		this->publisher->logRejectedVelocityForces();
	}
	this->publisher->logAppliedVelocityForces();
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
