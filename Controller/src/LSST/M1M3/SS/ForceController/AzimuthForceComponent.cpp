/*
 * AzimuthForceComponent.cpp
 *
 *  Created on: Jul 9, 2018
 *      Author: ccontaxis
 */

#include <AzimuthForceComponent.h>
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

AzimuthForceComponent::AzimuthForceComponent(M1M3SSPublisher* publisher, SafetyController* safetyController, ForceActuatorApplicationSettings* forceActuatorApplicationSettings, ForceActuatorSettings* forceActuatorSettings) {
	this->name = "Azimuth";

	this->publisher = publisher;
	this->safetyController = safetyController;
	this->forceActuatorApplicationSettings = forceActuatorApplicationSettings;
	this->forceActuatorSettings = forceActuatorSettings;
	this->forceActuatorState = this->publisher->getEventForceActuatorState();
	this->forceSetpointWarning = this->publisher->getEventForceSetpointWarning();
	this->appliedAzimuthForces = this->publisher->getEventAppliedAzimuthForces();
	this->rejectedAzimuthForces = this->publisher->getEventRejectedAzimuthForces();
	this->maxRateOfChange = this->forceActuatorSettings->AzimuthComponentSettings.MaxRateOfChange;
	this->nearZeroValue = this->forceActuatorSettings->AzimuthComponentSettings.NearZeroValue;
}

void AzimuthForceComponent::applyAzimuthForces(float* x, float* y, float* z) {
	Log.Info("AzimuthForceComponent: applyAzimuthForces()");
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

void AzimuthForceComponent::applyAzimuthForcesByAzimuthAngle(float azimuthAngle) {
	Log.Info("AzimuthForceComponent: applyAzimuthForcesByMirrorForces(%0.1f)", azimuthAngle);
	DistributedForces forces = ForceConverter::calculateForceFromAzimuthAngle(this->forceActuatorSettings, azimuthAngle);
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
	this->applyAzimuthForces(xForces, yForces, zForces);
}

void AzimuthForceComponent::postEnableDisableActions() {
	Log.Info("AzimuthForceComponent: postEnableDisableActions()");

	this->forceActuatorState->Timestamp = this->publisher->getTimestamp();
	this->forceActuatorState->AzimuthForcesApplied = this->enabled;
	this->publisher->tryLogForceActuatorState();
}

void AzimuthForceComponent::postUpdateActions() {
	Log.Trace("AzimuthForceController: postUpdateActions()");

	bool rejectionRequired = false;
	this->appliedAzimuthForces->Timestamp = this->publisher->getTimestamp();
	this->rejectedAzimuthForces->Timestamp = this->appliedAzimuthForces->Timestamp;
	for(int zIndex = 0; zIndex < 156; ++zIndex) {
		int xIndex = this->forceActuatorApplicationSettings->ZIndexToXIndex[zIndex];
		int yIndex = this->forceActuatorApplicationSettings->ZIndexToYIndex[zIndex];
		if (xIndex != -1) {
			float xLowFault = this->forceActuatorSettings->AzimuthLimitXTable[xIndex].LowFault;
			float xHighFault = this->forceActuatorSettings->AzimuthLimitXTable[xIndex].HighFault;
			this->rejectedAzimuthForces->XForces[xIndex] = this->xCurrent[xIndex];
			this->forceSetpointWarning->AzimuthForceWarning[zIndex] = this->forceSetpointWarning->AzimuthForceWarning[zIndex] ||
				!Range::InRangeAndCoerce(xLowFault, xHighFault, this->rejectedAzimuthForces->XForces[xIndex], this->appliedAzimuthForces->XForces + xIndex);
		}

		if (yIndex != -1) {
			float yLowFault = this->forceActuatorSettings->AzimuthLimitYTable[yIndex].LowFault;
			float yHighFault = this->forceActuatorSettings->AzimuthLimitYTable[yIndex].HighFault;
			this->rejectedAzimuthForces->YForces[yIndex] = this->yCurrent[yIndex];
			this->forceSetpointWarning->AzimuthForceWarning[zIndex] = this->forceSetpointWarning->AzimuthForceWarning[zIndex] ||
				!Range::InRangeAndCoerce(yLowFault, yHighFault, this->rejectedAzimuthForces->YForces[yIndex], this->appliedAzimuthForces->YForces + yIndex);
		}

		float zLowFault = this->forceActuatorSettings->AzimuthLimitZTable[zIndex].LowFault;
		float zHighFault = this->forceActuatorSettings->AzimuthLimitZTable[zIndex].HighFault;
		this->rejectedAzimuthForces->ZForces[zIndex] = this->zCurrent[zIndex];
		this->forceSetpointWarning->AzimuthForceWarning[zIndex] = this->forceSetpointWarning->AzimuthForceWarning[zIndex] ||
			!Range::InRangeAndCoerce(zLowFault, zHighFault, this->rejectedAzimuthForces->ZForces[zIndex], this->appliedAzimuthForces->ZForces + zIndex);
		rejectionRequired = rejectionRequired || this->forceSetpointWarning->AzimuthForceWarning[zIndex];
	}

	ForcesAndMoments fm = ForceConverter::calculateForcesAndMoments(this->forceActuatorApplicationSettings, this->forceActuatorSettings, this->appliedAzimuthForces->XForces, this->appliedAzimuthForces->YForces, this->appliedAzimuthForces->ZForces);
	this->appliedAzimuthForces->Fx = fm.Fx;
	this->appliedAzimuthForces->Fy = fm.Fy;
	this->appliedAzimuthForces->Fz = fm.Fz;
	this->appliedAzimuthForces->Mx = fm.Mx;
	this->appliedAzimuthForces->My = fm.My;
	this->appliedAzimuthForces->Mz = fm.Mz;
	this->appliedAzimuthForces->ForceMagnitude = fm.ForceMagnitude;

	fm = ForceConverter::calculateForcesAndMoments(this->forceActuatorApplicationSettings, this->forceActuatorSettings, this->rejectedAzimuthForces->XForces, this->rejectedAzimuthForces->YForces, this->rejectedAzimuthForces->ZForces);
	this->rejectedAzimuthForces->Fx = fm.Fx;
	this->rejectedAzimuthForces->Fy = fm.Fy;
	this->rejectedAzimuthForces->Fz = fm.Fz;
	this->rejectedAzimuthForces->Mx = fm.Mx;
	this->rejectedAzimuthForces->My = fm.My;
	this->rejectedAzimuthForces->Mz = fm.Mz;
	this->rejectedAzimuthForces->ForceMagnitude = fm.ForceMagnitude;

	this->safetyController->forceControllerNotifyAzimuthForceClipping(rejectionRequired);

	this->publisher->tryLogForceSetpointWarning();
	if (rejectionRequired) {
		this->publisher->logRejectedAzimuthForces();
	}
	this->publisher->logAppliedAzimuthForces();
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
