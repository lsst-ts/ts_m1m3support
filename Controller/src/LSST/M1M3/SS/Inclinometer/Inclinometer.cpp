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
#include <SAL_MTM1M3C.h>
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
	this->errorCleared = false;

	memset(this->inclinometerData, 0, sizeof(MTM1M3_inclinometerDataC));
	memset(this->inclinometerWarning, 0, sizeof(MTM1M3_logevent_inclinometerSensorWarningC));
}

void Inclinometer::processData() {
	// TODO: Handle no data available
	// TODO: Handle limits, push to safety controller
	Log.Trace("Inclinometer: processData()");
	if (this->fpgaData->InclinometerErrorTimestamp > this->lastErrorTimestamp) {
		this->lastErrorTimestamp = this->fpgaData->InclinometerErrorTimestamp;
		this->errorCleared = false;
		this->inclinometerWarning->timestamp = Timestamp::fromFPGA(this->fpgaData->InclinometerErrorTimestamp);
		this->inclinometerWarning->unknownAddress = this->fpgaData->InclinometerErrorCode == 1;
		this->inclinometerWarning->unknownFunction = this->fpgaData->InclinometerErrorCode == 2;
		this->inclinometerWarning->invalidLength = this->fpgaData->InclinometerErrorCode == 3;
		this->inclinometerWarning->invalidCRC = this->fpgaData->InclinometerErrorCode == 4;
		this->inclinometerWarning->unknownProblem = this->fpgaData->InclinometerErrorCode == 5;
		this->inclinometerWarning->responseTimeout = this->fpgaData->InclinometerErrorCode == 6;
		this->inclinometerWarning->sensorReportsIllegalFunction = this->fpgaData->InclinometerErrorCode == 7;
		this->inclinometerWarning->sensorReportsIllegalDataAddress = this->fpgaData->InclinometerErrorCode == 8;
		this->publisher->tryLogInclinometerSensorWarning();
		this->safetyController->inclinometerNotifyUnknownAddress(this->inclinometerWarning->unknownAddress);
		this->safetyController->inclinometerNotifyUnknownFunction(this->inclinometerWarning->unknownFunction);
		this->safetyController->inclinometerNotifyInvalidLength(this->inclinometerWarning->invalidLength);
		this->safetyController->inclinometerNotifyInvalidCRC(this->inclinometerWarning->invalidCRC);
		this->safetyController->inclinometerNotifyUnknownProblem(this->inclinometerWarning->unknownProblem);
		this->safetyController->inclinometerNotifyResponseTimeout(this->inclinometerWarning->responseTimeout);
		this->safetyController->inclinometerNotifySensorReportsIllegalFunction(this->inclinometerWarning->sensorReportsIllegalFunction);
		this->safetyController->inclinometerNotifySensorReportsIllegalDataAddress(this->inclinometerWarning->sensorReportsIllegalDataAddress);
	}
	if (this->fpgaData->InclinometerSampleTimestamp > this->lastSampleTimestamp) {
		this->lastSampleTimestamp = this->fpgaData->InclinometerSampleTimestamp;
		this->inclinometerData->timestamp = Timestamp::fromFPGA(this->fpgaData->InclinometerSampleTimestamp);
		// 0 = Zenith, we want 0 to = Horizon
		// Also we want below horizon to be negative
		float angle = (float)(this->fpgaData->InclinometerAngleRaw / 1000.0) + 90.0;
		if (angle > 180.0) {
			angle = angle - 360.0;
		}
		this->inclinometerData->inclinometerAngle = angle + this->inclinometerSettings->Offset;
		this->publisher->putInclinometerData();
		if (!this->errorCleared && this->fpgaData->InclinometerSampleTimestamp > this->fpgaData->InclinometerErrorTimestamp) {
			this->errorCleared = true;
			this->inclinometerWarning->timestamp = Timestamp::fromFPGA(this->fpgaData->InclinometerSampleTimestamp);
			this->inclinometerWarning->unknownAddress = false;
			this->inclinometerWarning->unknownFunction = false;
			this->inclinometerWarning->invalidLength = false;
			this->inclinometerWarning->invalidCRC = false;
			this->inclinometerWarning->unknownProblem = false;
			this->inclinometerWarning->responseTimeout = false;
			this->inclinometerWarning->sensorReportsIllegalFunction = false;
			this->inclinometerWarning->sensorReportsIllegalDataAddress = false;
			this->publisher->tryLogInclinometerSensorWarning();
			this->safetyController->inclinometerNotifyUnknownAddress(this->inclinometerWarning->unknownAddress);
			this->safetyController->inclinometerNotifyUnknownFunction(this->inclinometerWarning->unknownFunction);
			this->safetyController->inclinometerNotifyInvalidLength(this->inclinometerWarning->invalidLength);
			this->safetyController->inclinometerNotifyInvalidCRC(this->inclinometerWarning->invalidCRC);
			this->safetyController->inclinometerNotifyUnknownProblem(this->inclinometerWarning->unknownProblem);
			this->safetyController->inclinometerNotifyResponseTimeout(this->inclinometerWarning->responseTimeout);
			this->safetyController->inclinometerNotifySensorReportsIllegalFunction(this->inclinometerWarning->sensorReportsIllegalFunction);
			this->safetyController->inclinometerNotifySensorReportsIllegalDataAddress(this->inclinometerWarning->sensorReportsIllegalDataAddress);
		}
	}
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
