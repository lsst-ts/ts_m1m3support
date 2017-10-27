/*

 * Inclinometer.cpp
 *
 *  Created on: Oct 2, 2017
 *      Author: ccontaxis
 */

#include <Inclinometer.h>
#include <IPublisher.h>
#include <ISafetyController.h>
#include <IFPGA.h>
#include <FPGAAddresses.h>
#include <U8ArrayUtilities.h>
#include <CRC.h>
#include <Timestamp.h>
#include <SAL_m1m3C.h>
#include <cstring>

namespace LSST {
namespace M1M3 {
namespace SS {

Inclinometer::Inclinometer(IPublisher* publisher, ISafetyController* safetyController, IFPGA* fpga) {
	this->publisher = publisher;
	this->safetyController = safetyController;
	this->fpga = fpga;

	this->inclinometerData = this->publisher->getInclinometerData();
	this->inclinometerWarning = this->publisher->getEventInclinometerSensorWarning();

	memset(this->inclinometerData, 0, sizeof(m1m3_InclinometerDataC));
	memset(this->inclinometerWarning, 0, sizeof(m1m3_logevent_InclinometerSensorWarningC));

	this->createTxBuffer();
}

void Inclinometer::writeDataRequest() {
	this->fpga->writeCommandFIFO(this->txBuffer, 10, 0);
}

void Inclinometer::readDataResponse() {
	this->fpga->writeRequestFIFO(FPGAAddresses::Inclinometer, 0);
	this->fpga->readU8ResponseFIFO(this->rxBuffer, 2, 10);
	uint16_t length = U8ArrayUtilities::u16(this->rxBuffer, 0);
	if (length != 0) {
		this->fpga->readU8ResponseFIFO(this->rxBuffer, length, 10);
		double timestamp = Timestamp::fromRaw(U8ArrayUtilities::u64(this->rxBuffer, 0));
		uint16_t calculatedCRC = CRC::modbus(this->rxBuffer, 8, length - 10);
		uint16_t crc =
				((uint16_t)this->rxBuffer[length - 2]) |
				((uint16_t)this->rxBuffer[length - 1] << 8);
		if (crc == calculatedCRC) {
			uint8_t address = this->rxBuffer[8];
			if (address == 127) {
				uint8_t function = this->rxBuffer[9];
				if (function == 3) {
					uint8_t length = this->rxBuffer[10];
					if (length == 4) {
						uint32_t angle =
								((uint32_t)this->rxBuffer[11] << 8) |
								((uint32_t)this->rxBuffer[12]) |
								((uint32_t)this->rxBuffer[13] << 24) |
								((uint32_t)this->rxBuffer[14] << 16);
						this->inclinometerData->Timestamp = timestamp;
						this->inclinometerData->InclinometerAngle = angle / 1000.0;
						this->publisher->putInclinometerData();
						this->clearWarning(timestamp);
					}
					else {
						this->warnInvalidLength(timestamp);
					}
				}
				else if (function == 0x83) {
					switch(this->rxBuffer[10]) {
					case 1: this->warnInvalidFunction(timestamp); break;
					case 2: this->warnInvalidRegister(timestamp); break;
					default: this->warnUnknownProblem(timestamp); break;
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
	}
	else {
		this->warnResponseTimeout(this->publisher->getTimestamp());
	}
}

void Inclinometer::createTxBuffer() {
	this->txBuffer[0] = FPGAAddresses::Inclinometer;
	this->txBuffer[1] = 8;
	this->txBuffer[2] = 127;
	this->txBuffer[3] = 3;
	this->txBuffer[4] = 0;
	this->txBuffer[5] = 0;
	this->txBuffer[6] = 0;
	this->txBuffer[7] = 2;
	this->txBuffer[8] = 206;
	this->txBuffer[9] = 21;
}

void Inclinometer::clearWarning(double timestamp) {
	if (this->inclinometerWarning->AnyWarning) {
		memset(this->inclinometerWarning, 0, sizeof(m1m3_logevent_InclinometerSensorWarningC));
		this->inclinometerWarning->Timestamp = timestamp;
		this->safetyController->inclinometerNotifyResponseTimeout(this->inclinometerWarning->ResponseTimeout);
		this->safetyController->inclinometerNotifyInvalidCRC(this->inclinometerWarning->InvalidCRC);
		this->safetyController->inclinometerNotifyUnknownAddress(this->inclinometerWarning->UnknownAddress);
		this->safetyController->inclinometerNotifyUnknownFunction(this->inclinometerWarning->UnknownFunction);
		this->safetyController->inclinometerNotifyInvalidLength(this->inclinometerWarning->InvalidLength);
		this->safetyController->inclinometerNotifySensorReportsIllegalDataAddress(this->inclinometerWarning->SensorReportsIllegalDataAddress);
		this->safetyController->inclinometerNotifySensorReportsIllegalFunction(this->inclinometerWarning->SensorReportsIllegalFunction);
		this->safetyController->inclinometerNotifyUnknownProblem(this->inclinometerWarning->UnknownProblem);
		this->publisher->logInclinometerSensorWarning();
	}
}

void Inclinometer::warnResponseTimeout(double timestamp) {
	memset(this->inclinometerWarning, 0, sizeof(m1m3_logevent_InclinometerSensorWarningC));
	this->inclinometerWarning->Timestamp = timestamp;
	this->inclinometerWarning->AnyWarning = true;
	this->inclinometerWarning->ResponseTimeout = true;
	this->safetyController->inclinometerNotifyResponseTimeout(this->inclinometerWarning->ResponseTimeout);
	this->publisher->logInclinometerSensorWarning();
}

void Inclinometer::warnInvalidCRC(double timestamp) {
	memset(this->inclinometerWarning, 0, sizeof(m1m3_logevent_InclinometerSensorWarningC));
	this->inclinometerWarning->Timestamp = timestamp;
	this->inclinometerWarning->AnyWarning = true;
	this->inclinometerWarning->InvalidCRC = true;
	this->safetyController->inclinometerNotifyInvalidCRC(this->inclinometerWarning->InvalidCRC);
	this->publisher->logInclinometerSensorWarning();
}

void Inclinometer::warnUnknownAddress(double timestamp) {
	memset(this->inclinometerWarning, 0, sizeof(m1m3_logevent_InclinometerSensorWarningC));
	this->inclinometerWarning->Timestamp = timestamp;
	this->inclinometerWarning->AnyWarning = true;
	this->inclinometerWarning->UnknownAddress = true;
	this->safetyController->inclinometerNotifyUnknownAddress(this->inclinometerWarning->UnknownAddress);
	this->publisher->logInclinometerSensorWarning();
}

void Inclinometer::warnUnknownFunction(double timestamp) {
	memset(this->inclinometerWarning, 0, sizeof(m1m3_logevent_InclinometerSensorWarningC));
	this->inclinometerWarning->Timestamp = timestamp;
	this->inclinometerWarning->AnyWarning = true;
	this->inclinometerWarning->UnknownFunction = true;
	this->safetyController->inclinometerNotifyUnknownFunction(this->inclinometerWarning->UnknownFunction);
	this->publisher->logInclinometerSensorWarning();
}

void Inclinometer::warnInvalidLength(double timestamp) {
	memset(this->inclinometerWarning, 0, sizeof(m1m3_logevent_InclinometerSensorWarningC));
	this->inclinometerWarning->Timestamp = timestamp;
	this->inclinometerWarning->AnyWarning = true;
	this->inclinometerWarning->InvalidLength = true;
	this->safetyController->inclinometerNotifyInvalidLength(this->inclinometerWarning->InvalidLength);
	this->publisher->logInclinometerSensorWarning();
}

void Inclinometer::warnInvalidRegister(double timestamp) {
	memset(this->inclinometerWarning, 0, sizeof(m1m3_logevent_InclinometerSensorWarningC));
	this->inclinometerWarning->Timestamp = timestamp;
	this->inclinometerWarning->AnyWarning = true;
	this->inclinometerWarning->SensorReportsIllegalDataAddress = true;
	this->safetyController->inclinometerNotifySensorReportsIllegalDataAddress(this->inclinometerWarning->SensorReportsIllegalDataAddress);
	this->publisher->logInclinometerSensorWarning();
}

void Inclinometer::warnInvalidFunction(double timestamp) {
	memset(this->inclinometerWarning, 0, sizeof(m1m3_logevent_InclinometerSensorWarningC));
	this->inclinometerWarning->Timestamp = timestamp;
	this->inclinometerWarning->AnyWarning = true;
	this->inclinometerWarning->SensorReportsIllegalFunction = true;
	this->safetyController->inclinometerNotifySensorReportsIllegalFunction(this->inclinometerWarning->SensorReportsIllegalFunction);
	this->publisher->logInclinometerSensorWarning();
}

void Inclinometer::warnUnknownProblem(double timestamp) {
	memset(this->inclinometerWarning, 0, sizeof(m1m3_logevent_InclinometerSensorWarningC));
	this->inclinometerWarning->Timestamp = timestamp;
	this->inclinometerWarning->AnyWarning = true;
	this->inclinometerWarning->UnknownProblem = true;
	this->safetyController->inclinometerNotifyUnknownProblem(this->inclinometerWarning->UnknownProblem);
	this->publisher->logInclinometerSensorWarning();
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
