/*
 * Displacement.cpp
 *
 *  Created on: Oct 6, 2017
 *      Author: ccontaxis
 */

#include <Displacement.h>
#include <IFPGA.h>
#include <IPublisher.h>
#include <U8ArrayUtilities.h>
#include <Timestamp.h>
#include <boost/tokenizer.hpp>
#include <boost/lexical_cast.hpp>
#include <string>
#include <iostream>

using namespace std;

namespace LSST {
namespace M1M3 {
namespace SS {

Displacement::Displacement(IPublisher* publisher, IFPGA* fpga) {
	this->publisher = publisher;
	this->fpga = fpga;
	this->createTxBuffer();
	this->displacementWarning.Timestamp = 0;
	this->displacementWarning.AnyError = false;
	this->displacementWarning.SensorReportsInvalidCommand = false;
	this->displacementWarning.SensorReportsCommunicationTimeoutError = false;
	this->displacementWarning.SensorReportsDataLengthError = false;
	this->displacementWarning.SensorReportsNumberOfParametersError = false;
	this->displacementWarning.SensorReportsParameterError = false;
	this->displacementWarning.SensorReportsCommunicationError = false;
	this->displacementWarning.SensorReportsIDNumberError = false;
	this->displacementWarning.SensorReportsExpansionLineError = false;
	this->displacementWarning.SensorReportsWriteControlError = false;
	this->displacementWarning.ResponseTimeout = false;
	this->displacementWarning.InvalidLength = false;
	this->displacementWarning.UnknownCommand = false;
	this->displacementWarning.UnknownProblem = false;
}

void Displacement::writeDataRequest() {
	this->fpga->writeRS232TxFIFO(this->txBuffer, 7, 0);
}

void Displacement::readDataResponse(uint8_t* buffer, int32_t* index) {
	int32_t startIndex = *index;
	uint16_t length = U8ArrayUtilities::u16(buffer, startIndex);
	double timestamp = Timestamp::fromRaw(U8ArrayUtilities::u64(buffer, startIndex + 2));
	std::string response = U8ArrayUtilities::toString(buffer, startIndex + 10, length - 8);
	typedef boost::tokenizer< boost::escaped_list_separator<char> > tokenizer;
	tokenizer tokenize(response);
	tokenizer::iterator token = tokenize.begin();
	if ((*token) == "M0") {
		this->imsData.Timestamp = timestamp;
		++token;
		this->imsData.RawSensorData[0] = boost::lexical_cast<double>(*token);
		++token;
		this->imsData.RawSensorData[1] = boost::lexical_cast<double>(*token);
		++token;
		this->imsData.RawSensorData[2] = boost::lexical_cast<double>(*token);
		++token;
		this->imsData.RawSensorData[3] = boost::lexical_cast<double>(*token);
		++token;
		this->imsData.RawSensorData[4] = boost::lexical_cast<double>(*token);
		++token;
		this->imsData.RawSensorData[5] = boost::lexical_cast<double>(*token);
		this->clearWarning(timestamp);
		this->publisher->putIMSData(&this->imsData);
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

void Displacement::createTxBuffer() {
	this->txBuffer[0] = 1;
	this->txBuffer[1] = 0;
	this->txBuffer[2] = 4;
	this->txBuffer[3] = (uint8_t)'M';
	this->txBuffer[4] = (uint8_t)'0';
	this->txBuffer[5] = (uint8_t)'\r';
	this->txBuffer[6] = (uint8_t)'\n';
}

void Displacement::clearWarning(double timestamp) {
	if (this->displacementWarning.AnyError) {
		this->displacementWarning.Timestamp = timestamp;
		this->displacementWarning.AnyError = false;
		this->displacementWarning.SensorReportsInvalidCommand = false;
		this->displacementWarning.SensorReportsCommunicationTimeoutError = false;
		this->displacementWarning.SensorReportsDataLengthError = false;
		this->displacementWarning.SensorReportsNumberOfParametersError = false;
		this->displacementWarning.SensorReportsParameterError = false;
		this->displacementWarning.SensorReportsCommunicationError = false;
		this->displacementWarning.SensorReportsIDNumberError = false;
		this->displacementWarning.SensorReportsExpansionLineError = false;
		this->displacementWarning.SensorReportsWriteControlError = false;
		this->displacementWarning.ResponseTimeout = false;
		this->displacementWarning.InvalidLength = false;
		this->displacementWarning.UnknownCommand = false;
		this->displacementWarning.UnknownProblem = false;
	}
}

void Displacement::warnSensorReportsInvalidCommand(double timestamp) {
	this->displacementWarning.Timestamp = timestamp;
	this->displacementWarning.AnyError = true;
	this->displacementWarning.SensorReportsInvalidCommand = true;
	this->displacementWarning.SensorReportsCommunicationTimeoutError = false;
	this->displacementWarning.SensorReportsDataLengthError = false;
	this->displacementWarning.SensorReportsNumberOfParametersError = false;
	this->displacementWarning.SensorReportsParameterError = false;
	this->displacementWarning.SensorReportsCommunicationError = false;
	this->displacementWarning.SensorReportsIDNumberError = false;
	this->displacementWarning.SensorReportsExpansionLineError = false;
	this->displacementWarning.SensorReportsWriteControlError = false;
	this->displacementWarning.ResponseTimeout = false;
	this->displacementWarning.InvalidLength = false;
	this->displacementWarning.UnknownCommand = false;
	this->displacementWarning.UnknownProblem = false;
}

void Displacement::warnSensorReportsCommunicationTimeoutError(double timestamp) {
	this->displacementWarning.Timestamp = timestamp;
	this->displacementWarning.AnyError = true;
	this->displacementWarning.SensorReportsInvalidCommand = false;
	this->displacementWarning.SensorReportsCommunicationTimeoutError = true;
	this->displacementWarning.SensorReportsDataLengthError = false;
	this->displacementWarning.SensorReportsNumberOfParametersError = false;
	this->displacementWarning.SensorReportsParameterError = false;
	this->displacementWarning.SensorReportsCommunicationError = false;
	this->displacementWarning.SensorReportsIDNumberError = false;
	this->displacementWarning.SensorReportsExpansionLineError = false;
	this->displacementWarning.SensorReportsWriteControlError = false;
	this->displacementWarning.ResponseTimeout = false;
	this->displacementWarning.InvalidLength = false;
	this->displacementWarning.UnknownCommand = false;
	this->displacementWarning.UnknownProblem = false;
}

void Displacement::warnSensorReportsDataLengthError(double timestamp) {
	this->displacementWarning.Timestamp = timestamp;
	this->displacementWarning.AnyError = true;
	this->displacementWarning.SensorReportsInvalidCommand = false;
	this->displacementWarning.SensorReportsCommunicationTimeoutError = false;
	this->displacementWarning.SensorReportsDataLengthError = true;
	this->displacementWarning.SensorReportsNumberOfParametersError = false;
	this->displacementWarning.SensorReportsParameterError = false;
	this->displacementWarning.SensorReportsCommunicationError = false;
	this->displacementWarning.SensorReportsIDNumberError = false;
	this->displacementWarning.SensorReportsExpansionLineError = false;
	this->displacementWarning.SensorReportsWriteControlError = false;
	this->displacementWarning.ResponseTimeout = false;
	this->displacementWarning.InvalidLength = false;
	this->displacementWarning.UnknownCommand = false;
	this->displacementWarning.UnknownProblem = false;
}

void Displacement::warnSensorReportsNumberOfParametersError(double timestamp) {
	this->displacementWarning.Timestamp = timestamp;
	this->displacementWarning.AnyError = true;
	this->displacementWarning.SensorReportsInvalidCommand = false;
	this->displacementWarning.SensorReportsCommunicationTimeoutError = false;
	this->displacementWarning.SensorReportsDataLengthError = false;
	this->displacementWarning.SensorReportsNumberOfParametersError = true;
	this->displacementWarning.SensorReportsParameterError = false;
	this->displacementWarning.SensorReportsCommunicationError = false;
	this->displacementWarning.SensorReportsIDNumberError = false;
	this->displacementWarning.SensorReportsExpansionLineError = false;
	this->displacementWarning.SensorReportsWriteControlError = false;
	this->displacementWarning.ResponseTimeout = false;
	this->displacementWarning.InvalidLength = false;
	this->displacementWarning.UnknownCommand = false;
	this->displacementWarning.UnknownProblem = false;
}

void Displacement::warnSensorReportsParameterError(double timestamp) {
	this->displacementWarning.Timestamp = timestamp;
	this->displacementWarning.AnyError = true;
	this->displacementWarning.SensorReportsInvalidCommand = false;
	this->displacementWarning.SensorReportsCommunicationTimeoutError = false;
	this->displacementWarning.SensorReportsDataLengthError = false;
	this->displacementWarning.SensorReportsNumberOfParametersError = false;
	this->displacementWarning.SensorReportsParameterError = true;
	this->displacementWarning.SensorReportsCommunicationError = false;
	this->displacementWarning.SensorReportsIDNumberError = false;
	this->displacementWarning.SensorReportsExpansionLineError = false;
	this->displacementWarning.SensorReportsWriteControlError = false;
	this->displacementWarning.ResponseTimeout = false;
	this->displacementWarning.InvalidLength = false;
	this->displacementWarning.UnknownCommand = false;
	this->displacementWarning.UnknownProblem = false;
}

void Displacement::warnSensorReportsCommunicationError(double timestamp) {
	this->displacementWarning.Timestamp = timestamp;
	this->displacementWarning.AnyError = true;
	this->displacementWarning.SensorReportsInvalidCommand = false;
	this->displacementWarning.SensorReportsCommunicationTimeoutError = false;
	this->displacementWarning.SensorReportsDataLengthError = false;
	this->displacementWarning.SensorReportsNumberOfParametersError = false;
	this->displacementWarning.SensorReportsParameterError = false;
	this->displacementWarning.SensorReportsCommunicationError = true;
	this->displacementWarning.SensorReportsIDNumberError = false;
	this->displacementWarning.SensorReportsExpansionLineError = false;
	this->displacementWarning.SensorReportsWriteControlError = false;
	this->displacementWarning.ResponseTimeout = false;
	this->displacementWarning.InvalidLength = false;
	this->displacementWarning.UnknownCommand = false;
	this->displacementWarning.UnknownProblem = false;
}

void Displacement::warnSensorReportsIDNumberError(double timestamp) {
	this->displacementWarning.Timestamp = timestamp;
	this->displacementWarning.AnyError = true;
	this->displacementWarning.SensorReportsInvalidCommand = false;
	this->displacementWarning.SensorReportsCommunicationTimeoutError = false;
	this->displacementWarning.SensorReportsDataLengthError = false;
	this->displacementWarning.SensorReportsNumberOfParametersError = false;
	this->displacementWarning.SensorReportsParameterError = false;
	this->displacementWarning.SensorReportsCommunicationError = false;
	this->displacementWarning.SensorReportsIDNumberError = true;
	this->displacementWarning.SensorReportsExpansionLineError = false;
	this->displacementWarning.SensorReportsWriteControlError = false;
	this->displacementWarning.ResponseTimeout = false;
	this->displacementWarning.InvalidLength = false;
	this->displacementWarning.UnknownCommand = false;
	this->displacementWarning.UnknownProblem = false;
}

void Displacement::warnSensorReportsExpansionLineError(double timestamp) {
	this->displacementWarning.Timestamp = timestamp;
	this->displacementWarning.AnyError = true;
	this->displacementWarning.SensorReportsInvalidCommand = false;
	this->displacementWarning.SensorReportsCommunicationTimeoutError = false;
	this->displacementWarning.SensorReportsDataLengthError = false;
	this->displacementWarning.SensorReportsNumberOfParametersError = false;
	this->displacementWarning.SensorReportsParameterError = false;
	this->displacementWarning.SensorReportsCommunicationError = false;
	this->displacementWarning.SensorReportsIDNumberError = false;
	this->displacementWarning.SensorReportsExpansionLineError = true;
	this->displacementWarning.SensorReportsWriteControlError = false;
	this->displacementWarning.ResponseTimeout = false;
	this->displacementWarning.InvalidLength = false;
	this->displacementWarning.UnknownCommand = false;
	this->displacementWarning.UnknownProblem = false;
}

void Displacement::warnSensorReportsWriteControlError(double timestamp) {
	this->displacementWarning.Timestamp = timestamp;
	this->displacementWarning.AnyError = true;
	this->displacementWarning.SensorReportsInvalidCommand = false;
	this->displacementWarning.SensorReportsCommunicationTimeoutError = false;
	this->displacementWarning.SensorReportsDataLengthError = false;
	this->displacementWarning.SensorReportsNumberOfParametersError = false;
	this->displacementWarning.SensorReportsParameterError = false;
	this->displacementWarning.SensorReportsCommunicationError = false;
	this->displacementWarning.SensorReportsIDNumberError = false;
	this->displacementWarning.SensorReportsExpansionLineError = false;
	this->displacementWarning.SensorReportsWriteControlError = true;
	this->displacementWarning.ResponseTimeout = false;
	this->displacementWarning.InvalidLength = false;
	this->displacementWarning.UnknownCommand = false;
	this->displacementWarning.UnknownProblem = false;
}

void Displacement::warnResponseTimeoutError(double timestamp) {
	this->displacementWarning.Timestamp = timestamp;
	this->displacementWarning.AnyError = true;
	this->displacementWarning.SensorReportsInvalidCommand = false;
	this->displacementWarning.SensorReportsCommunicationTimeoutError = false;
	this->displacementWarning.SensorReportsDataLengthError = false;
	this->displacementWarning.SensorReportsNumberOfParametersError = false;
	this->displacementWarning.SensorReportsParameterError = false;
	this->displacementWarning.SensorReportsCommunicationError = false;
	this->displacementWarning.SensorReportsIDNumberError = false;
	this->displacementWarning.SensorReportsExpansionLineError = false;
	this->displacementWarning.SensorReportsWriteControlError = false;
	this->displacementWarning.ResponseTimeout = true;
	this->displacementWarning.InvalidLength = false;
	this->displacementWarning.UnknownCommand = false;
	this->displacementWarning.UnknownProblem = false;
}

void Displacement::warnInvalidLength(double timestamp) {
	this->displacementWarning.Timestamp = timestamp;
	this->displacementWarning.AnyError = true;
	this->displacementWarning.SensorReportsInvalidCommand = false;
	this->displacementWarning.SensorReportsCommunicationTimeoutError = false;
	this->displacementWarning.SensorReportsDataLengthError = false;
	this->displacementWarning.SensorReportsNumberOfParametersError = false;
	this->displacementWarning.SensorReportsParameterError = false;
	this->displacementWarning.SensorReportsCommunicationError = false;
	this->displacementWarning.SensorReportsIDNumberError = false;
	this->displacementWarning.SensorReportsExpansionLineError = false;
	this->displacementWarning.SensorReportsWriteControlError = false;
	this->displacementWarning.ResponseTimeout = false;
	this->displacementWarning.InvalidLength = true;
	this->displacementWarning.UnknownCommand = false;
	this->displacementWarning.UnknownProblem = false;
}

void Displacement::warnUnknownCommand(double timestamp) {
	this->displacementWarning.Timestamp = timestamp;
	this->displacementWarning.AnyError = true;
	this->displacementWarning.SensorReportsInvalidCommand = false;
	this->displacementWarning.SensorReportsCommunicationTimeoutError = false;
	this->displacementWarning.SensorReportsDataLengthError = false;
	this->displacementWarning.SensorReportsNumberOfParametersError = false;
	this->displacementWarning.SensorReportsParameterError = false;
	this->displacementWarning.SensorReportsCommunicationError = false;
	this->displacementWarning.SensorReportsIDNumberError = false;
	this->displacementWarning.SensorReportsExpansionLineError = false;
	this->displacementWarning.SensorReportsWriteControlError = false;
	this->displacementWarning.ResponseTimeout = false;
	this->displacementWarning.InvalidLength = false;
	this->displacementWarning.UnknownCommand = true;
	this->displacementWarning.UnknownProblem = false;
}

void Displacement::warnUnknownProblem(double timestamp) {
	this->displacementWarning.Timestamp = timestamp;
	this->displacementWarning.AnyError = true;
	this->displacementWarning.SensorReportsInvalidCommand = false;
	this->displacementWarning.SensorReportsCommunicationTimeoutError = false;
	this->displacementWarning.SensorReportsDataLengthError = false;
	this->displacementWarning.SensorReportsNumberOfParametersError = false;
	this->displacementWarning.SensorReportsParameterError = false;
	this->displacementWarning.SensorReportsCommunicationError = false;
	this->displacementWarning.SensorReportsIDNumberError = false;
	this->displacementWarning.SensorReportsExpansionLineError = false;
	this->displacementWarning.SensorReportsWriteControlError = false;
	this->displacementWarning.ResponseTimeout = false;
	this->displacementWarning.InvalidLength = false;
	this->displacementWarning.UnknownCommand = false;
	this->displacementWarning.UnknownProblem = true;
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
