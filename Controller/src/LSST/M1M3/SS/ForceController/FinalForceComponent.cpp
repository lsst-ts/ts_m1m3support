/*
 * FinalForceComponent.cpp
 *
 *  Created on: Jul 10, 2018
 *      Author: ccontaxis
 */

#include <FinalForceComponent.h>
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

FinalForceComponent::FinalForceComponent(M1M3SSPublisher* publisher, SafetyController* safetyController, ForceActuatorApplicationSettings* forceActuatorApplicationSettings, ForceActuatorSettings* forceActuatorSettings) {
	this->name = "Final";

	this->publisher = publisher;
	this->safetyController = safetyController;
	this->forceActuatorApplicationSettings = forceActuatorApplicationSettings;
	this->forceActuatorSettings = forceActuatorSettings;
	this->forceActuatorState = this->publisher->getEventForceActuatorState();
	this->forceSetpointWarning = this->publisher->getEventForceSetpointWarning();
	this->appliedForces = this->publisher->getEventAppliedForces();
	this->rejectedForces = this->publisher->getEventRejectedForces();
	this->maxRateOfChange = this->forceActuatorSettings->FinalComponentSettings.MaxRateOfChange;
	this->nearZeroValue = this->forceActuatorSettings->FinalComponentSettings.NearZeroValue;

	this->appliedAberrationForces = this->publisher->getEventAppliedAberrationForces();
	this->appliedAccelerationForces = this->publisher->getEventAppliedAccelerationForces();
	this->appliedActiveOpticForces = this->publisher->getEventAppliedActiveOpticForces();
	this->appliedAzimuthForces = this->publisher->getEventAppliedAzimuthForces();
	this->appliedBalanceForces = this->publisher->getEventAppliedBalanceForces();
	this->appliedElevationForces = this->publisher->getEventAppliedElevationForces();
	this->appliedOffsetForces = this->publisher->getEventAppliedOffsetForces();
	this->appliedStaticForces = this->publisher->getEventAppliedStaticForces();
	this->appliedThermalForces = this->publisher->getEventAppliedThermalForces();
	this->appliedVelocityForces = this->publisher->getEventAppliedVelocityForces();

	this->enable();
}

