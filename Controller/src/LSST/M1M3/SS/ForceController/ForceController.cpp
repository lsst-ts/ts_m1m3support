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
 : fx(0, pidSettings->Fx, publisher),
   fy(1, pidSettings->Fy, publisher),
   fz(2, pidSettings->Fz, publisher),
   mx(3, pidSettings->Mx, publisher),
   my(4, pidSettings->My, publisher),
   mz(5, pidSettings->Mz, publisher) {
	Log.Debug("ForceController: ForceController()");
	this->forceActuatorApplicationSettings = forceActuatorApplicationSettings;
	this->forceActuatorSettings = forceActuatorSettings;
	this->publisher = publisher;
	this->safetyController = safetyController;
	this->pidSettings = pidSettings;

	this->appliedAberrationForces = this->publisher->getEventAppliedAberrationForces();
	this->appliedAccelerationForces = this->publisher->getEventAppliedAccelerationForces();
	this->appliedActiveOpticForces = this->publisher->getEventAppliedActiveOpticForces();
	this->appliedAzimuthForces = this->publisher->getEventAppliedAzimuthForces();
	this->appliedBalanceForces = this->publisher->getEventAppliedBalanceForces();
	this->appliedCylinderForces = this->publisher->getEventAppliedCylinderForces();
	this->appliedElevationForces = this->publisher->getEventAppliedElevationForces();
	this->appliedForces = this->publisher->getEventAppliedForces();
	this->appliedOffsetForces = this->publisher->getEventAppliedOffsetForces();
	this->appliedStaticForces = this->publisher->getEventAppliedStaticForces();
	this->appliedThermalForces = this->publisher->getEventAppliedThermalForces();
	this->appliedVelocityForces = this->publisher->getEventAppliedVelocityForces();
	this->forceActuatorState = this->publisher->getEventForceActuatorState();
	this->forceSetpointWarning = this->publisher->getEventForceSetpointWarning();
	this->rejectedAberrationForces = this->publisher->getEventRejectedAberrationForces();
	this->rejectedAccelerationForces = this->publisher->getEventRejectedAccelerationForces();
	this->rejectedActiveOpticForces = this->publisher->getEventRejectedActiveOpticForces();
	this->rejectedAzimuthForces = this->publisher->getEventRejectedAzimuthForces();
	this->rejectedBalanceForces = this->publisher->getEventRejectedBalanceForces();
	this->rejectedCylinderForces = this->publisher->getEventRejectedCylinderForces();
	this->rejectedElevationForces = this->publisher->getEventRejectedElevationForces();
	this->rejectedForces = this->publisher->getEventRejectedForces();
	this->rejectedOffsetForces = this->publisher->getEventRejectedOffsetForces();
	this->rejectedStaticForces = this->publisher->getEventRejectedStaticForces();
	this->rejectedThermalForces = this->publisher->getEventRejectedThermalForces();
	this->rejectedVelocityForces = this->publisher->getEventRejectedVelocityForces();

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

	memset(this->appliedAberrationForces, 0, sizeof(m1m3_logevent_AppliedAberrationForcesC));
	memset(this->appliedAccelerationForces, 0, sizeof(m1m3_logevent_AppliedAccelerationForcesC));
	memset(this->appliedActiveOpticForces, 0, sizeof(m1m3_logevent_AppliedActiveOpticForcesC));
	memset(this->appliedAzimuthForces, 0, sizeof(m1m3_logevent_AppliedAzimuthForcesC));
	memset(this->appliedBalanceForces, 0, sizeof(m1m3_logevent_AppliedBalanceForcesC));
	memset(this->appliedElevationForces, 0, sizeof(m1m3_logevent_AppliedElevationForcesC));
	memset(this->appliedForces, 0, sizeof(m1m3_logevent_AppliedForcesC));
	memset(this->appliedOffsetForces, 0, sizeof(m1m3_logevent_AppliedOffsetForcesC));
	memset(this->appliedStaticForces, 0, sizeof(m1m3_logevent_AppliedStaticForcesC));
	memset(this->appliedThermalForces, 0, sizeof(m1m3_logevent_AppliedThermalForcesC));
	memset(this->appliedVelocityForces, 0, sizeof(m1m3_logevent_AppliedVelocityForcesC));
	memset(this->forceActuatorState, 0, sizeof(m1m3_logevent_ForceActuatorStateC));
	memset(this->forceSetpointWarning, 0, sizeof(m1m3_logevent_ForceSetpointWarningC));
	memset(this->rejectedAberrationForces, 0, sizeof(m1m3_logevent_RejectedAberrationForcesC));
	memset(this->rejectedAccelerationForces, 0, sizeof(m1m3_logevent_RejectedAccelerationForcesC));
	memset(this->rejectedActiveOpticForces, 0, sizeof(m1m3_logevent_RejectedActiveOpticForcesC));
	memset(this->rejectedAzimuthForces, 0, sizeof(m1m3_logevent_RejectedAzimuthForcesC));
	memset(this->rejectedBalanceForces, 0, sizeof(m1m3_logevent_RejectedBalanceForcesC));
	memset(this->rejectedCylinderForces, 0, sizeof(m1m3_logevent_RejectedCylinderForcesC));
	memset(this->rejectedElevationForces, 0, sizeof(m1m3_logevent_RejectedElevationForcesC));
	memset(this->rejectedForces, 0, sizeof(m1m3_logevent_RejectedForcesC));
	memset(this->rejectedOffsetForces, 0, sizeof(m1m3_logevent_RejectedOffsetForcesC));
	memset(this->rejectedStaticForces, 0, sizeof(m1m3_logevent_RejectedStaticForcesC));
	memset(this->rejectedThermalForces, 0, sizeof(m1m3_logevent_RejectedThermalForcesC));
	memset(this->rejectedVelocityForces, 0, sizeof(m1m3_logevent_RejectedVelocityForcesC));

	double timestamp = this->publisher->getTimestamp();
	this->appliedAberrationForces->Timestamp = timestamp;
	this->appliedAccelerationForces->Timestamp = timestamp;
	this->appliedActiveOpticForces->Timestamp = timestamp;
	this->appliedAzimuthForces->Timestamp = timestamp;
	this->appliedBalanceForces->Timestamp = timestamp;
	this->appliedElevationForces->Timestamp = timestamp;
	this->appliedForces->Timestamp = timestamp;
	this->appliedOffsetForces->Timestamp = timestamp;
	this->appliedStaticForces->Timestamp = timestamp;
	this->appliedThermalForces->Timestamp = timestamp;
	this->appliedVelocityForces->Timestamp = timestamp;
	this->forceActuatorState->Timestamp = timestamp;
	this->forceSetpointWarning->Timestamp = timestamp;
	this->rejectedAberrationForces->Timestamp = timestamp;
	this->rejectedAccelerationForces->Timestamp = timestamp;
	this->rejectedActiveOpticForces->Timestamp = timestamp;
	this->rejectedAzimuthForces->Timestamp = timestamp;
	this->rejectedBalanceForces->Timestamp = timestamp;
	this->rejectedCylinderForces->Timestamp = timestamp;
	this->rejectedElevationForces->Timestamp = timestamp;
	this->rejectedForces->Timestamp = timestamp;
	this->rejectedOffsetForces->Timestamp = timestamp;
	this->rejectedStaticForces->Timestamp = timestamp;
	this->rejectedThermalForces->Timestamp = timestamp;
	this->rejectedVelocityForces->Timestamp = timestamp;

	this->publisher->logAppliedAberrationForces();
	this->publisher->logAppliedAccelerationForces();
	this->publisher->logAppliedActiveOpticForces();
	this->publisher->logAppliedAzimuthForces();
	this->publisher->logAppliedBalanceForces();
	this->publisher->logAppliedElevationForces();
	this->publisher->logAppliedForces();
	this->publisher->logAppliedOffsetForces();
	this->publisher->logAppliedStaticForces();
	this->publisher->logAppliedThermalForces();
	this->publisher->logAppliedVelocityForces();
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

	if (this->forceActuatorState->AccelerationForcesApplied) {
		this->updateAccelerationForces();
	}
	if (this->forceActuatorState->BalanceForcesApplied) {
		this->updateBalanceForces();
	}
	if (this->forceActuatorState->ElevationForcesApplied) {
		this->updateElevationForces();
	}
	if (this->forceActuatorState->ThermalForcesApplied) {
		this->updateThermalForces();
	}
	if (this->forceActuatorState->VelocityForcesApplied) {
		this->updateVelocityForces();
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
	float zForces[FA_COUNT];
	for(int zIndex = 0; zIndex < FA_COUNT; ++zIndex) {
		int mIndex = zIndex * 22;
		zForces[zIndex] =
				this->forceActuatorSettings->BendingModeTable[mIndex + 0] * coefficients[0] +
				this->forceActuatorSettings->BendingModeTable[mIndex + 1] * coefficients[1] +
				this->forceActuatorSettings->BendingModeTable[mIndex + 2] * coefficients[2] +
				this->forceActuatorSettings->BendingModeTable[mIndex + 3] * coefficients[3] +
				this->forceActuatorSettings->BendingModeTable[mIndex + 4] * coefficients[4] +
				this->forceActuatorSettings->BendingModeTable[mIndex + 5] * coefficients[5] +
				this->forceActuatorSettings->BendingModeTable[mIndex + 6] * coefficients[6] +
				this->forceActuatorSettings->BendingModeTable[mIndex + 7] * coefficients[7] +
				this->forceActuatorSettings->BendingModeTable[mIndex + 8] * coefficients[8] +
				this->forceActuatorSettings->BendingModeTable[mIndex + 9] * coefficients[9] +
				this->forceActuatorSettings->BendingModeTable[mIndex + 10] * coefficients[10] +
				this->forceActuatorSettings->BendingModeTable[mIndex + 11] * coefficients[11] +
				this->forceActuatorSettings->BendingModeTable[mIndex + 12] * coefficients[12] +
				this->forceActuatorSettings->BendingModeTable[mIndex + 13] * coefficients[13] +
				this->forceActuatorSettings->BendingModeTable[mIndex + 14] * coefficients[14] +
				this->forceActuatorSettings->BendingModeTable[mIndex + 15] * coefficients[15] +
				this->forceActuatorSettings->BendingModeTable[mIndex + 16] * coefficients[16] +
				this->forceActuatorSettings->BendingModeTable[mIndex + 17] * coefficients[17] +
				this->forceActuatorSettings->BendingModeTable[mIndex + 18] * coefficients[18] +
				this->forceActuatorSettings->BendingModeTable[mIndex + 19] * coefficients[19] +
				this->forceActuatorSettings->BendingModeTable[mIndex + 20] * coefficients[20] +
				this->forceActuatorSettings->BendingModeTable[mIndex + 21] * coefficients[21];
	}
	this->applyAberrationForces(zForces);
}

void ForceController::applyAberrationForces(float* z) {
	Log.Info("ForceController: applyAberrationForces()");
	float netAberrationForce = 0;
	bool rejectionRequired = false;
	this->forceActuatorState->AberrationForcesApplied = true;
	this->forceActuatorState->Timestamp = this->publisher->getTimestamp();
	this->appliedAberrationForces->Timestamp = this->forceActuatorState->Timestamp;
	this->rejectedAberrationForces->Timestamp = this->forceActuatorState->Timestamp;
	for(int zIndex = 0; zIndex < FA_COUNT; ++zIndex) {
		float zLowFault = this->forceActuatorSettings->AberrationLimitZTable[zIndex].LowFault;
		float zHighFault = this->forceActuatorSettings->AberrationLimitZTable[zIndex].HighFault;

		this->forceSetpointWarning->AberrationForceWarning[zIndex] = false;

		this->rejectedAberrationForces->ZForces[zIndex] = z[zIndex];
		this->forceSetpointWarning->AberrationForceWarning[zIndex] = this->forceSetpointWarning->AberrationForceWarning[zIndex] ||
			!Range::InRangeAndCoerce(zLowFault, zHighFault, this->rejectedAberrationForces->ZForces[zIndex], this->appliedAberrationForces->ZForces + zIndex);

		netAberrationForce += this->rejectedAberrationForces->ZForces[zIndex];
		rejectionRequired = rejectionRequired || this->forceSetpointWarning->AberrationForceWarning[zIndex];
	}
	this->forceSetpointWarning->AberrationNetForceWarning = !Range::InRange(-this->forceActuatorSettings->NetAberrationForceTolerance, this->forceActuatorSettings->NetAberrationForceTolerance, netAberrationForce);
	this->setAppliedAberrationForcesAndMoments();
	this->setRejectedAberrationForcesAndMoments();
	this->safetyController->forceControllerNotifyAberrationForceClipping(rejectionRequired);
	this->safetyController->forceControllerNotifyAberrationNetForceCheck(this->forceSetpointWarning->AberrationNetForceWarning);
	this->publisher->tryLogForceSetpointWarning();
	if (rejectionRequired) {
		this->publisher->logRejectedAberrationForces();
	}
	this->publisher->logAppliedAberrationForces();
	this->publisher->tryLogForceActuatorState();
}

void ForceController::zeroAberrationForces() {
	Log.Info("ForceController: zeroAberrationForces()");
	this->forceActuatorState->AberrationForcesApplied = false;
	this->forceActuatorState->Timestamp = this->publisher->getTimestamp();
	this->appliedAberrationForces->Timestamp = this->forceActuatorState->Timestamp;
	for(int zIndex = 0; zIndex < FA_COUNT; ++zIndex) {
		this->appliedAberrationForces->ZForces[zIndex] = 0;
		this->forceSetpointWarning->AberrationForceWarning[zIndex] = false;
	}
	this->forceSetpointWarning->AberrationNetForceWarning = false;
	this->setAppliedAberrationForcesAndMoments();
	this->setRejectedAberrationForcesAndMoments();
	this->safetyController->forceControllerNotifyAberrationForceClipping(false);
	this->safetyController->forceControllerNotifyAberrationNetForceCheck(false);
	this->publisher->tryLogForceSetpointWarning();
	this->publisher->tryLogAppliedAberrationForces();
	this->publisher->tryLogForceActuatorState();
}

void ForceController::applyAccelerationForces() {
	Log.Info("ForceController: applyAccelerationForces()");
	this->forceActuatorState->Timestamp = this->publisher->getTimestamp();
	this->forceActuatorState->AccelerationForcesApplied = true;
	this->publisher->tryLogForceActuatorState();
}

void ForceController::updateAccelerationForces() {
	Log.Trace("ForceController: updateAccelerationForces()");
	float angularAccelerationX = this->accelerometerData->AngularAccelerationX;
	float angularAccelerationY = this->accelerometerData->AngularAccelerationY;
	float angularAccelerationZ = this->accelerometerData->AngularAccelerationZ;
	bool rejectionRequired = false;
	for(int zIndex = 0; zIndex < FA_COUNT; ++zIndex) {
		int xIndex = this->forceActuatorApplicationSettings->ZIndexToXIndex[zIndex];
		int yIndex = this->forceActuatorApplicationSettings->ZIndexToYIndex[zIndex];

		this->forceSetpointWarning->AccelerationForceWarning[zIndex] = false;

		int mIndex = zIndex * 3;
		if (xIndex != -1) {
			float xLowFault = this->forceActuatorSettings->AccelerationLimitXTable[xIndex].LowFault;
			float xHighFault = this->forceActuatorSettings->AccelerationLimitXTable[xIndex].HighFault;
			this->rejectedAccelerationForces->XForces[xIndex] =
				(this->forceActuatorSettings->AccelerationXTable[mIndex + 0] * angularAccelerationX +
				this->forceActuatorSettings->AccelerationYTable[mIndex + 0] * angularAccelerationY +
				this->forceActuatorSettings->AccelerationZTable[mIndex + 0] * angularAccelerationZ) / 1000.0;
			this->forceSetpointWarning->AccelerationForceWarning[zIndex] = this->forceSetpointWarning->AccelerationForceWarning[zIndex] ||
				!Range::InRangeAndCoerce(xLowFault, xHighFault, this->rejectedAccelerationForces->XForces[xIndex], this->appliedAccelerationForces->XForces + xIndex);
		}

		if (yIndex != -1) {
			float yLowFault = this->forceActuatorSettings->AccelerationLimitYTable[yIndex].LowFault;
			float yHighFault = this->forceActuatorSettings->AccelerationLimitYTable[yIndex].HighFault;
			this->rejectedAccelerationForces->YForces[yIndex] =
				(this->forceActuatorSettings->AccelerationXTable[mIndex + 1] * angularAccelerationX +
				this->forceActuatorSettings->AccelerationYTable[mIndex + 1] * angularAccelerationY +
				this->forceActuatorSettings->AccelerationZTable[mIndex + 1] * angularAccelerationZ) / 1000.0;
			this->forceSetpointWarning->AccelerationForceWarning[zIndex] = this->forceSetpointWarning->AccelerationForceWarning[zIndex] ||
				!Range::InRangeAndCoerce(yLowFault, yHighFault, this->rejectedAccelerationForces->YForces[yIndex], this->appliedAccelerationForces->YForces + yIndex);
		}

		float zLowFault = this->forceActuatorSettings->AccelerationLimitZTable[zIndex].LowFault;
		float zHighFault = this->forceActuatorSettings->AccelerationLimitZTable[zIndex].HighFault;
		this->rejectedAccelerationForces->ZForces[zIndex] =
				(this->forceActuatorSettings->AccelerationXTable[mIndex + 2] * angularAccelerationX +
				this->forceActuatorSettings->AccelerationYTable[mIndex + 2] * angularAccelerationY +
				this->forceActuatorSettings->AccelerationZTable[mIndex + 2] * angularAccelerationZ) / 1000.0;
		this->forceSetpointWarning->AccelerationForceWarning[zIndex] = this->forceSetpointWarning->AccelerationForceWarning[zIndex] ||
			!Range::InRangeAndCoerce(zLowFault, zHighFault, this->rejectedAccelerationForces->ZForces[zIndex], this->appliedAccelerationForces->ZForces + zIndex);

		rejectionRequired = rejectionRequired || this->forceSetpointWarning->AccelerationForceWarning[zIndex];
	}
	this->setAppliedAccelerationForcesAndMoments();
	this->setRejectedAccelerationForcesAndMoments();
	this->safetyController->forceControllerNotifyAccelerationForceClipping(rejectionRequired);
	this->publisher->tryLogForceSetpointWarning();
	if (rejectionRequired) {
		this->publisher->logRejectedAccelerationForces();
	}
	this->publisher->logAppliedAccelerationForces();
	this->publisher->tryLogForceActuatorState();
}

void ForceController::zeroAccelerationForces() {
	Log.Info("ForceController: zeroAccelerationForces()");
	this->forceActuatorState->Timestamp = this->publisher->getTimestamp();
	this->appliedAccelerationForces->Timestamp = this->forceActuatorState->Timestamp;
	this->forceActuatorState->AccelerationForcesApplied = false;
	for(int zIndex = 0; zIndex < FA_COUNT; ++zIndex) {
		int xIndex = this->forceActuatorApplicationSettings->ZIndexToXIndex[zIndex];
		int yIndex = this->forceActuatorApplicationSettings->ZIndexToYIndex[zIndex];

		if (xIndex != -1) {
			this->appliedAccelerationForces->XForces[xIndex] = 0;
		}
		if (yIndex != -1) {
			this->appliedAccelerationForces->YForces[yIndex] = 0;
		}
		this->appliedAccelerationForces->ZForces[zIndex] = 0;
		this->forceSetpointWarning->AccelerationForceWarning[zIndex] = false;
	}
	this->setAppliedAccelerationForcesAndMoments();
	this->setRejectedAccelerationForcesAndMoments();
	this->safetyController->forceControllerNotifyAccelerationForceClipping(false);
	this->publisher->tryLogForceSetpointWarning();
	this->publisher->tryLogAppliedAccelerationForces();
	this->publisher->tryLogForceActuatorState();
}

void ForceController::applyActiveOpticForcesByBendingModes(float* coefficients) {
	Log.Info("ForceController: applyActiveOpticForcesByBendingModes()");
	float zForces[FA_COUNT];
	for(int zIndex = 0; zIndex < FA_COUNT; ++zIndex) {
		int mIndex = zIndex * 22;
		zForces[zIndex] =
				this->forceActuatorSettings->BendingModeTable[mIndex + 0] * coefficients[0] +
				this->forceActuatorSettings->BendingModeTable[mIndex + 1] * coefficients[1] +
				this->forceActuatorSettings->BendingModeTable[mIndex + 2] * coefficients[2] +
				this->forceActuatorSettings->BendingModeTable[mIndex + 3] * coefficients[3] +
				this->forceActuatorSettings->BendingModeTable[mIndex + 4] * coefficients[4] +
				this->forceActuatorSettings->BendingModeTable[mIndex + 5] * coefficients[5] +
				this->forceActuatorSettings->BendingModeTable[mIndex + 6] * coefficients[6] +
				this->forceActuatorSettings->BendingModeTable[mIndex + 7] * coefficients[7] +
				this->forceActuatorSettings->BendingModeTable[mIndex + 8] * coefficients[8] +
				this->forceActuatorSettings->BendingModeTable[mIndex + 9] * coefficients[9] +
				this->forceActuatorSettings->BendingModeTable[mIndex + 10] * coefficients[10] +
				this->forceActuatorSettings->BendingModeTable[mIndex + 11] * coefficients[11] +
				this->forceActuatorSettings->BendingModeTable[mIndex + 12] * coefficients[12] +
				this->forceActuatorSettings->BendingModeTable[mIndex + 13] * coefficients[13] +
				this->forceActuatorSettings->BendingModeTable[mIndex + 14] * coefficients[14] +
				this->forceActuatorSettings->BendingModeTable[mIndex + 15] * coefficients[15] +
				this->forceActuatorSettings->BendingModeTable[mIndex + 16] * coefficients[16] +
				this->forceActuatorSettings->BendingModeTable[mIndex + 17] * coefficients[17] +
				this->forceActuatorSettings->BendingModeTable[mIndex + 18] * coefficients[18] +
				this->forceActuatorSettings->BendingModeTable[mIndex + 19] * coefficients[19] +
				this->forceActuatorSettings->BendingModeTable[mIndex + 20] * coefficients[20] +
				this->forceActuatorSettings->BendingModeTable[mIndex + 21] * coefficients[21];
	}
	this->applyActiveOpticForces(zForces);
}

void ForceController::applyActiveOpticForces(float* z) {
	Log.Info("ForceController: applyActiveOpticForces()");
	float netAOSForce = 0;
	bool rejectionRequired = false;
	this->forceActuatorState->ActiveOpticForcesApplied = true;
	this->forceActuatorState->Timestamp = this->publisher->getTimestamp();
	this->appliedActiveOpticForces->Timestamp = this->forceActuatorState->Timestamp;
	this->rejectedActiveOpticForces->Timestamp = this->forceActuatorState->Timestamp;
	for(int zIndex = 0; zIndex < FA_COUNT; ++zIndex) {
		float zLowFault = this->forceActuatorSettings->ActiveOpticLimitZTable[zIndex].LowFault;
		float zHighFault = this->forceActuatorSettings->ActiveOpticLimitZTable[zIndex].HighFault;

		this->forceSetpointWarning->ActiveOpticForceWarning[zIndex] = false;

		this->rejectedActiveOpticForces->ZForces[zIndex] = z[zIndex];
		this->forceSetpointWarning->ActiveOpticForceWarning[zIndex] = this->forceSetpointWarning->ActiveOpticForceWarning[zIndex] ||
			!Range::InRangeAndCoerce(zLowFault, zHighFault, this->rejectedActiveOpticForces->ZForces[zIndex], this->appliedActiveOpticForces->ZForces + zIndex);

		netAOSForce += this->rejectedActiveOpticForces->ZForces[zIndex];
		rejectionRequired = rejectionRequired || this->forceSetpointWarning->ActiveOpticForceWarning[zIndex];
	}
	this->forceSetpointWarning->ActiveOpticNetForceWarning = !Range::InRange(-this->forceActuatorSettings->NetActiveOpticForceTolerance, this->forceActuatorSettings->NetActiveOpticForceTolerance, netAOSForce);
	this->setAppliedActiveOpticForcesAndMoments();
	this->setRejectedActiveOpticForcesAndMoments();
	this->safetyController->forceControllerNotifyActiveOpticForceClipping(rejectionRequired);
	this->safetyController->forceControllerNotifyActiveOpticNetForceCheck(this->forceSetpointWarning->ActiveOpticNetForceWarning);
	this->publisher->tryLogForceSetpointWarning();
	if (rejectionRequired) {
		this->publisher->logRejectedActiveOpticForces();
	}
	this->publisher->logAppliedActiveOpticForces();
	this->publisher->tryLogForceActuatorState();
}

void ForceController::zeroActiveOpticForces() {
	Log.Info("ForceController: zeroActiveOpticForces()");
	this->forceActuatorState->ActiveOpticForcesApplied = false;
	this->forceActuatorState->Timestamp = this->publisher->getTimestamp();
	this->appliedActiveOpticForces->Timestamp = this->forceActuatorState->Timestamp;
	for(int zIndex = 0; zIndex < FA_COUNT; ++zIndex) {
		this->appliedActiveOpticForces->ZForces[zIndex] = 0;
		this->forceSetpointWarning->ActiveOpticForceWarning[zIndex] = false;
	}
	this->forceSetpointWarning->ActiveOpticNetForceWarning = false;
	this->setAppliedActiveOpticForcesAndMoments();
	this->setRejectedActiveOpticForcesAndMoments();
	this->safetyController->forceControllerNotifyActiveOpticForceClipping(false);
	this->safetyController->forceControllerNotifyActiveOpticNetForceCheck(false);
	this->publisher->tryLogForceSetpointWarning();
	this->publisher->tryLogAppliedActiveOpticForces();
	this->publisher->tryLogForceActuatorState();
}

void ForceController::applyAzimuthForces() {
	Log.Info("ForceController: applyAzimuthForces()");
	this->forceActuatorState->Timestamp = this->publisher->getTimestamp();
	this->forceActuatorState->AzimuthForcesApplied = true;
	this->publisher->tryLogForceActuatorState();
}

void ForceController::updateAzimuthForces(float azimuthAngle) {
	Log.Trace("ForceController: updateAzimuthForces()");
	float azimuthMatrix[] = { std::pow(azimuthAngle, 5.0f), std::pow(azimuthAngle, 4.0f), std::pow(azimuthAngle, 3.0f), std::pow(azimuthAngle, 2.0f), azimuthAngle, 1.0f };
	bool rejectionRequired = false;
	this->forceActuatorState->Timestamp = this->publisher->getTimestamp();
	this->appliedAzimuthForces->Timestamp = this->forceActuatorState->Timestamp;
	this->rejectedAzimuthForces->Timestamp = this->forceActuatorState->Timestamp;
	for(int zIndex = 0; zIndex < FA_COUNT; ++zIndex) {
		int xIndex = this->forceActuatorApplicationSettings->ZIndexToXIndex[zIndex];
		int yIndex = this->forceActuatorApplicationSettings->ZIndexToYIndex[zIndex];

		this->forceSetpointWarning->AzimuthForceWarning[zIndex] = false;

		int mIndex = zIndex * 6;
		if (xIndex != -1) {
			float xLowFault = this->forceActuatorSettings->AzimuthLimitXTable[xIndex].LowFault;
			float xHighFault = this->forceActuatorSettings->AzimuthLimitXTable[xIndex].HighFault;
			this->rejectedAzimuthForces->XForces[xIndex] =
				this->forceActuatorSettings->AzimuthXTable[mIndex + 0] * azimuthMatrix[0] +
				this->forceActuatorSettings->AzimuthXTable[mIndex + 1] * azimuthMatrix[1] +
				this->forceActuatorSettings->AzimuthXTable[mIndex + 2] * azimuthMatrix[2] +
				this->forceActuatorSettings->AzimuthXTable[mIndex + 3] * azimuthMatrix[3] +
				this->forceActuatorSettings->AzimuthXTable[mIndex + 4] * azimuthMatrix[4] +
				this->forceActuatorSettings->AzimuthXTable[mIndex + 5];
			this->forceSetpointWarning->AzimuthForceWarning[zIndex] = this->forceSetpointWarning->AzimuthForceWarning[zIndex] ||
				!Range::InRangeAndCoerce(xLowFault, xHighFault, this->rejectedAzimuthForces->XForces[xIndex], this->appliedAzimuthForces->XForces + xIndex);
		}

		if (yIndex != -1) {
			float yLowFault = this->forceActuatorSettings->AzimuthLimitYTable[yIndex].LowFault;
			float yHighFault = this->forceActuatorSettings->AzimuthLimitYTable[yIndex].HighFault;
			this->rejectedAzimuthForces->YForces[yIndex] =
				this->forceActuatorSettings->AzimuthYTable[mIndex + 0] * azimuthMatrix[0] +
				this->forceActuatorSettings->AzimuthYTable[mIndex + 1] * azimuthMatrix[1] +
				this->forceActuatorSettings->AzimuthYTable[mIndex + 2] * azimuthMatrix[2] +
				this->forceActuatorSettings->AzimuthYTable[mIndex + 3] * azimuthMatrix[3] +
				this->forceActuatorSettings->AzimuthYTable[mIndex + 4] * azimuthMatrix[4] +
				this->forceActuatorSettings->AzimuthYTable[mIndex + 5];
			this->forceSetpointWarning->AzimuthForceWarning[zIndex] = this->forceSetpointWarning->AzimuthForceWarning[zIndex] ||
				!Range::InRangeAndCoerce(yLowFault, yHighFault, this->rejectedAzimuthForces->YForces[yIndex], this->appliedAzimuthForces->YForces + yIndex);
		}

		float zLowFault = this->forceActuatorSettings->AzimuthLimitZTable[zIndex].LowFault;
		float zHighFault = this->forceActuatorSettings->AzimuthLimitZTable[zIndex].HighFault;
		this->rejectedAzimuthForces->ZForces[zIndex] =
				this->forceActuatorSettings->AzimuthZTable[mIndex + 0] * azimuthMatrix[0] +
				this->forceActuatorSettings->AzimuthZTable[mIndex + 1] * azimuthMatrix[1] +
				this->forceActuatorSettings->AzimuthZTable[mIndex + 2] * azimuthMatrix[2] +
				this->forceActuatorSettings->AzimuthZTable[mIndex + 3] * azimuthMatrix[3] +
				this->forceActuatorSettings->AzimuthZTable[mIndex + 4] * azimuthMatrix[4] +
				this->forceActuatorSettings->AzimuthZTable[mIndex + 5];
		this->forceSetpointWarning->AzimuthForceWarning[zIndex] = this->forceSetpointWarning->AzimuthForceWarning[zIndex] ||
			!Range::InRangeAndCoerce(zLowFault, zHighFault, this->rejectedAzimuthForces->ZForces[zIndex], this->appliedAzimuthForces->ZForces + zIndex);

		rejectionRequired = rejectionRequired || this->forceSetpointWarning->AzimuthForceWarning[zIndex];
	}
	this->setAppliedAzimuthForcesAndMoments();
	this->setRejectedAzimuthForcesAndMoments();
	this->safetyController->forceControllerNotifyAzimuthForceClipping(rejectionRequired);
	this->publisher->tryLogForceSetpointWarning();
	if (rejectionRequired) {
		this->publisher->logRejectedAzimuthForces();
	}
	this->publisher->logAppliedAzimuthForces();
	this->publisher->tryLogForceActuatorState();
}

void ForceController::zeroAzimuthForces() {
	Log.Info("ForceController: zeroAzimuthForces()");
	this->forceActuatorState->Timestamp = this->publisher->getTimestamp();
	this->appliedAzimuthForces->Timestamp = this->forceActuatorState->Timestamp;
	this->forceActuatorState->AzimuthForcesApplied = false;
	for(int zIndex = 0; zIndex < FA_COUNT; ++zIndex) {
		int xIndex = this->forceActuatorApplicationSettings->ZIndexToXIndex[zIndex];
		int yIndex = this->forceActuatorApplicationSettings->ZIndexToYIndex[zIndex];

		if (xIndex != -1) {
			this->appliedAzimuthForces->XForces[xIndex] = 0;
		}
		if (yIndex != -1) {
			this->appliedAzimuthForces->YForces[yIndex] = 0;
		}
		this->appliedAzimuthForces->ZForces[zIndex] = 0;
		this->forceSetpointWarning->AzimuthForceWarning[zIndex] = false;
	}
	this->setAppliedAzimuthForcesAndMoments();
	this->setRejectedAzimuthForcesAndMoments();
	this->safetyController->forceControllerNotifyAzimuthForceClipping(false);
	this->publisher->tryLogForceSetpointWarning();
	this->publisher->tryLogAppliedAzimuthForces();
	this->publisher->tryLogForceActuatorState();
}

void ForceController::applyBalanceForces() {
	Log.Info("ForceController: applyBalanceForces()");
	this->forceActuatorState->Timestamp = this->publisher->getTimestamp();
	this->forceActuatorState->BalanceForcesApplied = true;
	this->publisher->tryLogForceActuatorState();
	this->fx.resetPreviousValues();
	this->fy.resetPreviousValues();
	this->fz.resetPreviousValues();
	this->mx.resetPreviousValues();
	this->my.resetPreviousValues();
	this->mz.resetPreviousValues();
}

void ForceController::updateBalanceForces() {
	Log.Trace("ForceController: updateBalanceForces()");
	float fx = this->fx.process(0, this->hardpointActuatorData->Fx);
	float fy = this->fy.process(0, this->hardpointActuatorData->Fy);
	float fz = this->fz.process(0, this->hardpointActuatorData->Fz);
	float mx = this->mx.process(0, this->hardpointActuatorData->Mx);
	float my = this->my.process(0, this->hardpointActuatorData->My);
	float mz = this->mz.process(0, this->hardpointActuatorData->Mz);
	// Note: Publishing from any PID will publish ALL PID data
	this->fx.publishTelemetry();
	DistributedForces forces = this->calculateDistribution(fx, fy, fz, mx, my, mz);
	bool rejectionRequired = false;

	for(int zIndex = 0; zIndex < FA_COUNT; ++zIndex) {
		int xIndex = this->forceActuatorApplicationSettings->ZIndexToXIndex[zIndex];
		int yIndex = this->forceActuatorApplicationSettings->ZIndexToYIndex[zIndex];

		this->forceSetpointWarning->BalanceForceWarning[zIndex] = false;

		if (xIndex != -1) {
			float xLowFault = this->forceActuatorSettings->BalanceLimitXTable[xIndex].LowFault;
			float xHighFault = this->forceActuatorSettings->BalanceLimitXTable[xIndex].HighFault;
			this->rejectedBalanceForces->XForces[xIndex] = forces.XForces[zIndex];
			this->forceSetpointWarning->BalanceForceWarning[zIndex] = this->forceSetpointWarning->BalanceForceWarning[zIndex] ||
				!Range::InRangeAndCoerce(xLowFault, xHighFault, this->rejectedBalanceForces->XForces[xIndex], this->appliedBalanceForces->XForces + xIndex);
		}

		if (yIndex != -1) {
			float yLowFault = this->forceActuatorSettings->BalanceLimitYTable[yIndex].LowFault;
			float yHighFault = this->forceActuatorSettings->BalanceLimitYTable[yIndex].HighFault;
			this->rejectedBalanceForces->YForces[yIndex] = forces.YForces[zIndex];
			this->forceSetpointWarning->BalanceForceWarning[zIndex] = this->forceSetpointWarning->BalanceForceWarning[zIndex] ||
				!Range::InRangeAndCoerce(yLowFault, yHighFault, this->rejectedBalanceForces->YForces[yIndex], this->appliedBalanceForces->YForces + yIndex);
		}

		float zLowFault = this->forceActuatorSettings->BalanceLimitZTable[zIndex].LowFault;
		float zHighFault = this->forceActuatorSettings->BalanceLimitZTable[zIndex].HighFault;
		this->rejectedBalanceForces->ZForces[zIndex] = forces.ZForces[zIndex];
		this->forceSetpointWarning->BalanceForceWarning[zIndex] = this->forceSetpointWarning->BalanceForceWarning[zIndex] ||
			!Range::InRangeAndCoerce(zLowFault, zHighFault, this->rejectedBalanceForces->ZForces[zIndex], this->appliedBalanceForces->ZForces + zIndex);
		rejectionRequired = rejectionRequired || this->forceSetpointWarning->BalanceForceWarning[zIndex];
	}
	this->setAppliedBalanceForcesAndMoments();
	this->setRejectedBalanceForcesAndMoments();
	this->safetyController->forceControllerNotifyBalanceForceClipping(rejectionRequired);
	this->publisher->tryLogForceSetpointWarning();
	if (rejectionRequired) {
		this->publisher->logRejectedBalanceForces();
	}
	this->publisher->logAppliedBalanceForces();
	this->publisher->tryLogForceActuatorState();
}

void ForceController::zeroBalanceForces() {
	Log.Info("ForceController: zeroBalanceForces()");
	this->forceActuatorState->Timestamp = this->publisher->getTimestamp();
	this->appliedBalanceForces->Timestamp = this->forceActuatorState->Timestamp;
	this->forceActuatorState->BalanceForcesApplied = false;
	for(int zIndex = 0; zIndex < FA_COUNT; ++zIndex) {
		int xIndex = this->forceActuatorApplicationSettings->ZIndexToXIndex[zIndex];
		int yIndex = this->forceActuatorApplicationSettings->ZIndexToYIndex[zIndex];

		if (xIndex != -1) {
			this->appliedBalanceForces->XForces[xIndex] = 0;
		}
		if (yIndex != -1) {
			this->appliedBalanceForces->YForces[yIndex] = 0;
		}
		this->appliedBalanceForces->ZForces[zIndex] = 0;
		this->forceSetpointWarning->BalanceForceWarning[zIndex] = false;
	}
	this->setAppliedBalanceForcesAndMoments();
	this->setRejectedBalanceForcesAndMoments();
	this->safetyController->forceControllerNotifyBalanceForceClipping(false);
	this->publisher->tryLogForceSetpointWarning();
	this->publisher->tryLogAppliedBalanceForces();
	this->publisher->tryLogForceActuatorState();
}

void ForceController::updatePID(int id, PIDParameters parameters) {
	Log.Info("ForceController: updatePID()");
	PID* pid = this->idToPID(id);
	if (pid != 0) {
		pid->updateParameters(parameters);
	}
}

void ForceController::resetPID(int id) {
	Log.Info("ForceController: resetPID()");
	PID* pid = this->idToPID(id);
	if (pid != 0) {
		pid->restoreInitialParameters();
	}
}

void ForceController::resetPIDs() {
	Log.Info("ForceController: resetPIDs()");
	this->fx.restoreInitialParameters();
	this->fy.restoreInitialParameters();
	this->fz.restoreInitialParameters();
	this->mx.restoreInitialParameters();
	this->my.restoreInitialParameters();
	this->mz.restoreInitialParameters();
}

void ForceController::applyElevationForces() {
	Log.Info("ForceController: applyElevationForces()");
	this->forceActuatorState->Timestamp = this->publisher->getTimestamp();
	this->forceActuatorState->ElevationForcesApplied = true;
	this->publisher->tryLogForceActuatorState();
}

void ForceController::updateElevationForces() {
	Log.Trace("ForceController: updateElevationForces()");
	double elevationAngle = this->forceActuatorSettings->UseInclinometer ? this->inclinometerData->InclinometerAngle : this->tmaElevationData.Angle_Actual;
	double elevationMatrix[] = { std::pow(elevationAngle, 5.0), std::pow(elevationAngle, 4.0), std::pow(elevationAngle, 3.0), std::pow(elevationAngle, 2.0), elevationAngle, 1 };
	bool rejectionRequired = false;
	for(int zIndex = 0; zIndex < FA_COUNT; ++zIndex) {
		int xIndex = this->forceActuatorApplicationSettings->ZIndexToXIndex[zIndex];
		int yIndex = this->forceActuatorApplicationSettings->ZIndexToYIndex[zIndex];

		this->forceSetpointWarning->ElevationForceWarning[zIndex] = false;

		int mIndex = zIndex * 6;
		if (xIndex != -1) {
			float xLowFault = this->forceActuatorSettings->ElevationLimitXTable[xIndex].LowFault;
			float xHighFault = this->forceActuatorSettings->ElevationLimitXTable[xIndex].HighFault;
			this->rejectedElevationForces->XForces[xIndex] =
				(this->forceActuatorSettings->ElevationXTable[mIndex + 0] * elevationMatrix[0] +
				this->forceActuatorSettings->ElevationXTable[mIndex + 1] * elevationMatrix[1] +
				this->forceActuatorSettings->ElevationXTable[mIndex + 2] * elevationMatrix[2] +
				this->forceActuatorSettings->ElevationXTable[mIndex + 3] * elevationMatrix[3] +
				this->forceActuatorSettings->ElevationXTable[mIndex + 4] * elevationMatrix[4] +
				this->forceActuatorSettings->ElevationXTable[mIndex + 5]) * this->forceActuatorState->SupportPercentage;
			this->forceSetpointWarning->ElevationForceWarning[zIndex] = this->forceSetpointWarning->ElevationForceWarning[zIndex] ||
				!Range::InRangeAndCoerce(xLowFault, xHighFault, this->rejectedElevationForces->XForces[xIndex], this->appliedElevationForces->XForces + xIndex);
		}

		if (yIndex != -1) {
			float yLowFault = this->forceActuatorSettings->ElevationLimitYTable[yIndex].LowFault;
			float yHighFault = this->forceActuatorSettings->ElevationLimitYTable[yIndex].HighFault;
			this->rejectedElevationForces->YForces[yIndex] =
				(this->forceActuatorSettings->ElevationYTable[mIndex + 0] * elevationMatrix[0] +
				this->forceActuatorSettings->ElevationYTable[mIndex + 1] * elevationMatrix[1] +
				this->forceActuatorSettings->ElevationYTable[mIndex + 2] * elevationMatrix[2] +
				this->forceActuatorSettings->ElevationYTable[mIndex + 3] * elevationMatrix[3] +
				this->forceActuatorSettings->ElevationYTable[mIndex + 4] * elevationMatrix[4] +
				this->forceActuatorSettings->ElevationYTable[mIndex + 5]) * this->forceActuatorState->SupportPercentage;
			this->forceSetpointWarning->ElevationForceWarning[zIndex] = this->forceSetpointWarning->ElevationForceWarning[zIndex] ||
				!Range::InRangeAndCoerce(yLowFault, yHighFault, this->rejectedElevationForces->YForces[yIndex], this->appliedElevationForces->YForces + yIndex);
		}

		float zLowFault = this->forceActuatorSettings->ElevationLimitZTable[zIndex].LowFault;
		float zHighFault = this->forceActuatorSettings->ElevationLimitZTable[zIndex].HighFault;
		this->rejectedElevationForces->ZForces[zIndex] =
			(this->forceActuatorSettings->ElevationZTable[mIndex + 0] * elevationMatrix[0] +
			this->forceActuatorSettings->ElevationZTable[mIndex + 1] * elevationMatrix[1] +
			this->forceActuatorSettings->ElevationZTable[mIndex + 2] * elevationMatrix[2] +
			this->forceActuatorSettings->ElevationZTable[mIndex + 3] * elevationMatrix[3] +
			this->forceActuatorSettings->ElevationZTable[mIndex + 4] * elevationMatrix[4] +
			this->forceActuatorSettings->ElevationZTable[mIndex + 5]) * this->forceActuatorState->SupportPercentage;
		this->forceSetpointWarning->ElevationForceWarning[zIndex] = this->forceSetpointWarning->ElevationForceWarning[zIndex] ||
			!Range::InRangeAndCoerce(zLowFault, zHighFault, this->rejectedElevationForces->ZForces[zIndex], this->appliedElevationForces->ZForces + zIndex);
		rejectionRequired = rejectionRequired || this->forceSetpointWarning->ElevationForceWarning[zIndex];
	}
	this->setAppliedElevationForcesAndMoments();
	this->setRejectedElevationForcesAndMoments();
	this->safetyController->forceControllerNotifyElevationForceClipping(rejectionRequired);
	this->publisher->tryLogForceSetpointWarning();
	if (rejectionRequired) {
		this->publisher->logRejectedElevationForces();
	}
	this->publisher->logAppliedElevationForces();
	this->publisher->tryLogForceActuatorState();
}

void ForceController::zeroElevationForces() {
	Log.Info("ForceController: zeroElevationForces()");
	this->forceActuatorState->Timestamp = this->publisher->getTimestamp();
	this->appliedElevationForces->Timestamp = this->forceActuatorState->Timestamp;
	this->forceActuatorState->ElevationForcesApplied = false;
	for(int zIndex = 0; zIndex < FA_COUNT; ++zIndex) {
		int xIndex = this->forceActuatorApplicationSettings->ZIndexToXIndex[zIndex];
		int yIndex = this->forceActuatorApplicationSettings->ZIndexToYIndex[zIndex];

		if (xIndex != -1) {
			this->appliedElevationForces->XForces[xIndex] = 0;
		}
		if (yIndex != -1) {
			this->appliedElevationForces->YForces[yIndex] = 0;
		}
		this->appliedElevationForces->ZForces[zIndex] = 0;
		this->forceSetpointWarning->ElevationForceWarning[zIndex] = false;
	}
	this->setAppliedElevationForcesAndMoments();
	this->setRejectedElevationForcesAndMoments();
	this->safetyController->forceControllerNotifyElevationForceClipping(false);
	this->publisher->tryLogForceSetpointWarning();
	this->publisher->tryLogAppliedElevationForces();
	this->publisher->tryLogForceActuatorState();
}

void ForceController::applyOffsetForces(float* x, float* y, float* z) {
	Log.Info("ForceController: applyOffsetForces()");
	bool rejectionRequired = false;
	this->forceActuatorState->OffsetForcesApplied = true;
	this->forceActuatorState->Timestamp = this->publisher->getTimestamp();
	this->appliedOffsetForces->Timestamp = this->forceActuatorState->Timestamp;
	this->rejectedOffsetForces->Timestamp = this->forceActuatorState->Timestamp;
	for(int zIndex = 0; zIndex < FA_COUNT; ++zIndex) {
		int xIndex = this->forceActuatorApplicationSettings->ZIndexToXIndex[zIndex];
		int yIndex = this->forceActuatorApplicationSettings->ZIndexToYIndex[zIndex];

		this->forceSetpointWarning->OffsetForceWarning[zIndex] = false;

		if (xIndex != -1) {
			float xLowFault = this->forceActuatorSettings->OffsetLimitXTable[xIndex].LowFault;
			float xHighFault = this->forceActuatorSettings->OffsetLimitXTable[xIndex].HighFault;
			this->rejectedOffsetForces->XForces[xIndex] = x[xIndex];
			this->forceSetpointWarning->OffsetForceWarning[zIndex] = this->forceSetpointWarning->OffsetForceWarning[zIndex] ||
				!Range::InRangeAndCoerce(xLowFault, xHighFault, this->rejectedOffsetForces->XForces[xIndex], this->appliedOffsetForces->XForces + xIndex);
		}

		if (yIndex != -1) {
			float yLowFault = this->forceActuatorSettings->OffsetLimitYTable[yIndex].LowFault;
			float yHighFault = this->forceActuatorSettings->OffsetLimitYTable[yIndex].HighFault;
			this->rejectedOffsetForces->YForces[yIndex] = y[yIndex];
			this->forceSetpointWarning->OffsetForceWarning[zIndex] = this->forceSetpointWarning->OffsetForceWarning[zIndex] ||
				!Range::InRangeAndCoerce(yLowFault, yHighFault, this->rejectedOffsetForces->YForces[yIndex], this->appliedOffsetForces->YForces + yIndex);
		}

		float zLowFault = this->forceActuatorSettings->OffsetLimitZTable[zIndex].LowFault;
		float zHighFault = this->forceActuatorSettings->OffsetLimitZTable[zIndex].HighFault;
		this->rejectedOffsetForces->ZForces[zIndex] = z[zIndex];
		this->forceSetpointWarning->OffsetForceWarning[zIndex] = this->forceSetpointWarning->OffsetForceWarning[zIndex] ||
			!Range::InRangeAndCoerce(zLowFault, zHighFault, this->rejectedOffsetForces->ZForces[zIndex], this->appliedOffsetForces->ZForces + zIndex);
		rejectionRequired = rejectionRequired || this->forceSetpointWarning->OffsetForceWarning[zIndex];
	}
	this->setAppliedOffsetForcesAndMoments();
	this->setRejectedOffsetForcesAndMoments();
	this->safetyController->forceControllerNotifyOffsetForceClipping(rejectionRequired);
	this->publisher->tryLogForceSetpointWarning();
	if (rejectionRequired) {
		this->publisher->logRejectedOffsetForces();
	}
	this->publisher->logAppliedOffsetForces();
	this->publisher->tryLogForceActuatorState();
}

void ForceController::applyOffsetForcesByMirrorForces(float xForce, float yForce, float zForce, float xMoment, float yMoment, float zMoment) {
	Log.Info("ForceController: applyOffsetForcesByMirrorForces()");
	DistributedForces forces = this->calculateDistribution(xForce, yForce, zForce, xMoment, yMoment, zMoment);
	this->applyOffsetForces(forces.XForces, forces.YForces, forces.ZForces);
}

void ForceController::zeroOffsetForces() {
	Log.Info("ForceController: zeroOffsetForces()");
	this->forceActuatorState->OffsetForcesApplied = false;
	this->forceActuatorState->Timestamp = this->publisher->getTimestamp();
	this->appliedOffsetForces->Timestamp = this->forceActuatorState->Timestamp;
	for(int zIndex = 0; zIndex < FA_COUNT; ++zIndex) {
		int xIndex = this->forceActuatorApplicationSettings->ZIndexToXIndex[zIndex];
		int yIndex = this->forceActuatorApplicationSettings->ZIndexToYIndex[zIndex];

		if (xIndex != -1) {
			this->appliedOffsetForces->XForces[xIndex] = 0;
		}
		if (yIndex != -1) {
			this->appliedOffsetForces->YForces[yIndex] = 0;
		}
		this->appliedOffsetForces->ZForces[zIndex] = 0;
		this->forceSetpointWarning->OffsetForceWarning[zIndex] = false;
	}
	this->setAppliedOffsetForcesAndMoments();
	this->setRejectedOffsetForcesAndMoments();
	this->safetyController->forceControllerNotifyOffsetForceClipping(false);
	this->publisher->tryLogForceSetpointWarning();
	this->publisher->tryLogAppliedOffsetForces();
	this->publisher->tryLogForceActuatorState();
}

void ForceController::applyStaticForces() {
	Log.Info("ForceController: applyStaticForces()");
	bool rejectionRequired = false;
	this->forceActuatorState->StaticForcesApplied = true;
	this->forceActuatorState->Timestamp = this->publisher->getTimestamp();
	this->appliedStaticForces->Timestamp = this->forceActuatorState->Timestamp;
	this->rejectedStaticForces->Timestamp = this->forceActuatorState->Timestamp;
	for(int zIndex = 0; zIndex < FA_Z_COUNT; ++zIndex) {
		int xIndex = this->forceActuatorApplicationSettings->ZIndexToXIndex[zIndex];
		int yIndex = this->forceActuatorApplicationSettings->ZIndexToYIndex[zIndex];

		this->forceSetpointWarning->StaticForceWarning[zIndex] = false;

		if (xIndex != -1) {
			float xLowFault = this->forceActuatorSettings->StaticLimitXTable[xIndex].LowFault;
			float xHighFault = this->forceActuatorSettings->StaticLimitXTable[xIndex].HighFault;
			this->rejectedStaticForces->XForces[xIndex] = this->forceActuatorSettings->StaticXTable[xIndex];
			this->forceSetpointWarning->StaticForceWarning[zIndex] = this->forceSetpointWarning->StaticForceWarning[zIndex] ||
				!Range::InRangeAndCoerce(xLowFault, xHighFault, this->rejectedStaticForces->XForces[xIndex], this->appliedStaticForces->XForces + xIndex);
		}

		if (yIndex != -1) {
			float yLowFault = this->forceActuatorSettings->StaticLimitYTable[yIndex].LowFault;
			float yHighFault = this->forceActuatorSettings->StaticLimitYTable[yIndex].HighFault;
			this->rejectedStaticForces->YForces[yIndex] = this->forceActuatorSettings->StaticYTable[yIndex];
			this->forceSetpointWarning->StaticForceWarning[zIndex] = this->forceSetpointWarning->StaticForceWarning[zIndex] ||
				!Range::InRangeAndCoerce(yLowFault, yHighFault, this->rejectedStaticForces->YForces[yIndex], this->appliedStaticForces->YForces + yIndex);
		}

		float zLowFault = this->forceActuatorSettings->StaticLimitZTable[zIndex].LowFault;
		float zHighFault = this->forceActuatorSettings->StaticLimitZTable[zIndex].HighFault;
		this->rejectedStaticForces->ZForces[zIndex] = this->forceActuatorSettings->StaticZTable[zIndex];
		this->forceSetpointWarning->StaticForceWarning[zIndex] = this->forceSetpointWarning->StaticForceWarning[zIndex] ||
			!Range::InRangeAndCoerce(zLowFault, zHighFault, this->rejectedStaticForces->ZForces[zIndex], this->appliedStaticForces->ZForces + zIndex);
		rejectionRequired = rejectionRequired || this->forceSetpointWarning->StaticForceWarning[zIndex];
	}
	this->setAppliedStaticForcesAndMoments();
	this->setRejectedStaticForcesAndMoments();
	this->safetyController->forceControllerNotifyStaticForceClipping(rejectionRequired);
	this->publisher->tryLogForceSetpointWarning();
	if (rejectionRequired) {
		this->publisher->logRejectedStaticForces();
	}
	this->publisher->logAppliedStaticForces();
	this->publisher->tryLogForceActuatorState();
}

void ForceController::zeroStaticForces() {
	Log.Info("ForceController: zeroStaticForces()");
	this->forceActuatorState->StaticForcesApplied = false;
	this->forceActuatorState->Timestamp = this->publisher->getTimestamp();
	this->appliedStaticForces->Timestamp = this->forceActuatorState->Timestamp;
	for(int zIndex = 0; zIndex < FA_COUNT; ++zIndex) {
		int xIndex = this->forceActuatorApplicationSettings->ZIndexToXIndex[zIndex];
		int yIndex = this->forceActuatorApplicationSettings->ZIndexToYIndex[zIndex];

		if (xIndex != -1) {
			this->appliedStaticForces->XForces[xIndex] = 0;
		}
		if (yIndex != -1) {
			this->appliedStaticForces->YForces[yIndex] = 0;
		}
		this->appliedStaticForces->ZForces[zIndex] = 0;
		this->forceSetpointWarning->StaticForceWarning[zIndex] = false;
	}
	this->setAppliedStaticForcesAndMoments();
	this->setRejectedStaticForcesAndMoments();
	this->safetyController->forceControllerNotifyStaticForceClipping(false);
	this->publisher->tryLogForceSetpointWarning();
	this->publisher->tryLogAppliedStaticForces();
	this->publisher->tryLogForceActuatorState();
}

void ForceController::applyThermalForces() {
	Log.Info("ForceController: applyThermalForces()");
	this->forceActuatorState->Timestamp = this->publisher->getTimestamp();
	this->forceActuatorState->ThermalForcesApplied = true;
	this->publisher->tryLogForceActuatorState();
}

void ForceController::updateThermalForces() {
	Log.Trace("ForceController: updateThermalForces()");
	float temperature = 0; // TODO: Update
	float temperatureMatrix[] = { std::pow(temperature, 5.0f), std::pow(temperature, 4.0f), std::pow(temperature, 3.0f), std::pow(temperature, 2.0f), temperature, 1.0f };
	bool rejectionRequired = false;
	for(int zIndex = 0; zIndex < FA_COUNT; ++zIndex) {
		int xIndex = this->forceActuatorApplicationSettings->ZIndexToXIndex[zIndex];
		int yIndex = this->forceActuatorApplicationSettings->ZIndexToYIndex[zIndex];

		this->forceSetpointWarning->ThermalForceWarning[zIndex] = false;

		int mIndex = zIndex * 6;
		if (xIndex != -1) {
			float xLowFault = this->forceActuatorSettings->ThermalLimitXTable[xIndex].LowFault;
			float xHighFault = this->forceActuatorSettings->ThermalLimitXTable[xIndex].HighFault;
			this->rejectedThermalForces->XForces[xIndex] =
				this->forceActuatorSettings->ThermalXTable[mIndex + 0] * temperatureMatrix[0] +
				this->forceActuatorSettings->ThermalXTable[mIndex + 1] * temperatureMatrix[1] +
				this->forceActuatorSettings->ThermalXTable[mIndex + 2] * temperatureMatrix[2] +
				this->forceActuatorSettings->ThermalXTable[mIndex + 3] * temperatureMatrix[3] +
				this->forceActuatorSettings->ThermalXTable[mIndex + 4] * temperatureMatrix[4] +
				this->forceActuatorSettings->ThermalXTable[mIndex + 5];
			this->forceSetpointWarning->ThermalForceWarning[zIndex] = this->forceSetpointWarning->ThermalForceWarning[zIndex] ||
				!Range::InRangeAndCoerce(xLowFault, xHighFault, this->rejectedThermalForces->XForces[xIndex], this->appliedThermalForces->XForces + xIndex);
		}

		if (yIndex != -1) {
			float yLowFault = this->forceActuatorSettings->ThermalLimitYTable[yIndex].LowFault;
			float yHighFault = this->forceActuatorSettings->ThermalLimitYTable[yIndex].HighFault;
			this->rejectedThermalForces->YForces[yIndex] =
				this->forceActuatorSettings->ThermalYTable[mIndex + 0] * temperatureMatrix[0] +
				this->forceActuatorSettings->ThermalYTable[mIndex + 1] * temperatureMatrix[1] +
				this->forceActuatorSettings->ThermalYTable[mIndex + 2] * temperatureMatrix[2] +
				this->forceActuatorSettings->ThermalYTable[mIndex + 3] * temperatureMatrix[3] +
				this->forceActuatorSettings->ThermalYTable[mIndex + 4] * temperatureMatrix[4] +
				this->forceActuatorSettings->ThermalYTable[mIndex + 5];
			this->forceSetpointWarning->ThermalForceWarning[zIndex] = this->forceSetpointWarning->ThermalForceWarning[zIndex] ||
				!Range::InRangeAndCoerce(yLowFault, yHighFault, this->rejectedThermalForces->YForces[yIndex], this->appliedThermalForces->YForces + yIndex);
		}

		float zLowFault = this->forceActuatorSettings->ThermalLimitZTable[zIndex].LowFault;
		float zHighFault = this->forceActuatorSettings->ThermalLimitZTable[zIndex].HighFault;
		this->rejectedThermalForces->ZForces[zIndex] =
			this->forceActuatorSettings->ThermalZTable[mIndex + 0] * temperatureMatrix[0] +
			this->forceActuatorSettings->ThermalZTable[mIndex + 1] * temperatureMatrix[1] +
			this->forceActuatorSettings->ThermalZTable[mIndex + 2] * temperatureMatrix[2] +
			this->forceActuatorSettings->ThermalZTable[mIndex + 3] * temperatureMatrix[3] +
			this->forceActuatorSettings->ThermalZTable[mIndex + 4] * temperatureMatrix[4] +
			this->forceActuatorSettings->ThermalZTable[mIndex + 5];
		this->forceSetpointWarning->ThermalForceWarning[zIndex] = this->forceSetpointWarning->ThermalForceWarning[zIndex] ||
			!Range::InRangeAndCoerce(zLowFault, zHighFault, this->rejectedThermalForces->ZForces[zIndex], this->appliedThermalForces->ZForces + zIndex);

		rejectionRequired = rejectionRequired || this->forceSetpointWarning->ThermalForceWarning[zIndex];
	}
	this->setAppliedThermalForcesAndMoments();
	this->setRejectedThermalForcesAndMoments();
	this->safetyController->forceControllerNotifyThermalForceClipping(rejectionRequired);
	this->publisher->tryLogForceSetpointWarning();
	if (rejectionRequired) {
		this->publisher->logRejectedThermalForces();
	}
	this->publisher->logAppliedThermalForces();
	this->publisher->tryLogForceActuatorState();
}

void ForceController::zeroThermalForces() {
	Log.Info("ForceController: zeroThermalForces()");
	this->forceActuatorState->Timestamp = this->publisher->getTimestamp();
	this->appliedThermalForces->Timestamp = this->forceActuatorState->Timestamp;
	this->forceActuatorState->ThermalForcesApplied = false;
	for(int zIndex = 0; zIndex < FA_COUNT; ++zIndex) {
		int xIndex = this->forceActuatorApplicationSettings->ZIndexToXIndex[zIndex];
		int yIndex = this->forceActuatorApplicationSettings->ZIndexToYIndex[zIndex];

		if (xIndex != -1) {
			this->appliedThermalForces->XForces[xIndex] = 0;
		}
		if (yIndex != -1) {
			this->appliedThermalForces->YForces[yIndex] = 0;
		}
		this->appliedThermalForces->ZForces[zIndex] = 0;
		this->forceSetpointWarning->ThermalForceWarning[zIndex] = false;
	}
	this->setAppliedThermalForcesAndMoments();
	this->setRejectedThermalForcesAndMoments();
	this->safetyController->forceControllerNotifyThermalForceClipping(false);
	this->publisher->tryLogForceSetpointWarning();
	this->publisher->tryLogAppliedThermalForces();
	this->publisher->tryLogForceActuatorState();
}

void ForceController::applyVelocityForces() {
	Log.Info("ForceController: applyVelocityForces()");
	this->forceActuatorState->Timestamp = this->publisher->getTimestamp();
	this->forceActuatorState->VelocityForcesApplied = true;
	this->publisher->tryLogForceActuatorState();
}

void ForceController::updateVelocityForces() {
	Log.Trace("ForceController: updateVelocityForces()");
	float angularVelocityX = this->gyroData->AngularVelocityX * this->gyroData->AngularVelocityX;
	float angularVelocityY = this->gyroData->AngularVelocityY * this->gyroData->AngularVelocityY;
	float angularVelocityZ = this->gyroData->AngularVelocityZ * this->gyroData->AngularVelocityZ;
	float angularVelocityXZ = this->gyroData->AngularVelocityX * this->gyroData->AngularVelocityZ;
	float angularVelocityYZ = this->gyroData->AngularVelocityY * this->gyroData->AngularVelocityZ;
	bool rejectionRequired = false;
	for(int zIndex = 0; zIndex < FA_COUNT; ++zIndex) {
		int xIndex = this->forceActuatorApplicationSettings->ZIndexToXIndex[zIndex];
		int yIndex = this->forceActuatorApplicationSettings->ZIndexToYIndex[zIndex];

		this->forceSetpointWarning->VelocityForceWarning[zIndex] = false;

		// Stage Dynamic Forces
		int mIndex = zIndex * 3;
		if (xIndex != -1) {
			float xLowFault = this->forceActuatorSettings->VelocityLimitXTable[xIndex].LowFault;
			float xHighFault = this->forceActuatorSettings->VelocityLimitXTable[xIndex].HighFault;
			this->rejectedVelocityForces->XForces[xIndex] =
				(this->forceActuatorSettings->VelocityXTable[mIndex + 0] * angularVelocityX +
				this->forceActuatorSettings->VelocityYTable[mIndex + 0] * angularVelocityY +
				this->forceActuatorSettings->VelocityZTable[mIndex + 0] * angularVelocityZ +
				this->forceActuatorSettings->VelocityXZTable[mIndex + 0] * angularVelocityXZ +
				this->forceActuatorSettings->VelocityYZTable[mIndex + 0] * angularVelocityYZ) / 1000.0;
			this->forceSetpointWarning->VelocityForceWarning[zIndex] = this->forceSetpointWarning->VelocityForceWarning[zIndex] ||
				!Range::InRangeAndCoerce(xLowFault, xHighFault, this->rejectedVelocityForces->XForces[xIndex], this->appliedVelocityForces->XForces + xIndex);
		}

		if (yIndex != -1) {
			float yLowFault = this->forceActuatorSettings->VelocityLimitYTable[yIndex].LowFault;
			float yHighFault = this->forceActuatorSettings->VelocityLimitYTable[yIndex].HighFault;
			this->rejectedVelocityForces->YForces[yIndex] =
				(this->forceActuatorSettings->VelocityXTable[mIndex + 1] * angularVelocityX +
				this->forceActuatorSettings->VelocityYTable[mIndex + 1] * angularVelocityY +
				this->forceActuatorSettings->VelocityZTable[mIndex + 1] * angularVelocityZ +
				this->forceActuatorSettings->VelocityXZTable[mIndex + 1] * angularVelocityXZ +
				this->forceActuatorSettings->VelocityYZTable[mIndex + 1] * angularVelocityYZ) / 1000.0;
			this->forceSetpointWarning->VelocityForceWarning[zIndex] = this->forceSetpointWarning->VelocityForceWarning[zIndex] ||
				!Range::InRangeAndCoerce(yLowFault, yHighFault, this->rejectedVelocityForces->YForces[yIndex], this->appliedVelocityForces->YForces + yIndex);
		}

		float zLowFault = this->forceActuatorSettings->VelocityLimitZTable[zIndex].LowFault;
		float zHighFault = this->forceActuatorSettings->VelocityLimitZTable[zIndex].HighFault;
		this->rejectedVelocityForces->ZForces[zIndex] =
			(this->forceActuatorSettings->VelocityXTable[mIndex + 2] * angularVelocityX +
			this->forceActuatorSettings->VelocityYTable[mIndex + 2] * angularVelocityY +
			this->forceActuatorSettings->VelocityZTable[mIndex + 2] * angularVelocityZ +
			this->forceActuatorSettings->VelocityXZTable[mIndex + 2] * angularVelocityXZ +
			this->forceActuatorSettings->VelocityYZTable[mIndex + 2] * angularVelocityYZ) / 1000.0;
		this->forceSetpointWarning->VelocityForceWarning[zIndex] = this->forceSetpointWarning->VelocityForceWarning[zIndex] ||
			!Range::InRangeAndCoerce(zLowFault, zHighFault, this->rejectedVelocityForces->ZForces[zIndex], this->appliedVelocityForces->ZForces + zIndex);
		rejectionRequired = rejectionRequired || this->forceSetpointWarning->VelocityForceWarning[zIndex];
	}
	this->setAppliedVelocityForcesAndMoments();
	this->setRejectedVelocityForcesAndMoments();
	this->safetyController->forceControllerNotifyVelocityForceClipping(rejectionRequired);
	this->publisher->tryLogForceSetpointWarning();
	if (rejectionRequired) {
		this->publisher->logRejectedVelocityForces();
	}
	this->publisher->logAppliedVelocityForces();
	this->publisher->tryLogForceActuatorState();
}

void ForceController::zeroVelocityForces() {
	Log.Info("ForceController: zeroVelocityForces()");
	this->forceActuatorState->Timestamp = this->publisher->getTimestamp();
	this->appliedVelocityForces->Timestamp = this->forceActuatorState->Timestamp;
	this->forceActuatorState->VelocityForcesApplied = false;
	for(int zIndex = 0; zIndex < FA_COUNT; ++zIndex) {
		int xIndex = this->forceActuatorApplicationSettings->ZIndexToXIndex[zIndex];
		int yIndex = this->forceActuatorApplicationSettings->ZIndexToYIndex[zIndex];

		if (xIndex != -1) {
			this->appliedVelocityForces->XForces[xIndex] = 0;
		}
		if (yIndex != -1) {
			this->appliedVelocityForces->YForces[yIndex] = 0;
		}
		this->appliedVelocityForces->ZForces[zIndex] = 0;
		this->forceSetpointWarning->VelocityForceWarning[zIndex] = false;
	}
	this->setAppliedVelocityForcesAndMoments();
	this->setRejectedVelocityForcesAndMoments();
	this->safetyController->forceControllerNotifyVelocityForceClipping(false);
	this->publisher->tryLogForceSetpointWarning();
	this->publisher->tryLogAppliedVelocityForces();
	this->publisher->tryLogForceActuatorState();
}

DistributedForces ForceController::calculateDistribution(float xForce, float yForce, float zForce, float xMoment, float yMoment, float zMoment) {
	DistributedForces forces;
	for(int i = 0; i < FA_COUNT; ++i) {
		forces.XForces[i] = 0;
		forces.YForces[i] = 0;
		forces.ZForces[i] = 0;
		forces.XForces[i] += this->forceActuatorSettings->ForceDistributionXTable[i * 3 + 0] * xForce;
		forces.YForces[i] += this->forceActuatorSettings->ForceDistributionXTable[i * 3 + 1] * xForce;
		forces.ZForces[i] += this->forceActuatorSettings->ForceDistributionXTable[i * 3 + 2] * xForce;
		forces.XForces[i] += this->forceActuatorSettings->ForceDistributionYTable[i * 3 + 0] * yForce;
		forces.YForces[i] += this->forceActuatorSettings->ForceDistributionYTable[i * 3 + 1] * yForce;
		forces.ZForces[i] += this->forceActuatorSettings->ForceDistributionYTable[i * 3 + 2] * yForce;
		forces.XForces[i] += this->forceActuatorSettings->ForceDistributionZTable[i * 3 + 0] * zForce;
		forces.YForces[i] += this->forceActuatorSettings->ForceDistributionZTable[i * 3 + 1] * zForce;
		forces.ZForces[i] += this->forceActuatorSettings->ForceDistributionZTable[i * 3 + 2] * zForce;
		forces.XForces[i] += this->forceActuatorSettings->MomentDistributionXTable[i * 3 + 0] * xMoment;
		forces.YForces[i] += this->forceActuatorSettings->MomentDistributionXTable[i * 3 + 1] * xMoment;
		forces.ZForces[i] += this->forceActuatorSettings->MomentDistributionXTable[i * 3 + 2] * xMoment;
		forces.XForces[i] += this->forceActuatorSettings->MomentDistributionYTable[i * 3 + 0] * yMoment;
		forces.YForces[i] += this->forceActuatorSettings->MomentDistributionYTable[i * 3 + 1] * yMoment;
		forces.ZForces[i] += this->forceActuatorSettings->MomentDistributionYTable[i * 3 + 2] * yMoment;
		forces.XForces[i] += this->forceActuatorSettings->MomentDistributionZTable[i * 3 + 0] * zMoment;
		forces.YForces[i] += this->forceActuatorSettings->MomentDistributionZTable[i * 3 + 1] * zMoment;
		forces.ZForces[i] += this->forceActuatorSettings->MomentDistributionZTable[i * 3 + 2] * zMoment;
	}
	return forces;
}

ForcesAndMoments ForceController::calculateForcesAndMoments(float* xForces, float* yForces, float* zForces) {
	return ForceConverter::calculateForcesAndMoments(this->forceActuatorApplicationSettings, this->forceActuatorSettings, xForces, yForces, zForces);
}

void ForceController::setAppliedAberrationForcesAndMoments() {
	ForcesAndMoments fm = this->calculateForcesAndMoments(this->zero, this->zero, this->appliedAberrationForces->ZForces);
	this->appliedAberrationForces->Fz = fm.Fz;
	this->appliedAberrationForces->Mx = fm.Mx;
	this->appliedAberrationForces->My = fm.My;
}

void ForceController::setAppliedAccelerationForcesAndMoments() {
	ForcesAndMoments fm = this->calculateForcesAndMoments(this->appliedAccelerationForces->XForces, this->appliedAccelerationForces->YForces, this->appliedAccelerationForces->ZForces);
	this->appliedAccelerationForces->Fx = fm.Fx;
	this->appliedAccelerationForces->Fy = fm.Fy;
	this->appliedAccelerationForces->Fz = fm.Fz;
	this->appliedAccelerationForces->Mx = fm.Mx;
	this->appliedAccelerationForces->My = fm.My;
	this->appliedAccelerationForces->Mz = fm.Mz;
	this->appliedAccelerationForces->ForceMagnitude = fm.ForceMagnitude;
}

void ForceController::setAppliedActiveOpticForcesAndMoments() {
	ForcesAndMoments fm = this->calculateForcesAndMoments(this->zero, this->zero, this->appliedActiveOpticForces->ZForces);
	this->appliedActiveOpticForces->Fz = fm.Fz;
	this->appliedActiveOpticForces->Mx = fm.Mx;
	this->appliedActiveOpticForces->My = fm.My;
}

void ForceController::setAppliedAzimuthForcesAndMoments() {
	ForcesAndMoments fm = this->calculateForcesAndMoments(this->appliedAzimuthForces->XForces, this->appliedAzimuthForces->YForces, this->appliedAzimuthForces->ZForces);
	this->appliedAzimuthForces->Fx = fm.Fx;
	this->appliedAzimuthForces->Fy = fm.Fy;
	this->appliedAzimuthForces->Fz = fm.Fz;
	this->appliedAzimuthForces->Mx = fm.Mx;
	this->appliedAzimuthForces->My = fm.My;
	this->appliedAzimuthForces->Mz = fm.Mz;
	this->appliedAzimuthForces->ForceMagnitude = fm.ForceMagnitude;
}

void ForceController::setAppliedBalanceForcesAndMoments() {
	ForcesAndMoments fm = this->calculateForcesAndMoments(this->appliedBalanceForces->XForces, this->appliedBalanceForces->YForces, this->appliedBalanceForces->ZForces);
	this->appliedBalanceForces->Fx = fm.Fx;
	this->appliedBalanceForces->Fy = fm.Fy;
	this->appliedBalanceForces->Fz = fm.Fz;
	this->appliedBalanceForces->Mx = fm.Mx;
	this->appliedBalanceForces->My = fm.My;
	this->appliedBalanceForces->Mz = fm.Mz;
	this->appliedBalanceForces->ForceMagnitude = fm.ForceMagnitude;
}

void ForceController::setAppliedElevationForcesAndMoments() {
	ForcesAndMoments fm = this->calculateForcesAndMoments(this->appliedElevationForces->XForces, this->appliedElevationForces->YForces, this->appliedElevationForces->ZForces);
	this->appliedElevationForces->Fx = fm.Fx;
	this->appliedElevationForces->Fy = fm.Fy;
	this->appliedElevationForces->Fz = fm.Fz;
	this->appliedElevationForces->Mx = fm.Mx;
	this->appliedElevationForces->My = fm.My;
	this->appliedElevationForces->Mz = fm.Mz;
	this->appliedElevationForces->ForceMagnitude = fm.ForceMagnitude;
}

void ForceController::setAppliedForcesAndMoments() {
	ForcesAndMoments fm = this->calculateForcesAndMoments(this->appliedForces->XForces, this->appliedForces->YForces, this->appliedForces->ZForces);
	this->appliedForces->Fx = fm.Fx;
	this->appliedForces->Fy = fm.Fy;
	this->appliedForces->Fz = fm.Fz;
	this->appliedForces->Mx = fm.Mx;
	this->appliedForces->My = fm.My;
	this->appliedForces->Mz = fm.Mz;
	this->appliedForces->ForceMagnitude = fm.ForceMagnitude;
}

void ForceController::setAppliedOffsetForcesAndMoments() {
	ForcesAndMoments fm = this->calculateForcesAndMoments(this->appliedOffsetForces->XForces, this->appliedOffsetForces->YForces, this->appliedOffsetForces->ZForces);
	this->appliedOffsetForces->Fx = fm.Fx;
	this->appliedOffsetForces->Fy = fm.Fy;
	this->appliedOffsetForces->Fz = fm.Fz;
	this->appliedOffsetForces->Mx = fm.Mx;
	this->appliedOffsetForces->My = fm.My;
	this->appliedOffsetForces->Mz = fm.Mz;
	this->appliedOffsetForces->ForceMagnitude = fm.ForceMagnitude;
}

void ForceController::setAppliedStaticForcesAndMoments() {
	ForcesAndMoments fm = this->calculateForcesAndMoments(this->appliedStaticForces->XForces, this->appliedStaticForces->YForces, this->appliedStaticForces->ZForces);
	this->appliedStaticForces->Fx = fm.Fx;
	this->appliedStaticForces->Fy = fm.Fy;
	this->appliedStaticForces->Fz = fm.Fz;
	this->appliedStaticForces->Mx = fm.Mx;
	this->appliedStaticForces->My = fm.My;
	this->appliedStaticForces->Mz = fm.Mz;
	this->appliedStaticForces->ForceMagnitude = fm.ForceMagnitude;
}

void ForceController::setAppliedThermalForcesAndMoments() {
	ForcesAndMoments fm = this->calculateForcesAndMoments(this->appliedThermalForces->XForces, this->appliedThermalForces->YForces, this->appliedThermalForces->ZForces);
	this->appliedThermalForces->Fx = fm.Fx;
	this->appliedThermalForces->Fy = fm.Fy;
	this->appliedThermalForces->Fz = fm.Fz;
	this->appliedThermalForces->Mx = fm.Mx;
	this->appliedThermalForces->My = fm.My;
	this->appliedThermalForces->Mz = fm.Mz;
	this->appliedThermalForces->ForceMagnitude = fm.ForceMagnitude;
}

void ForceController::setAppliedVelocityForcesAndMoments() {
	ForcesAndMoments fm = this->calculateForcesAndMoments(this->appliedVelocityForces->XForces, this->appliedVelocityForces->YForces, this->appliedVelocityForces->ZForces);
	this->appliedVelocityForces->Fx = fm.Fx;
	this->appliedVelocityForces->Fy = fm.Fy;
	this->appliedVelocityForces->Fz = fm.Fz;
	this->appliedVelocityForces->Mx = fm.Mx;
	this->appliedVelocityForces->My = fm.My;
	this->appliedVelocityForces->Mz = fm.Mz;
	this->appliedVelocityForces->ForceMagnitude = fm.ForceMagnitude;
}

void ForceController::setRejectedAberrationForcesAndMoments() {
	ForcesAndMoments fm = this->calculateForcesAndMoments(this->zero, this->zero, this->rejectedAberrationForces->ZForces);
	this->rejectedAberrationForces->Fz = fm.Fz;
	this->rejectedAberrationForces->Mx = fm.Mx;
	this->rejectedAberrationForces->My = fm.My;
}

void ForceController::setRejectedAccelerationForcesAndMoments() {
	ForcesAndMoments fm = this->calculateForcesAndMoments(this->rejectedAccelerationForces->XForces, this->rejectedAccelerationForces->YForces, this->rejectedAccelerationForces->ZForces);
	this->rejectedAccelerationForces->Fx = fm.Fx;
	this->rejectedAccelerationForces->Fy = fm.Fy;
	this->rejectedAccelerationForces->Fz = fm.Fz;
	this->rejectedAccelerationForces->Mx = fm.Mx;
	this->rejectedAccelerationForces->My = fm.My;
	this->rejectedAccelerationForces->Mz = fm.Mz;
	this->rejectedAccelerationForces->ForceMagnitude = fm.ForceMagnitude;
}

void ForceController::setRejectedActiveOpticForcesAndMoments() {
	ForcesAndMoments fm = this->calculateForcesAndMoments(this->zero, this->zero, this->rejectedActiveOpticForces->ZForces);
	this->rejectedActiveOpticForces->Fz = fm.Fz;
	this->rejectedActiveOpticForces->Mx = fm.Mx;
	this->rejectedActiveOpticForces->My = fm.My;
}

void ForceController::setRejectedAzimuthForcesAndMoments() {
	ForcesAndMoments fm = this->calculateForcesAndMoments(this->rejectedAzimuthForces->XForces, this->rejectedAzimuthForces->YForces, this->rejectedAzimuthForces->ZForces);
	this->rejectedAzimuthForces->Fx = fm.Fx;
	this->rejectedAzimuthForces->Fy = fm.Fy;
	this->rejectedAzimuthForces->Fz = fm.Fz;
	this->rejectedAzimuthForces->Mx = fm.Mx;
	this->rejectedAzimuthForces->My = fm.My;
	this->rejectedAzimuthForces->Mz = fm.Mz;
	this->rejectedAzimuthForces->ForceMagnitude = fm.ForceMagnitude;
}

void ForceController::setRejectedBalanceForcesAndMoments() {
	ForcesAndMoments fm = this->calculateForcesAndMoments(this->rejectedBalanceForces->XForces, this->rejectedBalanceForces->YForces, this->rejectedBalanceForces->ZForces);
	this->rejectedBalanceForces->Fx = fm.Fx;
	this->rejectedBalanceForces->Fy = fm.Fy;
	this->rejectedBalanceForces->Fz = fm.Fz;
	this->rejectedBalanceForces->Mx = fm.Mx;
	this->rejectedBalanceForces->My = fm.My;
	this->rejectedBalanceForces->Mz = fm.Mz;
	this->rejectedBalanceForces->ForceMagnitude = fm.ForceMagnitude;
}

void ForceController::setRejectedElevationForcesAndMoments() {
	ForcesAndMoments fm = this->calculateForcesAndMoments(this->rejectedElevationForces->XForces, this->rejectedElevationForces->YForces, this->rejectedElevationForces->ZForces);
	this->rejectedElevationForces->Fx = fm.Fx;
	this->rejectedElevationForces->Fy = fm.Fy;
	this->rejectedElevationForces->Fz = fm.Fz;
	this->rejectedElevationForces->Mx = fm.Mx;
	this->rejectedElevationForces->My = fm.My;
	this->rejectedElevationForces->Mz = fm.Mz;
	this->rejectedElevationForces->ForceMagnitude = fm.ForceMagnitude;
}

void ForceController::setRejectedForcesAndMoments() {
	ForcesAndMoments fm = this->calculateForcesAndMoments(this->rejectedForces->XForces, this->rejectedForces->YForces, this->rejectedForces->ZForces);
	this->rejectedForces->Fx = fm.Fx;
	this->rejectedForces->Fy = fm.Fy;
	this->rejectedForces->Fz = fm.Fz;
	this->rejectedForces->Mx = fm.Mx;
	this->rejectedForces->My = fm.My;
	this->rejectedForces->Mz = fm.Mz;
	this->rejectedForces->ForceMagnitude = fm.ForceMagnitude;
}

void ForceController::setRejectedOffsetForcesAndMoments() {
	ForcesAndMoments fm = this->calculateForcesAndMoments(this->rejectedOffsetForces->XForces, this->rejectedOffsetForces->YForces, this->rejectedOffsetForces->ZForces);
	this->rejectedOffsetForces->Fx = fm.Fx;
	this->rejectedOffsetForces->Fy = fm.Fy;
	this->rejectedOffsetForces->Fz = fm.Fz;
	this->rejectedOffsetForces->Mx = fm.Mx;
	this->rejectedOffsetForces->My = fm.My;
	this->rejectedOffsetForces->Mz = fm.Mz;
	this->rejectedOffsetForces->ForceMagnitude = fm.ForceMagnitude;
}

void ForceController::setRejectedStaticForcesAndMoments() {
	ForcesAndMoments fm = this->calculateForcesAndMoments(this->rejectedStaticForces->XForces, this->rejectedStaticForces->YForces, this->rejectedStaticForces->ZForces);
	this->rejectedStaticForces->Fx = fm.Fx;
	this->rejectedStaticForces->Fy = fm.Fy;
	this->rejectedStaticForces->Fz = fm.Fz;
	this->rejectedStaticForces->Mx = fm.Mx;
	this->rejectedStaticForces->My = fm.My;
	this->rejectedStaticForces->Mz = fm.Mz;
	this->rejectedStaticForces->ForceMagnitude = fm.ForceMagnitude;
}

void ForceController::setRejectedThermalForcesAndMoments() {
	ForcesAndMoments fm = this->calculateForcesAndMoments(this->rejectedThermalForces->XForces, this->rejectedThermalForces->YForces, this->rejectedThermalForces->ZForces);
	this->rejectedThermalForces->Fx = fm.Fx;
	this->rejectedThermalForces->Fy = fm.Fy;
	this->rejectedThermalForces->Fz = fm.Fz;
	this->rejectedThermalForces->Mx = fm.Mx;
	this->rejectedThermalForces->My = fm.My;
	this->rejectedThermalForces->Mz = fm.Mz;
	this->rejectedThermalForces->ForceMagnitude = fm.ForceMagnitude;
}

void ForceController::setRejectedVelocityForcesAndMoments() {
	ForcesAndMoments fm = this->calculateForcesAndMoments(this->rejectedVelocityForces->XForces, this->rejectedVelocityForces->YForces, this->rejectedVelocityForces->ZForces);
	this->rejectedVelocityForces->Fx = fm.Fx;
	this->rejectedVelocityForces->Fy = fm.Fy;
	this->rejectedVelocityForces->Fz = fm.Fz;
	this->rejectedVelocityForces->Mx = fm.Mx;
	this->rejectedVelocityForces->My = fm.My;
	this->rejectedVelocityForces->Mz = fm.Mz;
	this->rejectedVelocityForces->ForceMagnitude = fm.ForceMagnitude;
}

void ForceController::sumAllForces() {
	Log.Trace("ForceController: sumAllForces()");
	bool rejectionRequired = false;
	for(int zIndex = 0; zIndex < FA_COUNT; ++zIndex) {
		int xIndex = this->forceActuatorApplicationSettings->ZIndexToXIndex[zIndex];
		int yIndex = this->forceActuatorApplicationSettings->ZIndexToYIndex[zIndex];

		this->forceSetpointWarning->ForceWarning[zIndex] = false;

		if (xIndex != -1) {
			float xLowFault = this->forceActuatorSettings->ForceLimitXTable[xIndex].LowFault;
			float xHighFault = this->forceActuatorSettings->ForceLimitXTable[xIndex].HighFault;
			this->rejectedForces->XForces[xIndex] =
				this->appliedAccelerationForces->XForces[xIndex] +
				this->appliedAzimuthForces->XForces[xIndex] +
				this->appliedBalanceForces->XForces[xIndex] +
				this->appliedElevationForces->XForces[xIndex] +
				this->appliedOffsetForces->XForces[xIndex] +
				this->appliedStaticForces->XForces[xIndex] +
				this->appliedThermalForces->XForces[xIndex] +
				this->appliedVelocityForces->XForces[xIndex];
			this->forceSetpointWarning->ForceWarning[zIndex] = this->forceSetpointWarning->ForceWarning[zIndex] ||
				!Range::InRangeAndCoerce(xLowFault, xHighFault, this->rejectedForces->XForces[xIndex], this->appliedForces->XForces + xIndex);
		}

		if (yIndex != -1) {
			float yLowFault = this->forceActuatorSettings->ForceLimitYTable[yIndex].LowFault;
			float yHighFault = this->forceActuatorSettings->ForceLimitYTable[yIndex].HighFault;
			this->rejectedForces->YForces[yIndex] =
				this->appliedAccelerationForces->YForces[yIndex] +
				this->appliedAzimuthForces->YForces[yIndex] +
				this->appliedBalanceForces->YForces[yIndex] +
				this->appliedElevationForces->YForces[yIndex] +
				this->appliedOffsetForces->YForces[yIndex] +
				this->appliedStaticForces->YForces[yIndex] +
				this->appliedThermalForces->YForces[yIndex] +
				this->appliedVelocityForces->YForces[yIndex];
			this->forceSetpointWarning->ForceWarning[zIndex] = this->forceSetpointWarning->ForceWarning[zIndex] ||
				!Range::InRangeAndCoerce(yLowFault, yHighFault, this->rejectedForces->YForces[yIndex], this->appliedForces->YForces + yIndex);
		}

		float zLowFault = this->forceActuatorSettings->ForceLimitZTable[zIndex].LowFault;
		float zHighFault = this->forceActuatorSettings->ForceLimitZTable[zIndex].HighFault;
		this->rejectedForces->ZForces[zIndex] =
			this->appliedAberrationForces->ZForces[zIndex] +
			this->appliedAccelerationForces->ZForces[zIndex] +
			this->appliedActiveOpticForces->ZForces[zIndex] +
			this->appliedAzimuthForces->ZForces[zIndex] +
			this->appliedBalanceForces->ZForces[zIndex] +
			this->appliedElevationForces->ZForces[zIndex] +
			this->appliedOffsetForces->ZForces[zIndex] +
			this->appliedStaticForces->ZForces[zIndex] +
			this->appliedThermalForces->ZForces[zIndex] +
			this->appliedVelocityForces->ZForces[zIndex];
		this->forceSetpointWarning->ForceWarning[zIndex] = this->forceSetpointWarning->ForceWarning[zIndex] ||
			!Range::InRangeAndCoerce(zLowFault, zHighFault, this->rejectedForces->ZForces[zIndex], this->appliedForces->ZForces + zIndex);
		rejectionRequired = rejectionRequired || this->forceSetpointWarning->SafetyLimitWarning[zIndex];
	}
	this->setAppliedForcesAndMoments();
	this->setRejectedForcesAndMoments();
	this->safetyController->forceControllerNotifyForceClipping(rejectionRequired);
	this->publisher->tryLogForceSetpointWarning();
	if (rejectionRequired) {
		this->publisher->logRejectedForces();
	}
	this->publisher->tryLogAppliedForces();
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
	this->forceSetpointWarning->XMomentWarning = !Range::InRange(this->forceActuatorSettings->MirrorXMoment * this->forceActuatorSettings->SetpointXMomentLowLimitPercentage, this->forceActuatorSettings->MirrorXMoment * this->forceActuatorSettings->SetpointXMomentHighLimitPercentage, xMoment);
	this->forceSetpointWarning->YMomentWarning = !Range::InRange(this->forceActuatorSettings->MirrorYMoment * this->forceActuatorSettings->SetpointYMomentLowLimitPercentage, this->forceActuatorSettings->MirrorYMoment * this->forceActuatorSettings->SetpointYMomentHighLimitPercentage, yMoment);
	this->forceSetpointWarning->ZMomentWarning = !Range::InRange(this->forceActuatorSettings->MirrorZMoment * this->forceActuatorSettings->SetpointZMomentLowLimitPercentage, this->forceActuatorSettings->MirrorZMoment * this->forceActuatorSettings->SetpointZMomentHighLimitPercentage, zMoment);
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

PID* ForceController::idToPID(int id) {
	switch(id) {
	case 1: return &this->fx;
	case 2: return &this->fy;
	case 3: return &this->fz;
	case 4: return &this->mx;
	case 5: return &this->my;
	case 6: return &this->mz;
	default: return 0;
	}
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
