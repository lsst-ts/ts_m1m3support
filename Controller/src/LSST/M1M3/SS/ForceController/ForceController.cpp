/*
 * ForceController.cpp
 *
 *  Created on: Oct 23, 2017
 *      Author: ccontaxis
 */

#include <ForceController.h>
#include <ForceActuatorOrientations.h>
#include <ForceActuatorSettings.h>
#include <SAL_m1m3C.h>
#include <cmath>
#include <vector>

namespace LSST {
namespace M1M3 {
namespace SS {

ForceController::ForceController(ForceActuatorApplicationSettings* forceActuatorApplicationSettings, ForceActuatorSettings* forceActuatorSettings, m1m3_logevent_ForceActuatorInfoC* forceInfo, m1m3_InclinometerDataC* inclinometerData, m1m3_ForceActuatorDataC* forceData) {
	this->forceActuatorApplicationSettings = forceActuatorApplicationSettings;
	this->forceActuatorSettings = forceActuatorSettings;
	this->forceInfo = forceInfo;
	this->inclinometerData = inclinometerData;
	this->forceData = forceData;
	this->applyingStaticForces = false;
	this->applyingOffsetForces = false;
	this->applyingAOSCorrection = false;
	this->applyingAberration = false;
	this->applyingElevationForces = false;
	this->zeroStaticForces();
	this->zeroOffsetForces();
	this->zeroAOSCorrection();
	this->zeroAberration();
	this->zeroElevationForces();
}

void ForceController::updateAppliedForces() {
	if (this->applyingElevationForces) {
		this->updateElevationForces();
	}
}

void ForceController::processAppliedForces() {
	this->sumAllForces();
	this->convertForcesToSetpoints();
}

void ForceController::applyStaticForces() {
	this->applyingStaticForces = true;
	for(int i = 0; i < FA_COUNT; ++i) {
		this->staticXSetpoint[i] = this->forceInfo->StaticXSetpoint[i];
		this->staticYSetpoint[i] = this->forceInfo->StaticYSetpoint[i];
		this->staticZSetpoint[i] = this->forceInfo->StaticZSetpoint[i];
	}
}

void ForceController::zeroStaticForces() {
	this->applyingStaticForces = false;
	for(int i = 0; i < FA_COUNT; ++i) {
		this->staticXSetpoint[i] = 0;
		this->staticYSetpoint[i] = 0;
		this->staticZSetpoint[i] = 0;
	}
}

void ForceController::applyOffsetForces(double* x, double* y, double* z) {
	this->applyingOffsetForces = true;
	for(int i = 0; i < FA_COUNT; ++i) {
		this->forceData->OffsetXSetpoint[i] = x[i];
		this->forceData->OffsetYSetpoint[i] = y[i];
		this->forceData->OffsetZSetpoint[i] = z[i];
	}
}

void ForceController::zeroOffsetForces() {
	this->applyingOffsetForces = false;
	for(int i = 0; i < FA_COUNT; ++i) {
		this->forceData->OffsetXSetpoint[i] = 0;
		this->forceData->OffsetYSetpoint[i] = 0;
		this->forceData->OffsetZSetpoint[i] = 0;
	}
}

void ForceController::applyAOSCorrectionByBendingModes(double* coefficients) {
	this->applyingAOSCorrection = false;
	for(int i = 0; i < FA_COUNT; ++i) {
		int mIndex = i * 22;
		this->forceData->ActiveOpticsZSetpoint[i] =
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
	}
}

void ForceController::applyAOSCorrectionByForces(double* z) {
	this->applyingAOSCorrection = false;
	for(int i = 0; i < FA_COUNT; ++i) {
		this->forceData->ActiveOpticsZSetpoint[i] = z[i];
	}
}

void ForceController::zeroAOSCorrection() {
	this->applyingAOSCorrection = false;
	for(int i = 0; i < FA_COUNT; ++i) {
		this->forceData->ActiveOpticsZSetpoint[i] = 0;
	}
}

void ForceController::applyAberrationByBendingModes(double* coefficients) {
	this->applyingAberration = false;
	for(int i = 0; i < FA_COUNT; ++i) {
		int mIndex = i * 22;
		this->forceData->AberationZSetpoint[i] =
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
	}
}

void ForceController::applyAberrationByForces(double* z) {
	this->applyingAberration = true;
	for(int i = 0; i < FA_COUNT; ++i) {
		this->forceData->AberationZSetpoint[i] = z[i];
	}
}

void ForceController::zeroAberration() {
	this->applyingAberration = false;
	for(int i = 0; i < FA_COUNT; ++i) {
		this->forceData->AberationZSetpoint[i] = 0;
	}
}

void ForceController::applyElevationForces() {
	this->applyingElevationForces = true;
}

void ForceController::zeroElevationForces() {
	this->applyingElevationForces = false;
	for(int i = 0; i < FA_COUNT; ++i) {
		this->forceData->ElevationXSetpoint[i] = 0;
		this->forceData->ElevationYSetpoint[i] = 0;
		this->forceData->ElevationZSetpoint[i] = 0;
	}
}

void ForceController::updateElevationForces() {
	double elevationAngle = this->inclinometerData->InclinometerAngle;
	double elevationMatrix[] = { std::pow(elevationAngle, 5.0), std::pow(elevationAngle, 4.0), std::pow(elevationAngle, 3.0), std::pow(elevationAngle, 2.0), elevationAngle, 1 };
	for(int i = 0; i < FA_COUNT; ++i) {
		int mIndex = i * 6;
		this->forceData->ElevationXSetpoint[i] =
				this->forceActuatorSettings->ElevationXAxisCoefficients[mIndex + 0] * elevationMatrix[0] +
				this->forceActuatorSettings->ElevationXAxisCoefficients[mIndex + 1] * elevationMatrix[1] +
				this->forceActuatorSettings->ElevationXAxisCoefficients[mIndex + 2] * elevationMatrix[2] +
				this->forceActuatorSettings->ElevationXAxisCoefficients[mIndex + 3] * elevationMatrix[3] +
				this->forceActuatorSettings->ElevationXAxisCoefficients[mIndex + 4] * elevationMatrix[4] +
				this->forceActuatorSettings->ElevationXAxisCoefficients[mIndex + 5];
		this->forceData->ElevationYSetpoint[i] =
				this->forceActuatorSettings->ElevationYAxisCoefficients[mIndex + 0] * elevationMatrix[0] +
				this->forceActuatorSettings->ElevationYAxisCoefficients[mIndex + 1] * elevationMatrix[1] +
				this->forceActuatorSettings->ElevationYAxisCoefficients[mIndex + 2] * elevationMatrix[2] +
				this->forceActuatorSettings->ElevationYAxisCoefficients[mIndex + 3] * elevationMatrix[3] +
				this->forceActuatorSettings->ElevationYAxisCoefficients[mIndex + 4] * elevationMatrix[4] +
				this->forceActuatorSettings->ElevationYAxisCoefficients[mIndex + 5];
		this->forceData->ElevationZSetpoint[i] =
				this->forceActuatorSettings->ElevationZAxisCoefficients[mIndex + 0] * elevationMatrix[0] +
				this->forceActuatorSettings->ElevationZAxisCoefficients[mIndex + 1] * elevationMatrix[1] +
				this->forceActuatorSettings->ElevationZAxisCoefficients[mIndex + 2] * elevationMatrix[2] +
				this->forceActuatorSettings->ElevationZAxisCoefficients[mIndex + 3] * elevationMatrix[3] +
				this->forceActuatorSettings->ElevationZAxisCoefficients[mIndex + 4] * elevationMatrix[4] +
				this->forceActuatorSettings->ElevationZAxisCoefficients[mIndex + 5];
	}
}

void ForceController::sumAllForces() {
	for(int i = 0; i < FA_COUNT; ++i) {
		this->forceData->XSetpoint[i] =
				this->staticXSetpoint[i] +
				this->forceData->OffsetXSetpoint[i] +
				this->forceData->ElevationXSetpoint[i];
		this->forceData->YSetpoint[i] =
				this->staticYSetpoint[i] +
				this->forceData->OffsetYSetpoint[i] +
				this->forceData->ElevationYSetpoint[i];
		this->forceData->ZSetpoint[i] =
				this->staticZSetpoint[i] +
				this->forceData->OffsetZSetpoint[i] +
				this->forceData->AberationZSetpoint[i] +
				this->forceData->ActiveOpticsZSetpoint[i] +
				this->forceData->ElevationZSetpoint[i];
	}
}

void ForceController::convertForcesToSetpoints() {
	for(int i = 0; i < FA_COUNT; i++) {
		switch(this->forceInfo->ActuatorOrientation[i]) {
		case ForceActuatorOrientations::PositiveY:
			this->forceData->PrimaryCylinderSetpointCommanded[i] = toInt24(this->forceData->ZSetpoint[i] - this->forceData->YSetpoint[i]);
			this->forceData->SecondaryCylinderSetpointCommanded[i] = toInt24(this->forceData->YSetpoint[i] * sqrt2);
			break;
		case ForceActuatorOrientations::NA:
			this->forceData->PrimaryCylinderSetpointCommanded[i] = toInt24(this->forceData->ZSetpoint[i]);
			break;
		case ForceActuatorOrientations::PositiveX:
			this->forceData->PrimaryCylinderSetpointCommanded[i] = toInt24(this->forceData->ZSetpoint[i] - this->forceData->XSetpoint[i]);
			this->forceData->SecondaryCylinderSetpointCommanded[i] = toInt24(this->forceData->XSetpoint[i] * sqrt2);
			break;
		case ForceActuatorOrientations::NegativeX:
			this->forceData->PrimaryCylinderSetpointCommanded[i] = toInt24(this->forceData->ZSetpoint[i] - -this->forceData->XSetpoint[i]);
			this->forceData->SecondaryCylinderSetpointCommanded[i] = toInt24(-this->forceData->XSetpoint[i] * sqrt2);
			break;
		case ForceActuatorOrientations::NegativeY:
			this->forceData->PrimaryCylinderSetpointCommanded[i] = toInt24(this->forceData->ZSetpoint[i] - -this->forceData->YSetpoint[i]);
			this->forceData->SecondaryCylinderSetpointCommanded[i] = toInt24(-this->forceData->YSetpoint[i] * sqrt2);
			break;
		}
	}
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
