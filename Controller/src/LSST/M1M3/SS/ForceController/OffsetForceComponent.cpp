/*
 * OffsetForceComponent.cpp
 *
 *  Created on: Jul 6, 2018
 *      Author: ccontaxis
 */

#include <OffsetForceComponent.h>
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

OffsetForceComponent::OffsetForceComponent(M1M3SSPublisher* publisher, SafetyController* safetyController, ForceActuatorApplicationSettings* forceActuatorApplicationSettings, ForceActuatorSettings* forceActuatorSettings) {
	this->name = "Offset";

	this->publisher = publisher;
	this->safetyController = safetyController;
	this->forceActuatorApplicationSettings = forceActuatorApplicationSettings;
	this->forceActuatorSettings = forceActuatorSettings;
	this->forceActuatorState = this->publisher->getEventForceActuatorState();
	this->forceSetpointWarning = this->publisher->getEventForceSetpointWarning();
	this->appliedOffsetForces = this->publisher->getEventAppliedOffsetForces();
	this->rejectedOffsetForces = this->publisher->getEventRejectedOffsetForces();
	this->maxRateOfChange = this->forceActuatorSettings->OffsetComponentSettings.MaxRateOfChange;
	this->nearZeroValue = this->forceActuatorSettings->OffsetComponentSettings.NearZeroValue;
}

void OffsetForceComponent::applyOffsetForces(float* x, float* y, float* z) {
	Log.Info("OffsetForceComponent: applyOffsetForces()");
	if (!this->enabled) {
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

void OffsetForceComponent::applyOffsetForcesByMirrorForces(float xForce, float yForce, float zForce, float xMoment, float yMoment, float zMoment) {
	Log.Info("OffsetForceComponent: applyOffsetForcesByMirrorForces(%0.1f, %0.1f, %0.1f, %0.1f, %0.1f, %0.1f)", xForce, yForce, zForce, xMoment, yMoment, zMoment);
	DistributedForces forces = ForceConverter::calculateForceDistribution(this->forceActuatorSettings, xForce, yForce, zForce, xMoment, yMoment, zMoment);
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
	this->applyOffsetForces(xForces, yForces, zForces);
}

void OffsetForceComponent::postEnableDisableActions() {
	Log.Info("OffsetForceComponent: postEnableDisableActions()");

	this->forceActuatorState->Timestamp = this->publisher->getTimestamp();
	this->forceActuatorState->OffsetForcesApplied = this->enabled;
	this->publisher->tryLogForceActuatorState();
}

void OffsetForceComponent::postUpdateActions() {
	Log.Trace("OffsetForceController: postUpdateActions()");

	bool rejectionRequired = false;
	this->appliedOffsetForces->Timestamp = this->publisher->getTimestamp();
	this->rejectedOffsetForces->Timestamp = this->appliedOffsetForces->Timestamp;
	for(int zIndex = 0; zIndex < 156; ++zIndex) {
		int xIndex = this->forceActuatorApplicationSettings->ZIndexToXIndex[zIndex];
		int yIndex = this->forceActuatorApplicationSettings->ZIndexToYIndex[zIndex];
		if (xIndex != -1) {
			float xLowFault = this->forceActuatorSettings->OffsetLimitXTable[xIndex].LowFault;
			float xHighFault = this->forceActuatorSettings->OffsetLimitXTable[xIndex].HighFault;
			this->rejectedOffsetForces->XForces[xIndex] = this->xCurrent[xIndex];
			this->forceSetpointWarning->OffsetForceWarning[zIndex] = this->forceSetpointWarning->OffsetForceWarning[zIndex] ||
				!Range::InRangeAndCoerce(xLowFault, xHighFault, this->rejectedOffsetForces->XForces[xIndex], this->appliedOffsetForces->XForces + xIndex);
		}

		if (yIndex != -1) {
			float yLowFault = this->forceActuatorSettings->OffsetLimitYTable[yIndex].LowFault;
			float yHighFault = this->forceActuatorSettings->OffsetLimitYTable[yIndex].HighFault;
			this->rejectedOffsetForces->YForces[yIndex] = this->yCurrent[yIndex];
			this->forceSetpointWarning->OffsetForceWarning[zIndex] = this->forceSetpointWarning->OffsetForceWarning[zIndex] ||
				!Range::InRangeAndCoerce(yLowFault, yHighFault, this->rejectedOffsetForces->YForces[yIndex], this->appliedOffsetForces->YForces + yIndex);
		}

		float zLowFault = this->forceActuatorSettings->OffsetLimitZTable[zIndex].LowFault;
		float zHighFault = this->forceActuatorSettings->OffsetLimitZTable[zIndex].HighFault;
		this->rejectedOffsetForces->ZForces[zIndex] = this->zCurrent[zIndex];
		this->forceSetpointWarning->OffsetForceWarning[zIndex] = this->forceSetpointWarning->OffsetForceWarning[zIndex] ||
			!Range::InRangeAndCoerce(zLowFault, zHighFault, this->rejectedOffsetForces->ZForces[zIndex], this->appliedOffsetForces->ZForces + zIndex);
		rejectionRequired = rejectionRequired || this->forceSetpointWarning->OffsetForceWarning[zIndex];
	}

	ForcesAndMoments fm = ForceConverter::calculateForcesAndMoments(this->forceActuatorApplicationSettings, this->forceActuatorSettings, this->appliedOffsetForces->XForces, this->appliedOffsetForces->YForces, this->appliedOffsetForces->ZForces);
	this->appliedOffsetForces->Fx = fm.Fx;
	this->appliedOffsetForces->Fy = fm.Fy;
	this->appliedOffsetForces->Fz = fm.Fz;
	this->appliedOffsetForces->Mx = fm.Mx;
	this->appliedOffsetForces->My = fm.My;
	this->appliedOffsetForces->Mz = fm.Mz;
	this->appliedOffsetForces->ForceMagnitude = fm.ForceMagnitude;

	fm = ForceConverter::calculateForcesAndMoments(this->forceActuatorApplicationSettings, this->forceActuatorSettings, this->rejectedOffsetForces->XForces, this->rejectedOffsetForces->YForces, this->rejectedOffsetForces->ZForces);
	this->rejectedOffsetForces->Fx = fm.Fx;
	this->rejectedOffsetForces->Fy = fm.Fy;
	this->rejectedOffsetForces->Fz = fm.Fz;
	this->rejectedOffsetForces->Mx = fm.Mx;
	this->rejectedOffsetForces->My = fm.My;
	this->rejectedOffsetForces->Mz = fm.Mz;
	this->rejectedOffsetForces->ForceMagnitude = fm.ForceMagnitude;

	this->safetyController->forceControllerNotifyOffsetForceClipping(rejectionRequired);

	this->publisher->tryLogForceSetpointWarning();
	if (rejectionRequired) {
		this->publisher->logRejectedOffsetForces();
	}
	this->publisher->logAppliedOffsetForces();
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
