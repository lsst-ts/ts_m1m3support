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
#include <SALEnumerations.h>
#include <BitHelper.h>
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
        BitHelper::set(&this->inclinometerWarning->inclinometerSensorFlags, InclinometerSensorFlags::UnknownAddress, this->fpgaData->InclinometerErrorCode == 1);
        BitHelper::set(&this->inclinometerWarning->inclinometerSensorFlags, InclinometerSensorFlags::UnknownFunction, this->fpgaData->InclinometerErrorCode == 2);
        BitHelper::set(&this->inclinometerWarning->inclinometerSensorFlags, InclinometerSensorFlags::InvalidLength, this->fpgaData->InclinometerErrorCode == 3);
        BitHelper::set(&this->inclinometerWarning->inclinometerSensorFlags, InclinometerSensorFlags::InvalidCRC, this->fpgaData->InclinometerErrorCode == 4);
        BitHelper::set(&this->inclinometerWarning->inclinometerSensorFlags, InclinometerSensorFlags::UnknownProblem, this->fpgaData->InclinometerErrorCode == 5);
        BitHelper::set(&this->inclinometerWarning->inclinometerSensorFlags, InclinometerSensorFlags::ResponseTimeout, this->fpgaData->InclinometerErrorCode == 6);
        BitHelper::set(&this->inclinometerWarning->inclinometerSensorFlags, InclinometerSensorFlags::SensorReportsIllegalFunction, this->fpgaData->InclinometerErrorCode == 7);
        BitHelper::set(&this->inclinometerWarning->inclinometerSensorFlags, InclinometerSensorFlags::SensorReportsIllegalDataAddress, this->fpgaData->InclinometerErrorCode == 8);
        this->publisher->tryLogInclinometerSensorWarning();
        this->safetyController->inclinometerNotifyUnknownAddress(BitHelper::get(this->inclinometerWarning->inclinometerSensorFlags, InclinometerSensorFlags::UnknownAddress));
        this->safetyController->inclinometerNotifyUnknownFunction(BitHelper::get(this->inclinometerWarning->inclinometerSensorFlags, InclinometerSensorFlags::UnknownFunction));
        this->safetyController->inclinometerNotifyInvalidLength(BitHelper::get(this->inclinometerWarning->inclinometerSensorFlags, InclinometerSensorFlags::InvalidLength));
        this->safetyController->inclinometerNotifyInvalidCRC(BitHelper::get(this->inclinometerWarning->inclinometerSensorFlags, InclinometerSensorFlags::InvalidCRC));
        this->safetyController->inclinometerNotifyUnknownProblem(BitHelper::get(this->inclinometerWarning->inclinometerSensorFlags, InclinometerSensorFlags::UnknownProblem));
        this->safetyController->inclinometerNotifyResponseTimeout(BitHelper::get(this->inclinometerWarning->inclinometerSensorFlags, InclinometerSensorFlags::ResponseTimeout));
        this->safetyController->inclinometerNotifySensorReportsIllegalFunction(BitHelper::get(this->inclinometerWarning->inclinometerSensorFlags, InclinometerSensorFlags::SensorReportsIllegalFunction));
        this->safetyController->inclinometerNotifySensorReportsIllegalDataAddress(BitHelper::get(this->inclinometerWarning->inclinometerSensorFlags, InclinometerSensorFlags::SensorReportsIllegalDataAddress));
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
            this->inclinometerWarning->inclinometerSensorFlags = 0;
            this->publisher->tryLogInclinometerSensorWarning();
            this->safetyController->inclinometerNotifyUnknownAddress(false);
            this->safetyController->inclinometerNotifyUnknownFunction(false);
            this->safetyController->inclinometerNotifyInvalidLength(false);
            this->safetyController->inclinometerNotifyInvalidCRC(false);
            this->safetyController->inclinometerNotifyUnknownProblem(false);
            this->safetyController->inclinometerNotifyResponseTimeout(false);
            this->safetyController->inclinometerNotifySensorReportsIllegalFunction(false);
            this->safetyController->inclinometerNotifySensorReportsIllegalDataAddress(false);
        }
    }
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
