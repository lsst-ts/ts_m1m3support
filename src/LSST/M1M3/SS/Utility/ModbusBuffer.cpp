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

#include <ModbusBuffer.h>
#include <IFPGA.h>
#include <Timestamp.h>
#include <string.h>
#include <spdlog/spdlog.h>
#include <stdexcept>

using namespace std;

namespace LSST {
namespace M1M3 {
namespace SS {

ModbusBuffer::ModbusBuffer() {
    reset();
    setLength(0);
}

ModbusBuffer::~ModbusBuffer() {}

int32_t ModbusBuffer::getIndex() { return this->index; }

void ModbusBuffer::setIndex(int32_t index) { this->index = index; }

void ModbusBuffer::incIndex(int32_t inc) { this->index += inc; }

void ModbusBuffer::skipToNextFrame() {
    // Scan for the end of frame marker
    while (!this->endOfFrame() && !this->endOfBuffer()) {
        this->index++;
    }
    // Increment to the address of the next message in the buffer
    this->index++;
}

uint16_t* ModbusBuffer::getBuffer() { return this->buffer; }

void ModbusBuffer::set(int32_t index, uint16_t data) { this->buffer[index] = data; }

void ModbusBuffer::setLength(int32_t length) { this->length = length; }

int32_t ModbusBuffer::getLength() { return this->length; }

void ModbusBuffer::reset() { this->index = 0; }

bool ModbusBuffer::endOfBuffer() { return this->index >= this->length; }

bool ModbusBuffer::endOfFrame() { return this->buffer[this->index] == FIFO_RX_ENDFRAME; }

std::vector<uint8_t> ModbusBuffer::getReadData(int32_t length) {
    std::vector<uint8_t> data;
    for (int i = index - length; i < index; i++) {
        data.push_back(readInstructionByte(buffer[i]));
    }
    return data;
}

uint16_t ModbusBuffer::calculateCRC(std::vector<uint8_t> data) {
    uint16_t crc = 0xFFFF;
    for (auto i : data) {
        crc = crc ^ (uint16_t(i));
        for (int j = 0; j < 8; j++) {
            if (crc & 0x0001) {
                crc = crc >> 1;
                crc = crc ^ 0xA001;
            } else {
                crc = crc >> 1;
            }
        }
    }
    return crc;
}

uint16_t ModbusBuffer::calculateCRC(int32_t length) { return calculateCRC(getReadData(length)); }

uint16_t ModbusBuffer::readLength() { return this->buffer[this->index++]; }

int32_t ModbusBuffer::readI32() {
    this->index += 4;
    return ((int32_t)this->readInstructionByte(this->buffer[this->index - 4]) << 24) |
           ((int32_t)this->readInstructionByte(this->buffer[this->index - 3]) << 16) |
           ((int32_t)this->readInstructionByte(this->buffer[this->index - 2]) << 8) |
           ((int32_t)this->readInstructionByte(this->buffer[this->index - 1]));
}

uint8_t ModbusBuffer::readU8() {
    this->index += 1;
    return this->readInstructionByte(this->buffer[this->index - 1]);
}

uint16_t ModbusBuffer::readU16() {
    this->index += 2;
    return ((uint16_t)this->readInstructionByte(this->buffer[this->index - 2]) << 8) |
           ((uint16_t)this->readInstructionByte(this->buffer[this->index - 1]));
}

uint32_t ModbusBuffer::readU32() {
    this->index += 4;
    return ((uint32_t)this->readInstructionByte(this->buffer[this->index - 4]) << 24) |
           ((uint32_t)this->readInstructionByte(this->buffer[this->index - 3]) << 16) |
           ((uint32_t)this->readInstructionByte(this->buffer[this->index - 2]) << 8) |
           ((uint32_t)this->readInstructionByte(this->buffer[this->index - 1]));
}

uint64_t ModbusBuffer::readU48() {
    this->index += 6;
    return ((uint64_t)this->readInstructionByte(this->buffer[this->index - 6]) << 40) |
           ((uint64_t)this->readInstructionByte(this->buffer[this->index - 5]) << 32) |
           ((uint64_t)this->readInstructionByte(this->buffer[this->index - 4]) << 24) |
           ((uint64_t)this->readInstructionByte(this->buffer[this->index - 3]) << 16) |
           ((uint64_t)this->readInstructionByte(this->buffer[this->index - 2]) << 8) |
           ((uint64_t)this->readInstructionByte(this->buffer[this->index - 1]));
}

float ModbusBuffer::readSGL() {
    this->index += 4;
    this->floatPointBuffer[3] = this->readInstructionByte(this->buffer[this->index - 4]);
    this->floatPointBuffer[2] = this->readInstructionByte(this->buffer[this->index - 3]);
    this->floatPointBuffer[1] = this->readInstructionByte(this->buffer[this->index - 2]);
    this->floatPointBuffer[0] = this->readInstructionByte(this->buffer[this->index - 1]);
    float data;
    memcpy(&data, this->floatPointBuffer, sizeof(float));
    return data;
}

std::string ModbusBuffer::readString(int32_t length) {
    for (int i = 0; i < length; i++) {
        this->stringBuffer[i] = this->readInstructionByte(this->buffer[this->index++]);
    }
    return std::string((const char*)this->stringBuffer, (size_t)length);
}

uint16_t ModbusBuffer::readCRC() {
    this->index += 2;
    return ((uint16_t)this->readInstructionByte(this->buffer[this->index - 2])) |
           ((uint16_t)this->readInstructionByte(this->buffer[this->index - 1]) << 8);
}

double ModbusBuffer::readTimestamp() {
    this->index += 8;
    uint64_t data = ((uint64_t)(this->buffer[this->index - 8] & 0xFF)) |
                    ((uint64_t)(this->buffer[this->index - 7] & 0xFF)) << 8 |
                    ((uint64_t)(this->buffer[this->index - 6] & 0xFF)) << 16 |
                    ((uint64_t)(this->buffer[this->index - 5] & 0xFF)) << 24 |
                    ((uint64_t)(this->buffer[this->index - 4] & 0xFF)) << 32 |
                    ((uint64_t)(this->buffer[this->index - 3] & 0xFF)) << 40 |
                    ((uint64_t)(this->buffer[this->index - 2] & 0xFF)) << 48 |
                    ((uint64_t)(this->buffer[this->index - 1] & 0xFF)) << 56;
    return Timestamp::fromRaw(data);
}

void ModbusBuffer::readEndOfFrame() { this->index++; }

void ModbusBuffer::writeSubnet(uint8_t data) {
    this->index += 1;
    this->buffer[this->index - 1] = (uint16_t)data;
}

void ModbusBuffer::writeLength(uint16_t data) {
    this->index += 1;
    this->buffer[this->index - 1] = data;
}

void ModbusBuffer::writeI8(int8_t data) {
    this->index += 1;
    this->buffer[this->index - 1] = this->writeByteInstruction((uint8_t)data);
}

void ModbusBuffer::writeI16(int16_t data) {
    this->index += 2;
    this->buffer[this->index - 2] = this->writeByteInstruction((uint8_t)(data >> 8));
    this->buffer[this->index - 1] = this->writeByteInstruction((uint8_t)data);
}

void ModbusBuffer::writeI24(int32_t data) {
    this->index += 3;
    this->buffer[this->index - 3] = this->writeByteInstruction((uint8_t)(data >> 16));
    this->buffer[this->index - 2] = this->writeByteInstruction((uint8_t)(data >> 8));
    this->buffer[this->index - 1] = this->writeByteInstruction((uint8_t)data);
}

void ModbusBuffer::writeI32(int32_t data) {
    this->index += 4;
    this->buffer[this->index - 4] = this->writeByteInstruction((uint8_t)(data >> 24));
    this->buffer[this->index - 3] = this->writeByteInstruction((uint8_t)(data >> 16));
    this->buffer[this->index - 2] = this->writeByteInstruction((uint8_t)(data >> 8));
    this->buffer[this->index - 1] = this->writeByteInstruction((uint8_t)data);
}

void ModbusBuffer::writeU8(uint8_t data) {
    this->index += 1;
    this->buffer[this->index - 1] = this->writeByteInstruction(data);
}

void ModbusBuffer::writeU16(uint16_t data) {
    this->index += 2;
    this->buffer[this->index - 2] = this->writeByteInstruction((uint8_t)(data >> 8));
    this->buffer[this->index - 1] = this->writeByteInstruction((uint8_t)data);
}

void ModbusBuffer::writeU32(uint32_t data) {
    this->index += 4;
    this->buffer[this->index - 4] = this->writeByteInstruction((uint8_t)(data >> 24));
    this->buffer[this->index - 3] = this->writeByteInstruction((uint8_t)(data >> 16));
    this->buffer[this->index - 2] = this->writeByteInstruction((uint8_t)(data >> 8));
    this->buffer[this->index - 1] = this->writeByteInstruction((uint8_t)data);
}

void ModbusBuffer::writeSGL(float data) {
    memcpy(this->floatPointBuffer, &data, sizeof(float));
    this->index += 4;
    this->buffer[this->index - 4] = this->writeByteInstruction((uint8_t)this->floatPointBuffer[3]);
    this->buffer[this->index - 3] = this->writeByteInstruction((uint8_t)this->floatPointBuffer[2]);
    this->buffer[this->index - 2] = this->writeByteInstruction((uint8_t)this->floatPointBuffer[1]);
    this->buffer[this->index - 1] = this->writeByteInstruction((uint8_t)this->floatPointBuffer[0]);
}

void ModbusBuffer::writeCRC(int32_t length) {
    uint16_t crc = this->calculateCRC(length);
    this->index += 2;
    this->buffer[this->index - 2] = this->writeByteInstruction((uint8_t)crc);
    this->buffer[this->index - 1] = this->writeByteInstruction((uint8_t)(crc >> 8));
}

void ModbusBuffer::writeDelay(uint32_t delayMicros) {
    this->index += 1;
    this->buffer[this->index - 1] =
            delayMicros > 4095 ? (((delayMicros / 1000) + 1) | 0x5000) : (delayMicros | 0x4000);
}

void ModbusBuffer::writeEndOfFrame() {
    this->index += 1;
    this->buffer[this->index - 1] = FIFO_TX_FRAMEEND;
}

void ModbusBuffer::writeSoftwareTrigger() {
    this->index += 1;
    this->buffer[this->index - 1] = FIFO_TX_WAIT_TRIGGER;
}

void ModbusBuffer::writeTimestamp() {
    this->index += 1;
    this->buffer[this->index - 1] = FIFO_TX_TIMESTAMP;
}

void ModbusBuffer::writeTriggerIRQ() {
    this->index += 1;
    this->buffer[this->index - 1] = FIFO_TX_IRQTRIGGER;
}

void ModbusBuffer::writeWaitForRx(uint32_t timeoutMicros) {
    this->index += 1;
    this->buffer[this->index - 1] = timeoutMicros > 4095
                                            ? (((timeoutMicros / 1000) + 1) | FIFO_TX_WAIT_LONG_RX)
                                            : (timeoutMicros | FIFO_TX_WAIT_RX);
}

void ModbusBuffer::pullModbusResponse(uint16_t request, uint64_t& beginTs, uint64_t& endTs,
                                      std::vector<uint8_t>& data) {
    IFPGA::get().writeRequestFIFO(&request, 1, 0);

    reset();

    uint16_t reportedLength;
    IFPGA::get().readU16ResponseFIFO(&reportedLength, 1, 20);
    if (reportedLength <= 4) {
        throw std::runtime_error("ModbusBuffer::pullModbusResponse: Timeout on response");
    }

    IFPGA::get().readU16ResponseFIFO(getBuffer(), reportedLength, 10);
    setLength(reportedLength);

    // check for frame ends - should be just one
    int endCount = 0;
    endTs = 0;
    int endTsShift = 0;
    for (int i = 4; i < reportedLength; i++) {
        switch (buffer[i] & FIFO_INSTRUCTION) {
            case FIFO_RX_READ:
                data.push_back(readInstructionByte(buffer[i]));
                break;
            case FIFO_RX_TIMESTAMP:
                endTs |= (uint64_t)(buffer[i] & 0xFF) << endTsShift;
                endTsShift += 8;
                break;
            case FIFO_RX_ENDFRAME:
                endCount++;
                break;
        }
    }

    // at least instruction and some payload shall be received
    if (data.size() < 3) {
        throw std::runtime_error("ModbusBuffer::pullModbusResponse: data not received");
    }

    if (endCount != 1) {
        throw std::runtime_error("ModbusBuffer::pullModbusResponse: wrong number of end frames");
    }

    if (endTsShift != 64) {
        throw std::runtime_error("ModbusBuffer::pollModbusResponse End timestamp not received");
    }

    if (buffer[getLength() - 1] != FIFO_RX_ENDFRAME)
        throw std::runtime_error("ModbusBuffer::pullModbusResponse: last received word isn't ENDFRAME");

    // remove CRC16
    uint16_t crc = data.back();
    data.pop_back();
    crc |= ((uint16_t)data.back()) << 8;
    data.pop_back();

    beginTs = *((uint64_t*)getBuffer());

    uint16_t calcCrc = calculateCRC(data);

    if (calcCrc != crc) {
        throw std::runtime_error(fmt::format("Invalid CRC: {:#06x} expected {:#06x}", crc, calcCrc));
    }
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