void FinalForceComponent::applyForces(float* x, float* y, float* z) {
	Log.Trace("FinalForceComponent: applyForces()");
	if (!this->isEnabled()) {
		this->enable();
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

void FinalForceComponent::applyForcesByComponents() {
	Log.Trace("FinalForceComponent: applyForcesByComponents()");
	if (!this->isEnabled()) {
		this->enable();
	}
	for(int i = 0; i < 156; ++i) {
		if (i < 12) {
			this->xTarget[i] =
					(this->appliedAccelerationForces->XForces[i] +
					this->appliedAzimuthForces->XForces[i] +
					this->appliedBalanceForces->XForces[i] +
					this->appliedElevationForces->XForces[i] +
					this->appliedOffsetForces->XForces[i] +
					this->appliedStaticForces->XForces[i] +
					this->appliedThermalForces->XForces[i] +
					this->appliedVelocityForces->XForces[i]);
		}

		if (i < 100) {
			this->yTarget[i] =
					(this->appliedAccelerationForces->YForces[i] +
					this->appliedAzimuthForces->YForces[i] +
					this->appliedBalanceForces->YForces[i] +
					this->appliedElevationForces->YForces[i] +
					this->appliedOffsetForces->YForces[i] +
					this->appliedStaticForces->YForces[i] +
					this->appliedThermalForces->YForces[i] +
					this->appliedVelocityForces->YForces[i]);
		}

		this->zTarget[i] =
				(this->appliedAberrationForces->ZForces[i] +
				this->appliedAccelerationForces->ZForces[i] +
				this->appliedActiveOpticForces->ZForces[i] +
				this->appliedAzimuthForces->ZForces[i] +
				this->appliedBalanceForces->ZForces[i] +
				this->appliedElevationForces->ZForces[i] +
				this->appliedOffsetForces->ZForces[i] +
				this->appliedStaticForces->ZForces[i] +
				this->appliedThermalForces->ZForces[i] +
				this->appliedVelocityForces->ZForces[i]);
	}
}

void FinalForceComponent::postEnableDisableActions() {
	Log.Trace("FinalForceComponent: postEnableDisableActions()");
}

void FinalForceComponent::postUpdateActions() {
	Log.Trace("FinalForceController: postUpdateActions()");

	bool notInRange = false;
	bool rejectionRequired = false;
	this->appliedForces->Timestamp = this->publisher->getTimestamp();
	this->rejectedForces->Timestamp = this->appliedForces->Timestamp;
	for(int zIndex = 0; zIndex < 156; ++zIndex) {
		int xIndex = this->forceActuatorApplicationSettings->ZIndexToXIndex[zIndex];
		int yIndex = this->forceActuatorApplicationSettings->ZIndexToYIndex[zIndex];

		this->forceSetpointWarning->ForceWarning[zIndex] = false;

		if (xIndex != -1) {
			float xLowFault = this->forceActuatorSettings->ForceLimitXTable[xIndex].LowFault;
			float xHighFault = this->forceActuatorSettings->ForceLimitXTable[xIndex].HighFault;
			this->rejectedForces->XForces[xIndex] = this->xCurrent[xIndex];
			notInRange = !Range::InRangeAndCoerce(xLowFault, xHighFault, this->rejectedForces->XForces[xIndex], this->appliedForces->XForces + xIndex);
			this->forceSetpointWarning->ForceWarning[zIndex] = this->forceSetpointWarning->ForceWarning[zIndex] || notInRange;
		}

		if (yIndex != -1) {
			float yLowFault = this->forceActuatorSettings->ForceLimitYTable[yIndex].LowFault;
			float yHighFault = this->forceActuatorSettings->ForceLimitYTable[yIndex].HighFault;
			this->rejectedForces->YForces[yIndex] = this->yCurrent[yIndex];
			notInRange = !Range::InRangeAndCoerce(yLowFault, yHighFault, this->rejectedForces->YForces[yIndex], this->appliedForces->YForces + yIndex);
			this->forceSetpointWarning->ForceWarning[zIndex] = this->forceSetpointWarning->ForceWarning[zIndex] || notInRange;
		}

		float zLowFault = this->forceActuatorSettings->ForceLimitZTable[zIndex].LowFault;
		float zHighFault = this->forceActuatorSettings->ForceLimitZTable[zIndex].HighFault;
		this->rejectedForces->ZForces[zIndex] = this->zCurrent[zIndex];
		notInRange = !Range::InRangeAndCoerce(zLowFault, zHighFault, this->rejectedForces->ZForces[zIndex], this->appliedForces->ZForces + zIndex);
		this->forceSetpointWarning->ForceWarning[zIndex] = this->forceSetpointWarning->ForceWarning[zIndex] || notInRange;
		rejectionRequired = rejectionRequired || this->forceSetpointWarning->ForceWarning[zIndex];
	}

	ForcesAndMoments fm = ForceConverter::calculateForcesAndMoments(this->forceActuatorApplicationSettings, this->forceActuatorSettings, this->appliedForces->XForces, this->appliedForces->YForces, this->appliedForces->ZForces);
	this->appliedForces->Fx = fm.Fx;
	this->appliedForces->Fy = fm.Fy;
	this->appliedForces->Fz = fm.Fz;
	this->appliedForces->Mx = fm.Mx;
	this->appliedForces->My = fm.My;
	this->appliedForces->Mz = fm.Mz;
	this->appliedForces->ForceMagnitude = fm.ForceMagnitude;

	fm = ForceConverter::calculateForcesAndMoments(this->forceActuatorApplicationSettings, this->forceActuatorSettings, this->rejectedForces->XForces, this->rejectedForces->YForces, this->rejectedForces->ZForces);
	this->rejectedForces->Fx = fm.Fx;
	this->rejectedForces->Fy = fm.Fy;
	this->rejectedForces->Fz = fm.Fz;
	this->rejectedForces->Mx = fm.Mx;
	this->rejectedForces->My = fm.My;
	this->rejectedForces->Mz = fm.Mz;
	this->rejectedForces->ForceMagnitude = fm.ForceMagnitude;

	this->safetyController->forceControllerNotifyForceClipping(rejectionRequired);

	this->publisher->tryLogForceSetpointWarning();
	if (rejectionRequired) {
		this->publisher->logRejectedForces();
	}
	this->publisher->logAppliedForces();
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
