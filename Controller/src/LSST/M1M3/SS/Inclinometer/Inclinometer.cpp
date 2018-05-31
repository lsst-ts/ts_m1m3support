/*

 * Inclinometer.cpp
 *
 *  Created on: Oct 2, 2017
 *      Author: ccontaxis
 */

#include <Inclinometer.h>
#include <SupportFPGAData.h>
#include <M1M3SSPublisher.h>
#include <SafetyController.h>
#include <InclinometerSettings.h>
#include <Timestamp.h>
#include <SAL_m1m3C.h>
#include <Log.h>

#include <cstring>

namespace LSST {
namespace M1M3 {
namespace SS {

Inclinometer::Inclinometer(SupportFPGAData* fpgaData, M1M3SSPublisher* publisher, SafetyController* safetyController, InclinometerSettings* inclinometerSettings) {
	Log.Debug("Inclinometer: Inclinometer()");
	this->fpgaData = fpgaData;
	this->publisher = publisher;
	this->safetyController = safetyController;
	this->inclinometerSettings = inclinometerSettings;

	this->inclinometerData = this->publisher->getInclinometerData();
	this->inclinometerWarning = this->publisher->getEventInclinometerSensorWarning();

	this->lastSampleTimestamp = 0;
	this->lastErrorTimestamp = 0;

	memset(this->inclinometerData, 0, sizeof(m1m3_InclinometerDataC));
	memset(this->inclinometerWarning, 0, sizeof(m1m3_logevent_InclinometerSensorWarningC));
}

void Inclinometer::processData() {
	// TODO: Handle no data available
	// TODO: Handle limits, push to safety controller
	Log.Trace("Inclinometer: processData()");
	if (this->fpgaData->InclinometerSampleTimestamp != this->lastSampleTimestamp) {
		this->lastSampleTimestamp = this->fpgaData->InclinometerSampleTimestamp;
		this->inclinometerData->Timestamp = Timestamp::fromFPGA(this->fpgaData->InclinometerSampleTimestamp);
		// 0 = Zenith, we want 0 to = Horizon
		// Also we want below horizon to be negative
		float angle = (float)(this->fpgaData->InclinometerAngleRaw / 1000.0) + 90.0;
		if (angle > 180.0) {
			angle = angle - 360.0;
		}
		this->inclinometerData->InclinometerAngle = angle + this->inclinometerSettings->Offset;
		this->publisher->putInclinometerData();
	}
	if (this->fpgaData->InclinometerErrorTimestamp != this->lastErrorTimestamp) {
		this->lastErrorTimestamp = this->fpgaData->InclinometerErrorTimestamp;
		this->inclinometerWarning->Timestamp = Timestamp::fromFPGA(this->fpgaData->InclinometerErrorTimestamp);
		this->inclinometerWarning->UnknownAddress = this->fpgaData->InclinometerErrorCode == 1;
		this->inclinometerWarning->UnknownFunction = this->fpgaData->InclinometerErrorCode == 2;
		this->inclinometerWarning->InvalidLength = this->fpgaData->InclinometerErrorCode == 3;
		this->inclinometerWarning->InvalidCRC = this->fpgaData->InclinometerErrorCode == 4;
		this->inclinometerWarning->UnknownProblem = this->fpgaData->InclinometerErrorCode == 5;
		this->inclinometerWarning->ResponseTimeout = this->fpgaData->InclinometerErrorCode == 6;
		this->inclinometerWarning->SensorReportsIllegalFunction = this->fpgaData->InclinometerErrorCode == 7;
		this->inclinometerWarning->SensorReportsIllegalDataAddress = this->fpgaData->InclinometerErrorCode == 8;
		this->publisher->tryLogInclinometerSensorWarning();
	}
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
