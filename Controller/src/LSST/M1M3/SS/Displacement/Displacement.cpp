/*
 * Displacement.cpp
 *
 *  Created on: Oct 6, 2017
 *      Author: ccontaxis
 */

#include <Displacement.h>
#include <DisplacementSensorSettings.h>
#include <SupportFPGAData.h>
#include <M1M3SSPublisher.h>
#include <SafetyController.h>
#include <Timestamp.h>
#include <SAL_MTM1M3C.h>
#include <SALEnumerations.h>
#include <BitHelper.h>
#include <Log.h>

#include <cstring>

namespace LSST {
namespace M1M3 {
namespace SS {

Displacement::Displacement(DisplacementSensorSettings* displacementSensorSettings, SupportFPGAData* fpgaData, M1M3SSPublisher* publisher, SafetyController* safetyController) {
    Log.Debug("Displacement: Displacement()");
    this->displacementSensorSettings = displacementSensorSettings;
    this->fpgaData = fpgaData;
    this->publisher = publisher;
    this->safetyController = safetyController;

    this->imsData = this->publisher->getIMSData();
    this->displacementWarning = this->publisher->getEventDisplacementSensorWarning();

    this->lastSampleTimestamp = 0;
    this->lastErrorTimestamp = 0;
    this->errorCleared = false;

    memset(this->imsData, 0, sizeof(MTM1M3_imsDataC));
    memset(this->displacementWarning, 0, sizeof(MTM1M3_logevent_displacementSensorWarningC));
}

void Displacement::processData() {
    // TODO: Handle no data available
    // TODO: Handle displacement limits, push to safety controller
    Log.Trace("Displacement: processData()");
    if (this->fpgaData->DisplacementErrorTimestamp > this->lastErrorTimestamp) {
        this->lastErrorTimestamp = this->fpgaData->DisplacementErrorTimestamp;
        this->errorCleared = false;
        BitHelper::set(&this->displacementWarning->displacementSensorFlags, DisplacementSensorFlags::UnknownProblem, this->fpgaData->DisplacementErrorCode == 1);
        BitHelper::set(&this->displacementWarning->displacementSensorFlags, DisplacementSensorFlags::InvalidResponse, this->fpgaData->DisplacementErrorCode == 2);
        BitHelper::set(&this->displacementWarning->displacementSensorFlags, DisplacementSensorFlags::ResponseTimeout, this->fpgaData->DisplacementErrorCode == 3);
        BitHelper::set(&this->displacementWarning->displacementSensorFlags, DisplacementSensorFlags::SensorReportsInvalidCommand, this->fpgaData->DisplacementErrorCode == 4);
        BitHelper::set(&this->displacementWarning->displacementSensorFlags, DisplacementSensorFlags::SensorReportsCommunicationTimeoutError, this->fpgaData->DisplacementErrorCode == 5);
        BitHelper::set(&this->displacementWarning->displacementSensorFlags, DisplacementSensorFlags::SensorReportsDataLengthError, this->fpgaData->DisplacementErrorCode == 6);
        BitHelper::set(&this->displacementWarning->displacementSensorFlags, DisplacementSensorFlags::SensorReportsNumberOfParametersError, this->fpgaData->DisplacementErrorCode == 7);
        BitHelper::set(&this->displacementWarning->displacementSensorFlags, DisplacementSensorFlags::SensorReportsParameterError, this->fpgaData->DisplacementErrorCode == 8);
        BitHelper::set(&this->displacementWarning->displacementSensorFlags, DisplacementSensorFlags::SensorReportsCommunicationError, this->fpgaData->DisplacementErrorCode == 9);
        BitHelper::set(&this->displacementWarning->displacementSensorFlags, DisplacementSensorFlags::SensorReportsIDNumberError, this->fpgaData->DisplacementErrorCode == 10);
        BitHelper::set(&this->displacementWarning->displacementSensorFlags, DisplacementSensorFlags::SensorReportsExpansionLineError, this->fpgaData->DisplacementErrorCode == 11);
        BitHelper::set(&this->displacementWarning->displacementSensorFlags, DisplacementSensorFlags::SensorReportsWriteControlError, this->fpgaData->DisplacementErrorCode == 12);

        this->safetyController->displacementNotifyUnknownProblem(BitHelper::get(this->displacementWarning->displacementSensorFlags, DisplacementSensorFlags::UnknownProblem));
        this->safetyController->displacementNotifyInvalidResponse(BitHelper::get(this->displacementWarning->displacementSensorFlags, DisplacementSensorFlags::InvalidResponse));
        this->safetyController->displacementNotifyResponseTimeoutError(BitHelper::get(this->displacementWarning->displacementSensorFlags, DisplacementSensorFlags::ResponseTimeout));
        this->safetyController->displacementNotifySensorReportsInvalidCommand(BitHelper::get(this->displacementWarning->displacementSensorFlags, DisplacementSensorFlags::SensorReportsInvalidCommand));
        this->safetyController->displacementNotifySensorReportsCommunicationTimeoutError(BitHelper::get(this->displacementWarning->displacementSensorFlags, DisplacementSensorFlags::SensorReportsCommunicationTimeoutError));
        this->safetyController->displacementNotifySensorReportsDataLengthError(BitHelper::get(this->displacementWarning->displacementSensorFlags, DisplacementSensorFlags::SensorReportsDataLengthError));
        this->safetyController->displacementNotifySensorReportsNumberOfParametersError(BitHelper::get(this->displacementWarning->displacementSensorFlags, DisplacementSensorFlags::SensorReportsNumberOfParametersError));
        this->safetyController->displacementNotifySensorReportsParameterError(BitHelper::get(this->displacementWarning->displacementSensorFlags, DisplacementSensorFlags::SensorReportsParameterError));
        this->safetyController->displacementNotifySensorReportsCommunicationError(BitHelper::get(this->displacementWarning->displacementSensorFlags, DisplacementSensorFlags::SensorReportsCommunicationError));
        this->safetyController->displacementNotifySensorReportsIDNumberError(BitHelper::get(this->displacementWarning->displacementSensorFlags, DisplacementSensorFlags::SensorReportsIDNumberError));
        this->safetyController->displacementNotifySensorReportsExpansionLineError(BitHelper::get(this->displacementWarning->displacementSensorFlags, DisplacementSensorFlags::SensorReportsExpansionLineError));
        this->safetyController->displacementNotifySensorReportsWriteControlError(BitHelper::get(this->displacementWarning->displacementSensorFlags, DisplacementSensorFlags::SensorReportsWriteControlError));
        this->publisher->tryLogDisplacementSensorWarning();
    }
    if (this->fpgaData->DisplacementSampleTimestamp > this->lastSampleTimestamp) {
        this->lastSampleTimestamp = this->fpgaData->DisplacementSampleTimestamp;
        this->imsData->timestamp = Timestamp::fromFPGA(this->fpgaData->DisplacementSampleTimestamp);
        // We need to swap the direction of the IMS reading, note sensor 5 and 7 are mounted opposite
        // We also need to convert the raw sensor values to millimeters
        this->imsData->rawSensorData[0] = 50.0 - ((this->fpgaData->DisplacementRaw1 + this->displacementSensorSettings->N1Offset) / 10000.0);
        this->imsData->rawSensorData[1] = 50.0 - ((this->fpgaData->DisplacementRaw2 + this->displacementSensorSettings->N2Offset) / 10000.0);
        this->imsData->rawSensorData[2] = 50.0 - ((this->fpgaData->DisplacementRaw3 + this->displacementSensorSettings->N3Offset) / 10000.0);
        this->imsData->rawSensorData[3] = 50.0 - ((this->fpgaData->DisplacementRaw4 + this->displacementSensorSettings->N4Offset) / 10000.0);
        this->imsData->rawSensorData[4] = 50.0 - ((this->fpgaData->DisplacementRaw5 + this->displacementSensorSettings->N5Offset) / 10000.0);
        this->imsData->rawSensorData[5] = ((this->fpgaData->DisplacementRaw6 + this->displacementSensorSettings->N6Offset) / 10000.0);
        this->imsData->rawSensorData[6] = 50.0 - ((this->fpgaData->DisplacementRaw7 + this->displacementSensorSettings->N7Offset) / 10000.0);
        this->imsData->rawSensorData[7] = ((this->fpgaData->DisplacementRaw8 + this->displacementSensorSettings->N8Offset) / 10000.0);
        int p1 = this->displacementSensorSettings->N1Port;
        int p2 = this->displacementSensorSettings->N2Port;
        int p3 = this->displacementSensorSettings->N3Port;
        int p4 = this->displacementSensorSettings->N4Port;
        int p5 = this->displacementSensorSettings->N5Port;
        int p6 = this->displacementSensorSettings->N6Port;
        int p7 = this->displacementSensorSettings->N7Port;
        int p8 = this->displacementSensorSettings->N8Port;
        this->imsData->xPosition =
                (this->displacementSensorSettings->ConverterMatrix[0] * this->imsData->rawSensorData[p1] +
                 this->displacementSensorSettings->ConverterMatrix[1] * this->imsData->rawSensorData[p2] +
                 this->displacementSensorSettings->ConverterMatrix[2] * this->imsData->rawSensorData[p3] +
                 this->displacementSensorSettings->ConverterMatrix[3] * this->imsData->rawSensorData[p4] +
                 this->displacementSensorSettings->ConverterMatrix[4] * this->imsData->rawSensorData[p5] +
                 this->displacementSensorSettings->ConverterMatrix[5] * this->imsData->rawSensorData[p6] +
                 this->displacementSensorSettings->ConverterMatrix[6] * this->imsData->rawSensorData[p7] +
                 this->displacementSensorSettings->ConverterMatrix[7] * this->imsData->rawSensorData[p8]) /
                        MILLIMETERS_PER_METER +
                this->displacementSensorSettings->XPositionOffset;
        this->imsData->yPosition =
                (this->displacementSensorSettings->ConverterMatrix[8] * this->imsData->rawSensorData[p1] +
                 this->displacementSensorSettings->ConverterMatrix[9] * this->imsData->rawSensorData[p2] +
                 this->displacementSensorSettings->ConverterMatrix[10] * this->imsData->rawSensorData[p3] +
                 this->displacementSensorSettings->ConverterMatrix[11] * this->imsData->rawSensorData[p4] +
                 this->displacementSensorSettings->ConverterMatrix[12] * this->imsData->rawSensorData[p5] +
                 this->displacementSensorSettings->ConverterMatrix[13] * this->imsData->rawSensorData[p6] +
                 this->displacementSensorSettings->ConverterMatrix[14] * this->imsData->rawSensorData[p7] +
                 this->displacementSensorSettings->ConverterMatrix[15] * this->imsData->rawSensorData[p8]) /
                        MILLIMETERS_PER_METER +
                this->displacementSensorSettings->YPositionOffset;
        this->imsData->zPosition =
                (this->displacementSensorSettings->ConverterMatrix[16] * this->imsData->rawSensorData[p1] +
                 this->displacementSensorSettings->ConverterMatrix[17] * this->imsData->rawSensorData[p2] +
                 this->displacementSensorSettings->ConverterMatrix[18] * this->imsData->rawSensorData[p3] +
                 this->displacementSensorSettings->ConverterMatrix[19] * this->imsData->rawSensorData[p4] +
                 this->displacementSensorSettings->ConverterMatrix[20] * this->imsData->rawSensorData[p5] +
                 this->displacementSensorSettings->ConverterMatrix[21] * this->imsData->rawSensorData[p6] +
                 this->displacementSensorSettings->ConverterMatrix[22] * this->imsData->rawSensorData[p7] +
                 this->displacementSensorSettings->ConverterMatrix[23] * this->imsData->rawSensorData[p8]) /
                        MILLIMETERS_PER_METER +
                this->displacementSensorSettings->ZPositionOffset;
        this->imsData->xRotation =
                (this->displacementSensorSettings->ConverterMatrix[24] * this->imsData->rawSensorData[p1] +
                 this->displacementSensorSettings->ConverterMatrix[25] * this->imsData->rawSensorData[p2] +
                 this->displacementSensorSettings->ConverterMatrix[26] * this->imsData->rawSensorData[p3] +
                 this->displacementSensorSettings->ConverterMatrix[27] * this->imsData->rawSensorData[p4] +
                 this->displacementSensorSettings->ConverterMatrix[28] * this->imsData->rawSensorData[p5] +
                 this->displacementSensorSettings->ConverterMatrix[29] * this->imsData->rawSensorData[p6] +
                 this->displacementSensorSettings->ConverterMatrix[30] * this->imsData->rawSensorData[p7] +
                 this->displacementSensorSettings->ConverterMatrix[31] * this->imsData->rawSensorData[p8]) /
                        MILLIMETERS_PER_METER +
                this->displacementSensorSettings->XRotationOffset;
        this->imsData->yRotation =
                (this->displacementSensorSettings->ConverterMatrix[32] * this->imsData->rawSensorData[p1] +
                 this->displacementSensorSettings->ConverterMatrix[33] * this->imsData->rawSensorData[p2] +
                 this->displacementSensorSettings->ConverterMatrix[34] * this->imsData->rawSensorData[p3] +
                 this->displacementSensorSettings->ConverterMatrix[35] * this->imsData->rawSensorData[p4] +
                 this->displacementSensorSettings->ConverterMatrix[36] * this->imsData->rawSensorData[p5] +
                 this->displacementSensorSettings->ConverterMatrix[37] * this->imsData->rawSensorData[p6] +
                 this->displacementSensorSettings->ConverterMatrix[38] * this->imsData->rawSensorData[p7] +
                 this->displacementSensorSettings->ConverterMatrix[39] * this->imsData->rawSensorData[p8]) /
                        MILLIMETERS_PER_METER +
                this->displacementSensorSettings->YRotationOffset;
        this->imsData->zRotation =
                (this->displacementSensorSettings->ConverterMatrix[40] * this->imsData->rawSensorData[p1] +
                 this->displacementSensorSettings->ConverterMatrix[41] * this->imsData->rawSensorData[p2] +
                 this->displacementSensorSettings->ConverterMatrix[42] * this->imsData->rawSensorData[p3] +
                 this->displacementSensorSettings->ConverterMatrix[43] * this->imsData->rawSensorData[p4] +
                 this->displacementSensorSettings->ConverterMatrix[44] * this->imsData->rawSensorData[p5] +
                 this->displacementSensorSettings->ConverterMatrix[45] * this->imsData->rawSensorData[p6] +
                 this->displacementSensorSettings->ConverterMatrix[46] * this->imsData->rawSensorData[p7] +
                 this->displacementSensorSettings->ConverterMatrix[47] * this->imsData->rawSensorData[p8]) /
                        MILLIMETERS_PER_METER +
                this->displacementSensorSettings->ZRotationOffset;
        this->publisher->putIMSData();
        if (!this->errorCleared && this->fpgaData->DisplacementSampleTimestamp > this->fpgaData->DisplacementErrorTimestamp) {
            this->errorCleared = true;
            this->displacementWarning->displacementSensorFlags = 0;
            this->safetyController->displacementNotifyUnknownProblem(false);
            this->safetyController->displacementNotifyInvalidResponse(false);
            this->safetyController->displacementNotifyResponseTimeoutError(false);
            this->safetyController->displacementNotifySensorReportsInvalidCommand(false);
            this->safetyController->displacementNotifySensorReportsCommunicationTimeoutError(false);
            this->safetyController->displacementNotifySensorReportsDataLengthError(false);
            this->safetyController->displacementNotifySensorReportsNumberOfParametersError(false);
            this->safetyController->displacementNotifySensorReportsParameterError(false);
            this->safetyController->displacementNotifySensorReportsCommunicationError(false);
            this->safetyController->displacementNotifySensorReportsIDNumberError(false);
            this->safetyController->displacementNotifySensorReportsExpansionLineError(false);
            this->safetyController->displacementNotifySensorReportsWriteControlError(false);
            this->publisher->tryLogDisplacementSensorWarning();
        }
    }
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
