/*
 * This file is part of LSST M1M3 support system package.
 *
 * Developed for the LSST Data Management System.
 * This product includes software developed by the LSST Project
 * (https://www.lsst.org).
 * See the COPYRIGHT file at the top-level directory of this distribution
 * for details of code ownership.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include <Inclinometer.h>
#include <SupportFPGAData.h>
#include <M1M3SSPublisher.h>
#include <SafetyController.h>
#include <InclinometerSettings.h>
#include <Timestamp.h>
#include <SAL_MTM1M3C.h>
#include <spdlog/spdlog.h>

#include <cstring>

namespace LSST {
namespace M1M3 {
namespace SS {

Inclinometer::Inclinometer(SupportFPGAData* fpgaData, M1M3SSPublisher* publisher,
                           SafetyController* safetyController, InclinometerSettings* inclinometerSettings) {
    spdlog::debug("Inclinometer: Inclinometer()");
    _fpgaData = fpgaData;
    _publisher = publisher;
    _safetyController = safetyController;
    _inclinometerSettings = inclinometerSettings;

    _inclinometerData = _publisher->getInclinometerData();
    _inclinometerWarning = _publisher->getEventInclinometerSensorWarning();

    _lastSampleTimestamp = 0;
    _lastErrorTimestamp = 0;
    _errorCleared = false;

    memset(_inclinometerData, 0, sizeof(MTM1M3_inclinometerDataC));
    memset(_inclinometerWarning, 0, sizeof(MTM1M3_logevent_inclinometerSensorWarningC));
}

void Inclinometer::processData() {
    // TODO: Handle no data available
    // TODO: Handle limits, push to safety controller
    spdlog::trace("Inclinometer: processData()");
    if (_fpgaData->InclinometerErrorTimestamp > _lastErrorTimestamp) {
        _lastErrorTimestamp = _fpgaData->InclinometerErrorTimestamp;
        _errorCleared = false;
        _inclinometerWarning->timestamp =
                Timestamp::fromFPGA(_fpgaData->InclinometerErrorTimestamp);
        _inclinometerWarning->unknownAddress = _fpgaData->InclinometerErrorCode == 1;
        _inclinometerWarning->unknownFunction = _fpgaData->InclinometerErrorCode == 2;
        _inclinometerWarning->invalidLength = _fpgaData->InclinometerErrorCode == 3;
        _inclinometerWarning->invalidCRC = _fpgaData->InclinometerErrorCode == 4;
        _inclinometerWarning->unknownProblem = _fpgaData->InclinometerErrorCode == 5;
        _inclinometerWarning->responseTimeout = _fpgaData->InclinometerErrorCode == 6;
        _inclinometerWarning->sensorReportsIllegalFunction = _fpgaData->InclinometerErrorCode == 7;
        _inclinometerWarning->sensorReportsIllegalDataAddress =
                _fpgaData->InclinometerErrorCode == 8;
        _publisher->tryLogInclinometerSensorWarning();
        _safetyController->inclinometerNotifyUnknownAddress(_inclinometerWarning->unknownAddress);
        _safetyController->inclinometerNotifyUnknownFunction(_inclinometerWarning->unknownFunction);
        _safetyController->inclinometerNotifyInvalidLength(_inclinometerWarning->invalidLength);
        _safetyController->inclinometerNotifyInvalidCRC(_inclinometerWarning->invalidCRC);
        _safetyController->inclinometerNotifyUnknownProblem(_inclinometerWarning->unknownProblem);
        _safetyController->inclinometerNotifyResponseTimeout(_inclinometerWarning->responseTimeout);
        _safetyController->inclinometerNotifySensorReportsIllegalFunction(
                _inclinometerWarning->sensorReportsIllegalFunction);
        _safetyController->inclinometerNotifySensorReportsIllegalDataAddress(
                _inclinometerWarning->sensorReportsIllegalDataAddress);
    }
    if (_fpgaData->InclinometerSampleTimestamp > _lastSampleTimestamp) {
        _lastSampleTimestamp = _fpgaData->InclinometerSampleTimestamp;
        _inclinometerData->timestamp = Timestamp::fromFPGA(_fpgaData->InclinometerSampleTimestamp);
        // 0 = Zenith, we want 0 to = Horizon
        // Also we want below horizon to be negative
        float angle = (float)(_fpgaData->InclinometerAngleRaw / 1000.0) + 90.0;
        if (angle > 180.0) {
            angle = angle - 360.0;
        }
        _inclinometerData->inclinometerAngle = angle + _inclinometerSettings->Offset;
        _publisher->putInclinometerData();
        if (!_errorCleared &&
            _fpgaData->InclinometerSampleTimestamp > _fpgaData->InclinometerErrorTimestamp) {
            _errorCleared = true;
            _inclinometerWarning->timestamp =
                    Timestamp::fromFPGA(_fpgaData->InclinometerSampleTimestamp);
            _inclinometerWarning->unknownAddress = false;
            _inclinometerWarning->unknownFunction = false;
            _inclinometerWarning->invalidLength = false;
            _inclinometerWarning->invalidCRC = false;
            _inclinometerWarning->unknownProblem = false;
            _inclinometerWarning->responseTimeout = false;
            _inclinometerWarning->sensorReportsIllegalFunction = false;
            _inclinometerWarning->sensorReportsIllegalDataAddress = false;
            _publisher->tryLogInclinometerSensorWarning();
            _safetyController->inclinometerNotifyUnknownAddress(
                    _inclinometerWarning->unknownAddress);
            _safetyController->inclinometerNotifyUnknownFunction(
                    _inclinometerWarning->unknownFunction);
            _safetyController->inclinometerNotifyInvalidLength(_inclinometerWarning->invalidLength);
            _safetyController->inclinometerNotifyInvalidCRC(_inclinometerWarning->invalidCRC);
            _safetyController->inclinometerNotifyUnknownProblem(
                    _inclinometerWarning->unknownProblem);
            _safetyController->inclinometerNotifyResponseTimeout(
                    _inclinometerWarning->responseTimeout);
            _safetyController->inclinometerNotifySensorReportsIllegalFunction(
                    _inclinometerWarning->sensorReportsIllegalFunction);
            _safetyController->inclinometerNotifySensorReportsIllegalDataAddress(
                    _inclinometerWarning->sensorReportsIllegalDataAddress);
        }
    }
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
