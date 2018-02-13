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
#include <SAL_m1m3C.h>
#include <cmath>
#include <cstring>
#include <vector>

#include <iostream>
using namespace std;

namespace LSST {
namespace M1M3 {
namespace SS {

ForceController::ForceController(ForceActuatorApplicationSettings* forceActuatorApplicationSettings, ForceActuatorSettings* forceActuatorSettings, PIDSettings* pidSettings, M1M3SSPublisher* publisher, SafetyController* safetyController)
 : fx(0, pidSettings->Fx),
   fy(1, pidSettings->Fy),
   fz(2, pidSettings->Fz),
   mx(3, pidSettings->Mx),
   my(4, pidSettings->My),
   mz(5, pidSettings->Mz) {
	this->forceActuatorApplicationSettings = forceActuatorApplicationSettings;
	this->forceActuatorSettings = forceActuatorSettings;
	this->publisher = publisher;
	this->safetyController = safetyController;
	this->pidSettings = pidSettings;
	this->appliedForces = this->publisher->getEventAppliedForces();
	this->forceInfo = this->publisher->getEventForceActuatorInfo();
	this->inclinometerData = this->publisher->getInclinometerData();
	this->forceData = this->publisher->getForceActuatorData();
	this->forceSetpoint = this->publisher->getEventForceActuatorDataRejection();
	this->forceSetpointWarning = this->publisher->getEventForceActuatorSetpointWarning();
	this->pidData = this->publisher->getPIDData();
	this->pidInfo = this->publisher->getEventPIDInfo();
	this->hardpointData = this->publisher->getHardpointData();
	this->mirrorForceData = this->publisher->getMirrorForceData();
	memset(this->appliedForces, 0, sizeof(m1m3_logevent_AppliedForcesC));
	memset(this->forceData, 0, sizeof(m1m3_ForceActuatorDataC));
	memset(this->forceSetpoint, 0, sizeof(m1m3_logevent_ForceActuatorDataRejectionC));
	memset(this->forceSetpointWarning, 0, sizeof(m1m3_logevent_ForceActuatorSetpointWarningC));
	memset(&this->tmaAzimuthData, 0, sizeof(MTMount_AzC));
	memset(&this->tmaElevationData, 0, sizeof(MTMount_AltC));
	this->publishAppliedForces();
	for(int i = 0; i < FA_COUNT; i++) {
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

void ForceController::updateTMAAzimuthData(MTMount_AzC* tmaAzimuthData) {
	memcpy(&this->tmaAzimuthData, tmaAzimuthData, sizeof(MTMount_AzC));
}

void ForceController::updateTMAElevationData(MTMount_AltC* tmaElevationData) {
	memcpy(&this->tmaElevationData, tmaElevationData, sizeof(MTMount_AltC));
}

void ForceController::incSupportPercentage() {
	this->forceSetpoint->ElevationSetpointPercentage += this->forceActuatorSettings->RaiseIncrementPercentage;
	if (this->supportPercentageFilled()) {
		this->forceSetpoint->ElevationSetpointPercentage = 1.0;
	}
	this->forceData->ElevationSetpointPercentage = this->forceSetpoint->ElevationSetpointPercentage;
}

void ForceController::decSupportPercentage() {
	this->forceSetpoint->ElevationSetpointPercentage -= this->forceActuatorSettings->LowerDecrementPercentage;
	if (this->supportPercentageZeroed()) {
		this->forceSetpoint->ElevationSetpointPercentage = 0.0;
	}
	this->forceData->ElevationSetpointPercentage = this->forceSetpoint->ElevationSetpointPercentage;
}

void ForceController::zeroSupportPercentage() {
	this->forceSetpoint->ElevationSetpointPercentage = 0.0;
	this->forceData->ElevationSetpointPercentage = 0.0;
}

void ForceController::fillSupportPercentage() {
	this->forceSetpoint->ElevationSetpointPercentage = 1.0;
	this->forceData->ElevationSetpointPercentage = 1.0;
}

bool ForceController::supportPercentageFilled() {
	return this->forceSetpoint->ElevationSetpointPercentage >= 1.0;
}

bool ForceController::supportPercentageZeroed() {
	return this->forceSetpoint->ElevationSetpointPercentage <= 0.0;
}

void ForceController::updateAppliedForces() {
	if (this->appliedForces->ElevationForcesApplied) {
		this->updateElevationForces();
	}
	if (this->appliedForces->AzimuthForcesApplied) {
		this->updateAzimuthForces();
	}
	if (this->appliedForces->TemperatureForcesApplied) {
		this->updateTemperatureForces();
	}
	if (this->appliedForces->HardpointOffloadingForcesApplied) {
		this->updateHardpointCorrectionForces();
	}
}

void ForceController::processAppliedForces() {
	this->sumAllForces();
	this->convertForcesToSetpoints();
	bool warningChanged = false;
	warningChanged = warningChanged || this->checkMirrorMoments();
	warningChanged = warningChanged || this->checkNearNeighbors();
	warningChanged = warningChanged || this->checkMirrorWeight();
	warningChanged = warningChanged || this->checkFarNeighbors();
	if (warningChanged) {
		this->publishForceSetpointWarning();
	}
}

void ForceController::applyStaticForces() {
	bool warningChanged = false;
	this->forceSetpointWarning->AnyStaticForceWarning = false;
	for(int i = 0; i < FA_COUNT; ++i) {
		// Stage Static Forces
		this->forceSetpoint->StaticXSetpoint[i] = this->forceInfo->StaticXSetpoint[i];
		this->forceSetpoint->StaticYSetpoint[i] = this->forceInfo->StaticYSetpoint[i];
		this->forceSetpoint->StaticZSetpoint[i] = this->forceInfo->StaticZSetpoint[i];
		// Check Static Forces
		bool previousWarning = this->forceSetpointWarning->StaticForceWarning[i];
		this->forceSetpointWarning->StaticForceWarning[i] =
				!Range::InRange(this->forceActuatorSettings->Limits[i].StaticXLowLimit, this->forceActuatorSettings->Limits[i].StaticXHighLimit, this->forceSetpoint->StaticXSetpoint[i]) ||
				!Range::InRange(this->forceActuatorSettings->Limits[i].StaticYLowLimit, this->forceActuatorSettings->Limits[i].StaticYHighLimit, this->forceSetpoint->StaticYSetpoint[i]) ||
				!Range::InRange(this->forceActuatorSettings->Limits[i].StaticZLowLimit, this->forceActuatorSettings->Limits[i].StaticZHighLimit, this->forceSetpoint->StaticZSetpoint[i]);
		this->forceSetpointWarning->AnyStaticForceWarning = this->forceSetpointWarning->AnyStaticForceWarning || this->forceSetpointWarning->StaticForceWarning[i];
		warningChanged = warningChanged || (this->forceSetpointWarning->StaticForceWarning[i] != previousWarning);
	}
	this->safetyController->forceControllerNotifyStaticForceClipping(this->forceSetpointWarning->AnyStaticForceWarning);
	if (warningChanged) {
		this->publishForceSetpointWarning();
	}
	if (this->forceSetpointWarning->AnyStaticForceWarning) {
		// Coerce Static Forces
		this->publishForceDataRejection();
		for(int i = 0; i < FA_COUNT; ++i) {
			this->forceSetpoint->StaticXSetpoint[i] = Range::CoerceIntoRange(this->forceActuatorSettings->Limits[i].StaticXLowLimit, this->forceActuatorSettings->Limits[i].StaticXHighLimit, this->forceSetpoint->StaticXSetpoint[i]);
			this->forceSetpoint->StaticYSetpoint[i] = Range::CoerceIntoRange(this->forceActuatorSettings->Limits[i].StaticYLowLimit, this->forceActuatorSettings->Limits[i].StaticYHighLimit, this->forceSetpoint->StaticYSetpoint[i]);
			this->forceSetpoint->StaticZSetpoint[i] = Range::CoerceIntoRange(this->forceActuatorSettings->Limits[i].StaticZLowLimit, this->forceActuatorSettings->Limits[i].StaticZHighLimit, this->forceSetpoint->StaticZSetpoint[i]);
		}
	}
	// Move Static Forces
	for(int i = 0; i < FA_COUNT; ++i) {
		this->forceData->StaticXSetpoint[i] = this->forceSetpoint->StaticXSetpoint[i];
		this->forceData->StaticYSetpoint[i] = this->forceSetpoint->StaticYSetpoint[i];
		this->forceData->StaticZSetpoint[i] = this->forceSetpoint->StaticZSetpoint[i];
	}
	this->appliedForces->StaticForcesApplied = true;
	this->publishAppliedForces();
}

void ForceController::zeroStaticForces() {
	this->appliedForces->StaticForcesApplied = false;
	bool warningChanged = this->forceSetpointWarning->AnyStaticForceWarning;
	for(int i = 0; i < FA_COUNT; ++i) {
		this->forceSetpoint->StaticXSetpoint[i] = 0;
		this->forceSetpoint->StaticYSetpoint[i] = 0;
		this->forceSetpoint->StaticZSetpoint[i] = 0;
		this->forceData->StaticXSetpoint[i] = 0;
		this->forceData->StaticYSetpoint[i] = 0;
		this->forceData->StaticZSetpoint[i] = 0;
		this->forceSetpointWarning->StaticForceWarning[i] = false;
	}
	this->forceSetpointWarning->AnyStaticForceWarning = false;
	this->safetyController->forceControllerNotifyStaticForceClipping(this->forceSetpointWarning->AnyStaticForceWarning);
	if (warningChanged) {
		this->publishForceSetpointWarning();
	}
	this->publishAppliedForces();
}

void ForceController::applyOffsetForces(double* x, double* y, double* z) {
	bool warningChanged = false;
	this->forceSetpointWarning->AnyOffsetForceWarning = false;
	for(int i = 0; i < FA_COUNT; ++i) {
		// Stage Offset Forces
		this->forceSetpoint->OffsetXSetpoint[i] = x[i];
		this->forceSetpoint->OffsetYSetpoint[i] = y[i];
		this->forceSetpoint->OffsetZSetpoint[i] = z[i];
		// Check Offset Forces
		bool previousWarning = this->forceSetpointWarning->OffsetForceWarning[i];
		this->forceSetpointWarning->OffsetForceWarning[i] =
				!Range::InRange(this->forceActuatorSettings->Limits[i].OffsetXLowLimit, this->forceActuatorSettings->Limits[i].OffsetXHighLimit, this->forceSetpoint->OffsetXSetpoint[i]) ||
				!Range::InRange(this->forceActuatorSettings->Limits[i].OffsetYLowLimit, this->forceActuatorSettings->Limits[i].OffsetYHighLimit, this->forceSetpoint->OffsetYSetpoint[i]) ||
				!Range::InRange(this->forceActuatorSettings->Limits[i].OffsetZLowLimit, this->forceActuatorSettings->Limits[i].OffsetZHighLimit, this->forceSetpoint->OffsetZSetpoint[i]);
		this->forceSetpointWarning->AnyOffsetForceWarning = this->forceSetpointWarning->AnyOffsetForceWarning || this->forceSetpointWarning->OffsetForceWarning[i];
		warningChanged = warningChanged || (this->forceSetpointWarning->StaticForceWarning[i] != previousWarning);
	}
	this->safetyController->forceControllerNotifyOffsetForceClipping(this->forceSetpointWarning->AnyOffsetForceWarning);
	if (warningChanged) {
		this->publishForceSetpointWarning();
	}
	if (this->forceSetpointWarning->AnyOffsetForceWarning) {
		// Coerce Offset Forces
		this->publishForceDataRejection();
		for(int i = 0; i < FA_COUNT; ++i) {
			this->forceSetpoint->OffsetXSetpoint[i] = Range::CoerceIntoRange(this->forceActuatorSettings->Limits[i].OffsetXLowLimit, this->forceActuatorSettings->Limits[i].OffsetXHighLimit, this->forceSetpoint->OffsetXSetpoint[i]);
			this->forceSetpoint->OffsetYSetpoint[i] = Range::CoerceIntoRange(this->forceActuatorSettings->Limits[i].OffsetYLowLimit, this->forceActuatorSettings->Limits[i].OffsetYHighLimit, this->forceSetpoint->OffsetYSetpoint[i]);
			this->forceSetpoint->OffsetZSetpoint[i] = Range::CoerceIntoRange(this->forceActuatorSettings->Limits[i].OffsetZLowLimit, this->forceActuatorSettings->Limits[i].OffsetZHighLimit, this->forceSetpoint->OffsetZSetpoint[i]);
		}
	}
	// Move Offset Forces
	for(int i = 0; i < FA_COUNT; ++i) {
		this->forceData->OffsetXSetpoint[i] = this->forceSetpoint->OffsetXSetpoint[i];
		this->forceData->OffsetYSetpoint[i] = this->forceSetpoint->OffsetYSetpoint[i];
		this->forceData->OffsetZSetpoint[i] = this->forceSetpoint->OffsetZSetpoint[i];
	}
	this->appliedForces->OffsetForcesApplied = true;
	this->publishAppliedForces();
}

void ForceController::applyOffsetForces(float xForce, float yForce, float zForce, float xMoment, float yMoment, float zMoment) {
	DistributedForces forces = this->calculateDistribution(xForce, yForce, zForce, xMoment, yMoment, zMoment);
	this->applyOffsetForces(forces.XForces, forces.YForces, forces.ZForces);
}

void ForceController::zeroOffsetForces() {
	this->appliedForces->OffsetForcesApplied = false;
	bool warningChanged = this->forceSetpointWarning->AnyOffsetForceWarning;
	for(int i = 0; i < FA_COUNT; ++i) {
		this->forceSetpoint->OffsetXSetpoint[i] = 0;
		this->forceSetpoint->OffsetYSetpoint[i] = 0;
		this->forceSetpoint->OffsetZSetpoint[i] = 0;
		this->forceData->OffsetXSetpoint[i] = 0;
		this->forceData->OffsetYSetpoint[i] = 0;
		this->forceData->OffsetZSetpoint[i] = 0;
		this->forceSetpointWarning->OffsetForceWarning[i] = false;
	}
	this->forceSetpointWarning->AnyOffsetForceWarning = false;
	this->safetyController->forceControllerNotifyOffsetForceClipping(this->forceSetpointWarning->AnyOffsetForceWarning);
	if (warningChanged) {
		this->publishForceSetpointWarning();
	}
	this->publishAppliedForces();
}

void ForceController::applyAOSCorrectionByBendingModes(double* coefficients) {
	double netAOSForce = 0;
	bool warningChanged = false;
	this->forceSetpointWarning->AnyAOSForceWarning = false;
	for(int i = 0; i < FA_COUNT; ++i) {
		// Stage AOS Correction Forces
		int mIndex = i * 22;
		this->forceSetpoint->ActiveOpticsZSetpoint[i] =
				this->forceActuatorSettings->BendingModeMatrix[mIndex + 0] * coefficients[0] +
				this->forceActuatorSettings->BendingModeMatrix[mIndex + 1] * coefficients[1] +
				this->forceActuatorSettings->BendingModeMatrix[mIndex + 2] * coefficients[2] +
				this->forceActuatorSettings->BendingModeMatrix[mIndex + 3] * coefficients[3] +
				this->forceActuatorSettings->BendingModeMatrix[mIndex + 4] * coefficients[4] +
				this->forceActuatorSettings->BendingModeMatrix[mIndex + 5] * coefficients[5] +
				this->forceActuatorSettings->BendingModeMatrix[mIndex + 6] * coefficients[6] +
				this->forceActuatorSettings->BendingModeMatrix[mIndex + 7] * coefficients[7] +
				this->forceActuatorSettings->BendingModeMatrix[mIndex + 8] * coefficients[8] +
				this->forceActuatorSettings->BendingModeMatrix[mIndex + 9] * coefficients[9] +
				this->forceActuatorSettings->BendingModeMatrix[mIndex + 10] * coefficients[10] +
				this->forceActuatorSettings->BendingModeMatrix[mIndex + 11] * coefficients[11] +
				this->forceActuatorSettings->BendingModeMatrix[mIndex + 12] * coefficients[12] +
				this->forceActuatorSettings->BendingModeMatrix[mIndex + 13] * coefficients[13] +
				this->forceActuatorSettings->BendingModeMatrix[mIndex + 14] * coefficients[14] +
				this->forceActuatorSettings->BendingModeMatrix[mIndex + 15] * coefficients[15] +
				this->forceActuatorSettings->BendingModeMatrix[mIndex + 16] * coefficients[16] +
				this->forceActuatorSettings->BendingModeMatrix[mIndex + 17] * coefficients[17] +
				this->forceActuatorSettings->BendingModeMatrix[mIndex + 18] * coefficients[18] +
				this->forceActuatorSettings->BendingModeMatrix[mIndex + 19] * coefficients[19] +
				this->forceActuatorSettings->BendingModeMatrix[mIndex + 20] * coefficients[20] +
				this->forceActuatorSettings->BendingModeMatrix[mIndex + 21] * coefficients[21];
		// Check AOS Correction Forces
		netAOSForce += this->forceSetpoint->ActiveOpticsZSetpoint[i];
		bool previousWarning = this->forceSetpointWarning->AOSForceWarning[i];
		this->forceSetpointWarning->AOSForceWarning[i] =
				!Range::InRange(this->forceActuatorSettings->Limits[i].AOSZLowLimit, this->forceActuatorSettings->Limits[i].AOSZHighLimit, this->forceSetpoint->ActiveOpticsZSetpoint[i]);
		this->forceSetpointWarning->AnyAOSForceWarning = this->forceSetpointWarning->AnyAOSForceWarning || this->forceSetpointWarning->AOSForceWarning[i];
		warningChanged = warningChanged || (this->forceSetpointWarning->StaticForceWarning[i] != previousWarning);
	}
	if (!Range::InRange(-this->forceActuatorSettings->NetAOSForceTolerance, this->forceActuatorSettings->NetAOSForceTolerance, netAOSForce)) {
		bool previousWarning = this->forceSetpointWarning->AOSNetForceWarning;
		this->forceSetpointWarning->AOSNetForceWarning = true;
		warningChanged = warningChanged || (this->forceSetpointWarning->AOSNetForceWarning != previousWarning);
	}
	this->safetyController->forceControllerNotifyAOSForceClipping(this->forceSetpointWarning->AnyAOSForceWarning);
	this->safetyController->forceControllerNotifyAOSNetForceCheck(this->forceSetpointWarning->AOSNetForceWarning);
	if (warningChanged) {
		this->publishForceSetpointWarning();
	}
	if (this->forceSetpointWarning->AnyAOSForceWarning || this->forceSetpointWarning->AOSNetForceWarning) {
		// Coerce AOS Correction Forces
		this->publishForceDataRejection();
		for(int i = 0; i < FA_COUNT; i++) {
			this->forceSetpoint->ActiveOpticsZSetpoint[i] = Range::CoerceIntoRange(this->forceActuatorSettings->Limits[i].AOSZLowLimit, this->forceActuatorSettings->Limits[i].AOSZHighLimit, this->forceSetpoint->ActiveOpticsZSetpoint[i]);
		}
	}
	// Move AOS Correction Forces
	for(int i = 0; i < FA_COUNT; i++) {
		this->forceData->ActiveOpticsZSetpoint[i] = this->forceSetpoint->ActiveOpticsZSetpoint[i];
	}
	this->appliedForces->AOSCorrectionApplied = true;
	this->publishAppliedForces();
}

void ForceController::applyAOSCorrectionByForces(double* z) {
	double netAOSForce = 0;
	bool warningChanged = false;
	this->forceSetpointWarning->AnyAOSForceWarning = false;
	for(int i = 0; i < FA_COUNT; ++i) {
		// Stage AOS Correction Forces
		this->forceSetpoint->ActiveOpticsZSetpoint[i] = z[i];
		// Check AOS Correction Forces
		netAOSForce += this->forceSetpoint->ActiveOpticsZSetpoint[i];
		bool previousWarning = this->forceSetpointWarning->AOSForceWarning[i];
		this->forceSetpointWarning->AOSForceWarning[i] =
				!Range::InRange(this->forceActuatorSettings->Limits[i].AOSZLowLimit, this->forceActuatorSettings->Limits[i].AOSZHighLimit, this->forceSetpoint->ActiveOpticsZSetpoint[i]);
		this->forceSetpointWarning->AnyAOSForceWarning = this->forceSetpointWarning->AnyAOSForceWarning || this->forceSetpointWarning->AOSForceWarning[i];
		warningChanged = warningChanged || (this->forceSetpointWarning->AOSForceWarning[i] != previousWarning);
	}
	if (!Range::InRange(-this->forceActuatorSettings->NetAOSForceTolerance, this->forceActuatorSettings->NetAOSForceTolerance, netAOSForce)) {
		bool previousWarning = this->forceSetpointWarning->AOSNetForceWarning;
		this->forceSetpointWarning->AOSNetForceWarning = true;
		warningChanged = warningChanged || (this->forceSetpointWarning->AOSNetForceWarning != previousWarning);
	}
	this->safetyController->forceControllerNotifyAOSForceClipping(this->forceSetpointWarning->AnyAOSForceWarning);
	this->safetyController->forceControllerNotifyAOSNetForceCheck(this->forceSetpointWarning->AOSNetForceWarning);
	if (warningChanged) {
		this->publishForceSetpointWarning();
	}
	if (this->forceSetpointWarning->AnyAOSForceWarning) {
		// Coerce AOS Correction Forces
		this->publishForceDataRejection();
		for(int i = 0; i < FA_COUNT; i++) {
			this->forceSetpoint->ActiveOpticsZSetpoint[i] = Range::CoerceIntoRange(this->forceActuatorSettings->Limits[i].AOSZLowLimit, this->forceActuatorSettings->Limits[i].AOSZHighLimit, this->forceSetpoint->ActiveOpticsZSetpoint[i]);
		}
	}
	// Move AOS Correction Forces
	for(int i = 0; i < FA_COUNT; i++) {
		this->forceData->ActiveOpticsZSetpoint[i] = this->forceSetpoint->ActiveOpticsZSetpoint[i];
	}
	this->appliedForces->AOSCorrectionApplied = true;
	this->publishAppliedForces();
}

void ForceController::zeroAOSCorrection() {
	this->appliedForces->AOSCorrectionApplied = false;
	bool warningChanged = this->forceSetpointWarning->AnyAOSForceWarning || this->forceSetpointWarning->AOSNetForceWarning;
	for(int i = 0; i < FA_COUNT; ++i) {
		this->forceSetpoint->ActiveOpticsZSetpoint[i] = 0;
		this->forceData->ActiveOpticsZSetpoint[i] = 0;
		this->forceSetpointWarning->AOSForceWarning[i] = false;
	}
	this->forceSetpointWarning->AnyAOSForceWarning = false;
	this->forceSetpointWarning->AOSNetForceWarning = false;
	this->safetyController->forceControllerNotifyAOSForceClipping(this->forceSetpointWarning->AnyAOSForceWarning);
	this->safetyController->forceControllerNotifyAOSNetForceCheck(this->forceSetpointWarning->AOSNetForceWarning);
	if (warningChanged) {
		this->publishForceSetpointWarning();
	}
	this->publishAppliedForces();
}

void ForceController::applyAberrationByBendingModes(double* coefficients) {
	double netAberrationForce = 0;
	bool warningChanged = false;
	this->forceSetpointWarning->AnyAberrationForceWarning = false;
	for(int i = 0; i < FA_COUNT; ++i) {
		// Stage Aberration Forces
		int mIndex = i * 22;
		this->forceSetpoint->AberrationZSetpoint[i] =
				this->forceActuatorSettings->BendingModeMatrix[mIndex + 0] * coefficients[0] +
				this->forceActuatorSettings->BendingModeMatrix[mIndex + 1] * coefficients[1] +
				this->forceActuatorSettings->BendingModeMatrix[mIndex + 2] * coefficients[2] +
				this->forceActuatorSettings->BendingModeMatrix[mIndex + 3] * coefficients[3] +
				this->forceActuatorSettings->BendingModeMatrix[mIndex + 4] * coefficients[4] +
				this->forceActuatorSettings->BendingModeMatrix[mIndex + 5] * coefficients[5] +
				this->forceActuatorSettings->BendingModeMatrix[mIndex + 6] * coefficients[6] +
				this->forceActuatorSettings->BendingModeMatrix[mIndex + 7] * coefficients[7] +
				this->forceActuatorSettings->BendingModeMatrix[mIndex + 8] * coefficients[8] +
				this->forceActuatorSettings->BendingModeMatrix[mIndex + 9] * coefficients[9] +
				this->forceActuatorSettings->BendingModeMatrix[mIndex + 10] * coefficients[10] +
				this->forceActuatorSettings->BendingModeMatrix[mIndex + 11] * coefficients[11] +
				this->forceActuatorSettings->BendingModeMatrix[mIndex + 12] * coefficients[12] +
				this->forceActuatorSettings->BendingModeMatrix[mIndex + 13] * coefficients[13] +
				this->forceActuatorSettings->BendingModeMatrix[mIndex + 14] * coefficients[14] +
				this->forceActuatorSettings->BendingModeMatrix[mIndex + 15] * coefficients[15] +
				this->forceActuatorSettings->BendingModeMatrix[mIndex + 16] * coefficients[16] +
				this->forceActuatorSettings->BendingModeMatrix[mIndex + 17] * coefficients[17] +
				this->forceActuatorSettings->BendingModeMatrix[mIndex + 18] * coefficients[18] +
				this->forceActuatorSettings->BendingModeMatrix[mIndex + 19] * coefficients[19] +
				this->forceActuatorSettings->BendingModeMatrix[mIndex + 20] * coefficients[20] +
				this->forceActuatorSettings->BendingModeMatrix[mIndex + 21] * coefficients[21];
		// Check Aberration Forces
		netAberrationForce += this->forceSetpoint->AberrationZSetpoint[i];
		bool previousWarning = this->forceSetpointWarning->AberrationForceWarning[i];
		this->forceSetpointWarning->AberrationForceWarning[i] =
				!Range::InRange(this->forceActuatorSettings->Limits[i].AberrationZLowLimit, this->forceActuatorSettings->Limits[i].AberrationZHighLimit, this->forceSetpoint->AberrationZSetpoint[i]);
		this->forceSetpointWarning->AnyAberrationForceWarning = this->forceSetpointWarning->AnyAberrationForceWarning || this->forceSetpointWarning->AberrationForceWarning[i];
		warningChanged = warningChanged || (this->forceSetpointWarning->AberrationForceWarning[i] != previousWarning);
	}
	if (!Range::InRange(-this->forceActuatorSettings->NetAberrationForceTolerance, this->forceActuatorSettings->NetAberrationForceTolerance, netAberrationForce)) {
		bool previousWarning = this->forceSetpointWarning->AberrationNetForceWarning;
		this->forceSetpointWarning->AberrationNetForceWarning = true;
		warningChanged = warningChanged || (this->forceSetpointWarning->AberrationNetForceWarning != previousWarning);
	}
	this->safetyController->forceControllerNotifyAberrationForceClipping(this->forceSetpointWarning->AnyAberrationForceWarning);
	this->safetyController->forceControllerNotifyAberrationNetForceCheck(this->forceSetpointWarning->AberrationNetForceWarning);
	if (warningChanged) {
		this->publishForceSetpointWarning();
	}
	if (this->forceSetpointWarning->AnyAberrationForceWarning) {
		// Coerce Aberration Forces
		this->publishForceDataRejection();
		for(int i = 0; i < FA_COUNT; i++) {
			this->forceSetpoint->AberrationZSetpoint[i] = Range::CoerceIntoRange(this->forceActuatorSettings->Limits[i].AberrationZLowLimit, this->forceActuatorSettings->Limits[i].AberrationZHighLimit, this->forceSetpoint->AberrationZSetpoint[i]);
		}
	}
	// Move Aberration Forces
	for(int i = 0; i < FA_COUNT; i++) {
		this->forceData->AberrationZSetpoint[i] = this->forceSetpoint->AberrationZSetpoint[i];
	}
	this->appliedForces->AberrationApplied = true;
	this->publishAppliedForces();
}

void ForceController::applyAberrationByForces(double* z) {
	double netAberrationForce = 0;
	bool warningChanged = false;
	this->forceSetpointWarning->AnyAberrationForceWarning = false;
	for(int i = 0; i < FA_COUNT; ++i) {
		// Stage Aberration Forces
		this->forceSetpoint->AberrationZSetpoint[i] = z[i];
		// Check Aberration Forces
		netAberrationForce += this->forceSetpoint->AberrationZSetpoint[i];
		bool previousWarning = this->forceSetpointWarning->AberrationForceWarning[i];
		this->forceSetpointWarning->AberrationForceWarning[i] =
				!Range::InRange(this->forceActuatorSettings->Limits[i].AberrationZLowLimit, this->forceActuatorSettings->Limits[i].AberrationZHighLimit, this->forceSetpoint->AberrationZSetpoint[i]);
		this->forceSetpointWarning->AnyAberrationForceWarning = this->forceSetpointWarning->AnyAberrationForceWarning || this->forceSetpointWarning->AberrationForceWarning[i];
		warningChanged = warningChanged || (this->forceSetpointWarning->AberrationForceWarning[i] != previousWarning);
	}
	if (!Range::InRange(-this->forceActuatorSettings->NetAberrationForceTolerance, this->forceActuatorSettings->NetAberrationForceTolerance, netAberrationForce)) {
		bool previousWarning = this->forceSetpointWarning->AberrationNetForceWarning;
		this->forceSetpointWarning->AberrationNetForceWarning = true;
		warningChanged = warningChanged || (this->forceSetpointWarning->AberrationNetForceWarning != previousWarning);
	}
	this->safetyController->forceControllerNotifyAberrationForceClipping(this->forceSetpointWarning->AnyAberrationForceWarning);
	this->safetyController->forceControllerNotifyAberrationNetForceCheck(this->forceSetpointWarning->AberrationNetForceWarning);
	if (warningChanged) {
		this->publishForceSetpointWarning();
	}
	if (this->forceSetpointWarning->AnyAberrationForceWarning) {
		// Coerce Aberration Forces
		this->publishForceDataRejection();
		for(int i = 0; i < FA_COUNT; i++) {
			this->forceSetpoint->AberrationZSetpoint[i] = Range::CoerceIntoRange(this->forceActuatorSettings->Limits[i].AberrationZLowLimit, this->forceActuatorSettings->Limits[i].AberrationZHighLimit, this->forceSetpoint->AberrationZSetpoint[i]);
		}
	}
	// Move Aberration Forces
	for(int i = 0; i < FA_COUNT; i++) {
		this->forceData->AberrationZSetpoint[i] = this->forceSetpoint->AberrationZSetpoint[i];
	}
	this->appliedForces->AberrationApplied = true;
	this->publishAppliedForces();
}

void ForceController::zeroAberration() {
	this->appliedForces->AberrationApplied = false;
	bool warningChanged = this->forceSetpointWarning->AnyAberrationForceWarning || this->forceSetpointWarning->AberrationNetForceWarning;
	for(int i = 0; i < FA_COUNT; ++i) {
		this->forceSetpoint->AberrationZSetpoint[i] = 0;
		this->forceData->AberrationZSetpoint[i] = 0;
		this->forceSetpointWarning->AberrationForceWarning[i] = false;
	}
	this->forceSetpointWarning->AnyAberrationForceWarning = false;
	this->forceSetpointWarning->AberrationNetForceWarning = false;
	this->safetyController->forceControllerNotifyAberrationForceClipping(this->forceSetpointWarning->AnyAberrationForceWarning);
	this->safetyController->forceControllerNotifyAberrationNetForceCheck(this->forceSetpointWarning->AberrationNetForceWarning);
	if (warningChanged) {
		this->publishForceSetpointWarning();
	}
	this->publishAppliedForces();
}

void ForceController::applyElevationForces() {
	this->appliedForces->ElevationForcesApplied = true;
	this->publishAppliedForces();
}

void ForceController::zeroElevationForces() {
	this->appliedForces->ElevationForcesApplied = false;
	bool warningChanged = this->forceSetpointWarning->AnyElevationForceWarning;
	for(int i = 0; i < FA_COUNT; ++i) {
		this->forceSetpoint->ElevationXSetpoint[i] = 0;
		this->forceSetpoint->ElevationYSetpoint[i] = 0;
		this->forceSetpoint->ElevationZSetpoint[i] = 0;
		this->forceData->ElevationXSetpoint[i] = 0;
		this->forceData->ElevationYSetpoint[i] = 0;
		this->forceData->ElevationZSetpoint[i] = 0;
		this->forceSetpointWarning->ElevationForceWarning[i] = false;
	}
	this->forceSetpointWarning->AnyElevationForceWarning = false;
	this->safetyController->forceControllerNotifyElevationForceClipping(this->forceSetpointWarning->AnyElevationForceWarning);
	if (warningChanged) {
		this->publishForceSetpointWarning();
	}
	this->publishAppliedForces();
}

void ForceController::applyAzimuthForces() {
	this->appliedForces->AzimuthForcesApplied = true;
	this->publishAppliedForces();
}

void ForceController::zeroAzimuthForces() {
	this->appliedForces->AzimuthForcesApplied = false;
	bool warningChanged = this->forceSetpointWarning->AnyAzimuthForceWarning;
	for(int i = 0; i < FA_COUNT; ++i) {
		this->forceSetpoint->AzimuthXSetpoint[i] = 0;
		this->forceSetpoint->AzimuthYSetpoint[i] = 0;
		this->forceSetpoint->AzimuthZSetpoint[i] = 0;
		this->forceData->AzimuthXSetpoint[i] = 0;
		this->forceData->AzimuthYSetpoint[i] = 0;
		this->forceData->AzimuthZSetpoint[i] = 0;
		this->forceSetpointWarning->AzimuthForceWarning[i] = false;
	}
	this->forceSetpointWarning->AnyAzimuthForceWarning = false;
	this->safetyController->forceControllerNotifyAzimuthForceClipping(this->forceSetpointWarning->AnyAzimuthForceWarning);
	if (warningChanged) {
		this->publishForceSetpointWarning();
	}
	this->publishAppliedForces();
}

void ForceController::applyTemperatureForces() {
	this->appliedForces->TemperatureForcesApplied = true;
	this->publishAppliedForces();
}

void ForceController::zeroTemperatureForces() {
	this->appliedForces->TemperatureForcesApplied = false;
	bool warningChanged = this->forceSetpointWarning->AnyTemperatureForceWarning;
	for(int i = 0; i < FA_COUNT; ++i) {
		this->forceSetpoint->TemperatureXSetpoint[i] = 0;
		this->forceSetpoint->TemperatureYSetpoint[i] = 0;
		this->forceSetpoint->TemperatureZSetpoint[i] = 0;
		this->forceData->TemperatureXSetpoint[i] = 0;
		this->forceData->TemperatureYSetpoint[i] = 0;
		this->forceData->TemperatureZSetpoint[i] = 0;
		this->forceSetpointWarning->TemperatureForceWarning[i] = false;
	}
	this->forceSetpointWarning->AnyTemperatureForceWarning = false;
	this->safetyController->forceControllerNotifyTemperatureForceClipping(this->forceSetpointWarning->AnyTemperatureForceWarning);
	if (warningChanged) {
		this->publishForceSetpointWarning();
	}
	this->publishAppliedForces();
}

void ForceController::applyDynamicForces() {
	this->appliedForces->DynamicForcesApplied = true;
	this->publishAppliedForces();
}

void ForceController::zeroDynamicForces() {
	this->appliedForces->DynamicForcesApplied = false;
	bool warningChanged = this->forceSetpointWarning->AnyDynamicForceWarning;
	for(int i = 0; i < FA_COUNT; ++i) {
		this->forceSetpoint->DynamicXSetpoint[i] = 0;
		this->forceSetpoint->DynamicYSetpoint[i] = 0;
		this->forceSetpoint->DynamicZSetpoint[i] = 0;
		this->forceData->DynamicXSetpoint[i] = 0;
		this->forceData->DynamicYSetpoint[i] = 0;
		this->forceData->DynamicZSetpoint[i] = 0;
		this->forceSetpointWarning->DynamicForceWarning[i] = false;
	}
	this->forceSetpointWarning->AnyDynamicForceWarning = false;
	this->safetyController->forceControllerNotifyDynamicForceClipping(this->forceSetpointWarning->AnyDynamicForceWarning);
	if (warningChanged) {
		this->publishForceSetpointWarning();
	}
	this->publishAppliedForces();
}

void ForceController::applyHardpointCorrections() {
	this->appliedForces->HardpointOffloadingForcesApplied = true;
	this->fx.resetPreviousValues();
	this->fy.resetPreviousValues();
	this->fz.resetPreviousValues();
	this->mx.resetPreviousValues();
	this->my.resetPreviousValues();
	this->mz.resetPreviousValues();
	this->publishAppliedForces();
}

void ForceController::zeroHardpointCorrections() {
	this->appliedForces->HardpointOffloadingForcesApplied = false;
	bool warningChanged = this->forceSetpointWarning->AnyHardpointOffloadForceWarning;
	for(int i = 0; i < FA_COUNT; ++i) {
		this->forceSetpoint->HardpointOffloadXSetpoint[i] = 0;
		this->forceSetpoint->HardpointOffloadYSetpoint[i] = 0;
		this->forceSetpoint->HardpointOffloadZSetpoint[i] = 0;
		this->forceData->HardpointOffloadXSetpoint[i] = 0;
		this->forceData->HardpointOffloadYSetpoint[i] = 0;
		this->forceData->HardpointOffloadZSetpoint[i] = 0;
		this->forceSetpointWarning->HardpointOffloadForceWarning[i] = false;
	}
	this->forceSetpointWarning->AnyHardpointOffloadForceWarning = false;
	this->safetyController->forceControllerNotifyHardpointOffloadForceClipping(this->forceSetpointWarning->AnyHardpointOffloadForceWarning);
	if (warningChanged) {
		this->publishForceSetpointWarning();
	}
	this->publishAppliedForces();
}

void ForceController::updatePID(int id, double timestep, double p, double i, double d, double n) {
	PID* pid = this->idToPID(id);
	if (pid != 0) {
		PIDParameters parameters;
		parameters.Timestep = timestep;
		parameters.P = p;
		parameters.I = i;
		parameters.D = d;
		parameters.N = n;
		pid->updateParameters(parameters);
	}
}

void ForceController::resetPID(int id) {
	PID* pid = this->idToPID(id);
	if (pid != 0) {
		pid->restoreInitialParameters();
	}
}

void ForceController::resetPIDs() {
	this->fx.restoreInitialParameters();
	this->fy.restoreInitialParameters();
	this->fz.restoreInitialParameters();
	this->mx.restoreInitialParameters();
	this->my.restoreInitialParameters();
	this->mz.restoreInitialParameters();
}

void ForceController::calculateMirrorForces() {
	std::vector<double> m = this->forceActuatorSettings->HardpointForceMomentMatrix;
	float* force = this->hardpointData->MeasuredForce;
	this->mirrorForceData->Timestamp = this->hardpointData->Timestamp;
	this->mirrorForceData->Fx = m[0] * force[0] + m[6] * force[1] + m[12] * force[2] + m[18] * force[3] + m[24] * force[4] + m[30] * force[5];
	this->mirrorForceData->Fy = m[1] * force[0] + m[7] * force[1] + m[13] * force[2] + m[19] * force[3] + m[25] * force[4] + m[31] * force[5];
	this->mirrorForceData->Fz = m[2] * force[0] + m[8] * force[1] + m[14] * force[2] + m[20] * force[3] + m[26] * force[4] + m[32] * force[5];
	this->mirrorForceData->Mx = m[3] * force[0] + m[9] * force[1] + m[15] * force[2] + m[21] * force[3] + m[27] * force[4] + m[33] * force[5];
	this->mirrorForceData->My = m[4] * force[0] + m[10] * force[1] + m[16] * force[2] + m[22] * force[3] + m[28] * force[4] + m[34] * force[5];
	this->mirrorForceData->Mz = m[5] * force[0] + m[11] * force[1] + m[17] * force[2] + m[23] * force[3] + m[29] * force[4] + m[35] * force[5];
	this->publisher->putMirrorForceData();
}

DistributedForces ForceController::calculateDistribution(float xForce, float yForce, float zForce, float xMoment, float yMoment, float zMoment) {
	DistributedForces forces;
	for(int i = 0; i < FA_COUNT; ++i) {
		forces.XForces[i] += this->forceActuatorSettings->ForceDistributionXMatrix[i * 3 + 0] * xForce;
		forces.YForces[i] += this->forceActuatorSettings->ForceDistributionXMatrix[i * 3 + 1] * xForce;
		forces.ZForces[i] += this->forceActuatorSettings->ForceDistributionXMatrix[i * 3 + 2] * xForce;
		forces.XForces[i] += this->forceActuatorSettings->ForceDistributionYMatrix[i * 3 + 0] * yForce;
		forces.YForces[i] += this->forceActuatorSettings->ForceDistributionYMatrix[i * 3 + 1] * yForce;
		forces.ZForces[i] += this->forceActuatorSettings->ForceDistributionYMatrix[i * 3 + 2] * yForce;
		forces.XForces[i] += this->forceActuatorSettings->ForceDistributionZMatrix[i * 3 + 0] * zForce;
		forces.YForces[i] += this->forceActuatorSettings->ForceDistributionZMatrix[i * 3 + 1] * zForce;
		forces.ZForces[i] += this->forceActuatorSettings->ForceDistributionZMatrix[i * 3 + 2] * zForce;
		forces.XForces[i] += this->forceActuatorSettings->MomentDistributionXMatrix[i * 3 + 0] * xMoment;
		forces.YForces[i] += this->forceActuatorSettings->MomentDistributionXMatrix[i * 3 + 1] * xMoment;
		forces.ZForces[i] += this->forceActuatorSettings->MomentDistributionXMatrix[i * 3 + 2] * xMoment;
		forces.XForces[i] += this->forceActuatorSettings->MomentDistributionYMatrix[i * 3 + 0] * yMoment;
		forces.YForces[i] += this->forceActuatorSettings->MomentDistributionYMatrix[i * 3 + 1] * yMoment;
		forces.ZForces[i] += this->forceActuatorSettings->MomentDistributionYMatrix[i * 3 + 2] * yMoment;
		forces.XForces[i] += this->forceActuatorSettings->MomentDistributionZMatrix[i * 3 + 0] * zMoment;
		forces.YForces[i] += this->forceActuatorSettings->MomentDistributionZMatrix[i * 3 + 1] * zMoment;
		forces.ZForces[i] += this->forceActuatorSettings->MomentDistributionZMatrix[i * 3 + 2] * zMoment;
	}
	return forces;
}

void ForceController::updateElevationForces() {
	double elevationAngle = this->forceActuatorSettings->UseInclinometer ? this->inclinometerData->InclinometerAngle : this->tmaElevationData.Angle_Actual;
	double elevationMatrix[] = { std::pow(elevationAngle, 5.0), std::pow(elevationAngle, 4.0), std::pow(elevationAngle, 3.0), std::pow(elevationAngle, 2.0), elevationAngle, 1 };
	bool warningChanged = false;
	this->forceSetpointWarning->AnyElevationForceWarning = false;
	for(int i = 0; i < FA_COUNT; ++i) {
		// Stage Elevation Forces
		int mIndex = i * 6;
		this->forceSetpoint->ElevationXSetpoint[i] =
				(this->forceActuatorSettings->ElevationXAxisCoefficients[mIndex + 0] * elevationMatrix[0] +
				this->forceActuatorSettings->ElevationXAxisCoefficients[mIndex + 1] * elevationMatrix[1] +
				this->forceActuatorSettings->ElevationXAxisCoefficients[mIndex + 2] * elevationMatrix[2] +
				this->forceActuatorSettings->ElevationXAxisCoefficients[mIndex + 3] * elevationMatrix[3] +
				this->forceActuatorSettings->ElevationXAxisCoefficients[mIndex + 4] * elevationMatrix[4] +
				this->forceActuatorSettings->ElevationXAxisCoefficients[mIndex + 5]) * this->forceSetpoint->ElevationSetpointPercentage;
		this->forceSetpoint->ElevationYSetpoint[i] =
				(this->forceActuatorSettings->ElevationYAxisCoefficients[mIndex + 0] * elevationMatrix[0] +
				this->forceActuatorSettings->ElevationYAxisCoefficients[mIndex + 1] * elevationMatrix[1] +
				this->forceActuatorSettings->ElevationYAxisCoefficients[mIndex + 2] * elevationMatrix[2] +
				this->forceActuatorSettings->ElevationYAxisCoefficients[mIndex + 3] * elevationMatrix[3] +
				this->forceActuatorSettings->ElevationYAxisCoefficients[mIndex + 4] * elevationMatrix[4] +
				this->forceActuatorSettings->ElevationYAxisCoefficients[mIndex + 5]) * this->forceSetpoint->ElevationSetpointPercentage;
		this->forceSetpoint->ElevationZSetpoint[i] =
				(this->forceActuatorSettings->ElevationZAxisCoefficients[mIndex + 0] * elevationMatrix[0] +
				this->forceActuatorSettings->ElevationZAxisCoefficients[mIndex + 1] * elevationMatrix[1] +
				this->forceActuatorSettings->ElevationZAxisCoefficients[mIndex + 2] * elevationMatrix[2] +
				this->forceActuatorSettings->ElevationZAxisCoefficients[mIndex + 3] * elevationMatrix[3] +
				this->forceActuatorSettings->ElevationZAxisCoefficients[mIndex + 4] * elevationMatrix[4] +
				this->forceActuatorSettings->ElevationZAxisCoefficients[mIndex + 5]) * this->forceSetpoint->ElevationSetpointPercentage;
		// Check Elevation Forces
		bool previousWarning = this->forceSetpointWarning->ElevationForceWarning[i];
		this->forceSetpointWarning->ElevationForceWarning[i] =
				!Range::InRange(this->forceActuatorSettings->Limits[i].ElevationXLowLimit, this->forceActuatorSettings->Limits[i].ElevationXHighLimit, this->forceSetpoint->ElevationXSetpoint[i]) ||
				!Range::InRange(this->forceActuatorSettings->Limits[i].ElevationYLowLimit, this->forceActuatorSettings->Limits[i].ElevationYHighLimit, this->forceSetpoint->ElevationYSetpoint[i]) ||
				!Range::InRange(this->forceActuatorSettings->Limits[i].ElevationZLowLimit, this->forceActuatorSettings->Limits[i].ElevationZHighLimit, this->forceSetpoint->ElevationZSetpoint[i]);
		this->forceSetpointWarning->AnyElevationForceWarning = this->forceSetpointWarning->AnyElevationForceWarning || this->forceSetpointWarning->ElevationForceWarning[i];
		warningChanged = warningChanged || (this->forceSetpointWarning->ElevationForceWarning[i] != previousWarning);
	}
	this->safetyController->forceControllerNotifyElevationForceClipping(this->forceSetpointWarning->AnyElevationForceWarning);
	if (warningChanged) {
		this->publishForceSetpointWarning();
	}
	if (this->forceSetpointWarning->AnyElevationForceWarning) {
		// Coerce Elevation Forces
		this->publishForceDataRejection();
		for(int i = 0; i < FA_COUNT; i++) {
			this->forceSetpoint->ElevationXSetpoint[i] = Range::CoerceIntoRange(this->forceActuatorSettings->Limits[i].ElevationXLowLimit, this->forceActuatorSettings->Limits[i].ElevationXHighLimit, this->forceSetpoint->ElevationXSetpoint[i]);
			this->forceSetpoint->ElevationYSetpoint[i] = Range::CoerceIntoRange(this->forceActuatorSettings->Limits[i].ElevationYLowLimit, this->forceActuatorSettings->Limits[i].ElevationYHighLimit, this->forceSetpoint->ElevationYSetpoint[i]);
			this->forceSetpoint->ElevationZSetpoint[i] = Range::CoerceIntoRange(this->forceActuatorSettings->Limits[i].ElevationZLowLimit, this->forceActuatorSettings->Limits[i].ElevationZHighLimit, this->forceSetpoint->ElevationZSetpoint[i]);
		}
	}
	// Move Elevation Forces
	for(int i = 0; i < FA_COUNT; i++) {
		this->forceData->ElevationXSetpoint[i] = this->forceSetpoint->ElevationXSetpoint[i];
		this->forceData->ElevationYSetpoint[i] = this->forceSetpoint->ElevationYSetpoint[i];
		this->forceData->ElevationZSetpoint[i] = this->forceSetpoint->ElevationZSetpoint[i];
	}
}

void ForceController::updateAzimuthForces() {
	double azimuthAngle = this->tmaAzimuthData.Angle_Actual;
	double azimuthMatrix[] = { std::pow(azimuthAngle, 5.0), std::pow(azimuthAngle, 4.0), std::pow(azimuthAngle, 3.0), std::pow(azimuthAngle, 2.0), azimuthAngle, 1 };
	bool warningChanged = false;
	this->forceSetpointWarning->AnyAzimuthForceWarning = false;
	for(int i = 0; i < FA_COUNT; ++i) {
		int mIndex = i * 6;
		this->forceSetpoint->AzimuthXSetpoint[i] =
				this->forceActuatorSettings->AzimuthXAxisCoefficients[mIndex + 0] * azimuthMatrix[0] +
				this->forceActuatorSettings->AzimuthXAxisCoefficients[mIndex + 1] * azimuthMatrix[1] +
				this->forceActuatorSettings->AzimuthXAxisCoefficients[mIndex + 2] * azimuthMatrix[2] +
				this->forceActuatorSettings->AzimuthXAxisCoefficients[mIndex + 3] * azimuthMatrix[3] +
				this->forceActuatorSettings->AzimuthXAxisCoefficients[mIndex + 4] * azimuthMatrix[4] +
				this->forceActuatorSettings->AzimuthXAxisCoefficients[mIndex + 5];
		this->forceSetpoint->AzimuthYSetpoint[i] =
				this->forceActuatorSettings->AzimuthYAxisCoefficients[mIndex + 0] * azimuthMatrix[0] +
				this->forceActuatorSettings->AzimuthYAxisCoefficients[mIndex + 1] * azimuthMatrix[1] +
				this->forceActuatorSettings->AzimuthYAxisCoefficients[mIndex + 2] * azimuthMatrix[2] +
				this->forceActuatorSettings->AzimuthYAxisCoefficients[mIndex + 3] * azimuthMatrix[3] +
				this->forceActuatorSettings->AzimuthYAxisCoefficients[mIndex + 4] * azimuthMatrix[4] +
				this->forceActuatorSettings->AzimuthYAxisCoefficients[mIndex + 5];
		this->forceSetpoint->AzimuthZSetpoint[i] =
				this->forceActuatorSettings->AzimuthZAxisCoefficients[mIndex + 0] * azimuthMatrix[0] +
				this->forceActuatorSettings->AzimuthZAxisCoefficients[mIndex + 1] * azimuthMatrix[1] +
				this->forceActuatorSettings->AzimuthZAxisCoefficients[mIndex + 2] * azimuthMatrix[2] +
				this->forceActuatorSettings->AzimuthZAxisCoefficients[mIndex + 3] * azimuthMatrix[3] +
				this->forceActuatorSettings->AzimuthZAxisCoefficients[mIndex + 4] * azimuthMatrix[4] +
				this->forceActuatorSettings->AzimuthZAxisCoefficients[mIndex + 5];
		// Check Azimuth Forces
		bool previousWarning = this->forceSetpointWarning->AzimuthForceWarning[i];
		this->forceSetpointWarning->AzimuthForceWarning[i] =
				!Range::InRange(this->forceActuatorSettings->Limits[i].AzimuthXLowLimit, this->forceActuatorSettings->Limits[i].AzimuthXHighLimit, this->forceSetpoint->AzimuthXSetpoint[i]) ||
				!Range::InRange(this->forceActuatorSettings->Limits[i].AzimuthYLowLimit, this->forceActuatorSettings->Limits[i].AzimuthYHighLimit, this->forceSetpoint->AzimuthYSetpoint[i]) ||
				!Range::InRange(this->forceActuatorSettings->Limits[i].AzimuthZLowLimit, this->forceActuatorSettings->Limits[i].AzimuthZHighLimit, this->forceSetpoint->AzimuthZSetpoint[i]);
		this->forceSetpointWarning->AnyAzimuthForceWarning = this->forceSetpointWarning->AnyAzimuthForceWarning || this->forceSetpointWarning->AzimuthForceWarning[i];
		warningChanged = warningChanged || (this->forceSetpointWarning->AzimuthForceWarning[i] != previousWarning);
	}
	this->safetyController->forceControllerNotifyAzimuthForceClipping(this->forceSetpointWarning->AnyAzimuthForceWarning);
	if (warningChanged) {
		this->publishForceSetpointWarning();
	}
	if (this->forceSetpointWarning->AnyAzimuthForceWarning) {
		// Coerce Azimuth Forces
		this->publishForceDataRejection();
		for(int i = 0; i < FA_COUNT; i++) {
			this->forceSetpoint->AzimuthXSetpoint[i] = Range::CoerceIntoRange(this->forceActuatorSettings->Limits[i].AzimuthXLowLimit, this->forceActuatorSettings->Limits[i].AzimuthXHighLimit, this->forceSetpoint->AzimuthXSetpoint[i]);
			this->forceSetpoint->AzimuthYSetpoint[i] = Range::CoerceIntoRange(this->forceActuatorSettings->Limits[i].AzimuthYLowLimit, this->forceActuatorSettings->Limits[i].AzimuthYHighLimit, this->forceSetpoint->AzimuthYSetpoint[i]);
			this->forceSetpoint->AzimuthZSetpoint[i] = Range::CoerceIntoRange(this->forceActuatorSettings->Limits[i].AzimuthZLowLimit, this->forceActuatorSettings->Limits[i].AzimuthZHighLimit, this->forceSetpoint->AzimuthZSetpoint[i]);
		}
	}
	// Move Azimuth Forces
	for(int i = 0; i < FA_COUNT; i++) {
		this->forceData->AzimuthXSetpoint[i] = this->forceSetpoint->AzimuthXSetpoint[i];
		this->forceData->AzimuthYSetpoint[i] = this->forceSetpoint->AzimuthYSetpoint[i];
		this->forceData->AzimuthZSetpoint[i] = this->forceSetpoint->AzimuthZSetpoint[i];
	}
}

void ForceController::updateTemperatureForces() {
	double temperature = 0; // TODO: Update
	double temperatureMatrix[] = { std::pow(temperature, 5.0), std::pow(temperature, 4.0), std::pow(temperature, 3.0), std::pow(temperature, 2.0), temperature, 1 };
	bool warningChanged = false;
	this->forceSetpointWarning->AnyTemperatureForceWarning = false;
	for(int i = 0; i < FA_COUNT; ++i) {
		// Stage Temperature Forces
		int mIndex = i * 6;
		this->forceSetpoint->TemperatureXSetpoint[i] =
				this->forceActuatorSettings->TemperatureXAxisCoefficients[mIndex + 0] * temperatureMatrix[0] +
				this->forceActuatorSettings->TemperatureXAxisCoefficients[mIndex + 1] * temperatureMatrix[1] +
				this->forceActuatorSettings->TemperatureXAxisCoefficients[mIndex + 2] * temperatureMatrix[2] +
				this->forceActuatorSettings->TemperatureXAxisCoefficients[mIndex + 3] * temperatureMatrix[3] +
				this->forceActuatorSettings->TemperatureXAxisCoefficients[mIndex + 4] * temperatureMatrix[4] +
				this->forceActuatorSettings->TemperatureXAxisCoefficients[mIndex + 5];
		this->forceSetpoint->TemperatureYSetpoint[i] =
				this->forceActuatorSettings->TemperatureYAxisCoefficients[mIndex + 0] * temperatureMatrix[0] +
				this->forceActuatorSettings->TemperatureYAxisCoefficients[mIndex + 1] * temperatureMatrix[1] +
				this->forceActuatorSettings->TemperatureYAxisCoefficients[mIndex + 2] * temperatureMatrix[2] +
				this->forceActuatorSettings->TemperatureYAxisCoefficients[mIndex + 3] * temperatureMatrix[3] +
				this->forceActuatorSettings->TemperatureYAxisCoefficients[mIndex + 4] * temperatureMatrix[4] +
				this->forceActuatorSettings->TemperatureYAxisCoefficients[mIndex + 5];
		this->forceSetpoint->TemperatureZSetpoint[i] =
				this->forceActuatorSettings->TemperatureZAxisCoefficients[mIndex + 0] * temperatureMatrix[0] +
				this->forceActuatorSettings->TemperatureZAxisCoefficients[mIndex + 1] * temperatureMatrix[1] +
				this->forceActuatorSettings->TemperatureZAxisCoefficients[mIndex + 2] * temperatureMatrix[2] +
				this->forceActuatorSettings->TemperatureZAxisCoefficients[mIndex + 3] * temperatureMatrix[3] +
				this->forceActuatorSettings->TemperatureZAxisCoefficients[mIndex + 4] * temperatureMatrix[4] +
				this->forceActuatorSettings->TemperatureZAxisCoefficients[mIndex + 5];
		// Check Temperature Forces
		bool previousWarning = this->forceSetpointWarning->TemperatureForceWarning[i];
		this->forceSetpointWarning->TemperatureForceWarning[i] =
				!Range::InRange(this->forceActuatorSettings->Limits[i].TemperatureXLowLimit, this->forceActuatorSettings->Limits[i].TemperatureXHighLimit, this->forceSetpoint->TemperatureXSetpoint[i]) ||
				!Range::InRange(this->forceActuatorSettings->Limits[i].TemperatureYLowLimit, this->forceActuatorSettings->Limits[i].TemperatureYHighLimit, this->forceSetpoint->TemperatureYSetpoint[i]) ||
				!Range::InRange(this->forceActuatorSettings->Limits[i].TemperatureZLowLimit, this->forceActuatorSettings->Limits[i].TemperatureZHighLimit, this->forceSetpoint->TemperatureZSetpoint[i]);
		this->forceSetpointWarning->AnyTemperatureForceWarning = this->forceSetpointWarning->AnyTemperatureForceWarning || this->forceSetpointWarning->TemperatureForceWarning[i];
		warningChanged = warningChanged || (this->forceSetpointWarning->TemperatureForceWarning[i] != previousWarning);
	}
	this->safetyController->forceControllerNotifyTemperatureForceClipping(this->forceSetpointWarning->AnyTemperatureForceWarning);
	if (warningChanged) {
		this->publishForceSetpointWarning();
	}
	if (this->forceSetpointWarning->AnyTemperatureForceWarning) {
		// Coerce Temperature Forces
		this->publishForceDataRejection();
		for(int i = 0; i < FA_COUNT; i++) {
			this->forceSetpoint->TemperatureXSetpoint[i] = Range::CoerceIntoRange(this->forceActuatorSettings->Limits[i].TemperatureXLowLimit, this->forceActuatorSettings->Limits[i].TemperatureXHighLimit, this->forceSetpoint->TemperatureXSetpoint[i]);
			this->forceSetpoint->TemperatureYSetpoint[i] = Range::CoerceIntoRange(this->forceActuatorSettings->Limits[i].TemperatureYLowLimit, this->forceActuatorSettings->Limits[i].TemperatureYHighLimit, this->forceSetpoint->TemperatureYSetpoint[i]);
			this->forceSetpoint->TemperatureZSetpoint[i] = Range::CoerceIntoRange(this->forceActuatorSettings->Limits[i].TemperatureZLowLimit, this->forceActuatorSettings->Limits[i].TemperatureZHighLimit, this->forceSetpoint->TemperatureZSetpoint[i]);
		}
	}
	// Move Temperature Forces
	for(int i = 0; i < FA_COUNT; i++) {
		this->forceData->TemperatureXSetpoint[i] = this->forceSetpoint->TemperatureXSetpoint[i];
		this->forceData->TemperatureYSetpoint[i] = this->forceSetpoint->TemperatureYSetpoint[i];
		this->forceData->TemperatureZSetpoint[i] = this->forceSetpoint->TemperatureZSetpoint[i];
	}
}

void ForceController::updateDynamicForces() {

}

void ForceController::updateHardpointCorrectionForces() {
	float fx = this->fx.process(0, this->mirrorForceData->Fx);
	float fy = this->fy.process(0, this->mirrorForceData->Fy);
	float fz = this->fz.process(0, this->mirrorForceData->Fz);
	float mx = this->mx.process(0, this->mirrorForceData->Mx);
	float my = this->my.process(0, this->mirrorForceData->My);
	float mz = this->mz.process(0, this->mirrorForceData->Mz);
	DistributedForces forces = this->calculateDistribution(fx, fy, fz, mx, my, mz);
	bool warningChanged = false;
	this->forceSetpointWarning->AnyHardpointOffloadForceWarning = false;
	for(int i = 0; i < FA_COUNT; ++i) {
		// Stage Hardpoint Offloading Forces
		this->forceSetpoint->HardpointOffloadXSetpoint[i] = forces.XForces[i];
		this->forceSetpoint->HardpointOffloadYSetpoint[i] = forces.YForces[i];
		this->forceSetpoint->HardpointOffloadZSetpoint[i] = forces.ZForces[i];
		// Check Hardpoint Offloading Forces
		bool previousWarning = this->forceSetpointWarning->HardpointOffloadForceWarning[i];
		this->forceSetpointWarning->HardpointOffloadForceWarning[i] =
				!Range::InRange(this->forceActuatorSettings->Limits[i].HardpointOffloadingXLowLimit, this->forceActuatorSettings->Limits[i].HardpointOffloadingXHighLimit, this->forceSetpoint->HardpointOffloadXSetpoint[i]) ||
				!Range::InRange(this->forceActuatorSettings->Limits[i].HardpointOffloadingYLowLimit, this->forceActuatorSettings->Limits[i].HardpointOffloadingYHighLimit, this->forceSetpoint->HardpointOffloadYSetpoint[i]) ||
				!Range::InRange(this->forceActuatorSettings->Limits[i].HardpointOffloadingZLowLimit, this->forceActuatorSettings->Limits[i].HardpointOffloadingZHighLimit, this->forceSetpoint->HardpointOffloadZSetpoint[i]);
		this->forceSetpointWarning->AnyHardpointOffloadForceWarning = this->forceSetpointWarning->AnyHardpointOffloadForceWarning || this->forceSetpointWarning->HardpointOffloadForceWarning[i];
		warningChanged = warningChanged || (this->forceSetpointWarning->HardpointOffloadForceWarning[i] != previousWarning);
	}
	this->safetyController->forceControllerNotifyHardpointOffloadForceClipping(this->forceSetpointWarning->AnyHardpointOffloadForceWarning);
	if (warningChanged) {
		this->publishForceSetpointWarning();
	}
	if (this->forceSetpointWarning->AnyHardpointOffloadForceWarning) {
		// Coerce Temperature Forces
		this->publishForceDataRejection();
		for(int i = 0; i < FA_COUNT; i++) {
			this->forceSetpoint->HardpointOffloadXSetpoint[i] = Range::CoerceIntoRange(this->forceActuatorSettings->Limits[i].HardpointOffloadingXLowLimit, this->forceActuatorSettings->Limits[i].HardpointOffloadingXHighLimit, this->forceSetpoint->HardpointOffloadXSetpoint[i]);
			this->forceSetpoint->HardpointOffloadYSetpoint[i] = Range::CoerceIntoRange(this->forceActuatorSettings->Limits[i].HardpointOffloadingYLowLimit, this->forceActuatorSettings->Limits[i].HardpointOffloadingYHighLimit, this->forceSetpoint->HardpointOffloadYSetpoint[i]);
			this->forceSetpoint->HardpointOffloadZSetpoint[i] = Range::CoerceIntoRange(this->forceActuatorSettings->Limits[i].HardpointOffloadingZLowLimit, this->forceActuatorSettings->Limits[i].HardpointOffloadingZHighLimit, this->forceSetpoint->HardpointOffloadZSetpoint[i]);
		}
	}
	// Move Hardpoint Offloading Forces
	for(int i = 0; i < FA_COUNT; i++) {
		this->forceData->HardpointOffloadXSetpoint[i] = this->forceSetpoint->HardpointOffloadXSetpoint[i];
		this->forceData->HardpointOffloadYSetpoint[i] = this->forceSetpoint->HardpointOffloadYSetpoint[i];
		this->forceData->HardpointOffloadZSetpoint[i] = this->forceSetpoint->HardpointOffloadZSetpoint[i];
	}
}

void ForceController::sumAllForces() {
	for(int i = 0; i < FA_COUNT; ++i) {
		this->forceSetpoint->XSetpoint[i] =
				this->forceSetpoint->StaticXSetpoint[i] +
				this->forceSetpoint->OffsetXSetpoint[i] +
				this->forceSetpoint->ElevationXSetpoint[i] +
				this->forceSetpoint->AzimuthXSetpoint[i] +
				this->forceSetpoint->TemperatureXSetpoint[i] +
				this->forceSetpoint->HardpointOffloadXSetpoint[i];
		this->forceSetpoint->YSetpoint[i] =
				this->forceSetpoint->StaticYSetpoint[i] +
				this->forceSetpoint->OffsetYSetpoint[i] +
				this->forceSetpoint->ElevationYSetpoint[i] +
				this->forceSetpoint->AzimuthYSetpoint[i] +
				this->forceSetpoint->TemperatureYSetpoint[i] +
				this->forceSetpoint->HardpointOffloadYSetpoint[i];
		this->forceSetpoint->ZSetpoint[i] =
				this->forceSetpoint->StaticZSetpoint[i] +
				this->forceSetpoint->OffsetZSetpoint[i] +
				this->forceSetpoint->AberrationZSetpoint[i] +
				this->forceSetpoint->ActiveOpticsZSetpoint[i] +
				this->forceSetpoint->ElevationZSetpoint[i] +
				this->forceSetpoint->AzimuthZSetpoint[i] +
				this->forceSetpoint->TemperatureZSetpoint[i] +
				this->forceSetpoint->HardpointOffloadZSetpoint[i];
		this->forceData->XSetpoint[i] = this->forceSetpoint->XSetpoint[i];
		this->forceData->YSetpoint[i] = this->forceSetpoint->YSetpoint[i];
		this->forceData->ZSetpoint[i] = this->forceSetpoint->ZSetpoint[i];
	}
}

void ForceController::convertForcesToSetpoints() {
	bool warningChanged = false;
	this->forceSetpointWarning->AnySafetyLimitWarning = false;
	for(int i = 0; i < FA_COUNT; i++) {
		// Stage Setpoints Forces
		switch(this->forceInfo->ActuatorOrientation[i]) {
		case ForceActuatorOrientations::PositiveY:
			this->forceSetpoint->PrimaryCylinderSetpointCommanded[i] = toInt24(this->forceSetpoint->ZSetpoint[i] - this->forceSetpoint->YSetpoint[i]);
			this->forceSetpoint->SecondaryCylinderSetpointCommanded[i] = toInt24(this->forceSetpoint->YSetpoint[i] * sqrt2);
			break;
		case ForceActuatorOrientations::NA:
			this->forceSetpoint->PrimaryCylinderSetpointCommanded[i] = toInt24(this->forceSetpoint->ZSetpoint[i]);
			break;
		case ForceActuatorOrientations::PositiveX:
			this->forceSetpoint->PrimaryCylinderSetpointCommanded[i] = toInt24(this->forceSetpoint->ZSetpoint[i] - this->forceSetpoint->XSetpoint[i]);
			this->forceSetpoint->SecondaryCylinderSetpointCommanded[i] = toInt24(this->forceSetpoint->XSetpoint[i] * sqrt2);
			break;
		case ForceActuatorOrientations::NegativeX:
			this->forceSetpoint->PrimaryCylinderSetpointCommanded[i] = toInt24(this->forceSetpoint->ZSetpoint[i] - -this->forceSetpoint->XSetpoint[i]);
			this->forceSetpoint->SecondaryCylinderSetpointCommanded[i] = toInt24(-this->forceSetpoint->XSetpoint[i] * sqrt2);
			break;
		case ForceActuatorOrientations::NegativeY:
			this->forceSetpoint->PrimaryCylinderSetpointCommanded[i] = toInt24(this->forceSetpoint->ZSetpoint[i] - -this->forceSetpoint->YSetpoint[i]);
			this->forceSetpoint->SecondaryCylinderSetpointCommanded[i] = toInt24(-this->forceSetpoint->YSetpoint[i] * sqrt2);
			break;
		}
		// Check Setpoints Forces
		bool previousWarning = this->forceSetpointWarning->SafetyLimitWarning[i];
		this->forceSetpointWarning->SafetyLimitWarning[i] =
				!Range::InRange(this->forceActuatorSettings->Limits[i].PrimaryAxisTotalLowLimit, this->forceActuatorSettings->Limits[i].PrimaryAxisTotalHighLimit, this->forceSetpoint->PrimaryCylinderSetpointCommanded[i]) ||
				!Range::InRange(this->forceActuatorSettings->Limits[i].SecondaryAxisTotalLowLimit, this->forceActuatorSettings->Limits[i].SecondaryAxisTotalHighLimit, this->forceSetpoint->SecondaryCylinderSetpointCommanded[i]);
		this->forceSetpointWarning->AnySafetyLimitWarning = this->forceSetpointWarning->AnySafetyLimitWarning || this->forceSetpointWarning->SafetyLimitWarning[i];
		warningChanged = warningChanged || (this->forceSetpointWarning->SafetyLimitWarning[i] != previousWarning);
	}
	this->safetyController->forceControllerNotifySafetyLimit(this->forceSetpointWarning->AnySafetyLimitWarning);
	if (warningChanged) {
		this->publishForceSetpointWarning();
	}
	if (this->forceSetpointWarning->AnySafetyLimitWarning) {
		// Coerce Setpoints Forces
		this->publishForceDataRejection();
		for(int i = 0; i < FA_COUNT; i++) {
			this->forceSetpoint->PrimaryCylinderSetpointCommanded[i] = Range::CoerceIntoRange(this->forceActuatorSettings->Limits[i].PrimaryAxisTotalLowLimit, this->forceActuatorSettings->Limits[i].PrimaryAxisTotalHighLimit, this->forceSetpoint->PrimaryCylinderSetpointCommanded[i]);
			this->forceSetpoint->SecondaryCylinderSetpointCommanded[i] = Range::CoerceIntoRange(this->forceActuatorSettings->Limits[i].SecondaryAxisTotalLowLimit, this->forceActuatorSettings->Limits[i].SecondaryAxisTotalHighLimit, this->forceSetpoint->SecondaryCylinderSetpointCommanded[i]);
		}
	}
	// Move Setpoints Forces
	for(int i = 0; i < FA_COUNT; i++) {
		this->forceData->PrimaryCylinderSetpointCommanded[i] = this->forceSetpoint->PrimaryCylinderSetpointCommanded[i];
		this->forceData->SecondaryCylinderSetpointCommanded[i] = this->forceSetpoint->SecondaryCylinderSetpointCommanded[i];
	}
}

bool ForceController::checkMirrorMoments() {
	double xMoment = 0;
	double yMoment = 0;
	double zMoment = 0;
	for(int i = 0; i < FA_COUNT; i++) {
		xMoment += this->forceInfo->YPosition[i] * this->forceData->ZSetpoint[i];
		yMoment += this->forceInfo->XPosition[i] * this->forceData->ZSetpoint[i];
		zMoment += this->forceInfo->XPosition[i] * this->forceData->YSetpoint[i] + this->forceInfo->YPosition[i] * this->forceData->XSetpoint[i];
	}
	bool previousXMomentWarning = this->forceSetpointWarning->XMomentWarning;
	bool previousYMomentWarning = this->forceSetpointWarning->YMomentWarning;
	bool previousZMomentWarning = this->forceSetpointWarning->ZMomentWarning;
	this->forceSetpointWarning->XMomentWarning = !Range::InRange(this->forceActuatorSettings->MirrorXMoment * this->forceActuatorSettings->SetpointXMomentLowLimitPercentage, this->forceActuatorSettings->MirrorXMoment * this->forceActuatorSettings->SetpointXMomentHighLimitPercentage, xMoment);
	this->forceSetpointWarning->YMomentWarning = !Range::InRange(this->forceActuatorSettings->MirrorYMoment * this->forceActuatorSettings->SetpointYMomentLowLimitPercentage, this->forceActuatorSettings->MirrorYMoment * this->forceActuatorSettings->SetpointYMomentHighLimitPercentage, yMoment);
	this->forceSetpointWarning->ZMomentWarning = !Range::InRange(this->forceActuatorSettings->MirrorZMoment * this->forceActuatorSettings->SetpointZMomentLowLimitPercentage, this->forceActuatorSettings->MirrorZMoment * this->forceActuatorSettings->SetpointZMomentHighLimitPercentage, zMoment);
	this->safetyController->forceControllerNotifyXMomentLimit(this->forceSetpointWarning->XMomentWarning);
	this->safetyController->forceControllerNotifyYMomentLimit(this->forceSetpointWarning->YMomentWarning);
	this->safetyController->forceControllerNotifyZMomentLimit(this->forceSetpointWarning->ZMomentWarning);
	return this->forceSetpointWarning->XMomentWarning != previousXMomentWarning ||
			this->forceSetpointWarning->YMomentWarning != previousYMomentWarning ||
			this->forceSetpointWarning->ZMomentWarning != previousZMomentWarning;
}

bool ForceController::checkNearNeighbors() {
	double globalX = 0;
	double globalY = 0;
	double globalZ = 0;
	for(int i = 0; i < FA_COUNT; i++) {
		globalX += this->forceData->XSetpoint[i];
		globalY += this->forceData->YSetpoint[i];
		globalZ += this->forceData->ZSetpoint[i];
	}
	double globalForce = sqrt(globalX * globalX + globalY * globalY + globalZ * globalZ);
	double globalAverageForce = globalForce / FA_COUNT;
	bool warningChanged = false;
	this->forceSetpointWarning->AnyNearNeighborWarning = false;
	for(int i = 0; i < FA_COUNT; i++) {
		double nearX = 0;
		double nearY = 0;
		double nearZ = 0;
		int nearNeighbors = this->neighbors[i].NearIDs.size();
		for(int j = 0; j < nearNeighbors; ++j) {
			int neighborIndex = this->neighbors[i].NearIDs[j];
			nearX += this->forceData->XSetpoint[neighborIndex];
			nearY += this->forceData->YSetpoint[neighborIndex];
			nearZ += this->forceData->ZSetpoint[neighborIndex];
		}
		nearX /= nearNeighbors;
		nearY /= nearNeighbors;
		nearZ /= nearNeighbors;
		double x = this->forceData->XSetpoint[i] - nearX;
		double y = this->forceData->YSetpoint[i] - nearY;
		double z = this->forceData->ZSetpoint[i] - nearZ;
		double magnitudeDifference = sqrt(x * x + y * y + z * z);
		bool previousWarning = this->forceSetpointWarning->NearNeighborWarning[i];
		this->forceSetpointWarning->NearNeighborWarning[i] = magnitudeDifference > (globalAverageForce * this->forceActuatorSettings->SetpointNearNeighborLimitPercentage);
		this->forceSetpointWarning->AnyNearNeighborWarning = this->forceSetpointWarning->AnyNearNeighborWarning || this->forceSetpointWarning->NearNeighborWarning[i];
		warningChanged = warningChanged || (this->forceSetpointWarning->NearNeighborWarning[i] != previousWarning);
	}
	this->safetyController->forceControllerNotifyNearNeighborCheck(this->forceSetpointWarning->AnyNearNeighborWarning);
	return warningChanged;
}

bool ForceController::checkMirrorWeight() {
	double globalX = 0;
	double globalY = 0;
	double globalZ = 0;
	for(int i = 0; i < FA_COUNT; i++) {
		globalX += this->forceData->XSetpoint[i];
		globalY += this->forceData->YSetpoint[i];
		globalZ += this->forceData->ZSetpoint[i];
	}
	double globalForce = sqrt(globalX * globalX + globalY * globalY + globalZ * globalZ);
	bool previousWarning = this->forceSetpointWarning->MagnitudeWarning;
	this->forceSetpointWarning->MagnitudeWarning = globalForce > (this->forceActuatorSettings->MirrorWeight * this->forceActuatorSettings->SetpointMirrorWeightLimitPercentage);
	this->safetyController->forceControllerNotifyMagnitudeLimit(this->forceSetpointWarning->MagnitudeWarning);
	return this->forceSetpointWarning->MagnitudeWarning != previousWarning;
}

bool ForceController::checkFarNeighbors() {
	double globalX = 0;
	double globalY = 0;
	double globalZ = 0;
	for(int i = 0; i < FA_COUNT; i++) {
		globalX += this->forceData->XSetpoint[i];
		globalY += this->forceData->YSetpoint[i];
		globalZ += this->forceData->ZSetpoint[i];
	}
	double globalForce = sqrt(globalX * globalX + globalY * globalY + globalZ * globalZ);
	double globalAverageForce = globalForce / FA_COUNT;
	double tolerance = globalAverageForce * this->forceActuatorSettings->SetpointNearNeighborLimitPercentage;
	if (tolerance < 1) {
		tolerance = 1;
	}
	bool warningChanged = false;
	this->forceSetpointWarning->AnyFarNeighborWarning = false;
	for(int i = 0; i < FA_COUNT; i++) {
		double x = this->forceData->XSetpoint[i];
		double y = this->forceData->YSetpoint[i];
		double z = this->forceData->ZSetpoint[i];
		int farNeighbors = this->neighbors[i].FarIDs.size();
		for(int j = 0; j < farNeighbors; ++j) {
			int neighborIndex = this->neighbors[i].FarIDs[j];
			x += this->forceData->XSetpoint[neighborIndex];
			y += this->forceData->YSetpoint[neighborIndex];
			z += this->forceData->ZSetpoint[neighborIndex];
		}
		double magnitude = sqrt(x * x + y * y + z * z);
		double magnitudeAverage = magnitude / (farNeighbors + 1.0);
		bool previousWarning = this->forceSetpointWarning->FarNeighborWarning[i];
		this->forceSetpointWarning->FarNeighborWarning[i] = !Range::InRange(-tolerance, tolerance, magnitudeAverage - globalAverageForce);
		this->forceSetpointWarning->AnyFarNeighborWarning = this->forceSetpointWarning->AnyFarNeighborWarning || this->forceSetpointWarning->FarNeighborWarning[i];
		warningChanged = warningChanged || (this->forceSetpointWarning->FarNeighborWarning[i] != previousWarning);
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

void ForceController::publishAppliedForces() {
	this->appliedForces->Timestamp = this->publisher->getTimestamp();
	this->publisher->logAppliedForces();
}

void ForceController::publishForceSetpointWarning() {
	this->forceSetpointWarning->Timestamp = this->publisher->getTimestamp();
	this->forceSetpointWarning->AnyWarning =
			this->forceSetpointWarning->AnyStaticForceWarning ||
			this->forceSetpointWarning->AnyElevationForceWarning ||
			this->forceSetpointWarning->AnyAzimuthForceWarning ||
			this->forceSetpointWarning->AnyTemperatureForceWarning ||
			this->forceSetpointWarning->AnyDynamicForceWarning ||
			this->forceSetpointWarning->AnyHardpointOffloadForceWarning ||
			this->forceSetpointWarning->AnyAOSForceWarning ||
			this->forceSetpointWarning->AOSNetForceWarning ||
			this->forceSetpointWarning->AnyAberrationForceWarning ||
			this->forceSetpointWarning->AberrationNetForceWarning ||
			this->forceSetpointWarning->AnyOffsetForceWarning ||
			this->forceSetpointWarning->AnySafetyLimitWarning ||
			this->forceSetpointWarning->XMomentWarning ||
			this->forceSetpointWarning->YMomentWarning ||
			this->forceSetpointWarning->ZMomentWarning ||
			this->forceSetpointWarning->AnyNearNeighborWarning ||
			this->forceSetpointWarning->MagnitudeWarning ||
			this->forceSetpointWarning->AnyFarNeighborWarning;
	this->publisher->logForceActuatorSetpointWarning();
}

void ForceController::publishForceDataRejection() {
	this->forceSetpoint->Timestamp = this->publisher->getTimestamp();
	this->publisher->logForceActuatorDataRejection();
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
