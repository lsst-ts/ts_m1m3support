/*
 * ElevationForceComponent.cpp
 *
 *  Created on: Jul 9, 2018
 *      Author: ccontaxis
 */

#include <ElevationForceComponent.h>
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

ElevationForceComponent::ElevationForceComponent(M1M3SSPublisher* publisher, SafetyController* safetyController, ForceActuatorApplicationSettings* forceActuatorApplicationSettings, ForceActuatorSettings* forceActuatorSettings) {
	this->name = "Elevation";

	this->publisher = publisher;
	this->safetyController = safetyController;
	this->forceActuatorApplicationSettings = forceActuatorApplicationSettings;
	this->forceActuatorSettings = forceActuatorSettings;
	this->forceActuatorState = this->publisher->getEventForceActuatorState();
	this->forceActuatorWarning = this->publisher->getEventForceActuatorWarning();
	this->appliedElevationForces = this->publisher->getEventAppliedElevationForces();
	this->rejectedElevationForces = this->publisher->getEventRejectedElevationForces();
	this->maxRateOfChange = this->forceActuatorSettings->ElevationComponentSettings.MaxRateOfChange;
	this->nearZeroValue = this->forceActuatorSettings->ElevationComponentSettings.NearZeroValue;
}

void ElevationForceComponent::applyElevationForces(float* x, float* y, float* z) {
	Log.Trace("ElevationForceComponent: applyElevationForces()");
	if (!this->enabled) {
		Log.Error("ElevationForceComponent: applyElevationForces() called when the component is not applied");
		return;
	}
	if (this->disabling) {
		Log.Warn("ElevationForceComponent: applyElevationForces() called when the component is disabling");
		return;
	}
	for(int i = 0; i < 156; ++i) {
		if (i < 12) {
			this->xTarget[i] = x[i] * this->forceActuatorState->supportPercentage;
		}

		if (i < 100) {
			this->yTarget[i] = y[i] * this->forceActuatorState->supportPercentage;
		}

		this->zTarget[i] = z[i] * this->forceActuatorState->supportPercentage;
	}
}

void ElevationForceComponent::applyElevationForcesByElevationAngle(float elevationAngle) {
	Log.Trace("ElevationForceComponent: applyElevationForcesByMirrorForces(%0.1f)", elevationAngle);
	DistributedForces forces = ForceConverter::calculateForceFromElevationAngle(this->forceActuatorSettings, elevationAngle);
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
	this->applyElevationForces(xForces, yForces, zForces);
}

void ElevationForceComponent::postEnableDisableActions() {
	Log.Debug("ElevationForceComponent: postEnableDisableActions()");

	this->forceActuatorState->elevationForcesApplied = this->enabled;
	this->publisher->tryLogForceActuatorState();
}

void ElevationForceComponent::postUpdateActions() {
	Log.Trace("ElevationForceController: postUpdateActions()");

	bool rejectionRequired = false;
	for(int zIndex = 0; zIndex < 156; ++zIndex) {
		int xIndex = this->forceActuatorApplicationSettings->ZIndexToXIndex[zIndex];
		int yIndex = this->forceActuatorApplicationSettings->ZIndexToYIndex[zIndex];
		bool clipping = false;

		if (xIndex != -1) {
			float xLowFault = this->forceActuatorSettings->ElevationLimitXTable[xIndex].LowFault;
			float xHighFault = this->forceActuatorSettings->ElevationLimitXTable[xIndex].HighFault;
			this->rejectedElevationForces->xForces[xIndex] = this->xCurrent[xIndex];
			clipping = clipping || !Range::InRangeAndCoerce(xLowFault, xHighFault, this->rejectedElevationForces->xForces[xIndex], this->appliedElevationForces->xForces + xIndex);
		}

		if (yIndex != -1) {
			float yLowFault = this->forceActuatorSettings->ElevationLimitYTable[yIndex].LowFault;
			float yHighFault = this->forceActuatorSettings->ElevationLimitYTable[yIndex].HighFault;
			this->rejectedElevationForces->yForces[yIndex] = this->yCurrent[yIndex];
			clipping = clipping || !Range::InRangeAndCoerce(yLowFault, yHighFault, this->rejectedElevationForces->yForces[yIndex], this->appliedElevationForces->yForces + yIndex);
		}

		float zLowFault = this->forceActuatorSettings->ElevationLimitZTable[zIndex].LowFault;
		float zHighFault = this->forceActuatorSettings->ElevationLimitZTable[zIndex].HighFault;
		this->rejectedElevationForces->zForces[zIndex] = this->zCurrent[zIndex];
		clipping = clipping || !Range::InRangeAndCoerce(zLowFault, zHighFault, this->rejectedElevationForces->zForces[zIndex], this->appliedElevationForces->zForces + zIndex);
		BitHelper::set(this->forceActuatorWarning->forceActuatorFlags + zIndex, ForceActuatorFlags::ForceSetpointElevationForceClipped, clipping);
		rejectionRequired = rejectionRequired || clipping;
	}

	ForcesAndMoments fm = ForceConverter::calculateForcesAndMoments(this->forceActuatorApplicationSettings, this->forceActuatorSettings, this->appliedElevationForces->xForces, this->appliedElevationForces->yForces, this->appliedElevationForces->zForces);
	this->appliedElevationForces->fX = fm.Fx;
	this->appliedElevationForces->fY = fm.Fy;
	this->appliedElevationForces->fZ = fm.Fz;
	this->appliedElevationForces->mX = fm.Mx;
	this->appliedElevationForces->mY = fm.My;
	this->appliedElevationForces->mZ = fm.Mz;
	this->appliedElevationForces->forceMagnitude = fm.ForceMagnitude;

	fm = ForceConverter::calculateForcesAndMoments(this->forceActuatorApplicationSettings, this->forceActuatorSettings, this->rejectedElevationForces->xForces, this->rejectedElevationForces->yForces, this->rejectedElevationForces->zForces);
	this->rejectedElevationForces->fX = fm.Fx;
	this->rejectedElevationForces->fY = fm.Fy;
	this->rejectedElevationForces->fZ = fm.Fz;
	this->rejectedElevationForces->mX = fm.Mx;
	this->rejectedElevationForces->mY = fm.My;
	this->rejectedElevationForces->mZ = fm.Mz;
	this->rejectedElevationForces->forceMagnitude = fm.ForceMagnitude;

	this->safetyController->forceControllerNotifyElevationForceClipping(rejectionRequired);

	this->publisher->tryLogForceActuatorWarning();
	if (rejectionRequired) {
		this->publisher->logRejectedElevationForces();
	}
	this->publisher->logAppliedElevationForces();
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
