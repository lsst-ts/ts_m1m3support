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

ForceController::ForceController(ForceActuatorApplicationSettings* forceActuatorApplicationSettings, ForceActuatorSettings* forceActuatorSettings, PIDSettings* pidSettings, M1M3SSPublisher* publisher, SafetyController* safetyController)
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

	memset(this->forceActuatorData, 0, sizeof(m1m3_ForceActuatorDataC));
	memset(&this->tmaAzimuthData, 0, sizeof(MTMount_AzC));
	memset(&this->tmaElevationData, 0, sizeof(MTMount_AltC));

	memset(this->appliedForces, 0, sizeof(m1m3_logevent_AppliedForcesC));
	memset(this->forceActuatorState, 0, sizeof(m1m3_logevent_ForceActuatorStateC));
	memset(this->forceSetpointWarning, 0, sizeof(m1m3_logevent_ForceSetpointWarningC));
	memset(this->rejectedCylinderForces, 0, sizeof(m1m3_logevent_RejectedCylinderForcesC));
	memset(this->rejectedForces, 0, sizeof(m1m3_logevent_RejectedForcesC));

	double timestamp = this->publisher->getTimestamp();
	this->forceActuatorState->Timestamp = timestamp;
	this->forceSetpointWarning->Timestamp = timestamp;

	this->publisher->logForceActuatorState();
	this->publisher->logForceSetpointWarning();

	for(int i = 0; i < FA_COUNT; i++) {
		this->zero[i] = 0;
		ForceActuatorNeighbors neighbors;
		for(unsigned int j = 0; j < this->forceActuatorSettings->Neighbors[i].NearIDs.size(); ++j) {
			int32_t id = this->forceActuatorSettings->Neighbors[i].NearIDs[j];
			for(unsigned int k = 0; k < this->forceActuatorApplicationSettings->Table.size(); ++k) {
				if (this->forceActuatorApplicationSettings->Table[k].ActuatorID == id) {
					neighbors.NearIDs.push_back(k);
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

void ForceController::updateTMAElevationData(MTMount_AltC* tmaElevationData) {
	Log.Trace("ForceController: updateTMAElevationData()");
	memcpy(&this->tmaElevationData, tmaElevationData, sizeof(MTMount_AltC));
}

void ForceController::incSupportPercentage() {
	Log.Trace("ForceController: incSupportPercentage()");
	this->forceActuatorState->SupportPercentage += this->forceActuatorSettings->RaiseIncrementPercentage;
	if (this->supportPercentageFilled()) {
		this->forceActuatorState->SupportPercentage = 1.0;
	}
}

void ForceController::decSupportPercentage() {
	Log.Trace("ForceController: decSupportPercentage()");
	this->forceActuatorState->SupportPercentage -= this->forceActuatorSettings->LowerDecrementPercentage;
	if (this->supportPercentageZeroed()) {
		this->forceActuatorState->SupportPercentage = 0.0;
	}
}

void ForceController::zeroSupportPercentage() {
	Log.Info("ForceController: zeroSupportPercentage()");
	this->forceActuatorState->SupportPercentage = 0.0;
}

void ForceController::fillSupportPercentage() {
	Log.Info("ForceController: fillSupportPercentage()");
	this->forceActuatorState->SupportPercentage = 1.0;
}

bool ForceController::supportPercentageFilled() {
	return this->forceActuatorState->SupportPercentage >= 1.0;
}

bool ForceController::supportPercentageZeroed() {
	return this->forceActuatorState->SupportPercentage <= 0.0;
}

void ForceController::updateAppliedForces() {
	Log.Trace("ForceController: updateAppliedForces()");

	this->aberrationForceComponent.update();
	this->accelerationForceComponent.update();
	this->activeOpticForceComponent.update();
	this->azimuthForceComponent.update();
	this->balanceForceComponent.update();
	this->offsetForceComponent.update();
	this->staticForceComponent.update();
	this->thermalForceComponent.update();
	this->velocityForceComponent.update();
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
	this->aberrationForceComponent.applyAberrationForcesByBendingModes(coefficients);
}

void ForceController::applyAberrationForces(float* z) {
	Log.Info("ForceController: applyAberrationForces()");
	this->aberrationForceComponent.applyAberrationForces(z);
}

void ForceController::zeroAberrationForces() {
	Log.Info("ForceController: zeroAberrationForces()");
	this->aberrationForceComponent.disable();
}

void ForceController::applyAccelerationForces() {
	Log.Info("ForceController: applyAccelerationForces()");
	this->accelerationForceComponent.enable();
}

void ForceController::updateAccelerationForces() {
	Log.Trace("ForceController: updateAccelerationForces()");
	this->accelerationForceComponent.applyAccelerationForcesByAngularAccelerations(this->accelerometerData->AngularAccelerationX, this->accelerometerData->AngularAccelerationY, this->accelerometerData->AngularAccelerationZ);
}

void ForceController::zeroAccelerationForces() {
	Log.Info("ForceController: zeroAccelerationForces()");
	this->accelerationForceComponent.disable();
}

void ForceController::applyActiveOpticForcesByBendingModes(float* coefficients) {
	Log.Info("ForceController: applyActiveOpticForcesByBendingModes()");
	this->activeOpticForceComponent.applyActiveOpticForcesByBendingModes(coefficients);
}

void ForceController::applyActiveOpticForces(float* z) {
	Log.Info("ForceController: applyActiveOpticForces()");
	this->activeOpticForceComponent.applyActiveOpticForces(z);
}

void ForceController::zeroActiveOpticForces() {
	Log.Info("ForceController: zeroActiveOpticForces()");
	this->activeOpticForceComponent.disable();
}

void ForceController::applyAzimuthForces() {
	Log.Info("ForceController: applyAzimuthForces()");
	this->azimuthForceComponent.enable();
}

void ForceController::updateAzimuthForces(float azimuthAngle) {
	Log.Trace("ForceController: updateAzimuthForces()");
	this->azimuthForceComponent.applyAzimuthForcesByAzimuthAngle(azimuthAngle);
}

void ForceController::zeroAzimuthForces() {
	Log.Info("ForceController: zeroAzimuthForces()");
	this->azimuthForceComponent.disable();
}

void ForceController::applyBalanceForces() {
	Log.Info("ForceController: applyBalanceForces()");
	this->balanceForceComponent.enable();
}

void ForceController::updateBalanceForces() {
	Log.Trace("ForceController: updateBalanceForces()");
	this->balanceForceComponent.applyBalanceForcesByMirrorForces(this->hardpointActuatorData->Fx, this->hardpointActuatorData->Fy, this->hardpointActuatorData->Fz, this->hardpointActuatorData->Mx, this->hardpointActuatorData->My, this->hardpointActuatorData->Mz);
}

void ForceController::zeroBalanceForces() {
	Log.Info("ForceController: zeroBalanceForces()");
	this->balanceForceComponent.disable();
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
	this->elevationForceComponent.enable();
}

void ForceController::updateElevationForces() {
	Log.Trace("ForceController: updateElevationForces()");
	double elevationAngle = this->forceActuatorSettings->UseInclinometer ? this->inclinometerData->InclinometerAngle : this->tmaElevationData.Angle_Actual;
	// Convert elevation angle to zenith angle (used be matrix)
	elevationAngle = 90.0 - elevationAngle;
	this->elevationForceComponent.applyElevationForcesByElevationAngle(elevationAngle);
}

void ForceController::zeroElevationForces() {
	Log.Info("ForceController: zeroElevationForces()");
	this->elevationForceComponent.disable();
}

void ForceController::applyOffsetForces(float* x, float* y, float* z) {
	Log.Info("ForceController: applyOffsetForces()");
	this->offsetForceComponent.applyOffsetForces(x, y, z);
}

void ForceController::applyOffsetForcesByMirrorForces(float xForce, float yForce, float zForce, float xMoment, float yMoment, float zMoment) {
	Log.Info("ForceController: applyOffsetForcesByMirrorForces(%0.1f, %0.1f, %0.1f, %0.1f, %0.1f, %0.1f)", xForce, yForce, zForce, xMoment, yMoment, zMoment);
	this->offsetForceComponent.applyOffsetForcesByMirrorForces(xForce, yForce, zForce, xMoment, yMoment, zMoment);
}

void ForceController::zeroOffsetForces() {
	Log.Info("ForceController: zeroOffsetForces()");
	this->offsetForceComponent.disable();
}

void ForceController::applyStaticForces() {
	Log.Info("ForceController: applyStaticForces()");
	this->staticForceComponent.applyStaticForces(&this->forceActuatorSettings->StaticXTable, &this->forceActuatorSettings->StaticYTable, &this->forceActuatorSettings->StaticZTable);
}

void ForceController::zeroStaticForces() {
	Log.Info("ForceController: zeroStaticForces()");
	this->staticForceComponent.disable();
}

void ForceController::applyThermalForces() {
	Log.Info("ForceController: applyThermalForces()");
	this->thermalForceComponent.enable();
}

void ForceController::updateThermalForces() {
	Log.Trace("ForceController: updateThermalForces()");
	float temperature = 0; // TODO: Update
	this->thermalForceComponent.applyThermalForcesByMirrorTemperature(temperature);
}

void ForceController::zeroThermalForces() {
	Log.Info("ForceController: zeroThermalForces()");
	this->thermalForceComponent.disable();
}

void ForceController::applyVelocityForces() {
	Log.Info("ForceController: applyVelocityForces()");
	this->velocityForceComponent.enable();
}

void ForceController::updateVelocityForces() {
	Log.Trace("ForceController: updateVelocityForces()");
	this->velocityForceComponent.applyVelocityForcesByAngularVelocity(this->gyroData->AngularVelocityX, this->gyroData->AngularVelocityY, this->gyroData->AngularVelocityZ);
}

void ForceController::zeroVelocityForces() {
	Log.Info("ForceController: zeroVelocityForces()");
	this->velocityForceComponent.disable();
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

		this->forceSetpointWarning->SafetyLimitWarning[pIndex] = false;

		if (sIndex != -1) {
			float secondaryLowFault = this->forceActuatorSettings->CylinderLimitSecondaryTable[sIndex].LowFault;
			float secondaryHighFault = this->forceActuatorSettings->CylinderLimitSecondaryTable[sIndex].HighFault;
			switch(this->forceActuatorInfo->ActuatorOrientation[pIndex]) {
			case ForceActuatorOrientations::PositiveY:
				this->rejectedCylinderForces->SecondaryCylinderForces[sIndex] = toInt24(this->appliedForces->YForces[yIndex] * sqrt2);
				break;
			case ForceActuatorOrientations::PositiveX:
				this->rejectedCylinderForces->SecondaryCylinderForces[sIndex] = toInt24(this->appliedForces->XForces[xIndex] * sqrt2);
				break;
			case ForceActuatorOrientations::NegativeX:
				this->rejectedCylinderForces->SecondaryCylinderForces[sIndex] = toInt24(-this->appliedForces->XForces[xIndex] * sqrt2);
				break;
			case ForceActuatorOrientations::NegativeY:
				this->rejectedCylinderForces->SecondaryCylinderForces[sIndex] = toInt24(-this->appliedForces->YForces[yIndex] * sqrt2);
				break;
			}
			this->forceSetpointWarning->SafetyLimitWarning[pIndex] = this->forceSetpointWarning->SafetyLimitWarning[pIndex] ||
				!Range::InRangeAndCoerce((int)secondaryLowFault, (int)secondaryHighFault, this->rejectedCylinderForces->SecondaryCylinderForces[sIndex], this->appliedCylinderForces->SecondaryCylinderForces + sIndex);
		}

		float primaryLowFault = this->forceActuatorSettings->CylinderLimitPrimaryTable[pIndex].LowFault;
		float primaryHighFault = this->forceActuatorSettings->CylinderLimitPrimaryTable[pIndex].HighFault;
		switch(this->forceActuatorInfo->ActuatorOrientation[pIndex]) {
		case ForceActuatorOrientations::PositiveY:
			this->rejectedCylinderForces->PrimaryCylinderForces[pIndex] = toInt24(this->appliedForces->ZForces[pIndex] - this->appliedForces->YForces[yIndex]);
			break;
		case ForceActuatorOrientations::NA:
			this->rejectedCylinderForces->PrimaryCylinderForces[pIndex] = toInt24(this->appliedForces->ZForces[pIndex]);
			break;
		case ForceActuatorOrientations::PositiveX:
			this->rejectedCylinderForces->PrimaryCylinderForces[pIndex] = toInt24(this->appliedForces->ZForces[pIndex] - this->appliedForces->XForces[xIndex]);
			break;
		case ForceActuatorOrientations::NegativeX:
			this->rejectedCylinderForces->PrimaryCylinderForces[pIndex] = toInt24(this->appliedForces->ZForces[pIndex] - -this->appliedForces->XForces[xIndex]);
			break;
		case ForceActuatorOrientations::NegativeY:
			this->rejectedCylinderForces->PrimaryCylinderForces[pIndex] = toInt24(this->appliedForces->ZForces[pIndex] - -this->appliedForces->YForces[yIndex]);
			break;
		}
		this->forceSetpointWarning->SafetyLimitWarning[pIndex] = this->forceSetpointWarning->SafetyLimitWarning[pIndex] ||
			!Range::InRangeAndCoerce((int)primaryLowFault, (int)primaryHighFault, this->rejectedCylinderForces->PrimaryCylinderForces[pIndex], this->appliedCylinderForces->PrimaryCylinderForces + pIndex);

		rejectionRequired = rejectionRequired || this->forceSetpointWarning->SafetyLimitWarning[pIndex];
	}
	this->appliedCylinderForces->Timestamp = this->publisher->getTimestamp();
	this->rejectedCylinderForces->Timestamp = this->appliedCylinderForces->Timestamp;
	this->safetyController->forceControllerNotifySafetyLimit(rejectionRequired);
	if (rejectionRequired) {
		this->publisher->logRejectedCylinderForces();
	}
	this->publisher->logAppliedCylinderForces();
}

bool ForceController::checkMirrorMoments() {
	Log.Trace("ForceController: checkMirrorMoments()");
	float xMoment = this->appliedForces->Mx;
	float yMoment = this->appliedForces->My;
	float zMoment = this->appliedForces->Mz;
	this->forceSetpointWarning->XMomentWarning = !Range::InRange(this->forceActuatorSettings->MirrorXMoment * this->forceActuatorSettings->SetpointXMomentHighLimitPercentage, this->forceActuatorSettings->MirrorXMoment * this->forceActuatorSettings->SetpointXMomentLowLimitPercentage, xMoment);
	this->forceSetpointWarning->YMomentWarning = !Range::InRange(this->forceActuatorSettings->MirrorYMoment * this->forceActuatorSettings->SetpointYMomentHighLimitPercentage, this->forceActuatorSettings->MirrorYMoment * this->forceActuatorSettings->SetpointYMomentLowLimitPercentage, yMoment);
	this->forceSetpointWarning->ZMomentWarning = !Range::InRange(this->forceActuatorSettings->MirrorZMoment * this->forceActuatorSettings->SetpointZMomentHighLimitPercentage, this->forceActuatorSettings->MirrorZMoment * this->forceActuatorSettings->SetpointZMomentLowLimitPercentage, zMoment);
	this->safetyController->forceControllerNotifyXMomentLimit(this->forceSetpointWarning->XMomentWarning);
	this->safetyController->forceControllerNotifyYMomentLimit(this->forceSetpointWarning->YMomentWarning);
	this->safetyController->forceControllerNotifyZMomentLimit(this->forceSetpointWarning->ZMomentWarning);
	return this->forceSetpointWarning->XMomentWarning ||
			this->forceSetpointWarning->YMomentWarning ||
			this->forceSetpointWarning->ZMomentWarning;
}

bool ForceController::checkNearNeighbors() {
	Log.Trace("ForceController: checkNearNeighbors()");
	double globalX = 0;
	double globalY = 0;
	double globalZ = 0;
	for(int zIndex = 0; zIndex < FA_COUNT; zIndex++) {
		int xIndex = this->forceActuatorApplicationSettings->ZIndexToXIndex[zIndex];
		int yIndex = this->forceActuatorApplicationSettings->ZIndexToYIndex[zIndex];

		if (xIndex != -1) {
			globalX += this->appliedForces->XForces[xIndex];
		}

		if (yIndex != -1) {
			globalY += this->appliedForces->YForces[yIndex];
		}

		globalZ += this->appliedForces->ZForces[zIndex];
	}
	double globalForce = sqrt(globalX * globalX + globalY * globalY + globalZ * globalZ);
	double globalAverageForce = globalForce / FA_COUNT;
	bool warningChanged = false;
	this->forceSetpointWarning->AnyNearNeighborWarning = false;
	for(int zIndex = 0; zIndex < FA_COUNT; zIndex++) {
		int xIndex = this->forceActuatorApplicationSettings->ZIndexToXIndex[zIndex];
		int yIndex = this->forceActuatorApplicationSettings->ZIndexToYIndex[zIndex];

		double nearX = 0;
		double nearY = 0;
		double nearZ = 0;
		int nearNeighbors = this->neighbors[zIndex].NearIDs.size();
		for(int j = 0; j < nearNeighbors; ++j) {
			int neighborZIndex = this->neighbors[zIndex].NearIDs[j];
			int neighborXIndex = this->forceActuatorApplicationSettings->ZIndexToXIndex[neighborZIndex];
			int neighborYIndex = this->forceActuatorApplicationSettings->ZIndexToYIndex[neighborZIndex];

			if (neighborXIndex != -1) {
				nearX += this->appliedForces->XForces[neighborXIndex];
			}

			if (neighborYIndex != -1) {
				nearY += this->appliedForces->YForces[neighborYIndex];
			}

			nearZ += this->appliedForces->ZForces[neighborZIndex];
		}
		nearX /= nearNeighbors;
		nearY /= nearNeighbors;
		nearZ /= nearNeighbors;
		double x = 0;
		double y = 0;
		double z = 0;

		if (xIndex != -1) {
			x = this->appliedForces->XForces[xIndex] - nearX;
		}

		if (yIndex != -1) {
			y = this->appliedForces->YForces[yIndex] - nearY;
		}

		z = this->appliedForces->ZForces[zIndex] - nearZ;

		double magnitudeDifference = sqrt(x * x + y * y + z * z);
		bool previousWarning = this->forceSetpointWarning->NearNeighborWarning[zIndex];
		this->forceSetpointWarning->NearNeighborWarning[zIndex] = magnitudeDifference > (globalAverageForce * this->forceActuatorSettings->SetpointNearNeighborLimitPercentage);
		this->forceSetpointWarning->AnyNearNeighborWarning = this->forceSetpointWarning->AnyNearNeighborWarning || this->forceSetpointWarning->NearNeighborWarning[zIndex];
		warningChanged = warningChanged || (this->forceSetpointWarning->NearNeighborWarning[zIndex] != previousWarning);
	}
	this->safetyController->forceControllerNotifyNearNeighborCheck(this->forceSetpointWarning->AnyNearNeighborWarning);
	return warningChanged;
}

bool ForceController::checkMirrorWeight() {
	Log.Trace("ForceController: checkMirrorWeight()");
	float x = 0;
	float y = 0;
	float z = 0;
	for (int i = 0; i < FA_COUNT; ++i) {
		if (i < FA_X_COUNT) {
			x += abs(this->appliedForces->XForces[i]);
		}
		if (i < FA_Y_COUNT) {
			y += abs(this->appliedForces->YForces[i]);
		}
		z += abs(this->appliedForces->ZForces[i]);
	}
	float globalForce = x + y + z;
	bool previousWarning = this->forceSetpointWarning->MagnitudeWarning;
	this->forceSetpointWarning->MagnitudeWarning = globalForce > (this->forceActuatorSettings->MirrorWeight * this->forceActuatorSettings->SetpointMirrorWeightLimitPercentage);
	this->safetyController->forceControllerNotifyMagnitudeLimit(this->forceSetpointWarning->MagnitudeWarning);
	return this->forceSetpointWarning->MagnitudeWarning != previousWarning;
}

bool ForceController::checkFarNeighbors() {
	Log.Trace("ForceController: checkFarNeighbors()");
	float globalX = this->appliedForces->Fx;
	float globalY = this->appliedForces->Fy;
	float globalZ = this->appliedForces->Fz;
	float globalForce = sqrt(globalX * globalX + globalY * globalY + globalZ * globalZ);
	float globalAverageForce = globalForce / FA_COUNT;
	float tolerance = globalAverageForce * this->forceActuatorSettings->SetpointNearNeighborLimitPercentage;
	if (tolerance < 1) {
		tolerance = 1;
	}
	bool warningChanged = false;
	this->forceSetpointWarning->AnyFarNeighborWarning = false;
	for(int zIndex = 0; zIndex < FA_COUNT; zIndex++) {
		int xIndex = this->forceActuatorApplicationSettings->ZIndexToXIndex[zIndex];
		int yIndex = this->forceActuatorApplicationSettings->ZIndexToYIndex[zIndex];

		double x = 0;
		double y = 0;
		double z = 0;
		if (xIndex != -1) {
			x = this->appliedForces->XForces[xIndex];
		}

		if (yIndex != -1) {
			y = this->appliedForces->YForces[yIndex];
		}

		z = this->appliedForces->ZForces[zIndex];
		int farNeighbors = this->neighbors[zIndex].FarIDs.size();
		for(int j = 0; j < farNeighbors; ++j) {
			int neighborZIndex = this->neighbors[zIndex].FarIDs[j];
			int neighborXIndex = this->forceActuatorApplicationSettings->ZIndexToXIndex[neighborZIndex];
			int neighborYIndex = this->forceActuatorApplicationSettings->ZIndexToYIndex[neighborZIndex];

			if (neighborXIndex != -1) {
				x += this->appliedForces->XForces[neighborXIndex];
			}

			if (neighborYIndex != -1) {
				y += this->appliedForces->YForces[neighborYIndex];
			}

			z += this->appliedForces->ZForces[neighborZIndex];
		}
		float magnitude = sqrt(x * x + y * y + z * z);
		float magnitudeAverage = magnitude / (farNeighbors + 1.0);
		bool previousWarning = this->forceSetpointWarning->FarNeighborWarning[zIndex];
		this->forceSetpointWarning->FarNeighborWarning[zIndex] = !Range::InRange(-tolerance, tolerance, magnitudeAverage - globalAverageForce);
		this->forceSetpointWarning->AnyFarNeighborWarning = this->forceSetpointWarning->AnyFarNeighborWarning || this->forceSetpointWarning->FarNeighborWarning[zIndex];
		warningChanged = warningChanged || (this->forceSetpointWarning->FarNeighborWarning[zIndex] != previousWarning);
	}
	this->safetyController->forceControllerNotifyFarNeighborCheck(this->forceSetpointWarning->AnyFarNeighborWarning);
	return warningChanged;
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
