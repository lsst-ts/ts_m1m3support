/*
 * ModbusBuffer.cpp
 *
 *  Created on: Sep 29, 2017
 *      Author: ccontaxis
 */

#include <ModbusBuffer.h>
#include <Timestamp.h>
#include <string.h>

namespace LSST {
namespace M1M3 {
namespace SS {

ModbusBuffer::ModbusBuffer() {
	this->index = 0;
	this->length = 0;
}

ModbusBuffer::~ModbusBuffer() {
	// TODO Auto-generated destructor stub
}

int32_t ModbusBuffer::getIndex() {
	return this->index;
}

void ModbusBuffer::setIndex(int32_t index) {
	this->index = index;
}

void ModbusBuffer::incIndex(int32_t inc) {
	this->index += inc;
}

void ModbusBuffer::skipToNextFrame() {
	this->index += 11;
}

uint16_t* ModbusBuffer::getBuffer() {
	return this->buffer;
}

void ModbusBuffer::set(int32_t index, uint16_t data) {
	this->buffer[index] = data;
}

void ModbusBuffer::setLength(int32_t length) {
	this->length = length;
}

int32_t ModbusBuffer::getLength() {
	return this->length;
}

void ModbusBuffer::reset() {
	this->index = 0;
}

bool ModbusBuffer::endOfBuffer() {
	return this->index >= this->length;
}

bool ModbusBuffer::endOfFrame() {
	return this->buffer[this->index] == 0xA000;
}

uint16_t ModbusBuffer::calculateCRC(int32_t length) {
	uint16_t crc = 0xFFFF;
	for(int i = this->index - length; i < this->index; i++) {
		crc = crc ^ ((uint16_t)this->readInstructionByte(this->buffer[i]));
		for(int j = 0; j < 8; j++) {
			if (crc & 0x0001) {
				crc = crc >> 1;
				crc = crc ^ 0xA001;
			}
			else {
				crc = crc >> 1;
			}
		}
	}
	return crc;
}

uint8_t ModbusBuffer::readSubnet() {
	return (uint8_t)this->buffer[this->index++];
}

uint16_t ModbusBuffer::readLength() {
	return this->buffer[this->index++];
}

int8_t ModbusBuffer::readI8() {
	return (int8_t)this->readInstructionByte(this->buffer[this->index++]);
}

int16_t ModbusBuffer::readI16() {
	return ((int16_t)this->readInstructionByte(this->buffer[this->index++]) << 8) |
			((int16_t)this->readInstructionByte(this->buffer[this->index++]));
}

int32_t ModbusBuffer::readI32() {
	return ((int32_t)this->readInstructionByte(this->buffer[this->index++]) << 24) |
			((int32_t)this->readInstructionByte(this->buffer[this->index++]) << 16) |
			((int32_t)this->readInstructionByte(this->buffer[this->index++]) << 8) |
			((int32_t)this->readInstructionByte(this->buffer[this->index++]));
}
int64_t ModbusBuffer::readI64() {
	return ((int64_t)this->readInstructionByte(this->buffer[this->index++]) << 56) |
			((int64_t)this->readInstructionByte(this->buffer[this->index++]) << 48) |
			((int64_t)this->readInstructionByte(this->buffer[this->index++]) << 40) |
			((int64_t)this->readInstructionByte(this->buffer[this->index++]) << 32) |
			((int64_t)this->readInstructionByte(this->buffer[this->index++]) << 24) |
			((int64_t)this->readInstructionByte(this->buffer[this->index++]) << 16) |
			((int64_t)this->readInstructionByte(this->buffer[this->index++]) << 8) |
			((int64_t)this->readInstructionByte(this->buffer[this->index++]));
}
uint8_t ModbusBuffer::readU8() {
	return this->readInstructionByte(this->buffer[this->index++]);
}

uint16_t ModbusBuffer::readU16() {
	return ((uint16_t)this->readInstructionByte(this->buffer[this->index++]) << 8) |
			((uint16_t)this->readInstructionByte(this->buffer[this->index++]));
}

uint32_t ModbusBuffer::readU32() {
	return ((uint32_t)this->readInstructionByte(this->buffer[this->index++]) << 24) |
			((uint32_t)this->readInstructionByte(this->buffer[this->index++]) << 16) |
			((uint32_t)this->readInstructionByte(this->buffer[this->index++]) << 8) |
			((uint32_t)this->readInstructionByte(this->buffer[this->index++]));
}

uint64_t ModbusBuffer::readU48() {
	return ((uint64_t)this->readInstructionByte(this->buffer[this->index++]) << 40) |
			((uint64_t)this->readInstructionByte(this->buffer[this->index++]) << 32) |
			((uint64_t)this->readInstructionByte(this->buffer[this->index++]) << 24) |
			((uint64_t)this->readInstructionByte(this->buffer[this->index++]) << 16) |
			((uint64_t)this->readInstructionByte(this->buffer[this->index++]) << 8) |
			((uint64_t)this->readInstructionByte(this->buffer[this->index++]));
}

uint64_t ModbusBuffer::readU64() {
	return ((uint64_t)this->readInstructionByte(this->buffer[this->index++]) << 56) |
			((uint64_t)this->readInstructionByte(this->buffer[this->index++]) << 48) |
			((uint64_t)this->readInstructionByte(this->buffer[this->index++]) << 40) |
			((uint64_t)this->readInstructionByte(this->buffer[this->index++]) << 32) |
			((uint64_t)this->readInstructionByte(this->buffer[this->index++]) << 24) |
			((uint64_t)this->readInstructionByte(this->buffer[this->index++]) << 16) |
			((uint64_t)this->readInstructionByte(this->buffer[this->index++]) << 8) |
			((uint64_t)this->readInstructionByte(this->buffer[this->index++]));
}

float ModbusBuffer::readSGL() {
	this->floatPointBuffer[0] = this->readInstructionByte(this->buffer[this->index++]);
	this->floatPointBuffer[1] = this->readInstructionByte(this->buffer[this->index++]);
	this->floatPointBuffer[2] = this->readInstructionByte(this->buffer[this->index++]);
	this->floatPointBuffer[3] = this->readInstructionByte(this->buffer[this->index++]);
	float data;
	memcpy(&data, this->floatPointBuffer, sizeof(float));
	return data;
}

double ModbusBuffer::readDBL() {
	this->floatPointBuffer[0] = this->readInstructionByte(this->buffer[this->index++]);
	this->floatPointBuffer[1] = this->readInstructionByte(this->buffer[this->index++]);
	this->floatPointBuffer[2] = this->readInstructionByte(this->buffer[this->index++]);
	this->floatPointBuffer[3] = this->readInstructionByte(this->buffer[this->index++]);
	this->floatPointBuffer[4] = this->readInstructionByte(this->buffer[this->index++]);
	this->floatPointBuffer[5] = this->readInstructionByte(this->buffer[this->index++]);
	this->floatPointBuffer[6] = this->readInstructionByte(this->buffer[this->index++]);
	this->floatPointBuffer[7] = this->readInstructionByte(this->buffer[this->index++]);
	double data;
	memcpy(&data, this->floatPointBuffer, sizeof(double));
	return data;
}

std::string ModbusBuffer::readString(int32_t length) {
	for(int i = 0; i < length; i++) {
		this->stringBuffer[i] = this->readInstructionByte(this->buffer[this->index++]);
	}
	return std::string((const char*)this->stringBuffer, (size_t)length);
}

uint16_t ModbusBuffer::readCRC() {
	return ((uint16_t)this->readInstructionByte(this->buffer[this->index++])) |
			((uint16_t)this->readInstructionByte(this->buffer[this->index++]) << 8);
}

double ModbusBuffer::readTimestamp() {
	uint64_t data =
			((uint64_t)this->readInstructionByte(this->buffer[this->index++])) |
			((uint64_t)this->readInstructionByte(this->buffer[this->index++])) << 8 |
			((uint64_t)this->readInstructionByte(this->buffer[this->index++])) << 16 |
			((uint64_t)this->readInstructionByte(this->buffer[this->index++])) << 24 |
			((uint64_t)this->readInstructionByte(this->buffer[this->index++])) << 32 |
			((uint64_t)this->readInstructionByte(this->buffer[this->index++])) << 40 |
			((uint64_t)this->readInstructionByte(this->buffer[this->index++])) << 48 |
			((uint64_t)this->readInstructionByte(this->buffer[this->index++])) << 56;
	return Timestamp::fromRaw(data);
}

uint8_t ModbusBuffer::readInstructionByte(uint16_t instruction) {
	return (uint8_t)((instruction >> 1) & 0xFF);
}

uint8_t ModbusBuffer::readTimestampByte(uint16_t instruction) {
	return (uint8_t)instruction;
}

void ModbusBuffer::readEndOfFrame() {
	this->index++;
}

void ModbusBuffer::writeSubnet(uint8_t data) {
	this->buffer[this->index++] = (uint16_t)data;
}

void ModbusBuffer::writeLength(uint16_t data) {
	this->buffer[this->index++] = data;
}

void ModbusBuffer::writeI8(int8_t data) {
	this->buffer[this->index++] = this->writeByteInstruction((uint8_t)data);
}

void ModbusBuffer::writeI16(int16_t data) {
	this->buffer[this->index++] = this->writeByteInstruction((uint8_t)(data >> 8));
	this->buffer[this->index++] = this->writeByteInstruction((uint8_t)data);
}

void ModbusBuffer::writeI24(int32_t data) {
	this->buffer[this->index++] = this->writeByteInstruction((uint8_t)(data >> 16));
	this->buffer[this->index++] = this->writeByteInstruction((uint8_t)(data >> 8));
	this->buffer[this->index++] = this->writeByteInstruction((uint8_t)data);
}

void ModbusBuffer::writeI32(int32_t data) {
	this->buffer[this->index++] = this->writeByteInstruction((uint8_t)(data >> 24));
	this->buffer[this->index++] = this->writeByteInstruction((uint8_t)(data >> 16));
	this->buffer[this->index++] = this->writeByteInstruction((uint8_t)(data >> 8));
	this->buffer[this->index++] = this->writeByteInstruction((uint8_t)data);
}

void ModbusBuffer::writeI64(int64_t data) {
	this->buffer[this->index++] = this->writeByteInstruction((uint8_t)(data >> 56));
	this->buffer[this->index++] = this->writeByteInstruction((uint8_t)(data >> 48));
	this->buffer[this->index++] = this->writeByteInstruction((uint8_t)(data >> 40));
	this->buffer[this->index++] = this->writeByteInstruction((uint8_t)(data >> 32));
	this->buffer[this->index++] = this->writeByteInstruction((uint8_t)(data >> 24));
	this->buffer[this->index++] = this->writeByteInstruction((uint8_t)(data >> 16));
	this->buffer[this->index++] = this->writeByteInstruction((uint8_t)(data >> 8));
	this->buffer[this->index++] = this->writeByteInstruction((uint8_t)data);
}

void ModbusBuffer::writeU8(uint8_t data) {
	this->buffer[this->index++] = this->writeByteInstruction(data);
}

void ModbusBuffer::writeU16(uint16_t data) {
	this->buffer[this->index++] = this->writeByteInstruction((uint8_t)(data >> 8));
	this->buffer[this->index++] = this->writeByteInstruction((uint8_t)data);
}

void ModbusBuffer::writeU32(uint32_t data) {
	this->buffer[this->index++] = this->writeByteInstruction((uint8_t)(data >> 24));
	this->buffer[this->index++] = this->writeByteInstruction((uint8_t)(data >> 16));
	this->buffer[this->index++] = this->writeByteInstruction((uint8_t)(data >> 8));
	this->buffer[this->index++] = this->writeByteInstruction((uint8_t)data);
}

void ModbusBuffer::writeU64(uint64_t data) {
	this->buffer[this->index++] = this->writeByteInstruction((uint8_t)(data >> 56));
	this->buffer[this->index++] = this->writeByteInstruction((uint8_t)(data >> 48));
	this->buffer[this->index++] = this->writeByteInstruction((uint8_t)(data >> 40));
	this->buffer[this->index++] = this->writeByteInstruction((uint8_t)(data >> 32));
	this->buffer[this->index++] = this->writeByteInstruction((uint8_t)(data >> 24));
	this->buffer[this->index++] = this->writeByteInstruction((uint8_t)(data >> 16));
	this->buffer[this->index++] = this->writeByteInstruction((uint8_t)(data >> 8));
	this->buffer[this->index++] = this->writeByteInstruction((uint8_t)data);
}

void ModbusBuffer::writeSGL(float data) {
	memcpy(this->floatPointBuffer, &data, sizeof(float));
	this->buffer[this->index++] = this->writeByteInstruction((uint8_t)this->floatPointBuffer[0]);
	this->buffer[this->index++] = this->writeByteInstruction((uint8_t)this->floatPointBuffer[1]);
	this->buffer[this->index++] = this->writeByteInstruction((uint8_t)this->floatPointBuffer[2]);
	this->buffer[this->index++] = this->writeByteInstruction((uint8_t)this->floatPointBuffer[3]);
}

void ModbusBuffer::writeDBL(double data) {
	memcpy(this->floatPointBuffer, &data, sizeof(double));
	this->buffer[this->index++] = this->writeByteInstruction((uint8_t)this->floatPointBuffer[0]);
	this->buffer[this->index++] = this->writeByteInstruction((uint8_t)this->floatPointBuffer[1]);
	this->buffer[this->index++] = this->writeByteInstruction((uint8_t)this->floatPointBuffer[2]);
	this->buffer[this->index++] = this->writeByteInstruction((uint8_t)this->floatPointBuffer[3]);
	this->buffer[this->index++] = this->writeByteInstruction((uint8_t)this->floatPointBuffer[4]);
	this->buffer[this->index++] = this->writeByteInstruction((uint8_t)this->floatPointBuffer[5]);
	this->buffer[this->index++] = this->writeByteInstruction((uint8_t)this->floatPointBuffer[6]);
	this->buffer[this->index++] = this->writeByteInstruction((uint8_t)this->floatPointBuffer[7]);
}

void ModbusBuffer::writeCRC(int32_t length) {
	uint16_t crc = this->calculateCRC(length);
	this->buffer[this->index++] = this->writeByteInstruction((uint8_t)crc);
	this->buffer[this->index++] = this->writeByteInstruction((uint8_t)(crc >> 8));
}

uint16_t ModbusBuffer::writeByteInstruction(uint8_t data) {
	return (((uint16_t)data) << 1) | 0x1200;
}

void ModbusBuffer::writeDelay(uint32_t delayMicros) {
	this->buffer[this->index++] = delayMicros > 4095 ? (((delayMicros / 1000) + 1) | 0x5000) : (delayMicros | 0x4000);
}

void ModbusBuffer::writeEndOfFrame() {
	this->buffer[this->index++] = 0x20DA;
}

void ModbusBuffer::writeSoftwareTrigger() {
	this->buffer[this->index++] = 0x8000;
}

void ModbusBuffer::writeTimestamp() {
	this->buffer[this->index++] = 0x3000;
}

void ModbusBuffer::writeTriggerIRQ() {
	this->buffer[this->index++] = 0x7000;
}

void ModbusBuffer::writeWaitForRx(uint32_t timeoutMicros) {
	this->buffer[this->index++] = timeoutMicros > 4095 ? (((timeoutMicros / 1000) + 1) | 0x9000) : (timeoutMicros | 0x6000);
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
