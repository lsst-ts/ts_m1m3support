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

#include <Displacement.h>
#include <DisplacementSensorSettings.h>
#include <SupportFPGAData.h>
#include <M1M3SSPublisher.h>
#include <SafetyController.h>
#include <Timestamp.h>
#include <SAL_MTM1M3C.h>
#include <spdlog/spdlog.h>

#include <cstring>

namespace LSST {
namespace M1M3 {
namespace SS {

Displacement::Displacement(DisplacementSensorSettings* displacementSensorSettings, SupportFPGAData* fpgaData,
                           M1M3SSPublisher* publisher, SafetyController* safetyController) {
    spdlog::debug("Displacement: Displacement()");
    _displacementSensorSettings = displacementSensorSettings;
    _fpgaData = fpgaData;
    _publisher = publisher;
    _safetyController = safetyController;

    _imsData = _publisher->getIMSData();
    _displacementWarning = _publisher->getEventDisplacementSensorWarning();

    _lastSampleTimestamp = 0;
    _lastErrorTimestamp = 0;
    _errorCleared = false;

    memset(_imsData, 0, sizeof(MTM1M3_imsDataC));
    memset(_displacementWarning, 0, sizeof(MTM1M3_logevent_displacementSensorWarningC));
}

void Displacement::processData() {
    // TODO: Handle no data available
    // TODO: Handle displacement limits, push to safety controller
    spdlog::trace("Displacement: processData()");
    if (_fpgaData->DisplacementErrorTimestamp > _lastErrorTimestamp) {
        _lastErrorTimestamp = _fpgaData->DisplacementErrorTimestamp;
        _errorCleared = false;
        _displacementWarning->timestamp = Timestamp::fromFPGA(_fpgaData->DisplacementErrorTimestamp);
        _displacementWarning->anyWarning = _fpgaData->DisplacementErrorCode != 0;
        _displacementWarning->unknownProblem = _fpgaData->DisplacementErrorCode == 1;
        _displacementWarning->invalidResponse = _fpgaData->DisplacementErrorCode == 2;
        _displacementWarning->responseTimeout = _fpgaData->DisplacementErrorCode == 3;
        _displacementWarning->sensorReportsInvalidCommand = _fpgaData->DisplacementErrorCode == 4;
        _displacementWarning->sensorReportsCommunicationTimeoutError = _fpgaData->DisplacementErrorCode == 5;
        _displacementWarning->sensorReportsDataLengthError = _fpgaData->DisplacementErrorCode == 6;
        _displacementWarning->sensorReportsNumberOfParametersError = _fpgaData->DisplacementErrorCode == 7;
        _displacementWarning->sensorReportsParameterError = _fpgaData->DisplacementErrorCode == 8;
        _displacementWarning->sensorReportsCommunicationError = _fpgaData->DisplacementErrorCode == 9;
        _displacementWarning->sensorReportsIDNumberError = _fpgaData->DisplacementErrorCode == 10;
        _displacementWarning->sensorReportsExpansionLineError = _fpgaData->DisplacementErrorCode == 11;
        _displacementWarning->sensorReportsWriteControlError = _fpgaData->DisplacementErrorCode == 12;
        _safetyController->displacementNotifyUnknownProblem(_displacementWarning->unknownProblem);
        _safetyController->displacementNotifyInvalidResponse(_displacementWarning->invalidResponse);
        _safetyController->displacementNotifyResponseTimeoutError(_displacementWarning->responseTimeout);
        _safetyController->displacementNotifySensorReportsInvalidCommand(
                _displacementWarning->sensorReportsInvalidCommand);
        _safetyController->displacementNotifySensorReportsCommunicationTimeoutError(
                _displacementWarning->sensorReportsCommunicationTimeoutError);
        _safetyController->displacementNotifySensorReportsDataLengthError(
                _displacementWarning->sensorReportsDataLengthError);
        _safetyController->displacementNotifySensorReportsNumberOfParametersError(
                _displacementWarning->sensorReportsNumberOfParametersError);
        _safetyController->displacementNotifySensorReportsParameterError(
                _displacementWarning->sensorReportsParameterError);
        _safetyController->displacementNotifySensorReportsCommunicationError(
                _displacementWarning->sensorReportsCommunicationError);
        _safetyController->displacementNotifySensorReportsIDNumberError(
                _displacementWarning->sensorReportsIDNumberError);
        _safetyController->displacementNotifySensorReportsExpansionLineError(
                _displacementWarning->sensorReportsExpansionLineError);
        _safetyController->displacementNotifySensorReportsWriteControlError(
                _displacementWarning->sensorReportsWriteControlError);
        _publisher->tryLogDisplacementSensorWarning();
    }
    if (_fpgaData->DisplacementSampleTimestamp > _lastSampleTimestamp) {
        _lastSampleTimestamp = _fpgaData->DisplacementSampleTimestamp;
        _imsData->timestamp = Timestamp::fromFPGA(_fpgaData->DisplacementSampleTimestamp);
        // We need to swap the direction of the IMS reading, note sensor 5 and 7 are mounted opposite
        // We also need to convert the raw sensor values to millimeters
        _imsData->rawSensorData[0] =
                50.0 - ((_fpgaData->DisplacementRaw1 + _displacementSensorSettings->N1Offset) / 10000.0);
        _imsData->rawSensorData[1] =
                50.0 - ((_fpgaData->DisplacementRaw2 + _displacementSensorSettings->N2Offset) / 10000.0);
        _imsData->rawSensorData[2] =
                50.0 - ((_fpgaData->DisplacementRaw3 + _displacementSensorSettings->N3Offset) / 10000.0);
        _imsData->rawSensorData[3] =
                50.0 - ((_fpgaData->DisplacementRaw4 + _displacementSensorSettings->N4Offset) / 10000.0);
        _imsData->rawSensorData[4] =
                50.0 - ((_fpgaData->DisplacementRaw5 + _displacementSensorSettings->N5Offset) / 10000.0);
        _imsData->rawSensorData[5] =
                ((_fpgaData->DisplacementRaw6 + _displacementSensorSettings->N6Offset) / 10000.0);
        _imsData->rawSensorData[6] =
                50.0 - ((_fpgaData->DisplacementRaw7 + _displacementSensorSettings->N7Offset) / 10000.0);
        _imsData->rawSensorData[7] =
                ((_fpgaData->DisplacementRaw8 + _displacementSensorSettings->N8Offset) / 10000.0);
        int p1 = _displacementSensorSettings->N1Port;
        int p2 = _displacementSensorSettings->N2Port;
        int p3 = _displacementSensorSettings->N3Port;
        int p4 = _displacementSensorSettings->N4Port;
        int p5 = _displacementSensorSettings->N5Port;
        int p6 = _displacementSensorSettings->N6Port;
        int p7 = _displacementSensorSettings->N7Port;
        int p8 = _displacementSensorSettings->N8Port;
        _imsData->xPosition =
                (_displacementSensorSettings->ConverterMatrix[0] * _imsData->rawSensorData[p1] +
                 _displacementSensorSettings->ConverterMatrix[1] * _imsData->rawSensorData[p2] +
                 _displacementSensorSettings->ConverterMatrix[2] * _imsData->rawSensorData[p3] +
                 _displacementSensorSettings->ConverterMatrix[3] * _imsData->rawSensorData[p4] +
                 _displacementSensorSettings->ConverterMatrix[4] * _imsData->rawSensorData[p5] +
                 _displacementSensorSettings->ConverterMatrix[5] * _imsData->rawSensorData[p6] +
                 _displacementSensorSettings->ConverterMatrix[6] * _imsData->rawSensorData[p7] +
                 _displacementSensorSettings->ConverterMatrix[7] * _imsData->rawSensorData[p8]) /
                        MILLIMETERS_PER_METER +
                _displacementSensorSettings->XPositionOffset;
        _imsData->yPosition =
                (_displacementSensorSettings->ConverterMatrix[8] * _imsData->rawSensorData[p1] +
                 _displacementSensorSettings->ConverterMatrix[9] * _imsData->rawSensorData[p2] +
                 _displacementSensorSettings->ConverterMatrix[10] * _imsData->rawSensorData[p3] +
                 _displacementSensorSettings->ConverterMatrix[11] * _imsData->rawSensorData[p4] +
                 _displacementSensorSettings->ConverterMatrix[12] * _imsData->rawSensorData[p5] +
                 _displacementSensorSettings->ConverterMatrix[13] * _imsData->rawSensorData[p6] +
                 _displacementSensorSettings->ConverterMatrix[14] * _imsData->rawSensorData[p7] +
                 _displacementSensorSettings->ConverterMatrix[15] * _imsData->rawSensorData[p8]) /
                        MILLIMETERS_PER_METER +
                _displacementSensorSettings->YPositionOffset;
        _imsData->zPosition =
                (_displacementSensorSettings->ConverterMatrix[16] * _imsData->rawSensorData[p1] +
                 _displacementSensorSettings->ConverterMatrix[17] * _imsData->rawSensorData[p2] +
                 _displacementSensorSettings->ConverterMatrix[18] * _imsData->rawSensorData[p3] +
                 _displacementSensorSettings->ConverterMatrix[19] * _imsData->rawSensorData[p4] +
                 _displacementSensorSettings->ConverterMatrix[20] * _imsData->rawSensorData[p5] +
                 _displacementSensorSettings->ConverterMatrix[21] * _imsData->rawSensorData[p6] +
                 _displacementSensorSettings->ConverterMatrix[22] * _imsData->rawSensorData[p7] +
                 _displacementSensorSettings->ConverterMatrix[23] * _imsData->rawSensorData[p8]) /
                        MILLIMETERS_PER_METER +
                _displacementSensorSettings->ZPositionOffset;
        _imsData->xRotation =
                (_displacementSensorSettings->ConverterMatrix[24] * _imsData->rawSensorData[p1] +
                 _displacementSensorSettings->ConverterMatrix[25] * _imsData->rawSensorData[p2] +
                 _displacementSensorSettings->ConverterMatrix[26] * _imsData->rawSensorData[p3] +
                 _displacementSensorSettings->ConverterMatrix[27] * _imsData->rawSensorData[p4] +
                 _displacementSensorSettings->ConverterMatrix[28] * _imsData->rawSensorData[p5] +
                 _displacementSensorSettings->ConverterMatrix[29] * _imsData->rawSensorData[p6] +
                 _displacementSensorSettings->ConverterMatrix[30] * _imsData->rawSensorData[p7] +
                 _displacementSensorSettings->ConverterMatrix[31] * _imsData->rawSensorData[p8]) /
                        MILLIMETERS_PER_METER +
                _displacementSensorSettings->XRotationOffset;
        _imsData->yRotation =
                (_displacementSensorSettings->ConverterMatrix[32] * _imsData->rawSensorData[p1] +
                 _displacementSensorSettings->ConverterMatrix[33] * _imsData->rawSensorData[p2] +
                 _displacementSensorSettings->ConverterMatrix[34] * _imsData->rawSensorData[p3] +
                 _displacementSensorSettings->ConverterMatrix[35] * _imsData->rawSensorData[p4] +
                 _displacementSensorSettings->ConverterMatrix[36] * _imsData->rawSensorData[p5] +
                 _displacementSensorSettings->ConverterMatrix[37] * _imsData->rawSensorData[p6] +
                 _displacementSensorSettings->ConverterMatrix[38] * _imsData->rawSensorData[p7] +
                 _displacementSensorSettings->ConverterMatrix[39] * _imsData->rawSensorData[p8]) /
                        MILLIMETERS_PER_METER +
                _displacementSensorSettings->YRotationOffset;
        _imsData->zRotation =
                (_displacementSensorSettings->ConverterMatrix[40] * _imsData->rawSensorData[p1] +
                 _displacementSensorSettings->ConverterMatrix[41] * _imsData->rawSensorData[p2] +
                 _displacementSensorSettings->ConverterMatrix[42] * _imsData->rawSensorData[p3] +
                 _displacementSensorSettings->ConverterMatrix[43] * _imsData->rawSensorData[p4] +
                 _displacementSensorSettings->ConverterMatrix[44] * _imsData->rawSensorData[p5] +
                 _displacementSensorSettings->ConverterMatrix[45] * _imsData->rawSensorData[p6] +
                 _displacementSensorSettings->ConverterMatrix[46] * _imsData->rawSensorData[p7] +
                 _displacementSensorSettings->ConverterMatrix[47] * _imsData->rawSensorData[p8]) /
                        MILLIMETERS_PER_METER +
                _displacementSensorSettings->ZRotationOffset;
        _publisher->putIMSData();
        if (!_errorCleared &&
            _fpgaData->DisplacementSampleTimestamp > _fpgaData->DisplacementErrorTimestamp) {
            _errorCleared = true;
            _displacementWarning->timestamp = Timestamp::fromFPGA(_fpgaData->DisplacementSampleTimestamp);
            _displacementWarning->unknownProblem = false;
            _displacementWarning->invalidResponse = false;
            _displacementWarning->responseTimeout = false;
            _displacementWarning->sensorReportsInvalidCommand = false;
            _displacementWarning->sensorReportsCommunicationTimeoutError = false;
            _displacementWarning->sensorReportsDataLengthError = false;
            _displacementWarning->sensorReportsNumberOfParametersError = false;
            _displacementWarning->sensorReportsParameterError = false;
            _displacementWarning->sensorReportsCommunicationError = false;
            _displacementWarning->sensorReportsIDNumberError = false;
            _displacementWarning->sensorReportsExpansionLineError = false;
            _displacementWarning->sensorReportsWriteControlError = false;
            _safetyController->displacementNotifyUnknownProblem(_displacementWarning->unknownProblem);
            _safetyController->displacementNotifyInvalidResponse(_displacementWarning->invalidResponse);
            _safetyController->displacementNotifyResponseTimeoutError(_displacementWarning->responseTimeout);
            _safetyController->displacementNotifySensorReportsInvalidCommand(
                    _displacementWarning->sensorReportsInvalidCommand);
            _safetyController->displacementNotifySensorReportsCommunicationTimeoutError(
                    _displacementWarning->sensorReportsCommunicationTimeoutError);
            _safetyController->displacementNotifySensorReportsDataLengthError(
                    _displacementWarning->sensorReportsDataLengthError);
            _safetyController->displacementNotifySensorReportsNumberOfParametersError(
                    _displacementWarning->sensorReportsNumberOfParametersError);
            _safetyController->displacementNotifySensorReportsParameterError(
                    _displacementWarning->sensorReportsParameterError);
            _safetyController->displacementNotifySensorReportsCommunicationError(
                    _displacementWarning->sensorReportsCommunicationError);
            _safetyController->displacementNotifySensorReportsIDNumberError(
                    _displacementWarning->sensorReportsIDNumberError);
            _safetyController->displacementNotifySensorReportsExpansionLineError(
                    _displacementWarning->sensorReportsExpansionLineError);
            _safetyController->displacementNotifySensorReportsWriteControlError(
                    _displacementWarning->sensorReportsWriteControlError);
            _publisher->tryLogDisplacementSensorWarning();
        }
    }
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
