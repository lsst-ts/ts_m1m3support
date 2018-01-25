/*
 * Displacement.cpp
 *
 *  Created on: Oct 6, 2017
 *      Author: ccontaxis
 */

#include <Displacement.h>
#include <DisplacementSensorSettings.h>
#include <M1M3SSPublisher.h>
#include <SafetyController.h>
#include <FPGA.h>
#include <FPGAAddresses.h>
#include <U8ArrayUtilities.h>
#include <Timestamp.h>
#include <SAL_m1m3C.h>
#include <boost/tokenizer.hpp>
#include <boost/lexical_cast.hpp>
#include <string>
#include <cstring>

#include <iostream>
using namespace std;

namespace LSST {
namespace M1M3 {
namespace SS {

Displacement::Displacement(DisplacementSensorSettings* displacementSensorSettings, M1M3SSPublisher* publisher, SafetyController* safetyController, FPGA* fpga) {
	this->displacementSensorSettings = displacementSensorSettings;
	this->publisher = publisher;
	this->safetyController = safetyController;
	this->fpga = fpga;
	this->imsData = this->publisher->getIMSData();
	this->displacementWarning = this->publisher->getEventDisplacementSensorWarning();

	memset(this->imsData, 0, sizeof(m1m3_IMSDataC));
	memset(this->displacementWarning, 0, sizeof(m1m3_logevent_DisplacementSensorWarningC));

	this->createTxBuffer();
}

void Displacement::writeDataRequest() {
	this->fpga->writeCommandFIFO(this->txBuffer, 6, 0);
}

void Displacement::readDataResponse() {
	this->fpga->writeRequestFIFO(FPGAAddresses::Displacement, 0);
	this->fpga->readU8ResponseFIFO(this->rxBuffer, 2, 50);
	uint16_t length = U8ArrayUtilities::u16(this->rxBuffer, 0);
	// A response is expected to be 92 or 18
	// A valid response is 84 characters + 8 bytes timestamp (92)
	// An invalid response is 10 characters + 8 bytes timestamp (18)
	if (length == 92 || length == 18) {
		this->fpga->readU8ResponseFIFO(this->rxBuffer, length, 50);
		double timestamp = Timestamp::fromRaw(U8ArrayUtilities::u64(this->rxBuffer, 0));
		std::string response = U8ArrayUtilities::toString(this->rxBuffer, 8, length - 10);
		typedef boost::tokenizer< boost::escaped_list_separator<char> > tokenizer;
		if (response.length() > 0) {
			tokenizer tokenize(response);
			tokenizer::iterator token = tokenize.begin();
			if ((*token) == "M0") {
				this->imsData->Timestamp = timestamp;
				++token;
				try {
					double rawSensor[8];
					int sensorsProcessed = 0;
					for(int i = 0; i < 8 && token != tokenize.end(); ++i) {
						rawSensor[i] = boost::lexical_cast<double>(*token);
						++token;
						++sensorsProcessed;
					}
					if (sensorsProcessed == 8) {
						for(int i = 0; i < 8; ++i) {
							this->imsData->RawSensorData[i] = rawSensor[i];
						}
						this->clearWarning(timestamp);
						this->convertRawData();
						this->publisher->putIMSData();
					}
					else {
						this->warnInvalidResponse(timestamp);
					}
				}
				catch(boost::bad_lexical_cast& e) {
					this->warnInvalidResponse(timestamp);
				}
			}
			else if ((*token) == "ER") {
				++token;
				++token;
				switch(boost::lexical_cast<int32_t>(*token)) {
				case 0: this->warnSensorReportsInvalidCommand(timestamp); break;
				case 2: this->warnSensorReportsCommunicationTimeoutError(timestamp); break;
				case 20: this->warnSensorReportsDataLengthError(timestamp); break;
				case 21: this->warnSensorReportsNumberOfParametersError(timestamp); break;
				case 22: this->warnSensorReportsParameterError(timestamp); break;
				case 29: this->warnSensorReportsCommunicationError(timestamp); break;
				case 65: this->warnSensorReportsIDNumberError(timestamp); break;
				case 66: this->warnSensorReportsExpansionLineError(timestamp); break;
				case 67: this->warnSensorReportsWriteControlError(timestamp); break;
				default: this->warnUnknownProblem(timestamp); break;
				}
			}
			else {
				this->warnUnknownCommand(timestamp);
			}
		}
		else {
			this->warnInvalidResponse(timestamp);
		}
	}
	else if (length == 0) {
		this->warnResponseTimeoutError(this->publisher->getTimestamp());
	}
	else {
		this->warnInvalidResponse(this->publisher->getTimestamp());
	}
}

void Displacement::createTxBuffer() {
	this->txBuffer[0] = FPGAAddresses::Displacement;
	this->txBuffer[1] = 4;
	this->txBuffer[2] = (uint8_t)'M';
	this->txBuffer[3] = (uint8_t)'0';
	this->txBuffer[4] = (uint8_t)'\r';
	this->txBuffer[5] = (uint8_t)'\n';
}

void Displacement::convertRawData() {
	double displacements[8] = {
			this->imsData->RawSensorData[this->displacementSensorSettings->N1Port],
			this->imsData->RawSensorData[this->displacementSensorSettings->N2Port],
			this->imsData->RawSensorData[this->displacementSensorSettings->N3Port],
			this->imsData->RawSensorData[this->displacementSensorSettings->N4Port],
			this->imsData->RawSensorData[this->displacementSensorSettings->N5Port],
			this->imsData->RawSensorData[this->displacementSensorSettings->N6Port],
			this->imsData->RawSensorData[this->displacementSensorSettings->N7Port],
			this->imsData->RawSensorData[this->displacementSensorSettings->N8Port] };
	this->imsData->XPosition =
			(this->displacementSensorSettings->ConverterMatrix[0] * displacements[0] +
			this->displacementSensorSettings->ConverterMatrix[1] * displacements[1] +
			this->displacementSensorSettings->ConverterMatrix[2] * displacements[2] +
			this->displacementSensorSettings->ConverterMatrix[3] * displacements[3] +
			this->displacementSensorSettings->ConverterMatrix[4] * displacements[4] +
			this->displacementSensorSettings->ConverterMatrix[5] * displacements[5] +
			this->displacementSensorSettings->ConverterMatrix[6] * displacements[6] +
			this->displacementSensorSettings->ConverterMatrix[7] * displacements[7]) /
			MILLIMETERS_PER_METER;
	this->imsData->YPosition =
			(this->displacementSensorSettings->ConverterMatrix[8] * displacements[0] +
			this->displacementSensorSettings->ConverterMatrix[9] * displacements[1] +
			this->displacementSensorSettings->ConverterMatrix[10] * displacements[2] +
			this->displacementSensorSettings->ConverterMatrix[11] * displacements[3] +
			this->displacementSensorSettings->ConverterMatrix[12] * displacements[4] +
			this->displacementSensorSettings->ConverterMatrix[13] * displacements[5] +
			this->displacementSensorSettings->ConverterMatrix[14] * displacements[6] +
			this->displacementSensorSettings->ConverterMatrix[15] * displacements[7]) /
			MILLIMETERS_PER_METER;
	this->imsData->ZPosition =
			(this->displacementSensorSettings->ConverterMatrix[16] * displacements[0] +
			this->displacementSensorSettings->ConverterMatrix[17] * displacements[1] +
			this->displacementSensorSettings->ConverterMatrix[18] * displacements[2] +
			this->displacementSensorSettings->ConverterMatrix[19] * displacements[3] +
			this->displacementSensorSettings->ConverterMatrix[20] * displacements[4] +
			this->displacementSensorSettings->ConverterMatrix[21] * displacements[5] +
			this->displacementSensorSettings->ConverterMatrix[22] * displacements[6] +
			this->displacementSensorSettings->ConverterMatrix[23] * displacements[7]) /
			MILLIMETERS_PER_METER;
	this->imsData->XRotation =
			(this->displacementSensorSettings->ConverterMatrix[24] * displacements[0] +
			this->displacementSensorSettings->ConverterMatrix[25] * displacements[1] +
			this->displacementSensorSettings->ConverterMatrix[26] * displacements[2] +
			this->displacementSensorSettings->ConverterMatrix[27] * displacements[3] +
			this->displacementSensorSettings->ConverterMatrix[28] * displacements[4] +
			this->displacementSensorSettings->ConverterMatrix[29] * displacements[5] +
			this->displacementSensorSettings->ConverterMatrix[30] * displacements[6] +
			this->displacementSensorSettings->ConverterMatrix[31] * displacements[7]) /
			MILLIMETERS_PER_METER;
	this->imsData->YRotation =
			(this->displacementSensorSettings->ConverterMatrix[32] * displacements[0] +
			this->displacementSensorSettings->ConverterMatrix[33] * displacements[1] +
			this->displacementSensorSettings->ConverterMatrix[34] * displacements[2] +
			this->displacementSensorSettings->ConverterMatrix[35] * displacements[3] +
			this->displacementSensorSettings->ConverterMatrix[36] * displacements[4] +
			this->displacementSensorSettings->ConverterMatrix[37] * displacements[5] +
			this->displacementSensorSettings->ConverterMatrix[38] * displacements[6] +
			this->displacementSensorSettings->ConverterMatrix[39] * displacements[7]) /
			MILLIMETERS_PER_METER;
	this->imsData->ZRotation =
			(this->displacementSensorSettings->ConverterMatrix[40] * displacements[0] +
			this->displacementSensorSettings->ConverterMatrix[41] * displacements[1] +
			this->displacementSensorSettings->ConverterMatrix[42] * displacements[2] +
			this->displacementSensorSettings->ConverterMatrix[43] * displacements[3] +
			this->displacementSensorSettings->ConverterMatrix[44] * displacements[4] +
			this->displacementSensorSettings->ConverterMatrix[45] * displacements[5] +
			this->displacementSensorSettings->ConverterMatrix[46] * displacements[6] +
			this->displacementSensorSettings->ConverterMatrix[47] * displacements[7]) /
			MILLIMETERS_PER_METER;
}

void Displacement::clearWarning(double timestamp) {
	if (this->displacementWarning->AnyWarning) {
		memset(this->displacementWarning, 0, sizeof(m1m3_logevent_DisplacementSensorWarningC));
		this->displacementWarning->Timestamp = timestamp;
		this->safetyController->displacementNotifySensorReportsInvalidCommand(this->displacementWarning->SensorReportsInvalidCommand);
		this->safetyController->displacementNotifySensorReportsCommunicationTimeoutError(this->displacementWarning->SensorReportsCommunicationTimeoutError);
		this->safetyController->displacementNotifySensorReportsNumberOfParametersError(this->displacementWarning->SensorReportsNumberOfParametersError);
		this->safetyController->displacementNotifySensorReportsParameterError(this->displacementWarning->SensorReportsParameterError);
		this->safetyController->displacementNotifySensorReportsCommunicationError(this->displacementWarning->SensorReportsCommunicationError);
		this->safetyController->displacementNotifySensorReportsIDNumberError(this->displacementWarning->SensorReportsIDNumberError);
		this->safetyController->displacementNotifySensorReportsExpansionLineError(this->displacementWarning->SensorReportsExpansionLineError);
		this->safetyController->displacementNotifySensorReportsWriteControlError(this->displacementWarning->SensorReportsWriteControlError);
		this->safetyController->displacementNotifyResponseTimeoutError(this->displacementWarning->ResponseTimeout);
		this->safetyController->displacementNotifyInvalidLength(this->displacementWarning->InvalidLength);
		this->safetyController->displacementNotifyUnknownCommand(this->displacementWarning->UnknownCommand);
		this->safetyController->displacementNotifyUnknownProblem(this->displacementWarning->UnknownProblem);
		this->publisher->logDisplacementSensorWarning();
	}
}

void Displacement::warnSensorReportsInvalidCommand(double timestamp) {
	memset(this->displacementWarning, 0, sizeof(m1m3_logevent_DisplacementSensorWarningC));
	this->displacementWarning->Timestamp = timestamp;
	this->displacementWarning->AnyWarning = true;
	this->displacementWarning->SensorReportsInvalidCommand = true;
	this->safetyController->displacementNotifySensorReportsInvalidCommand(this->displacementWarning->SensorReportsInvalidCommand);
	this->publisher->logDisplacementSensorWarning();
}

void Displacement::warnSensorReportsCommunicationTimeoutError(double timestamp) {
	memset(this->displacementWarning, 0, sizeof(m1m3_logevent_DisplacementSensorWarningC));
	this->displacementWarning->Timestamp = timestamp;
	this->displacementWarning->AnyWarning = true;
	this->displacementWarning->SensorReportsCommunicationTimeoutError = true;
	this->safetyController->displacementNotifySensorReportsCommunicationTimeoutError(this->displacementWarning->SensorReportsCommunicationTimeoutError);
	this->publisher->logDisplacementSensorWarning();
}

void Displacement::warnSensorReportsDataLengthError(double timestamp) {
	memset(this->displacementWarning, 0, sizeof(m1m3_logevent_DisplacementSensorWarningC));
	this->displacementWarning->Timestamp = timestamp;
	this->displacementWarning->AnyWarning = true;
	this->displacementWarning->SensorReportsDataLengthError = true;
	this->safetyController->displacementNotifySensorReportsDataLengthError(this->displacementWarning->SensorReportsDataLengthError);
	this->publisher->logDisplacementSensorWarning();
}

void Displacement::warnSensorReportsNumberOfParametersError(double timestamp) {
	memset(this->displacementWarning, 0, sizeof(m1m3_logevent_DisplacementSensorWarningC));
	this->displacementWarning->Timestamp = timestamp;
	this->displacementWarning->AnyWarning = true;
	this->displacementWarning->SensorReportsNumberOfParametersError = true;
	this->safetyController->displacementNotifySensorReportsNumberOfParametersError(this->displacementWarning->SensorReportsNumberOfParametersError);
	this->publisher->logDisplacementSensorWarning();
}

void Displacement::warnSensorReportsParameterError(double timestamp) {
	memset(&this->displacementWarning, 0, sizeof(m1m3_logevent_DisplacementSensorWarningC));
	this->displacementWarning->Timestamp = timestamp;
	this->displacementWarning->AnyWarning = true;
	this->displacementWarning->SensorReportsParameterError = true;
	this->safetyController->displacementNotifySensorReportsParameterError(this->displacementWarning->SensorReportsParameterError);
	this->publisher->logDisplacementSensorWarning();
}

void Displacement::warnSensorReportsCommunicationError(double timestamp) {
	memset(this->displacementWarning, 0, sizeof(m1m3_logevent_DisplacementSensorWarningC));
	this->displacementWarning->Timestamp = timestamp;
	this->displacementWarning->AnyWarning = true;
	this->displacementWarning->SensorReportsCommunicationError = true;
	this->safetyController->displacementNotifySensorReportsCommunicationError(this->displacementWarning->SensorReportsCommunicationError);
	this->publisher->logDisplacementSensorWarning();
}

void Displacement::warnSensorReportsIDNumberError(double timestamp) {
	memset(this->displacementWarning, 0, sizeof(m1m3_logevent_DisplacementSensorWarningC));
	this->displacementWarning->Timestamp = timestamp;
	this->displacementWarning->AnyWarning = true;
	this->displacementWarning->SensorReportsIDNumberError = true;
	this->safetyController->displacementNotifySensorReportsIDNumberError(this->displacementWarning->SensorReportsIDNumberError);
	this->publisher->logDisplacementSensorWarning();
}

void Displacement::warnSensorReportsExpansionLineError(double timestamp) {
	memset(this->displacementWarning, 0, sizeof(m1m3_logevent_DisplacementSensorWarningC));
	this->displacementWarning->Timestamp = timestamp;
	this->displacementWarning->AnyWarning = true;
	this->displacementWarning->SensorReportsExpansionLineError = true;
	this->safetyController->displacementNotifySensorReportsExpansionLineError(this->displacementWarning->SensorReportsExpansionLineError);
	this->publisher->logDisplacementSensorWarning();
}

void Displacement::warnSensorReportsWriteControlError(double timestamp) {
	memset(this->displacementWarning, 0, sizeof(m1m3_logevent_DisplacementSensorWarningC));
	this->displacementWarning->Timestamp = timestamp;
	this->displacementWarning->AnyWarning = true;
	this->displacementWarning->SensorReportsWriteControlError = true;
	this->safetyController->displacementNotifySensorReportsWriteControlError(this->displacementWarning->SensorReportsWriteControlError);
	this->publisher->logDisplacementSensorWarning();
}

void Displacement::warnResponseTimeoutError(double timestamp) {
	memset(this->displacementWarning, 0, sizeof(m1m3_logevent_DisplacementSensorWarningC));
	this->displacementWarning->Timestamp = timestamp;
	this->displacementWarning->AnyWarning = true;
	this->displacementWarning->ResponseTimeout = true;
	this->safetyController->displacementNotifyResponseTimeoutError(this->displacementWarning->ResponseTimeout);
	this->publisher->logDisplacementSensorWarning();
}

void Displacement::warnInvalidLength(double timestamp) {
	memset(this->displacementWarning, 0, sizeof(m1m3_logevent_DisplacementSensorWarningC));
	this->displacementWarning->Timestamp = timestamp;
	this->displacementWarning->AnyWarning = true;
	this->displacementWarning->InvalidLength = true;
	this->safetyController->displacementNotifyInvalidLength(this->displacementWarning->InvalidLength);
	this->publisher->logDisplacementSensorWarning();
}

void Displacement::warnInvalidResponse(double timestamp) {
	memset(this->displacementWarning, 0, sizeof(m1m3_logevent_DisplacementSensorWarningC));
	this->displacementWarning->Timestamp = timestamp;
	this->displacementWarning->AnyWarning = true;
	this->displacementWarning->InvalidResponse = true;
	this->safetyController->displacementNotifyInvalidResponse(this->displacementWarning->InvalidResponse);
	this->publisher->logDisplacementSensorWarning();
}

void Displacement::warnUnknownCommand(double timestamp) {
	memset(this->displacementWarning, 0, sizeof(m1m3_logevent_DisplacementSensorWarningC));
	this->displacementWarning->Timestamp = timestamp;
	this->displacementWarning->AnyWarning = true;
	this->displacementWarning->UnknownCommand = true;
	this->safetyController->displacementNotifyUnknownCommand(this->displacementWarning->UnknownCommand);
	this->publisher->logDisplacementSensorWarning();
}

void Displacement::warnUnknownProblem(double timestamp) {
	memset(this->displacementWarning, 0, sizeof(m1m3_logevent_DisplacementSensorWarningC));
	this->displacementWarning->Timestamp = timestamp;
	this->displacementWarning->AnyWarning = true;
	this->displacementWarning->UnknownProblem = true;
	this->safetyController->displacementNotifyUnknownProblem(this->displacementWarning->UnknownProblem);
	this->publisher->logDisplacementSensorWarning();
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
