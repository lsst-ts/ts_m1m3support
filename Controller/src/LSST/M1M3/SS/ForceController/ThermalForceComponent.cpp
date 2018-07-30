/*
 * ThermalForceComponent.cpp
 *
 *  Created on: Jul 9, 2018
 *      Author: ccontaxis
 */

#include <ThermalForceComponent.h>
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

ThermalForceComponent::ThermalForceComponent(M1M3SSPublisher* publisher, SafetyController* safetyController, ForceActuatorApplicationSettings* forceActuatorApplicationSettings, ForceActuatorSettings* forceActuatorSettings) {
	this->name = "Thermal";

	this->publisher = publisher;
	this->safetyController = safetyController;
	this->forceActuatorApplicationSettings = forceActuatorApplicationSettings;
	this->forceActuatorSettings = forceActuatorSettings;
	this->forceActuatorState = this->publisher->getEventForceActuatorState();
	this->forceSetpointWarning = this->publisher->getEventForceSetpointWarning();
	this->appliedThermalForces = this->publisher->getEventAppliedThermalForces();
	this->rejectedThermalForces = this->publisher->getEventRejectedThermalForces();
	this->maxRateOfChange = this->forceActuatorSettings->ThermalComponentSettings.MaxRateOfChange;
	this->nearZeroValue = this->forceActuatorSettings->ThermalComponentSettings.NearZeroValue;
}

void ThermalForceComponent::applyThermalForces(float* x, float* y, float* z) {
	Log.Trace("ThermalForceComponent: applyThermalForces()");
	if (!this->enabled) {
		Log.Error("ThermalForceComponent: applyThermalForces() called when the component is not applied");
		return;
	}
	if (this->disabling) {
		Log.Warn("ThermalForceComponent: applyThermalForces() called when the component is disabling");
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

void ThermalForceComponent::applyThermalForcesByMirrorTemperature(float temperature) {
	Log.Trace("ThermalForceComponent: applyThermalForcesByMirrorForces(%0.1f)", temperature);
	DistributedForces forces = ForceConverter::calculateForceFromTemperature(this->forceActuatorSettings, temperature);
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
	this->applyThermalForces(xForces, yForces, zForces);
}

void ThermalForceComponent::postEnableDisableActions() {
	Log.Debug("ThermalForceComponent: postEnableDisableActions()");

	this->forceActuatorState->Timestamp = this->publisher->getTimestamp();
	this->forceActuatorState->ThermalForcesApplied = this->enabled;
	this->publisher->tryLogForceActuatorState();
}

void ThermalForceComponent::postUpdateActions() {
	Log.Trace("ThermalForceController: postUpdateActions()");

	bool notInRange = false;
	bool rejectionRequired = false;
	this->appliedThermalForces->Timestamp = this->publisher->getTimestamp();
	this->rejectedThermalForces->Timestamp = this->appliedThermalForces->Timestamp;
	for(int zIndex = 0; zIndex < 156; ++zIndex) {
		int xIndex = this->forceActuatorApplicationSettings->ZIndexToXIndex[zIndex];
		int yIndex = this->forceActuatorApplicationSettings->ZIndexToYIndex[zIndex];

		this->forceSetpointWarning->ThermalForceWarning[zIndex] = false;

		if (xIndex != -1) {
			float xLowFault = this->forceActuatorSettings->ThermalLimitXTable[xIndex].LowFault;
			float xHighFault = this->forceActuatorSettings->ThermalLimitXTable[xIndex].HighFault;
			this->rejectedThermalForces->XForces[xIndex] = this->xCurrent[xIndex];
			notInRange = !Range::InRangeAndCoerce(xLowFault, xHighFault, this->rejectedThermalForces->XForces[xIndex], this->appliedThermalForces->XForces + xIndex);
			this->forceSetpointWarning->ThermalForceWarning[zIndex] = this->forceSetpointWarning->ThermalForceWarning[zIndex] || notInRange;
		}

		if (yIndex != -1) {
			float yLowFault = this->forceActuatorSettings->ThermalLimitYTable[yIndex].LowFault;
			float yHighFault = this->forceActuatorSettings->ThermalLimitYTable[yIndex].HighFault;
			this->rejectedThermalForces->YForces[yIndex] = this->yCurrent[yIndex];
			notInRange = !Range::InRangeAndCoerce(yLowFault, yHighFault, this->rejectedThermalForces->YForces[yIndex], this->appliedThermalForces->YForces + yIndex);
			this->forceSetpointWarning->ThermalForceWarning[zIndex] = this->forceSetpointWarning->ThermalForceWarning[zIndex] || notInRange;
		}

		float zLowFault = this->forceActuatorSettings->ThermalLimitZTable[zIndex].LowFault;
		float zHighFault = this->forceActuatorSettings->ThermalLimitZTable[zIndex].HighFault;
		this->rejectedThermalForces->ZForces[zIndex] = this->zCurrent[zIndex];
		notInRange = !Range::InRangeAndCoerce(zLowFault, zHighFault, this->rejectedThermalForces->ZForces[zIndex], this->appliedThermalForces->ZForces + zIndex);
		this->forceSetpointWarning->ThermalForceWarning[zIndex] = this->forceSetpointWarning->ThermalForceWarning[zIndex] || notInRange;
		rejectionRequired = rejectionRequired || this->forceSetpointWarning->ThermalForceWarning[zIndex];
	}

	ForcesAndMoments fm = ForceConverter::calculateForcesAndMoments(this->forceActuatorApplicationSettings, this->forceActuatorSettings, this->appliedThermalForces->XForces, this->appliedThermalForces->YForces, this->appliedThermalForces->ZForces);
	this->appliedThermalForces->Fx = fm.Fx;
	this->appliedThermalForces->Fy = fm.Fy;
	this->appliedThermalForces->Fz = fm.Fz;
	this->appliedThermalForces->Mx = fm.Mx;
	this->appliedThermalForces->My = fm.My;
	this->appliedThermalForces->Mz = fm.Mz;
	this->appliedThermalForces->ForceMagnitude = fm.ForceMagnitude;

	fm = ForceConverter::calculateForcesAndMoments(this->forceActuatorApplicationSettings, this->forceActuatorSettings, this->rejectedThermalForces->XForces, this->rejectedThermalForces->YForces, this->rejectedThermalForces->ZForces);
	this->rejectedThermalForces->Fx = fm.Fx;
	this->rejectedThermalForces->Fy = fm.Fy;
	this->rejectedThermalForces->Fz = fm.Fz;
	this->rejectedThermalForces->Mx = fm.Mx;
	this->rejectedThermalForces->My = fm.My;
	this->rejectedThermalForces->Mz = fm.Mz;
	this->rejectedThermalForces->ForceMagnitude = fm.ForceMagnitude;

	this->safetyController->forceControllerNotifyThermalForceClipping(rejectionRequired);

	this->publisher->tryLogForceSetpointWarning();
	if (rejectionRequired) {
		this->publisher->logRejectedThermalForces();
	}
	this->publisher->logAppliedThermalForces();
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
