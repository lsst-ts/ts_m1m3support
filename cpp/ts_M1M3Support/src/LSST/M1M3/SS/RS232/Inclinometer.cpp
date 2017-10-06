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
	this->inclinometerData.Timestamp = 0;
	this->inclinometerData.InclinometerAngle = 0;
	this->inclinometerWarning.Timestamp = 0;
	this->inclinometerWarning.InvalidCRC = false;
	this->inclinometerWarning.UnknownAddress = false;
	this->inclinometerWarning.UnknownFunction = false;
	this->inclinometerWarning.InvalidLength = false;
	this->inclinometerWarning.IllegalDataAddress = false;
	this->inclinometerWarning.IllegalFunction = false;
	this->inclinometerWarning.ResponseTimeout = false;
	this->inclinometerWarning.UnknownProblem = false;
}

Inclinometer::~Inclinometer() {
	// TODO Auto-generated destructor stub
}

void Inclinometer::queryData() {
	this->fpga->writeRS232TxFIFO(this->txBuffer, 11, 10);
}

void Inclinometer::parseData(uint8_t* buffer, int32_t* index) {
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
	this->txBuffer[0] = 1;
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
	if (this->inclinometerWarning.InvalidCRC || this->inclinometerWarning.UnknownAddress || this->inclinometerWarning.UnknownFunction ||
			this->inclinometerWarning.InvalidLength || this->inclinometerWarning.IllegalDataAddress || this->inclinometerWarning.IllegalFunction ||
			this->inclinometerWarning.ResponseTimeout || this->inclinometerWarning.UnknownProblem) {
		this->inclinometerWarning.Timestamp = timestamp;
		this->inclinometerWarning.InvalidCRC = false;
		this->inclinometerWarning.UnknownAddress = false;
		this->inclinometerWarning.UnknownFunction = false;
		this->inclinometerWarning.InvalidLength = false;
		this->inclinometerWarning.IllegalDataAddress = false;
		this->inclinometerWarning.IllegalFunction = false;
		this->inclinometerWarning.ResponseTimeout = false;
		this->inclinometerWarning.UnknownProblem = false;
		this->publisher->logInclinometerWarning(&this->inclinometerWarning);
	}
}

void Inclinometer::warnInvalidCRC(double timestamp) {
	if (!this->inclinometerWarning.InvalidCRC) {
		this->inclinometerWarning.Timestamp = timestamp;
		this->inclinometerWarning.InvalidCRC = true;
		this->inclinometerWarning.UnknownAddress = false;
		this->inclinometerWarning.UnknownFunction = false;
		this->inclinometerWarning.InvalidLength = false;
		this->inclinometerWarning.IllegalDataAddress = false;
		this->inclinometerWarning.IllegalFunction = false;
		this->inclinometerWarning.ResponseTimeout = false;
		this->inclinometerWarning.UnknownProblem = false;
		this->publisher->logInclinometerWarning(&this->inclinometerWarning);
	}
}

void Inclinometer::warnUnknownAddress(double timestamp) {
	if (!this->inclinometerWarning.UnknownAddress) {
		this->inclinometerWarning.Timestamp = timestamp;
		this->inclinometerWarning.InvalidCRC = false;
		this->inclinometerWarning.UnknownAddress = true;
		this->inclinometerWarning.UnknownFunction = false;
		this->inclinometerWarning.InvalidLength = false;
		this->inclinometerWarning.IllegalDataAddress = false;
		this->inclinometerWarning.IllegalFunction = false;
		this->inclinometerWarning.ResponseTimeout = false;
		this->inclinometerWarning.UnknownProblem = false;
		this->publisher->logInclinometerWarning(&this->inclinometerWarning);
	}
}

void Inclinometer::warnUnknownFunction(double timestamp) {
	if (!this->inclinometerWarning.UnknownFunction) {
		this->inclinometerWarning.Timestamp = timestamp;
		this->inclinometerWarning.InvalidCRC = false;
		this->inclinometerWarning.UnknownAddress = false;
		this->inclinometerWarning.UnknownFunction = true;
		this->inclinometerWarning.InvalidLength = false;
		this->inclinometerWarning.IllegalDataAddress = false;
		this->inclinometerWarning.IllegalFunction = false;
		this->inclinometerWarning.ResponseTimeout = false;
		this->inclinometerWarning.UnknownProblem = false;
		this->publisher->logInclinometerWarning(&this->inclinometerWarning);
	}
}

void Inclinometer::warnInvalidLength(double timestamp) {
	if (!this->inclinometerWarning.InvalidLength) {
		this->inclinometerWarning.Timestamp = timestamp;
		this->inclinometerWarning.InvalidCRC = false;
		this->inclinometerWarning.UnknownAddress = false;
		this->inclinometerWarning.UnknownFunction = false;
		this->inclinometerWarning.InvalidLength = true;
		this->inclinometerWarning.IllegalDataAddress = false;
		this->inclinometerWarning.IllegalFunction = false;
		this->inclinometerWarning.ResponseTimeout = false;
		this->inclinometerWarning.UnknownProblem = false;
		this->publisher->logInclinometerWarning(&this->inclinometerWarning);
	}
}

void Inclinometer::warnInvalidRegister(double timestamp) {
	if (!this->inclinometerWarning.IllegalDataAddress) {
		this->inclinometerWarning.Timestamp = timestamp;
		this->inclinometerWarning.InvalidCRC = false;
		this->inclinometerWarning.UnknownAddress = false;
		this->inclinometerWarning.UnknownFunction = false;
		this->inclinometerWarning.InvalidLength = false;
		this->inclinometerWarning.IllegalDataAddress = true;
		this->inclinometerWarning.IllegalFunction = false;
		this->inclinometerWarning.ResponseTimeout = false;
		this->inclinometerWarning.UnknownProblem = false;
		this->publisher->logInclinometerWarning(&this->inclinometerWarning);
	}
}

void Inclinometer::warnInvalidFunction(double timestamp) {
	if (!this->inclinometerWarning.IllegalFunction) {
		this->inclinometerWarning.Timestamp = timestamp;
		this->inclinometerWarning.InvalidCRC = false;
		this->inclinometerWarning.UnknownAddress = false;
		this->inclinometerWarning.UnknownFunction = false;
		this->inclinometerWarning.InvalidLength = false;
		this->inclinometerWarning.IllegalDataAddress = false;
		this->inclinometerWarning.IllegalFunction = true;
		this->inclinometerWarning.ResponseTimeout = false;
		this->inclinometerWarning.UnknownProblem = false;
		this->publisher->logInclinometerWarning(&this->inclinometerWarning);
	}
}

void Inclinometer::warnUnknownProblem(double timestamp) {
	if (!this->inclinometerWarning.UnknownProblem) {
		this->inclinometerWarning.Timestamp = timestamp;
		this->inclinometerWarning.InvalidCRC = false;
		this->inclinometerWarning.UnknownAddress = false;
		this->inclinometerWarning.UnknownFunction = false;
		this->inclinometerWarning.InvalidLength = false;
		this->inclinometerWarning.IllegalDataAddress = false;
		this->inclinometerWarning.IllegalFunction = false;
		this->inclinometerWarning.ResponseTimeout = false;
		this->inclinometerWarning.UnknownProblem = true;
		this->publisher->logInclinometerWarning(&this->inclinometerWarning);
	}
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
