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
#include <SAL_m1m3C.h>
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

	memset(this->imsData, 0, sizeof(m1m3_IMSDataC));
	memset(this->displacementWarning, 0, sizeof(m1m3_logevent_DisplacementSensorWarningC));
}

void Displacement::processData() {
	// TODO: Handle no data available
	// TODO: Handle displacement limits, push to safety controller
	Log.Trace("Displacement: processData()");
	if (this->fpgaData->DisplacementErrorTimestamp > this->lastErrorTimestamp) {
		this->lastErrorTimestamp = this->fpgaData->DisplacementErrorTimestamp;
		this->errorCleared = false;
		this->displacementWarning->Timestamp = Timestamp::fromFPGA(this->fpgaData->DisplacementErrorTimestamp);
		this->displacementWarning->AnyWarning = this->fpgaData->DisplacementErrorCode != 0;
		this->displacementWarning->UnknownProblem = this->fpgaData->DisplacementErrorCode == 1;
		this->displacementWarning->InvalidResponse = this->fpgaData->DisplacementErrorCode == 2;
		this->displacementWarning->ResponseTimeout = this->fpgaData->DisplacementErrorCode == 3;
		this->displacementWarning->SensorReportsInvalidCommand = this->fpgaData->DisplacementErrorCode == 4;
		this->displacementWarning->SensorReportsCommunicationTimeoutError = this->fpgaData->DisplacementErrorCode == 5;
		this->displacementWarning->SensorReportsDataLengthError = this->fpgaData->DisplacementErrorCode == 6;
		this->displacementWarning->SensorReportsNumberOfParametersError = this->fpgaData->DisplacementErrorCode == 7;
		this->displacementWarning->SensorReportsParameterError = this->fpgaData->DisplacementErrorCode == 8;
		this->displacementWarning->SensorReportsCommunicationError = this->fpgaData->DisplacementErrorCode == 9;
		this->displacementWarning->SensorReportsIDNumberError = this->fpgaData->DisplacementErrorCode == 10;
		this->displacementWarning->SensorReportsExpansionLineError = this->fpgaData->DisplacementErrorCode == 11;
		this->displacementWarning->SensorReportsWriteControlError = this->fpgaData->DisplacementErrorCode == 12;
		this->safetyController->displacementNotifyUnknownProblem(this->displacementWarning->UnknownProblem);
		this->safetyController->displacementNotifyInvalidResponse(this->displacementWarning->InvalidResponse);
		this->safetyController->displacementNotifyResponseTimeoutError(this->displacementWarning->ResponseTimeout);
		this->safetyController->displacementNotifySensorReportsInvalidCommand(this->displacementWarning->SensorReportsInvalidCommand);
		this->safetyController->displacementNotifySensorReportsCommunicationTimeoutError(this->displacementWarning->SensorReportsCommunicationTimeoutError);
		this->safetyController->displacementNotifySensorReportsDataLengthError(this->displacementWarning->SensorReportsDataLengthError);
		this->safetyController->displacementNotifySensorReportsNumberOfParametersError(this->displacementWarning->SensorReportsNumberOfParametersError);
		this->safetyController->displacementNotifySensorReportsParameterError(this->displacementWarning->SensorReportsParameterError);
		this->safetyController->displacementNotifySensorReportsCommunicationError(this->displacementWarning->SensorReportsCommunicationError);
		this->safetyController->displacementNotifySensorReportsIDNumberError(this->displacementWarning->SensorReportsIDNumberError);
		this->safetyController->displacementNotifySensorReportsExpansionLineError(this->displacementWarning->SensorReportsExpansionLineError);
		this->safetyController->displacementNotifySensorReportsWriteControlError(this->displacementWarning->SensorReportsWriteControlError);
		this->publisher->tryLogDisplacementSensorWarning();
	}
	if (this->fpgaData->DisplacementSampleTimestamp > this->lastSampleTimestamp) {
		this->lastSampleTimestamp = this->fpgaData->DisplacementSampleTimestamp;
		this->imsData->Timestamp = Timestamp::fromFPGA(this->fpgaData->DisplacementSampleTimestamp);
		// We need to swap the direction of the IMS reading
		// We also need to convert the raw sensor values to millimeters
		this->imsData->RawSensorData[0] = 50.0 - ((this->fpgaData->DisplacementRaw1 + this->displacementSensorSettings->N1Offset) / 10000.0);
		this->imsData->RawSensorData[1] = 50.0 - ((this->fpgaData->DisplacementRaw2 + this->displacementSensorSettings->N2Offset) / 10000.0);
		this->imsData->RawSensorData[2] = 50.0 - ((this->fpgaData->DisplacementRaw3 + this->displacementSensorSettings->N3Offset) / 10000.0);
		this->imsData->RawSensorData[3] = 50.0 - ((this->fpgaData->DisplacementRaw4 + this->displacementSensorSettings->N4Offset) / 10000.0);
		this->imsData->RawSensorData[4] = 50.0 - ((this->fpgaData->DisplacementRaw5 + this->displacementSensorSettings->N5Offset) / 10000.0);
		this->imsData->RawSensorData[5] = 50.0 - ((this->fpgaData->DisplacementRaw6 + this->displacementSensorSettings->N6Offset) / 10000.0);
		this->imsData->RawSensorData[6] = 50.0 - ((this->fpgaData->DisplacementRaw7 + this->displacementSensorSettings->N7Offset) / 10000.0);
		this->imsData->RawSensorData[7] = 50.0 - ((this->fpgaData->DisplacementRaw8 + this->displacementSensorSettings->N8Offset) / 10000.0);
		int p1 = this->displacementSensorSettings->N1Port;
		int p2 = this->displacementSensorSettings->N2Port;
		int p3 = this->displacementSensorSettings->N3Port;
		int p4 = this->displacementSensorSettings->N4Port;
		int p5 = this->displacementSensorSettings->N5Port;
		int p6 = this->displacementSensorSettings->N6Port;
		int p7 = this->displacementSensorSettings->N7Port;
		int p8 = this->displacementSensorSettings->N8Port;
		this->imsData->XPosition =
					(this->displacementSensorSettings->ConverterMatrix[0] * this->imsData->RawSensorData[p1] +
					this->displacementSensorSettings->ConverterMatrix[1] * this->imsData->RawSensorData[p2] +
					this->displacementSensorSettings->ConverterMatrix[2] * this->imsData->RawSensorData[p3] +
					this->displacementSensorSettings->ConverterMatrix[3] * this->imsData->RawSensorData[p4] +
					this->displacementSensorSettings->ConverterMatrix[4] * this->imsData->RawSensorData[p5] +
					this->displacementSensorSettings->ConverterMatrix[5] * this->imsData->RawSensorData[p6] +
					this->displacementSensorSettings->ConverterMatrix[6] * this->imsData->RawSensorData[p7] +
					this->displacementSensorSettings->ConverterMatrix[7] * this->imsData->RawSensorData[p8]) /
					MILLIMETERS_PER_METER +
					this->displacementSensorSettings->XPositionOffset;
			this->imsData->YPosition =
					(this->displacementSensorSettings->ConverterMatrix[8] * this->imsData->RawSensorData[p1] +
					this->displacementSensorSettings->ConverterMatrix[9] * this->imsData->RawSensorData[p2] +
					this->displacementSensorSettings->ConverterMatrix[10] * this->imsData->RawSensorData[p3] +
					this->displacementSensorSettings->ConverterMatrix[11] * this->imsData->RawSensorData[p4] +
					this->displacementSensorSettings->ConverterMatrix[12] * this->imsData->RawSensorData[p5] +
					this->displacementSensorSettings->ConverterMatrix[13] * this->imsData->RawSensorData[p6] +
					this->displacementSensorSettings->ConverterMatrix[14] * this->imsData->RawSensorData[p7] +
					this->displacementSensorSettings->ConverterMatrix[15] * this->imsData->RawSensorData[p8]) /
					MILLIMETERS_PER_METER +
					this->displacementSensorSettings->YPositionOffset;
			this->imsData->ZPosition =
					(this->displacementSensorSettings->ConverterMatrix[16] * this->imsData->RawSensorData[p1] +
					this->displacementSensorSettings->ConverterMatrix[17] * this->imsData->RawSensorData[p2] +
					this->displacementSensorSettings->ConverterMatrix[18] * this->imsData->RawSensorData[p3] +
					this->displacementSensorSettings->ConverterMatrix[19] * this->imsData->RawSensorData[p4] +
					this->displacementSensorSettings->ConverterMatrix[20] * this->imsData->RawSensorData[p5] +
					this->displacementSensorSettings->ConverterMatrix[21] * this->imsData->RawSensorData[p6] +
					this->displacementSensorSettings->ConverterMatrix[22] * this->imsData->RawSensorData[p7] +
					this->displacementSensorSettings->ConverterMatrix[23] * this->imsData->RawSensorData[p8]) /
					MILLIMETERS_PER_METER +
					this->displacementSensorSettings->ZPositionOffset;
			this->imsData->XRotation =
					(this->displacementSensorSettings->ConverterMatrix[24] * this->imsData->RawSensorData[p1] +
					this->displacementSensorSettings->ConverterMatrix[25] * this->imsData->RawSensorData[p2] +
					this->displacementSensorSettings->ConverterMatrix[26] * this->imsData->RawSensorData[p3] +
					this->displacementSensorSettings->ConverterMatrix[27] * this->imsData->RawSensorData[p4] +
					this->displacementSensorSettings->ConverterMatrix[28] * this->imsData->RawSensorData[p5] +
					this->displacementSensorSettings->ConverterMatrix[29] * this->imsData->RawSensorData[p6] +
					this->displacementSensorSettings->ConverterMatrix[30] * this->imsData->RawSensorData[p7] +
					this->displacementSensorSettings->ConverterMatrix[31] * this->imsData->RawSensorData[p8]) /
					MILLIMETERS_PER_METER +
					this->displacementSensorSettings->XRotationOffset;
			this->imsData->YRotation =
					(this->displacementSensorSettings->ConverterMatrix[32] * this->imsData->RawSensorData[p1] +
					this->displacementSensorSettings->ConverterMatrix[33] * this->imsData->RawSensorData[p2] +
					this->displacementSensorSettings->ConverterMatrix[34] * this->imsData->RawSensorData[p3] +
					this->displacementSensorSettings->ConverterMatrix[35] * this->imsData->RawSensorData[p4] +
					this->displacementSensorSettings->ConverterMatrix[36] * this->imsData->RawSensorData[p5] +
					this->displacementSensorSettings->ConverterMatrix[37] * this->imsData->RawSensorData[p6] +
					this->displacementSensorSettings->ConverterMatrix[38] * this->imsData->RawSensorData[p7] +
					this->displacementSensorSettings->ConverterMatrix[39] * this->imsData->RawSensorData[p8]) /
					MILLIMETERS_PER_METER +
					this->displacementSensorSettings->YRotationOffset;
			this->imsData->ZRotation =
					(this->displacementSensorSettings->ConverterMatrix[40] * this->imsData->RawSensorData[p1] +
					this->displacementSensorSettings->ConverterMatrix[41] * this->imsData->RawSensorData[p2] +
					this->displacementSensorSettings->ConverterMatrix[42] * this->imsData->RawSensorData[p3] +
					this->displacementSensorSettings->ConverterMatrix[43] * this->imsData->RawSensorData[p4] +
					this->displacementSensorSettings->ConverterMatrix[44] * this->imsData->RawSensorData[p5] +
					this->displacementSensorSettings->ConverterMatrix[45] * this->imsData->RawSensorData[p6] +
					this->displacementSensorSettings->ConverterMatrix[46] * this->imsData->RawSensorData[p7] +
					this->displacementSensorSettings->ConverterMatrix[47] * this->imsData->RawSensorData[p8]) /
					MILLIMETERS_PER_METER +
					this->displacementSensorSettings->ZRotationOffset;
		this->publisher->putIMSData();
		if (!this->errorCleared && this->fpgaData->DisplacementSampleTimestamp > this->fpgaData->DisplacementErrorTimestamp) {
			this->errorCleared = true;
			this->displacementWarning->Timestamp = Timestamp::fromFPGA(this->fpgaData->DisplacementSampleTimestamp);
			this->displacementWarning->UnknownProblem = false;
			this->displacementWarning->InvalidResponse = false;
			this->displacementWarning->ResponseTimeout = false;
			this->displacementWarning->SensorReportsInvalidCommand = false;
			this->displacementWarning->SensorReportsCommunicationTimeoutError = false;
			this->displacementWarning->SensorReportsDataLengthError = false;
			this->displacementWarning->SensorReportsNumberOfParametersError = false;
			this->displacementWarning->SensorReportsParameterError = false;
			this->displacementWarning->SensorReportsCommunicationError = false;
			this->displacementWarning->SensorReportsIDNumberError = false;
			this->displacementWarning->SensorReportsExpansionLineError = false;
			this->displacementWarning->SensorReportsWriteControlError = false;
			this->safetyController->displacementNotifyUnknownProblem(this->displacementWarning->UnknownProblem);
			this->safetyController->displacementNotifyInvalidResponse(this->displacementWarning->InvalidResponse);
			this->safetyController->displacementNotifyResponseTimeoutError(this->displacementWarning->ResponseTimeout);
			this->safetyController->displacementNotifySensorReportsInvalidCommand(this->displacementWarning->SensorReportsInvalidCommand);
			this->safetyController->displacementNotifySensorReportsCommunicationTimeoutError(this->displacementWarning->SensorReportsCommunicationTimeoutError);
			this->safetyController->displacementNotifySensorReportsDataLengthError(this->displacementWarning->SensorReportsDataLengthError);
			this->safetyController->displacementNotifySensorReportsNumberOfParametersError(this->displacementWarning->SensorReportsNumberOfParametersError);
			this->safetyController->displacementNotifySensorReportsParameterError(this->displacementWarning->SensorReportsParameterError);
			this->safetyController->displacementNotifySensorReportsCommunicationError(this->displacementWarning->SensorReportsCommunicationError);
			this->safetyController->displacementNotifySensorReportsIDNumberError(this->displacementWarning->SensorReportsIDNumberError);
			this->safetyController->displacementNotifySensorReportsExpansionLineError(this->displacementWarning->SensorReportsExpansionLineError);
			this->safetyController->displacementNotifySensorReportsWriteControlError(this->displacementWarning->SensorReportsWriteControlError);
			this->publisher->tryLogDisplacementSensorWarning();
		}
	}
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
