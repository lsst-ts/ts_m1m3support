/*
 * ForceController.cpp
 *
 *  Created on: Oct 23, 2017
 *      Author: ccontaxis
 */

#include <ForceController.h>
#include <ForceActuatorApplicationSettings.h>
#include <ForceActuatorOrientations.h>
#include <ForceActuatorSettings.h>
#include <ForceActuatorLimits.h>
#include <M1M3SSPublisher.h>
#include <SafetyController.h>
#include <PIDSettings.h>
#include <Range.h>
#include <cmath>
#include <cstring>
#include <vector>
#include <Log.h>
#include <unistd.h>
#include <ForceConverter.h>

#include <iostream>
using namespace std;

namespace LSST {
namespace M1M3 {
namespace SS {

ForceController::ForceController(ForceActuatorApplicationSettings* forceActuatorApplicationSettings, ForceActuatorSettings* forceActuatorSettings, PIDSettings* pidSettings, M1M3SSPublisher* publisher, SafetyController* safetyController, MTMount_AltC* tmaAlt, MTMount_AzC* tmaAz)
 : aberrationForceComponent(publisher, safetyController, forceActuatorApplicationSettings, forceActuatorSettings),
   accelerationForceComponent(publisher, safetyController, forceActuatorApplicationSettings, forceActuatorSettings),
   activeOpticForceComponent(publisher, safetyController, forceActuatorApplicationSettings, forceActuatorSettings),
   azimuthForceComponent(publisher, safetyController, forceActuatorApplicationSettings, forceActuatorSettings),
   balanceForceComponent(publisher, safetyController, forceActuatorApplicationSettings, forceActuatorSettings, pidSettings),
   elevationForceComponent(publisher, safetyController, forceActuatorApplicationSettings, forceActuatorSettings),
   offsetForceComponent(publisher, safetyController, forceActuatorApplicationSettings, forceActuatorSettings),
   staticForceComponent(publisher, safetyController, forceActuatorApplicationSettings, forceActuatorSettings),
   thermalForceComponent(publisher, safetyController, forceActuatorApplicationSettings, forceActuatorSettings),
   velocityForceComponent(publisher, safetyController, forceActuatorApplicationSettings, forceActuatorSettings),
   finalForceComponent(publisher, safetyController, forceActuatorApplicationSettings, forceActuatorSettings) {
	Log.Debug("ForceController: ForceController()");
	this->forceActuatorApplicationSettings = forceActuatorApplicationSettings;
	this->forceActuatorSettings = forceActuatorSettings;
	this->publisher = publisher;
	this->safetyController = safetyController;
	this->pidSettings = pidSettings;

	this->appliedCylinderForces = this->publisher->getEventAppliedCylinderForces();
	this->appliedForces = this->publisher->getEventAppliedForces();
	this->forceActuatorState = this->publisher->getEventForceActuatorState();
	this->forceSetpointWarning = this->publisher->getEventForceSetpointWarning();
	this->rejectedCylinderForces = this->publisher->getEventRejectedCylinderForces();
	this->rejectedForces = this->publisher->getEventRejectedForces();

	this->forceActuatorInfo = this->publisher->getEventForceActuatorInfo();
	this->inclinometerData = this->publisher->getInclinometerData();
	this->forceActuatorData = this->publisher->getForceActuatorData();

	this->pidData = this->publisher->getPIDData();
	this->pidInfo = this->publisher->getEventPIDInfo();
	this->hardpointActuatorData = this->publisher->getHardpointActuatorData();
	this->accelerometerData = this->publisher->getAccelerometerData();
	this->gyroData = this->publisher->getGyroData();
	this->tmaElevationData = tmaAlt;
	this->tmaAzimuthData = tmaAz;

	memset(this->forceActuatorData, 0, sizeof(MTM1M3_forceActuatorDataC));
	memset(this->tmaAzimuthData, 0, sizeof(MTMount_AzC));
	memset(this->tmaElevationData, 0, sizeof(MTMount_AltC));

	memset(this->appliedForces, 0, sizeof(MTM1M3_logevent_appliedForcesC));
	memset(this->forceActuatorState, 0, sizeof(MTM1M3_logevent_forceActuatorStateC));
	memset(this->forceSetpointWarning, 0, sizeof(MTM1M3_logevent_forceSetpointWarningC));
	memset(this->rejectedCylinderForces, 0, sizeof(MTM1M3_logevent_rejectedCylinderForcesC));
	memset(this->rejectedForces, 0, sizeof(MTM1M3_logevent_rejectedForcesC));

	this->aberrationForceComponent.reset();
	this->accelerationForceComponent.reset();
	this->activeOpticForceComponent.reset();
	this->azimuthForceComponent.reset();
	this->balanceForceComponent.reset();
	this->elevationForceComponent.reset();
	this->offsetForceComponent.reset();
	this->staticForceComponent.reset();
	this->thermalForceComponent.reset();
	this->velocityForceComponent.reset();
	this->finalForceComponent.reset();

	double timestamp = this->publisher->getTimestamp();
	this->forceActuatorState->timestamp = timestamp;
	this->forceSetpointWarning->timestamp = timestamp;

	this->publisher->logForceActuatorState();
	this->publisher->logForceSetpointWarning();


	this->mirrorWeight = 0.0;
	float* zForces = ForceConverter::calculateForceFromElevationAngle(this->forceActuatorSettings, 0.0).ZForces;
	for(int i = 0; i < FA_COUNT; i++) {
		this->mirrorWeight += zForces[i];
		this->zero[i] = 0;
		ForceActuatorNeighbors neighbors;
		for(unsigned int j = 0; j < this->forceActuatorSettings->Neighbors[i].NearZIDs.size(); ++j) {
			int32_t id = this->forceActuatorSettings->Neighbors[i].NearZIDs[j];
			for(unsigned int k = 0; k < this->forceActuatorApplicationSettings->Table.size(); ++k) {
				if (this->forceActuatorApplicationSettings->Table[k].ActuatorID == id) {
					neighbors.NearZIDs.push_back(k);
					break;
				}
			}
		}
		for(unsigned int j = 0; j < this->forceActuatorSettings->Neighbors[i].FarIDs.size(); ++j) {
			int32_t id = this->forceActuatorSettings->Neighbors[i].FarIDs[j];
			for(unsigned int k = 0; k < this->forceActuatorApplicationSettings->Table.size(); ++k) {
				if (this->forceActuatorApplicationSettings->Table[k].ActuatorID == id) {
					neighbors.FarIDs.push_back(k);
					break;
				}
			}
		}
		this->neighbors.push_back(neighbors);
	}
}

void ForceController::reset() {
	Log.Info("ForceController: reset()");
	this->aberrationForceComponent.reset();
	this->accelerationForceComponent.reset();
	this->activeOpticForceComponent.reset();
	this->azimuthForceComponent.reset();
	this->balanceForceComponent.reset();
	this->elevationForceComponent.reset();
	this->offsetForceComponent.reset();
	this->staticForceComponent.reset();
	this->thermalForceComponent.reset();
	this->velocityForceComponent.reset();
	this->finalForceComponent.reset();
}

void ForceController::updateTMAElevationData(MTMount_AltC* tmaElevationData) {
	Log.Trace("ForceController: updateTMAElevationData()");
	memcpy(this->tmaElevationData, tmaElevationData, sizeof(MTMount_AltC));
}

void ForceController::incSupportPercentage() {
	Log.Trace("ForceController: incSupportPercentage()");
	this->forceActuatorState->supportPercentage += this->forceActuatorSettings->RaiseIncrementPercentage;
	if (this->supportPercentageFilled()) {
		this->forceActuatorState->supportPercentage = 1.0;
	}
}

void ForceController::decSupportPercentage() {
	Log.Trace("ForceController: decSupportPercentage()");
	this->forceActuatorState->supportPercentage -= this->forceActuatorSettings->LowerDecrementPercentage;
	if (this->supportPercentageZeroed()) {
		this->forceActuatorState->supportPercentage = 0.0;
	}
}

void ForceController::zeroSupportPercentage() {
	Log.Info("ForceController: zeroSupportPercentage()");
	this->forceActuatorState->supportPercentage = 0.0;
}

void ForceController::fillSupportPercentage() {
	Log.Info("ForceController: fillSupportPercentage()");
	this->forceActuatorState->supportPercentage = 1.0;
}

bool ForceController::supportPercentageFilled() {
	return this->forceActuatorState->supportPercentage >= 1.0;
}

bool ForceController::supportPercentageZeroed() {
	return this->forceActuatorState->supportPercentage <= 0.0;
}

bool ForceController::followingErrorInTolerance() {
	Log.Trace("ForceController: followingErrorInTolerance()");
	float limit = this->forceActuatorSettings->RaiseLowerFollowingErrorLimit;
	for(int i = 0; i < FA_COUNT; ++i) {
		if ((i < FA_X_COUNT && !Range::InRange(-limit, limit, this->forceActuatorData->xForces[i] - this->appliedForces->xForces[i])) ||
			(i < FA_Y_COUNT && !Range::InRange(-limit, limit, this->forceActuatorData->yForces[i] - this->appliedForces->yForces[i])) ||
			(!Range::InRange(-limit, limit, this->forceActuatorData->zForces[i] - this->appliedForces->zForces[i]))) {
			return false;
		}
	}
	return true;
}

void ForceController::updateAppliedForces() {
	Log.Trace("ForceController: updateAppliedForces()");

	if (this->aberrationForceComponent.isEnabled() || this->aberrationForceComponent.isDisabling()) {
		this->aberrationForceComponent.update();
	}
	if (this->accelerationForceComponent.isEnabled() || this->accelerationForceComponent.isDisabling()) {
		if (this->accelerationForceComponent.isEnabled()) {
			this->accelerationForceComponent.applyAccelerationForcesByAngularAccelerations(this->accelerometerData->angularAccelerationX, this->accelerometerData->angularAccelerationY, this->accelerometerData->angularAccelerationZ);
		}
		this->accelerationForceComponent.update();
	}
	if (this->activeOpticForceComponent.isEnabled() || this->activeOpticForceComponent.isDisabling()) {
		this->activeOpticForceComponent.update();
	}
	if (this->azimuthForceComponent.isEnabled() || this->azimuthForceComponent.isDisabling()) {
		this->azimuthForceComponent.update();
	}
	if (this->balanceForceComponent.isEnabled() || this->balanceForceComponent.isDisabling()) {
		if (this->balanceForceComponent.isEnabled()) {
			this->balanceForceComponent.applyBalanceForcesByMirrorForces(this->hardpointActuatorData->fX, this->hardpointActuatorData->fY, this->hardpointActuatorData->fZ, this->hardpointActuatorData->mX, this->hardpointActuatorData->mY, this->hardpointActuatorData->mZ);
		}
		this->balanceForceComponent.update();
	}
	if (this->elevationForceComponent.isEnabled() || this->elevationForceComponent.isDisabling()) {
		if (this->elevationForceComponent.isEnabled()) {
			double elevationAngle = this->forceActuatorSettings->UseInclinometer ? this->inclinometerData->inclinometerAngle : this->tmaElevationData->Angle_Actual;
			// Convert elevation angle to zenith angle (used by matrix)
			elevationAngle = 90.0 - elevationAngle;
			this->elevationForceComponent.applyElevationForcesByElevationAngle(elevationAngle);
		}
		this->elevationForceComponent.update();
	}
	if (this->offsetForceComponent.isEnabled() || this->offsetForceComponent.isDisabling()) {
		this->offsetForceComponent.update();
	}
	if (this->staticForceComponent.isEnabled() || this->staticForceComponent.isDisabling()) {
		this->staticForceComponent.update();
	}
	if (this->thermalForceComponent.isEnabled() || this->thermalForceComponent.isDisabling()) {
		this->thermalForceComponent.update();
	}
	if (this->velocityForceComponent.isEnabled() || this->velocityForceComponent.isDisabling()) {
		if (this->velocityForceComponent.isEnabled()) {
			this->velocityForceComponent.applyVelocityForcesByAngularVelocity(this->gyroData->angularVelocityX, this->gyroData->angularVelocityY, this->gyroData->angularVelocityZ);
		}
		this->velocityForceComponent.update();
	}
}

void ForceController::processAppliedForces() {
	Log.Trace("ForceController: processAppliedForces()");
	this->sumAllForces();
	this->convertForcesToSetpoints();
	this->checkMirrorMoments();
	this->checkNearNeighbors();
	this->checkMirrorWeight();
	this->checkFarNeighbors();
	this->publisher->tryLogForceSetpointWarning();
}

void ForceController::applyAberrationForcesByBendingModes(float* coefficients) {
	Log.Info("ForceController: applyAberrationForcesByBendingModes()");
	if(!this->aberrationForceComponent.isEnabled()) {
		this->aberrationForceComponent.enable();
	}
	this->aberrationForceComponent.applyAberrationForcesByBendingModes(coefficients);
}

void ForceController::applyAberrationForces(float* z) {
	Log.Info("ForceController: applyAberrationForces()");
	if (!this->aberrationForceComponent.isEnabled()) {
		this->aberrationForceComponent.enable();
	}
	this->aberrationForceComponent.applyAberrationForces(z);
}

void ForceController::zeroAberrationForces() {
	Log.Info("ForceController: zeroAberrationForces()");
	if (this->aberrationForceComponent.isEnabled()) {
		this->aberrationForceComponent.disable();
	}
}

void ForceController::applyAccelerationForces() {
	Log.Info("ForceController: applyAccelerationForces()");
	if (!this->accelerationForceComponent.isEnabled()) {
		this->accelerationForceComponent.enable();
	}
}

void ForceController::zeroAccelerationForces() {
	Log.Info("ForceController: zeroAccelerationForces()");
	if (this->accelerationForceComponent.isEnabled()) {
		this->accelerationForceComponent.disable();
	}
}

void ForceController::applyActiveOpticForcesByBendingModes(float* coefficients) {
	Log.Info("ForceController: applyActiveOpticForcesByBendingModes()");
	if (!this->activeOpticForceComponent.isEnabled()) {
		this->activeOpticForceComponent.enable();
	}
	this->activeOpticForceComponent.applyActiveOpticForcesByBendingModes(coefficients);
}

void ForceController::applyActiveOpticForces(float* z) {
	Log.Info("ForceController: applyActiveOpticForces()");
	if (!this->activeOpticForceComponent.isEnabled()) {
		this->activeOpticForceComponent.enable();
	}
	this->activeOpticForceComponent.applyActiveOpticForces(z);
}

void ForceController::zeroActiveOpticForces() {
	Log.Info("ForceController: zeroActiveOpticForces()");
	if (this->activeOpticForceComponent.isEnabled()) {
		this->activeOpticForceComponent.disable();
	}
}

void ForceController::applyAzimuthForces() {
	Log.Info("ForceController: applyAzimuthForces()");
	if (!this->azimuthForceComponent.isEnabled()) {
		this->azimuthForceComponent.enable();
	}
}

void ForceController::updateAzimuthForces(float azimuthAngle) {
	Log.Trace("ForceController: updateAzimuthForces()");
	this->azimuthForceComponent.applyAzimuthForcesByAzimuthAngle(azimuthAngle);
}

void ForceController::zeroAzimuthForces() {
	Log.Info("ForceController: zeroAzimuthForces()");
	if (this->azimuthForceComponent.isEnabled()) {
		this->azimuthForceComponent.disable();
	}
}

void ForceController::applyBalanceForces() {
	Log.Info("ForceController: applyBalanceForces()");
	if (!this->balanceForceComponent.isEnabled()) {
		this->balanceForceComponent.enable();
	}
}

void ForceController::zeroBalanceForces() {
	Log.Info("ForceController: zeroBalanceForces()");
	if (this->balanceForceComponent.isEnabled()) {
		this->balanceForceComponent.disable();
	}
}

void ForceController::updatePID(int id, PIDParameters parameters) {
	Log.Info("ForceController: updatePID()");
	this->balanceForceComponent.updatePID(id, parameters);
}

void ForceController::resetPID(int id) {
	Log.Info("ForceController: resetPID()");
	this->balanceForceComponent.resetPID(id);
}

void ForceController::resetPIDs() {
	Log.Info("ForceController: resetPIDs()");
	this->balanceForceComponent.resetPIDs();
}

void ForceController::applyElevationForces() {
	Log.Info("ForceController: applyElevationForces()");
	if (!this->elevationForceComponent.isEnabled()) {
		this->elevationForceComponent.enable();
	}
}

void ForceController::zeroElevationForces() {
	Log.Info("ForceController: zeroElevationForces()");
	if (this->elevationForceComponent.isEnabled()) {
		this->elevationForceComponent.disable();
	}
}

void ForceController::applyOffsetForces(float* x, float* y, float* z) {
	Log.Info("ForceController: applyOffsetForces()");
	if (!this->offsetForceComponent.isEnabled()) {
		this->offsetForceComponent.enable();
	}
	this->offsetForceComponent.applyOffsetForces(x, y, z);
}

void ForceController::applyOffsetForcesByMirrorForces(float xForce, float yForce, float zForce, float xMoment, float yMoment, float zMoment) {
	Log.Info("ForceController: applyOffsetForcesByMirrorForces(%0.1f, %0.1f, %0.1f, %0.1f, %0.1f, %0.1f)", xForce, yForce, zForce, xMoment, yMoment, zMoment);
	if (!this->offsetForceComponent.isEnabled()) {
		this->offsetForceComponent.enable();
	}
	this->offsetForceComponent.applyOffsetForcesByMirrorForces(xForce, yForce, zForce, xMoment, yMoment, zMoment);
}

void ForceController::zeroOffsetForces() {
	Log.Info("ForceController: zeroOffsetForces()");
	if (this->offsetForceComponent.isEnabled()) {
		this->offsetForceComponent.disable();
	}
}

void ForceController::applyStaticForces() {
	Log.Info("ForceController: applyStaticForces()");
	if (!this->staticForceComponent.isEnabled()) {
		this->staticForceComponent.enable();
	}
	this->staticForceComponent.applyStaticForces(&this->forceActuatorSettings->StaticXTable, &this->forceActuatorSettings->StaticYTable, &this->forceActuatorSettings->StaticZTable);
}

void ForceController::zeroStaticForces() {
	Log.Info("ForceController: zeroStaticForces()");
	if (this->staticForceComponent.isEnabled()) {
		this->staticForceComponent.disable();
	}
}

void ForceController::applyThermalForces() {
	Log.Info("ForceController: applyThermalForces()");
	if (!this->thermalForceComponent.isEnabled()) {
		this->thermalForceComponent.enable();
	}
}

void ForceController::updateThermalForces(float temperature) {
	Log.Trace("ForceController: updateThermalForces()");
	this->thermalForceComponent.applyThermalForcesByMirrorTemperature(temperature);
}

void ForceController::zeroThermalForces() {
	Log.Info("ForceController: zeroThermalForces()");
	if (this->thermalForceComponent.isEnabled()) {
		this->thermalForceComponent.disable();
	}
}

void ForceController::applyVelocityForces() {
	Log.Info("ForceController: applyVelocityForces()");
	if (!this->velocityForceComponent.isEnabled()) {
		this->velocityForceComponent.enable();
	}
}

void ForceController::zeroVelocityForces() {
	Log.Info("ForceController: zeroVelocityForces()");
	if (this->velocityForceComponent.isEnabled()) {
		this->velocityForceComponent.disable();
	}
}

void ForceController::sumAllForces() {
	Log.Trace("ForceController: sumAllForces()");
	this->finalForceComponent.applyForcesByComponents();
	this->finalForceComponent.update();
}

void ForceController::convertForcesToSetpoints() {
	Log.Trace("ForceController: convertForcesToSetpoints()");
	bool rejectionRequired = false;
	for(int pIndex = 0; pIndex < FA_COUNT; pIndex++) {
		int xIndex = this->forceActuatorApplicationSettings->ZIndexToXIndex[pIndex];
		int yIndex = this->forceActuatorApplicationSettings->ZIndexToYIndex[pIndex];
		int sIndex = this->forceActuatorApplicationSettings->ZIndexToSecondaryCylinderIndex[pIndex];

		this->forceSetpointWarning->safetyLimitWarning[pIndex] = false;

		if (sIndex != -1) {
			float secondaryLowFault = this->forceActuatorSettings->CylinderLimitSecondaryTable[sIndex].LowFault;
			float secondaryHighFault = this->forceActuatorSettings->CylinderLimitSecondaryTable[sIndex].HighFault;
			switch(this->forceActuatorInfo->actuatorOrientation[pIndex]) {
			case ForceActuatorOrientations::PositiveY:
				this->rejectedCylinderForces->secondaryCylinderForces[sIndex] = toInt24(this->appliedForces->yForces[yIndex] * sqrt2);
				break;
			case ForceActuatorOrientations::PositiveX:
				this->rejectedCylinderForces->secondaryCylinderForces[sIndex] = toInt24(this->appliedForces->xForces[xIndex] * sqrt2);
				break;
			case ForceActuatorOrientations::NegativeX:
				this->rejectedCylinderForces->secondaryCylinderForces[sIndex] = toInt24(-this->appliedForces->xForces[xIndex] * sqrt2);
				break;
			case ForceActuatorOrientations::NegativeY:
				this->rejectedCylinderForces->secondaryCylinderForces[sIndex] = toInt24(-this->appliedForces->yForces[yIndex] * sqrt2);
				break;
			}
			this->forceSetpointWarning->safetyLimitWarning[pIndex] = this->forceSetpointWarning->safetyLimitWarning[pIndex] ||
				!Range::InRangeAndCoerce((int)secondaryLowFault, (int)secondaryHighFault, this->rejectedCylinderForces->secondaryCylinderForces[sIndex], this->appliedCylinderForces->secondaryCylinderForces + sIndex);
		}

		float primaryLowFault = this->forceActuatorSettings->CylinderLimitPrimaryTable[pIndex].LowFault;
		float primaryHighFault = this->forceActuatorSettings->CylinderLimitPrimaryTable[pIndex].HighFault;
		switch(this->forceActuatorInfo->actuatorOrientation[pIndex]) {
		case ForceActuatorOrientations::PositiveY:
			this->rejectedCylinderForces->primaryCylinderForces[pIndex] = toInt24(this->appliedForces->zForces[pIndex] - this->appliedForces->yForces[yIndex]);
			break;
		case ForceActuatorOrientations::NA:
			this->rejectedCylinderForces->primaryCylinderForces[pIndex] = toInt24(this->appliedForces->zForces[pIndex]);
			break;
		case ForceActuatorOrientations::PositiveX:
			this->rejectedCylinderForces->primaryCylinderForces[pIndex] = toInt24(this->appliedForces->zForces[pIndex] - this->appliedForces->xForces[xIndex]);
			break;
		case ForceActuatorOrientations::NegativeX:
			this->rejectedCylinderForces->primaryCylinderForces[pIndex] = toInt24(this->appliedForces->zForces[pIndex] - -this->appliedForces->xForces[xIndex]);
			break;
		case ForceActuatorOrientations::NegativeY:
			this->rejectedCylinderForces->primaryCylinderForces[pIndex] = toInt24(this->appliedForces->zForces[pIndex] - -this->appliedForces->yForces[yIndex]);
			break;
		}
		this->forceSetpointWarning->safetyLimitWarning[pIndex] = this->forceSetpointWarning->safetyLimitWarning[pIndex] ||
			!Range::InRangeAndCoerce((int)primaryLowFault, (int)primaryHighFault, this->rejectedCylinderForces->primaryCylinderForces[pIndex], this->appliedCylinderForces->primaryCylinderForces + pIndex);

		rejectionRequired = rejectionRequired || this->forceSetpointWarning->safetyLimitWarning[pIndex];
	}
	this->appliedCylinderForces->timestamp = this->publisher->getTimestamp();
	this->rejectedCylinderForces->timestamp = this->appliedCylinderForces->timestamp;
	this->safetyController->forceControllerNotifySafetyLimit(rejectionRequired);
	if (rejectionRequired) {
		this->publisher->logRejectedCylinderForces();
	}
	this->publisher->logAppliedCylinderForces();
}

bool ForceController::checkMirrorMoments() {
	Log.Trace("ForceController: checkMirrorMoments()");
	float xMoment = this->appliedForces->mX;
	float yMoment = this->appliedForces->mY;
	float zMoment = this->appliedForces->mZ;
	this->forceSetpointWarning->xMomentWarning = !Range::InRange(this->forceActuatorSettings->MirrorXMoment * this->forceActuatorSettings->SetpointXMomentHighLimitPercentage, this->forceActuatorSettings->MirrorXMoment * this->forceActuatorSettings->SetpointXMomentLowLimitPercentage, xMoment);
	this->forceSetpointWarning->yMomentWarning = !Range::InRange(this->forceActuatorSettings->MirrorYMoment * this->forceActuatorSettings->SetpointYMomentHighLimitPercentage, this->forceActuatorSettings->MirrorYMoment * this->forceActuatorSettings->SetpointYMomentLowLimitPercentage, yMoment);
	this->forceSetpointWarning->zMomentWarning = !Range::InRange(this->forceActuatorSettings->MirrorZMoment * this->forceActuatorSettings->SetpointZMomentHighLimitPercentage, this->forceActuatorSettings->MirrorZMoment * this->forceActuatorSettings->SetpointZMomentLowLimitPercentage, zMoment);
	this->safetyController->forceControllerNotifyXMomentLimit(this->forceSetpointWarning->xMomentWarning);
	this->safetyController->forceControllerNotifyYMomentLimit(this->forceSetpointWarning->yMomentWarning);
	this->safetyController->forceControllerNotifyZMomentLimit(this->forceSetpointWarning->zMomentWarning);
	return this->forceSetpointWarning->xMomentWarning ||
			this->forceSetpointWarning->yMomentWarning ||
			this->forceSetpointWarning->zMomentWarning;
}

bool ForceController::checkNearNeighbors() {
	Log.Trace("ForceController: checkNearNeighbors()");
	float nominalZ = this->mirrorWeight / (float)FA_COUNT;
	bool warningChanged = false;
	this->forceSetpointWarning->anyNearNeighborWarning = false;
	for(int zIndex = 0; zIndex < FA_COUNT; zIndex++) {
		float nearZ = 0;
		int nearNeighbors = this->neighbors[zIndex].NearZIDs.size();
		for(int j = 0; j < nearNeighbors; ++j) {
			int neighborZIndex = this->neighbors[zIndex].NearZIDs[j];

			nearZ += this->appliedForces->zForces[neighborZIndex];
		}
		nearZ /= nearNeighbors;
		float deltaZ = 0;

		deltaZ = std::abs(this->appliedForces->zForces[zIndex] - nearZ);

		bool previousWarning = this->forceSetpointWarning->nearNeighborWarning[zIndex];
		this->forceSetpointWarning->nearNeighborWarning[zIndex] = deltaZ > (nominalZ * this->forceActuatorSettings->SetpointNearNeighborLimitPercentage);
		this->forceSetpointWarning->anyNearNeighborWarning = this->forceSetpointWarning->anyNearNeighborWarning || this->forceSetpointWarning->nearNeighborWarning[zIndex];
		warningChanged = warningChanged || (this->forceSetpointWarning->nearNeighborWarning[zIndex] != previousWarning);
	}
	this->safetyController->forceControllerNotifyNearNeighborCheck(this->forceSetpointWarning->anyNearNeighborWarning);
	return warningChanged;
}

bool ForceController::checkMirrorWeight() {
	Log.Trace("ForceController: checkMirrorWeight()");
	float x = 0;
	float y = 0;
	float z = 0;
	for (int i = 0; i < FA_COUNT; ++i) {
		if (i < FA_X_COUNT) {
			x += abs(this->appliedForces->xForces[i]);
		}
		if (i < FA_Y_COUNT) {
			y += abs(this->appliedForces->yForces[i]);
		}
		z += abs(this->appliedForces->zForces[i]);
	}
	float globalForce = x + y + z;
	bool previousWarning = this->forceSetpointWarning->magnitudeWarning;
	this->forceSetpointWarning->magnitudeWarning = globalForce > (this->mirrorWeight * this->forceActuatorSettings->SetpointMirrorWeightLimitPercentage);
	this->safetyController->forceControllerNotifyMagnitudeLimit(this->forceSetpointWarning->magnitudeWarning);
	return this->forceSetpointWarning->magnitudeWarning != previousWarning;
}

bool ForceController::checkFarNeighbors() {
	Log.Trace("ForceController: checkFarNeighbors()");
	float globalX = this->appliedForces->fX;
	float globalY = this->appliedForces->fY;
	float globalZ = this->appliedForces->fZ;
	float globalForce = sqrt(globalX * globalX + globalY * globalY + globalZ * globalZ);
	float globalAverageForce = globalForce / FA_COUNT;
	float tolerance = globalAverageForce * this->forceActuatorSettings->SetpointNearNeighborLimitPercentage;
	if (tolerance < 1) {
		tolerance = 1;
	}
	bool warningChanged = false;
	this->forceSetpointWarning->anyFarNeighborWarning = false;
	for(int zIndex = 0; zIndex < FA_COUNT; zIndex++) {
		int xIndex = this->forceActuatorApplicationSettings->ZIndexToXIndex[zIndex];
		int yIndex = this->forceActuatorApplicationSettings->ZIndexToYIndex[zIndex];

		double x = 0;
		double y = 0;
		double z = 0;
		if (xIndex != -1) {
			x = this->appliedForces->xForces[xIndex];
		}

		if (yIndex != -1) {
			y = this->appliedForces->yForces[yIndex];
		}

		z = this->appliedForces->zForces[zIndex];
		int farNeighbors = this->neighbors[zIndex].FarIDs.size();
		for(int j = 0; j < farNeighbors; ++j) {
			int neighborZIndex = this->neighbors[zIndex].FarIDs[j];
			int neighborXIndex = this->forceActuatorApplicationSettings->ZIndexToXIndex[neighborZIndex];
			int neighborYIndex = this->forceActuatorApplicationSettings->ZIndexToYIndex[neighborZIndex];

			if (neighborXIndex != -1) {
				x += this->appliedForces->xForces[neighborXIndex];
			}

			if (neighborYIndex != -1) {
				y += this->appliedForces->yForces[neighborYIndex];
			}

			z += this->appliedForces->zForces[neighborZIndex];
		}
		float magnitude = sqrt(x * x + y * y + z * z);
		float magnitudeAverage = magnitude / (farNeighbors + 1.0);
		bool previousWarning = this->forceSetpointWarning->farNeighborWarning[zIndex];
		this->forceSetpointWarning->farNeighborWarning[zIndex] = !Range::InRange(-tolerance, tolerance, magnitudeAverage - globalAverageForce);
		this->forceSetpointWarning->anyFarNeighborWarning = this->forceSetpointWarning->anyFarNeighborWarning || this->forceSetpointWarning->farNeighborWarning[zIndex];
		warningChanged = warningChanged || (this->forceSetpointWarning->farNeighborWarning[zIndex] != previousWarning);
	}
	this->safetyController->forceControllerNotifyFarNeighborCheck(this->forceSetpointWarning->anyFarNeighborWarning);
	return warningChanged;
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
