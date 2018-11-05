/*
 * OffsetForceComponent.cpp
 *
 *  Created on: Jul 6, 2018
 *      Author: ccontaxis
 */

#include <OffsetForceComponent.h>
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
	Log.Debug("OffsetForceComponent: applyOffsetForces()");
	if (!this->enabled) {
		Log.Error("OffsetForceComponent: applyOffsetForces() called when the component is not applied");
		return;
	}
	if (this->disabling) {
		Log.Warn("ThermalOffsetComponent: applyOffsetForces() called when the component is disabling");
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
	Log.Debug("OffsetForceComponent: applyOffsetForcesByMirrorForces(%0.1f, %0.1f, %0.1f, %0.1f, %0.1f, %0.1f)", xForce, yForce, zForce, xMoment, yMoment, zMoment);
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
	Log.Debug("OffsetForceComponent: postEnableDisableActions()");

	this->forceActuatorState->timestamp = this->publisher->getTimestamp();
	this->forceActuatorState->offsetForcesApplied = this->enabled;
	this->publisher->tryLogForceActuatorState();
}

void OffsetForceComponent::postUpdateActions() {
	Log.Trace("OffsetForceController: postUpdateActions()");

	bool notInRange = false;
	bool rejectionRequired = false;
	this->appliedOffsetForces->timestamp = this->publisher->getTimestamp();
	this->rejectedOffsetForces->timestamp = this->appliedOffsetForces->timestamp;
	for(int zIndex = 0; zIndex < 156; ++zIndex) {
		int xIndex = this->forceActuatorApplicationSettings->ZIndexToXIndex[zIndex];
		int yIndex = this->forceActuatorApplicationSettings->ZIndexToYIndex[zIndex];

		this->forceSetpointWarning->offsetForceWarning[zIndex] = false;

		if (xIndex != -1) {
			float xLowFault = this->forceActuatorSettings->OffsetLimitXTable[xIndex].LowFault;
			float xHighFault = this->forceActuatorSettings->OffsetLimitXTable[xIndex].HighFault;
			this->rejectedOffsetForces->xForces[xIndex] = this->xCurrent[xIndex];
			notInRange = !Range::InRangeAndCoerce(xLowFault, xHighFault, this->rejectedOffsetForces->xForces[xIndex], this->appliedOffsetForces->xForces + xIndex);
			this->forceSetpointWarning->offsetForceWarning[zIndex] = this->forceSetpointWarning->offsetForceWarning[zIndex] || notInRange;
		}

		if (yIndex != -1) {
			float yLowFault = this->forceActuatorSettings->OffsetLimitYTable[yIndex].LowFault;
			float yHighFault = this->forceActuatorSettings->OffsetLimitYTable[yIndex].HighFault;
			this->rejectedOffsetForces->yForces[yIndex] = this->yCurrent[yIndex];
			notInRange = !Range::InRangeAndCoerce(yLowFault, yHighFault, this->rejectedOffsetForces->yForces[yIndex], this->appliedOffsetForces->yForces + yIndex);
			this->forceSetpointWarning->offsetForceWarning[zIndex] = this->forceSetpointWarning->offsetForceWarning[zIndex] || notInRange;
		}

		float zLowFault = this->forceActuatorSettings->OffsetLimitZTable[zIndex].LowFault;
		float zHighFault = this->forceActuatorSettings->OffsetLimitZTable[zIndex].HighFault;
		this->rejectedOffsetForces->zForces[zIndex] = this->zCurrent[zIndex];
		notInRange = !Range::InRangeAndCoerce(zLowFault, zHighFault, this->rejectedOffsetForces->zForces[zIndex], this->appliedOffsetForces->zForces + zIndex);
		this->forceSetpointWarning->offsetForceWarning[zIndex] = this->forceSetpointWarning->offsetForceWarning[zIndex] || notInRange;
		rejectionRequired = rejectionRequired || this->forceSetpointWarning->offsetForceWarning[zIndex];
	}

	ForcesAndMoments fm = ForceConverter::calculateForcesAndMoments(this->forceActuatorApplicationSettings, this->forceActuatorSettings, this->appliedOffsetForces->xForces, this->appliedOffsetForces->yForces, this->appliedOffsetForces->zForces);
	this->appliedOffsetForces->fX = fm.Fx;
	this->appliedOffsetForces->fY = fm.Fy;
	this->appliedOffsetForces->fZ = fm.Fz;
	this->appliedOffsetForces->mX = fm.Mx;
	this->appliedOffsetForces->mY = fm.My;
	this->appliedOffsetForces->mZ = fm.Mz;
	this->appliedOffsetForces->forceMagnitude = fm.ForceMagnitude;

	fm = ForceConverter::calculateForcesAndMoments(this->forceActuatorApplicationSettings, this->forceActuatorSettings, this->rejectedOffsetForces->xForces, this->rejectedOffsetForces->yForces, this->rejectedOffsetForces->zForces);
	this->rejectedOffsetForces->fX = fm.Fx;
	this->rejectedOffsetForces->fY = fm.Fy;
	this->rejectedOffsetForces->fZ = fm.Fz;
	this->rejectedOffsetForces->mX = fm.Mx;
	this->rejectedOffsetForces->mY = fm.My;
	this->rejectedOffsetForces->mZ = fm.Mz;
	this->rejectedOffsetForces->forceMagnitude = fm.ForceMagnitude;

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
