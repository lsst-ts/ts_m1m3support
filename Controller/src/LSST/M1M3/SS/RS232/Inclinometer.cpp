/*

 * Inclinometer.cpp
 *
 *  Created on: Oct 2, 2017
 *      Author: ccontaxis
 */

#include <Inclinometer.h>
#include <IFPGA.h>
#include <IPublisher.h>
#include <U8ArrayUtilities.h>
#include <CRC.h>
#include <Timestamp.h>

namespace LSST {
namespace M1M3 {
namespace SS {

Inclinometer::Inclinometer() {
	this->fpga = 0;
	this->publisher = 0;
	this->createTxBuffer();
}

Inclinometer::Inclinometer(IFPGA* fpga, IPublisher* publisher) {
	this->fpga = fpga;
	this->publisher = publisher;
	this->createTxBuffer();
	memset(&this->inclinometerData, 0, sizeof(m1m3_InclinometerDataC));
	memset(&this->inclinometerWarning, 0, sizeof(m1m3_logevent_InclinometerSensorWarningC));
}

void Inclinometer::writeDataRequest() {
	this->fpga->writeRS232TxFIFO(this->txBuffer, 11, 10);
}

void Inclinometer::readDataResponse(uint8_t* buffer, int32_t* index) {
	int32_t startIndex = *index;
	uint16_t length = U8ArrayUtilities::u16(buffer, startIndex);
	double timestamp = Timestamp::fromRaw(U8ArrayUtilities::u64(buffer, startIndex + 2));
	uint16_t calculatedCRC = CRC::modbus(buffer, startIndex + 10, length - 10);
	uint16_t crc =
			((uint16_t)buffer[startIndex + length]) |
			((uint16_t)buffer[startIndex + length + 1] << 8);
	if (crc == calculatedCRC) {
		uint8_t address = buffer[startIndex + 10];
		if (address == 127) {
			uint8_t function = buffer[startIndex + 11];
			if (function == 3) {
				uint8_t length = buffer[startIndex + 12];
				if (length == 4) {
					uint32_t angle =
							((uint32_t)buffer[startIndex + 13] << 8) |
							((uint32_t)buffer[startIndex + 14]) |
							((uint32_t)buffer[startIndex + 15] << 24) |
							((uint32_t)buffer[startIndex + 16] << 16);
					this->inclinometerData.Timestamp = timestamp;
					this->inclinometerData.InclinometerAngle = angle / 1000.0;
					this->publisher->putInclinometerData(&this->inclinometerData);
					this->clearWarning(timestamp);
				}
				else {
					this->warnInvalidLength(timestamp);
				}
			}
			else if (function == 0x83) {
				uint8_t exceptionCode = buffer[startIndex + 12];
				if (exceptionCode == 1) {
					this->warnInvalidFunction(timestamp);
				}
				else if (exceptionCode == 2) {
					this->warnInvalidRegister(timestamp);
				}
				else {
					this->warnUnknownProblem(timestamp);
				}
			}
			else {
				this->warnUnknownFunction(timestamp);
			}
		}
		else {
			this->warnUnknownAddress(timestamp);
		}
	}
	else {
		this->warnInvalidCRC(timestamp);
	}

	(*index) = startIndex + length + 2;
}

void Inclinometer::createTxBuffer() {
	this->txBuffer[0] = 2;
	this->txBuffer[1] = 0;
	this->txBuffer[2] = 8;
	this->txBuffer[3] = 127;
	this->txBuffer[4] = 3;
	this->txBuffer[5] = 0;
	this->txBuffer[6] = 0;
	this->txBuffer[7] = 0;
	this->txBuffer[8] = 2;
	this->txBuffer[9] = 206;
	this->txBuffer[10] = 21;
}

void Inclinometer::clearWarning(double timestamp) {
	if (this->inclinometerWarning.AnyError) {
		memset(&this->inclinometerWarning, 0, sizeof(m1m3_logevent_InclinometerSensorWarningC));
		this->inclinometerWarning.Timestamp = timestamp;
		this->publisher->logInclinometerSensorWarning(&this->inclinometerWarning);
	}
}

void Inclinometer::warnInvalidCRC(double timestamp) {
	memset(&this->inclinometerWarning, 0, sizeof(m1m3_logevent_InclinometerSensorWarningC));
	this->inclinometerWarning.Timestamp = timestamp;
	this->inclinometerWarning.AnyError = true;
	this->inclinometerWarning.InvalidCRC = true;
	this->publisher->logInclinometerSensorWarning(&this->inclinometerWarning);
}

void Inclinometer::warnUnknownAddress(double timestamp) {
	memset(&this->inclinometerWarning, 0, sizeof(m1m3_logevent_InclinometerSensorWarningC));
	this->inclinometerWarning.Timestamp = timestamp;
	this->inclinometerWarning.AnyError = true;
	this->inclinometerWarning.UnknownAddress = true;
	this->publisher->logInclinometerSensorWarning(&this->inclinometerWarning);
}

void Inclinometer::warnUnknownFunction(double timestamp) {
	memset(&this->inclinometerWarning, 0, sizeof(m1m3_logevent_InclinometerSensorWarningC));
	this->inclinometerWarning.Timestamp = timestamp;
	this->inclinometerWarning.AnyError = true;
	this->inclinometerWarning.UnknownFunction= true;
	this->publisher->logInclinometerSensorWarning(&this->inclinometerWarning);
}

void Inclinometer::warnInvalidLength(double timestamp) {
	memset(&this->inclinometerWarning, 0, sizeof(m1m3_logevent_InclinometerSensorWarningC));
	this->inclinometerWarning.Timestamp = timestamp;
	this->inclinometerWarning.AnyError = true;
	this->inclinometerWarning.InvalidLength = true;
	this->publisher->logInclinometerSensorWarning(&this->inclinometerWarning);
}

void Inclinometer::warnInvalidRegister(double timestamp) {
	memset(&this->inclinometerWarning, 0, sizeof(m1m3_logevent_InclinometerSensorWarningC));
	this->inclinometerWarning.Timestamp = timestamp;
	this->inclinometerWarning.AnyError = true;
	this->inclinometerWarning.SensorReportsIllegalDataAddress = true;
	this->publisher->logInclinometerSensorWarning(&this->inclinometerWarning);
}

void Inclinometer::warnInvalidFunction(double timestamp) {
	memset(&this->inclinometerWarning, 0, sizeof(m1m3_logevent_InclinometerSensorWarningC));
	this->inclinometerWarning.Timestamp = timestamp;
	this->inclinometerWarning.AnyError = true;
	this->inclinometerWarning.SensorReportsIllegalFunction = true;
	this->publisher->logInclinometerSensorWarning(&this->inclinometerWarning);
}

void Inclinometer::warnUnknownProblem(double timestamp) {
	memset(&this->inclinometerWarning, 0, sizeof(m1m3_logevent_InclinometerSensorWarningC));
	this->inclinometerWarning.Timestamp = timestamp;
	this->inclinometerWarning.AnyError = true;
	this->inclinometerWarning.UnknownProblem = true;
	this->publisher->logInclinometerSensorWarning(&this->inclinometerWarning);
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
