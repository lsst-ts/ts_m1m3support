/*
 * AccelerationForceComponent.cpp
 *
 *  Created on: Jul 9, 2018
 *      Author: ccontaxis
 */

#include <AccelerationForceComponent.h>
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

AccelerationForceComponent::AccelerationForceComponent(M1M3SSPublisher* publisher, SafetyController* safetyController, ForceActuatorApplicationSettings* forceActuatorApplicationSettings, ForceActuatorSettings* forceActuatorSettings) {
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
	Log.Trace("AccelerationForceComponent: applyAccelerationForces()");
	if (!this->enabled) {
		Log.Error("AccelerationForceComponent: applyAccelerationForces() called when the component is not applied");
		return;
	}
	if (this->disabling) {
		Log.Warn("AccelerationForceComponent: applyAccelerationForces() called when the component is disabling");
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

void AccelerationForceComponent::applyAccelerationForcesByAngularAccelerations(float angularAccelerationX, float angularAccelerationY, float angularAccelerationZ) {
	Log.Trace("AccelerationForceComponent: applyAccelerationForcesByAngularAccelerations(%0.1f, %0.1f, %0.1f)", angularAccelerationX, angularAccelerationY, angularAccelerationZ);
	DistributedForces forces = ForceConverter::calculateForceFromAngularAcceleration(this->forceActuatorSettings, angularAccelerationX, angularAccelerationY, angularAccelerationZ);
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
	this->applyAccelerationForces(xForces, yForces, zForces);
}

void AccelerationForceComponent::postEnableDisableActions() {
	Log.Debug("AccelerationForceComponent: postEnableDisableActions()");

	this->forceActuatorState->Timestamp = this->publisher->getTimestamp();
	this->forceActuatorState->AccelerationForcesApplied = this->enabled;
	this->publisher->tryLogForceActuatorState();
}

void AccelerationForceComponent::postUpdateActions() {
	Log.Trace("AccelerationForceController: postUpdateActions()");

	bool rejectionRequired = false;
	this->appliedAccelerationForces->Timestamp = this->publisher->getTimestamp();
	this->rejectedAccelerationForces->Timestamp = this->appliedAccelerationForces->Timestamp;
	for(int zIndex = 0; zIndex < 156; ++zIndex) {
		int xIndex = this->forceActuatorApplicationSettings->ZIndexToXIndex[zIndex];
		int yIndex = this->forceActuatorApplicationSettings->ZIndexToYIndex[zIndex];
		if (xIndex != -1) {
			float xLowFault = this->forceActuatorSettings->AccelerationLimitXTable[xIndex].LowFault;
			float xHighFault = this->forceActuatorSettings->AccelerationLimitXTable[xIndex].HighFault;
			this->rejectedAccelerationForces->XForces[xIndex] = this->xCurrent[xIndex];
			this->forceSetpointWarning->AccelerationForceWarning[zIndex] = this->forceSetpointWarning->AccelerationForceWarning[zIndex] ||
				!Range::InRangeAndCoerce(xLowFault, xHighFault, this->rejectedAccelerationForces->XForces[xIndex], this->appliedAccelerationForces->XForces + xIndex);
		}

		if (yIndex != -1) {
			float yLowFault = this->forceActuatorSettings->AccelerationLimitYTable[yIndex].LowFault;
			float yHighFault = this->forceActuatorSettings->AccelerationLimitYTable[yIndex].HighFault;
			this->rejectedAccelerationForces->YForces[yIndex] = this->yCurrent[yIndex];
			this->forceSetpointWarning->AccelerationForceWarning[zIndex] = this->forceSetpointWarning->AccelerationForceWarning[zIndex] ||
				!Range::InRangeAndCoerce(yLowFault, yHighFault, this->rejectedAccelerationForces->YForces[yIndex], this->appliedAccelerationForces->YForces + yIndex);
		}

		float zLowFault = this->forceActuatorSettings->AccelerationLimitZTable[zIndex].LowFault;
		float zHighFault = this->forceActuatorSettings->AccelerationLimitZTable[zIndex].HighFault;
		this->rejectedAccelerationForces->ZForces[zIndex] = this->zCurrent[zIndex];
		this->forceSetpointWarning->AccelerationForceWarning[zIndex] = this->forceSetpointWarning->AccelerationForceWarning[zIndex] ||
			!Range::InRangeAndCoerce(zLowFault, zHighFault, this->rejectedAccelerationForces->ZForces[zIndex], this->appliedAccelerationForces->ZForces + zIndex);
		rejectionRequired = rejectionRequired || this->forceSetpointWarning->AccelerationForceWarning[zIndex];
	}

	ForcesAndMoments fm = ForceConverter::calculateForcesAndMoments(this->forceActuatorApplicationSettings, this->forceActuatorSettings, this->appliedAccelerationForces->XForces, this->appliedAccelerationForces->YForces, this->appliedAccelerationForces->ZForces);
	this->appliedAccelerationForces->Fx = fm.Fx;
	this->appliedAccelerationForces->Fy = fm.Fy;
	this->appliedAccelerationForces->Fz = fm.Fz;
	this->appliedAccelerationForces->Mx = fm.Mx;
	this->appliedAccelerationForces->My = fm.My;
	this->appliedAccelerationForces->Mz = fm.Mz;
	this->appliedAccelerationForces->ForceMagnitude = fm.ForceMagnitude;

	fm = ForceConverter::calculateForcesAndMoments(this->forceActuatorApplicationSettings, this->forceActuatorSettings, this->rejectedAccelerationForces->XForces, this->rejectedAccelerationForces->YForces, this->rejectedAccelerationForces->ZForces);
	this->rejectedAccelerationForces->Fx = fm.Fx;
	this->rejectedAccelerationForces->Fy = fm.Fy;
	this->rejectedAccelerationForces->Fz = fm.Fz;
	this->rejectedAccelerationForces->Mx = fm.Mx;
	this->rejectedAccelerationForces->My = fm.My;
	this->rejectedAccelerationForces->Mz = fm.Mz;
	this->rejectedAccelerationForces->ForceMagnitude = fm.ForceMagnitude;

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
