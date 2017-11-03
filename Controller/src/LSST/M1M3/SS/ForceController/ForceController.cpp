/*
 * ForceController.cpp
 *
 *  Created on: Oct 23, 2017
 *      Author: ccontaxis
 */

#include <ForceController.h>
#include <ForceActuatorOrientations.h>
#include <ForceActuatorSettings.h>
#include <IPublisher.h>
#include <SAL_m1m3C.h>
#include <cmath>
#include <cstring>
#include <vector>

namespace LSST {
namespace M1M3 {
namespace SS {

ForceController::ForceController(ForceActuatorApplicationSettings* forceActuatorApplicationSettings, ForceActuatorSettings* forceActuatorSettings, IPublisher* publisher) {
	this->forceActuatorApplicationSettings = forceActuatorApplicationSettings;
	this->forceActuatorSettings = forceActuatorSettings;
	this->publisher = publisher;
	this->appliedForces = this->publisher->getEventAppliedForces();
	this->forceInfo = this->publisher->getEventForceActuatorInfo();
	this->inclinometerData = this->publisher->getInclinometerData();
	this->forceData = this->publisher->getForceActuatorData();
	this->appliedForces->Timestamp = this->publisher->getTimestamp();
	this->appliedForces->StaticForcesApplied = false;
	this->appliedForces->OffsetForcesApplied = false;
	this->appliedForces->ElevationForcesApplied = false;
	this->appliedForces->AzimuthForcesApplied = false;
	this->appliedForces->TemperatureForcesApplied = false;
	this->appliedForces->AOSCorrectionApplied = false;
	this->appliedForces->AberrationApplied = false;
	this->appliedForces->DynamicForcesApplied = false;
	this->appliedForces->HardpointOffloadingForcesApplied = false;
	this->publisher->logAppliedForces();
	for(int i = 0; i < FA_COUNT; i++) {
		this->staticXSetpoint[i] = 0;
		this->staticYSetpoint[i] = 0;
		this->staticZSetpoint[i] = 0;
		this->forceData->OffsetXSetpoint[i] = 0;
		this->forceData->OffsetYSetpoint[i] = 0;
		this->forceData->OffsetZSetpoint[i] = 0;
		this->forceData->ActiveOpticsZSetpoint[i] = 0;
		this->forceData->AberationZSetpoint[i] = 0;
		this->forceData->ElevationXSetpoint[i] = 0;
		this->forceData->ElevationYSetpoint[i] = 0;
		this->forceData->ElevationZSetpoint[i] = 0;
	}
	//memset(&this->tmaAzimuthData, 0, sizeof(MTMount_AzC));
	//memset(&this->tmaElevationData, 0, sizeof(MTMount_AltC));
}

void ForceController::updateTMAAzimuthData(MTMount_AzC* tmaAzimuthData) {
	//memcpy(&this->tmaAzimuthData, tmaAzimuthData, sizeof(MTMount_AzC));
}

void ForceController::updateTMAElevationData(MTMount_AltC* tmaElevationData) {
	//memcpy(&this->tmaElevationData, tmaElevationData, sizeof(MTMount_AltC));
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
}

void ForceController::processAppliedForces() {
	this->sumAllForces();
	this->convertForcesToSetpoints();
}

void ForceController::applyStaticForces() {
	this->appliedForces->StaticForcesApplied = true;
	for(int i = 0; i < FA_COUNT; ++i) {
		this->staticXSetpoint[i] = this->forceInfo->StaticXSetpoint[i];
		this->staticYSetpoint[i] = this->forceInfo->StaticYSetpoint[i];
		this->staticZSetpoint[i] = this->forceInfo->StaticZSetpoint[i];
	}
	this->publishAppliedForces();
}

void ForceController::zeroStaticForces() {
	this->appliedForces->StaticForcesApplied = false;
	for(int i = 0; i < FA_COUNT; ++i) {
		this->staticXSetpoint[i] = 0;
		this->staticYSetpoint[i] = 0;
		this->staticZSetpoint[i] = 0;
	}
	this->publishAppliedForces();
}

void ForceController::applyOffsetForces(double* x, double* y, double* z) {
	this->appliedForces->OffsetForcesApplied = true;
	for(int i = 0; i < FA_COUNT; ++i) {
		this->forceData->OffsetXSetpoint[i] = x[i];
		this->forceData->OffsetYSetpoint[i] = y[i];
		this->forceData->OffsetZSetpoint[i] = z[i];
	}
	this->publishAppliedForces();
}

void ForceController::zeroOffsetForces() {
	this->appliedForces->OffsetForcesApplied = false;
	for(int i = 0; i < FA_COUNT; ++i) {
		this->forceData->OffsetXSetpoint[i] = 0;
		this->forceData->OffsetYSetpoint[i] = 0;
		this->forceData->OffsetZSetpoint[i] = 0;
	}
	this->publishAppliedForces();
}

void ForceController::applyAOSCorrectionByBendingModes(double* coefficients) {
	this->appliedForces->AOSCorrectionApplied = true;
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
	this->publishAppliedForces();
}

void ForceController::applyAOSCorrectionByForces(double* z) {
	this->appliedForces->AOSCorrectionApplied = true;
	for(int i = 0; i < FA_COUNT; ++i) {
		this->forceData->ActiveOpticsZSetpoint[i] = z[i];
	}
	this->publishAppliedForces();
}

void ForceController::zeroAOSCorrection() {
	this->appliedForces->AOSCorrectionApplied = false;
	for(int i = 0; i < FA_COUNT; ++i) {
		this->forceData->ActiveOpticsZSetpoint[i] = 0;
	}
	this->publishAppliedForces();
}

void ForceController::applyAberrationByBendingModes(double* coefficients) {
	this->appliedForces->AberrationApplied = true;
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
	this->publishAppliedForces();
}

void ForceController::applyAberrationByForces(double* z) {
	this->appliedForces->AberrationApplied = true;
	for(int i = 0; i < FA_COUNT; ++i) {
		this->forceData->AberationZSetpoint[i] = z[i];
	}
	this->publishAppliedForces();
}

void ForceController::zeroAberration() {
	this->appliedForces->AberrationApplied = false;
	for(int i = 0; i < FA_COUNT; ++i) {
		this->forceData->AberationZSetpoint[i] = 0;
	}
	this->publishAppliedForces();
}

void ForceController::applyElevationForces() {
	this->appliedForces->ElevationForcesApplied = true;
	this->publishAppliedForces();
}

void ForceController::zeroElevationForces() {
	this->appliedForces->ElevationForcesApplied = false;
	for(int i = 0; i < FA_COUNT; ++i) {
		this->forceData->ElevationXSetpoint[i] = 0;
		this->forceData->ElevationYSetpoint[i] = 0;
		this->forceData->ElevationZSetpoint[i] = 0;
	}
	this->publishAppliedForces();
}

void ForceController::applyAzimuthForces() {
	this->appliedForces->AzimuthForcesApplied = true;
	this->publishAppliedForces();
}

void ForceController::zeroAzimuthForces() {
	this->appliedForces->AzimuthForcesApplied = false;
	for(int i = 0; i < FA_COUNT; ++i) {
		this->forceData->AzimuthXSetpoint[i] = 0;
		this->forceData->AzimuthYSetpoint[i] = 0;
		this->forceData->AzimuthZSetpoint[i] = 0;
	}
	this->publishAppliedForces();
}

void ForceController::applyTemperatureForces() {
	this->appliedForces->TemperatureForcesApplied = true;
	this->publishAppliedForces();
}

void ForceController::zeroTemperatureForces() {
	this->appliedForces->TemperatureForcesApplied = false;
	for(int i = 0; i < FA_COUNT; ++i) {
		this->forceData->TemperatureXSetpoint[i] = 0;
		this->forceData->TemperatureYSetpoint[i] = 0;
		this->forceData->TemperatureZSetpoint[i] = 0;
	}
	this->publishAppliedForces();
}

void ForceController::applyDynamicForces() {
	this->appliedForces->DynamicForcesApplied = true;
	this->publishAppliedForces();
}

void ForceController::zeroDynamicForces() {
	this->appliedForces->DynamicForcesApplied = false;
	for(int i = 0; i < FA_COUNT; ++i) {
		this->forceData->DynamicXSetpoint[i] = 0;
		this->forceData->DynamicYSetpoint[i] = 0;
		this->forceData->DynamicZSetpoint[i] = 0;
	}
	this->publishAppliedForces();
}

void ForceController::updateElevationForces() {
	double elevationAngle = this->inclinometerData->InclinometerAngle;//this->forceActuatorSettings->UseInclinometer ? this->inclinometerData->InclinometerAngle : this->tmaElevationData.Angle_Actual;
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

void ForceController::updateAzimuthForces() {
	double azimuthAngle = 0;//this->tmaAzimuthData.Angle_Actual;
	double azimuthMatrix[] = { std::pow(azimuthAngle, 5.0), std::pow(azimuthAngle, 4.0), std::pow(azimuthAngle, 3.0), std::pow(azimuthAngle, 2.0), azimuthAngle, 1 };
	for(int i = 0; i < FA_COUNT; ++i) {
		int mIndex = i * 6;
		this->forceData->AzimuthXSetpoint[i] =
				this->forceActuatorSettings->AzimuthXAxisCoefficients[mIndex + 0] * azimuthMatrix[0] +
				this->forceActuatorSettings->AzimuthXAxisCoefficients[mIndex + 1] * azimuthMatrix[1] +
				this->forceActuatorSettings->AzimuthXAxisCoefficients[mIndex + 2] * azimuthMatrix[2] +
				this->forceActuatorSettings->AzimuthXAxisCoefficients[mIndex + 3] * azimuthMatrix[3] +
				this->forceActuatorSettings->AzimuthXAxisCoefficients[mIndex + 4] * azimuthMatrix[4] +
				this->forceActuatorSettings->AzimuthXAxisCoefficients[mIndex + 5];
		this->forceData->AzimuthYSetpoint[i] =
				this->forceActuatorSettings->AzimuthYAxisCoefficients[mIndex + 0] * azimuthMatrix[0] +
				this->forceActuatorSettings->AzimuthYAxisCoefficients[mIndex + 1] * azimuthMatrix[1] +
				this->forceActuatorSettings->AzimuthYAxisCoefficients[mIndex + 2] * azimuthMatrix[2] +
				this->forceActuatorSettings->AzimuthYAxisCoefficients[mIndex + 3] * azimuthMatrix[3] +
				this->forceActuatorSettings->AzimuthYAxisCoefficients[mIndex + 4] * azimuthMatrix[4] +
				this->forceActuatorSettings->AzimuthYAxisCoefficients[mIndex + 5];
		this->forceData->AzimuthZSetpoint[i] =
				this->forceActuatorSettings->AzimuthZAxisCoefficients[mIndex + 0] * azimuthMatrix[0] +
				this->forceActuatorSettings->AzimuthZAxisCoefficients[mIndex + 1] * azimuthMatrix[1] +
				this->forceActuatorSettings->AzimuthZAxisCoefficients[mIndex + 2] * azimuthMatrix[2] +
				this->forceActuatorSettings->AzimuthZAxisCoefficients[mIndex + 3] * azimuthMatrix[3] +
				this->forceActuatorSettings->AzimuthZAxisCoefficients[mIndex + 4] * azimuthMatrix[4] +
				this->forceActuatorSettings->AzimuthZAxisCoefficients[mIndex + 5];
	}
}

void ForceController::updateTemperatureForces() {
	double temperature = 0; // TODO: Update
	double temperatureMatrix[] = { std::pow(temperature, 5.0), std::pow(temperature, 4.0), std::pow(temperature, 3.0), std::pow(temperature, 2.0), temperature, 1 };
	for(int i = 0; i < FA_COUNT; ++i) {
		int mIndex = i * 6;
		this->forceData->TemperatureXSetpoint[i] =
				this->forceActuatorSettings->TemperatureXAxisCoefficients[mIndex + 0] * temperatureMatrix[0] +
				this->forceActuatorSettings->TemperatureXAxisCoefficients[mIndex + 1] * temperatureMatrix[1] +
				this->forceActuatorSettings->TemperatureXAxisCoefficients[mIndex + 2] * temperatureMatrix[2] +
				this->forceActuatorSettings->TemperatureXAxisCoefficients[mIndex + 3] * temperatureMatrix[3] +
				this->forceActuatorSettings->TemperatureXAxisCoefficients[mIndex + 4] * temperatureMatrix[4] +
				this->forceActuatorSettings->TemperatureXAxisCoefficients[mIndex + 5];
		this->forceData->TemperatureYSetpoint[i] =
				this->forceActuatorSettings->TemperatureYAxisCoefficients[mIndex + 0] * temperatureMatrix[0] +
				this->forceActuatorSettings->TemperatureYAxisCoefficients[mIndex + 1] * temperatureMatrix[1] +
				this->forceActuatorSettings->TemperatureYAxisCoefficients[mIndex + 2] * temperatureMatrix[2] +
				this->forceActuatorSettings->TemperatureYAxisCoefficients[mIndex + 3] * temperatureMatrix[3] +
				this->forceActuatorSettings->TemperatureYAxisCoefficients[mIndex + 4] * temperatureMatrix[4] +
				this->forceActuatorSettings->TemperatureYAxisCoefficients[mIndex + 5];
		this->forceData->TemperatureZSetpoint[i] =
				this->forceActuatorSettings->TemperatureZAxisCoefficients[mIndex + 0] * temperatureMatrix[0] +
				this->forceActuatorSettings->TemperatureZAxisCoefficients[mIndex + 1] * temperatureMatrix[1] +
				this->forceActuatorSettings->TemperatureZAxisCoefficients[mIndex + 2] * temperatureMatrix[2] +
				this->forceActuatorSettings->TemperatureZAxisCoefficients[mIndex + 3] * temperatureMatrix[3] +
				this->forceActuatorSettings->TemperatureZAxisCoefficients[mIndex + 4] * temperatureMatrix[4] +
				this->forceActuatorSettings->TemperatureZAxisCoefficients[mIndex + 5];
	}
}

void ForceController::updateDynamicForces() {

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

void ForceController::publishAppliedForces() {
	this->appliedForces->Timestamp = this->publisher->getTimestamp();
	this->publisher->logAppliedForces();
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
