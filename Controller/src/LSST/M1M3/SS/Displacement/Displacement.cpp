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

	memset(this->imsData, 0, sizeof(m1m3_IMSDataC));
	memset(this->displacementWarning, 0, sizeof(m1m3_logevent_DisplacementSensorWarningC));
}

void Displacement::processData() {
	// TODO: Handle no data available
	// TODO: Handle displacement limits, push to safety controller
	Log.Trace("Displacement: processData()");
	if (this->fpgaData->DisplacementSampleTimestamp != this->lastSampleTimestamp) {
		this->lastSampleTimestamp = this->fpgaData->DisplacementSampleTimestamp;
		this->imsData->Timestamp = Timestamp::fromFPGA(this->fpgaData->DisplacementSampleTimestamp);
		this->imsData->RawSensorData[0] = this->fpgaData->DisplacementRaw1 / 10000.0;
		this->imsData->RawSensorData[1] = this->fpgaData->DisplacementRaw2 / 10000.0;
		this->imsData->RawSensorData[2] = this->fpgaData->DisplacementRaw3 / 10000.0;
		this->imsData->RawSensorData[3] = this->fpgaData->DisplacementRaw4 / 10000.0;
		this->imsData->RawSensorData[4] = this->fpgaData->DisplacementRaw5 / 10000.0;
		this->imsData->RawSensorData[5] = this->fpgaData->DisplacementRaw6 / 10000.0;
		this->imsData->RawSensorData[6] = this->fpgaData->DisplacementRaw7 / 10000.0;
		this->imsData->RawSensorData[7] = this->fpgaData->DisplacementRaw8 / 10000.0;
		this->imsData->XPosition =
					(this->displacementSensorSettings->ConverterMatrix[0] * this->imsData->RawSensorData[0] +
					this->displacementSensorSettings->ConverterMatrix[1] * this->imsData->RawSensorData[1] +
					this->displacementSensorSettings->ConverterMatrix[2] * this->imsData->RawSensorData[2] +
					this->displacementSensorSettings->ConverterMatrix[3] * this->imsData->RawSensorData[3] +
					this->displacementSensorSettings->ConverterMatrix[4] * this->imsData->RawSensorData[4] +
					this->displacementSensorSettings->ConverterMatrix[5] * this->imsData->RawSensorData[5] +
					this->displacementSensorSettings->ConverterMatrix[6] * this->imsData->RawSensorData[6] +
					this->displacementSensorSettings->ConverterMatrix[7] * this->imsData->RawSensorData[7]) /
					MILLIMETERS_PER_METER;
			this->imsData->YPosition =
					(this->displacementSensorSettings->ConverterMatrix[8] * this->imsData->RawSensorData[0] +
					this->displacementSensorSettings->ConverterMatrix[9] * this->imsData->RawSensorData[1] +
					this->displacementSensorSettings->ConverterMatrix[10] * this->imsData->RawSensorData[2] +
					this->displacementSensorSettings->ConverterMatrix[11] * this->imsData->RawSensorData[3] +
					this->displacementSensorSettings->ConverterMatrix[12] * this->imsData->RawSensorData[4] +
					this->displacementSensorSettings->ConverterMatrix[13] * this->imsData->RawSensorData[5] +
					this->displacementSensorSettings->ConverterMatrix[14] * this->imsData->RawSensorData[6] +
					this->displacementSensorSettings->ConverterMatrix[15] * this->imsData->RawSensorData[7]) /
					MILLIMETERS_PER_METER;
			this->imsData->ZPosition =
					(this->displacementSensorSettings->ConverterMatrix[16] * this->imsData->RawSensorData[0] +
					this->displacementSensorSettings->ConverterMatrix[17] * this->imsData->RawSensorData[1] +
					this->displacementSensorSettings->ConverterMatrix[18] * this->imsData->RawSensorData[2] +
					this->displacementSensorSettings->ConverterMatrix[19] * this->imsData->RawSensorData[3] +
					this->displacementSensorSettings->ConverterMatrix[20] * this->imsData->RawSensorData[4] +
					this->displacementSensorSettings->ConverterMatrix[21] * this->imsData->RawSensorData[5] +
					this->displacementSensorSettings->ConverterMatrix[22] * this->imsData->RawSensorData[6] +
					this->displacementSensorSettings->ConverterMatrix[23] * this->imsData->RawSensorData[7]) /
					MILLIMETERS_PER_METER;
			this->imsData->XRotation =
					(this->displacementSensorSettings->ConverterMatrix[24] * this->imsData->RawSensorData[0] +
					this->displacementSensorSettings->ConverterMatrix[25] * this->imsData->RawSensorData[1] +
					this->displacementSensorSettings->ConverterMatrix[26] * this->imsData->RawSensorData[2] +
					this->displacementSensorSettings->ConverterMatrix[27] * this->imsData->RawSensorData[3] +
					this->displacementSensorSettings->ConverterMatrix[28] * this->imsData->RawSensorData[4] +
					this->displacementSensorSettings->ConverterMatrix[29] * this->imsData->RawSensorData[5] +
					this->displacementSensorSettings->ConverterMatrix[30] * this->imsData->RawSensorData[6] +
					this->displacementSensorSettings->ConverterMatrix[31] * this->imsData->RawSensorData[7]) /
					MILLIMETERS_PER_METER;
			this->imsData->YRotation =
					(this->displacementSensorSettings->ConverterMatrix[32] * this->imsData->RawSensorData[0] +
					this->displacementSensorSettings->ConverterMatrix[33] * this->imsData->RawSensorData[1] +
					this->displacementSensorSettings->ConverterMatrix[34] * this->imsData->RawSensorData[2] +
					this->displacementSensorSettings->ConverterMatrix[35] * this->imsData->RawSensorData[3] +
					this->displacementSensorSettings->ConverterMatrix[36] * this->imsData->RawSensorData[4] +
					this->displacementSensorSettings->ConverterMatrix[37] * this->imsData->RawSensorData[5] +
					this->displacementSensorSettings->ConverterMatrix[38] * this->imsData->RawSensorData[6] +
					this->displacementSensorSettings->ConverterMatrix[39] * this->imsData->RawSensorData[7]) /
					MILLIMETERS_PER_METER;
			this->imsData->ZRotation =
					(this->displacementSensorSettings->ConverterMatrix[40] * this->imsData->RawSensorData[0] +
					this->displacementSensorSettings->ConverterMatrix[41] * this->imsData->RawSensorData[1] +
					this->displacementSensorSettings->ConverterMatrix[42] * this->imsData->RawSensorData[2] +
					this->displacementSensorSettings->ConverterMatrix[43] * this->imsData->RawSensorData[3] +
					this->displacementSensorSettings->ConverterMatrix[44] * this->imsData->RawSensorData[4] +
					this->displacementSensorSettings->ConverterMatrix[45] * this->imsData->RawSensorData[5] +
					this->displacementSensorSettings->ConverterMatrix[46] * this->imsData->RawSensorData[6] +
					this->displacementSensorSettings->ConverterMatrix[47] * this->imsData->RawSensorData[7]) /
					MILLIMETERS_PER_METER;
		this->publisher->putIMSData();
	}
	if (this->fpgaData->DisplacementErrorTimestamp != this->lastErrorTimestamp) {
		this->lastErrorTimestamp = this->fpgaData->DisplacementErrorTimestamp;
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
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
