/*
 * This file is part of LSST M1M3 support system package.
 *
 * Developed for the Vera C. Rubin Telescope and Site System.
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

// define that to printout bytes for CRC
#include <stdexcept>
#include <string.h>

#ifdef DEBUG_CRC
#include <cRIO/CliApp.h>
#include <iomanip>
#include <iostream>
#endif

#include <spdlog/spdlog.h>

#include <IFPGA.h>
#include <ModbusBuffer.h>
#include <Timestamp.h>

using namespace std;

// masks for FPGA FIFO commands
const static uint16_t FIFO_INSTRUCTION = 0xF000;
const static uint16_t FIFO_TX_WRITE = 0x1000;
const static uint16_t FIFO_TX_FRAMEEND = 0x20DA;
const static uint16_t FIFO_TX_TIMESTAMP = 0x3000;
const static uint16_t FIFO_TX_WAIT_US = 0x4000;
const static uint16_t FIFO_TX_WAIT_MS = 0x5000;
const static uint16_t FIFO_TX_WAIT_RX = 0x6000;
const static uint16_t FIFO_TX_IRQTRIGGER = 0x7000;
const static uint16_t FIFO_TX_WAIT_TRIGGER = 0x8000;
const static uint16_t FIFO_TX_WAIT_LONG_RX = 0x9000;
const static uint16_t FIFO_RX_READ = 0x9000;
const static uint16_t FIFO_RX_ENDFRAME = 0xA000;
const static uint16_t FIFO_RX_TIMESTAMP = 0xB000;

namespace LSST {
namespace M1M3 {
namespace SS {

ModbusBuffer::ModbusBuffer() {
    reset();
    setLength(0);
}

ModbusBuffer::~ModbusBuffer() {}

int32_t ModbusBuffer::getIndex() { return _index; }

void ModbusBuffer::setIndex(int32_t index) { _index = index; }

void ModbusBuffer::incIndex(int32_t inc) { _index += inc; }

void ModbusBuffer::skipToNextFrame() {
    // Scan for the end of frame marker
    while (!this->endOfFrame() && !this->endOfBuffer()) {
        _index++;
    }
    // Increment to the address of the next message in the buffer
    _index++;
}

uint16_t* ModbusBuffer::getBuffer() { return _buffer; }

void ModbusBuffer::set(int32_t index, uint16_t data) { _buffer[index] = data; }

void ModbusBuffer::setLength(int32_t length) { _length = length; }

int32_t ModbusBuffer::getLength() { return _length; }

void ModbusBuffer::reset() { _index = 0; }

bool ModbusBuffer::endOfBuffer() { return _index >= _length; }

bool ModbusBuffer::endOfFrame() { return _buffer[_index] == FIFO_RX_ENDFRAME; }

std::vector<uint8_t> ModbusBuffer::getReadData(int32_t length) {
    std::vector<uint8_t> data;
    for (int i = _index - length; i < _index; i++) {
        data.push_back(readInstructionByte(_buffer[i]));
    }
    return data;
}

uint16_t ModbusBuffer::calculateCRC(std::vector<uint8_t> data) {
    uint16_t crc = 0xFFFF;
    for (std::vector<uint8_t>::iterator i = data.begin(); i < data.end(); i++) {
        crc = crc ^ static_cast<uint16_t>(*i);
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

uint16_t ModbusBuffer::calculateCRC(int32_t length) {
#ifdef DEBUG_CRC
    std::cout << "Data in ";
    LSST::cRIO::CliApp::printHexBuffer(_buffer + _index - length, length);
    std::cout << std::endl;
#endif
    return calculateCRC(getReadData(length));
}

uint16_t ModbusBuffer::readLength() { return _buffer[_index++]; }

int32_t ModbusBuffer::readI32() {
    _index += 4;
    return ((int32_t)this->readInstructionByte(_buffer[_index - 4]) << 24) |
           ((int32_t)this->readInstructionByte(_buffer[_index - 3]) << 16) |
           ((int32_t)this->readInstructionByte(_buffer[_index - 2]) << 8) |
           ((int32_t)this->readInstructionByte(_buffer[_index - 1]));
}

uint8_t ModbusBuffer::readU8() {
    _index += 1;
    return this->readInstructionByte(_buffer[_index - 1]);
}

uint16_t ModbusBuffer::readU16() {
    _index += 2;
    return ((uint16_t)this->readInstructionByte(_buffer[_index - 2]) << 8) |
           ((uint16_t)this->readInstructionByte(_buffer[_index - 1]));
}

uint32_t ModbusBuffer::readU32() {
    _index += 4;
    return ((uint32_t)this->readInstructionByte(_buffer[_index - 4]) << 24) |
           ((uint32_t)this->readInstructionByte(_buffer[_index - 3]) << 16) |
           ((uint32_t)this->readInstructionByte(_buffer[_index - 2]) << 8) |
           ((uint32_t)this->readInstructionByte(_buffer[_index - 1]));
}

uint64_t ModbusBuffer::readU48() {
    _index += 6;
    return ((uint64_t)this->readInstructionByte(_buffer[_index - 6]) << 40) |
           ((uint64_t)this->readInstructionByte(_buffer[_index - 5]) << 32) |
           ((uint64_t)this->readInstructionByte(_buffer[_index - 4]) << 24) |
           ((uint64_t)this->readInstructionByte(_buffer[_index - 3]) << 16) |
           ((uint64_t)this->readInstructionByte(_buffer[_index - 2]) << 8) |
           ((uint64_t)this->readInstructionByte(_buffer[_index - 1]));
}

float ModbusBuffer::readSGL() {
    _index += 4;
    _floatPointBuffer[3] = this->readInstructionByte(_buffer[_index - 4]);
    _floatPointBuffer[2] = this->readInstructionByte(_buffer[_index - 3]);
    _floatPointBuffer[1] = this->readInstructionByte(_buffer[_index - 2]);
    _floatPointBuffer[0] = this->readInstructionByte(_buffer[_index - 1]);
    float data;
    memcpy(&data, _floatPointBuffer, sizeof(float));
    return data;
}

std::string ModbusBuffer::readString(int32_t length) {
    for (int i = 0; i < length; i++) {
        _stringBuffer[i] = this->readInstructionByte(_buffer[_index++]);
    }
    return std::string((const char*)_stringBuffer, (size_t)length);
}

uint16_t ModbusBuffer::readCRC() {
    _index += 2;
    return static_cast<uint16_t>(readInstructionByte(_buffer[_index - 2])) |
           static_cast<uint16_t>(readInstructionByte(_buffer[_index - 1])) << 8;
}

double ModbusBuffer::readTimestamp() {
    _index += 8;
    uint64_t data =
            ((uint64_t)(_buffer[_index - 8] & 0xFF)) | ((uint64_t)(_buffer[_index - 7] & 0xFF)) << 8 |
            ((uint64_t)(_buffer[_index - 6] & 0xFF)) << 16 | ((uint64_t)(_buffer[_index - 5] & 0xFF)) << 24 |
            ((uint64_t)(_buffer[_index - 4] & 0xFF)) << 32 | ((uint64_t)(_buffer[_index - 3] & 0xFF)) << 40 |
            ((uint64_t)(_buffer[_index - 2] & 0xFF)) << 48 | ((uint64_t)(_buffer[_index - 1] & 0xFF)) << 56;
    return Timestamp::fromRaw(data);
}

void ModbusBuffer::readEndOfFrame() { _index++; }

void ModbusBuffer::writeSubnet(uint8_t data) {
    _index += 1;
    _buffer[_index - 1] = (uint16_t)data;
}

void ModbusBuffer::writeLength(uint16_t data) {
    _index += 1;
    _buffer[_index - 1] = data;
}

void ModbusBuffer::writeI8(int8_t data) {
    _index += 1;
    _buffer[_index - 1] = this->writeByteInstruction((uint8_t)data);
}

void ModbusBuffer::writeI16(int16_t data) {
    _index += 2;
    _buffer[_index - 2] = this->writeByteInstruction((uint8_t)(data >> 8));
    _buffer[_index - 1] = this->writeByteInstruction((uint8_t)data);
}

void ModbusBuffer::writeI24(int32_t data) {
    _index += 3;
    _buffer[_index - 3] = this->writeByteInstruction((uint8_t)(data >> 16));
    _buffer[_index - 2] = this->writeByteInstruction((uint8_t)(data >> 8));
    _buffer[_index - 1] = this->writeByteInstruction((uint8_t)data);
}

void ModbusBuffer::writeI32(int32_t data) {
    _index += 4;
    _buffer[_index - 4] = this->writeByteInstruction((uint8_t)(data >> 24));
    _buffer[_index - 3] = this->writeByteInstruction((uint8_t)(data >> 16));
    _buffer[_index - 2] = this->writeByteInstruction((uint8_t)(data >> 8));
    _buffer[_index - 1] = this->writeByteInstruction((uint8_t)data);
}

void ModbusBuffer::writeU8(uint8_t data) {
    _index += 1;
    _buffer[_index - 1] = this->writeByteInstruction(data);
}

void ModbusBuffer::writeU16(uint16_t data) {
    _index += 2;
    _buffer[_index - 2] = this->writeByteInstruction((uint8_t)(data >> 8));
    _buffer[_index - 1] = this->writeByteInstruction((uint8_t)data);
}

void ModbusBuffer::writeU32(uint32_t data) {
    _index += 4;
    _buffer[_index - 4] = this->writeByteInstruction((uint8_t)(data >> 24));
    _buffer[_index - 3] = this->writeByteInstruction((uint8_t)(data >> 16));
    _buffer[_index - 2] = this->writeByteInstruction((uint8_t)(data >> 8));
    _buffer[_index - 1] = this->writeByteInstruction((uint8_t)data);
}

void ModbusBuffer::writeSGL(float data) {
    memcpy(_floatPointBuffer, &data, sizeof(float));
    _index += 4;
    _buffer[_index - 4] = this->writeByteInstruction((uint8_t)_floatPointBuffer[3]);
    _buffer[_index - 3] = this->writeByteInstruction((uint8_t)_floatPointBuffer[2]);
    _buffer[_index - 2] = this->writeByteInstruction((uint8_t)_floatPointBuffer[1]);
    _buffer[_index - 1] = this->writeByteInstruction((uint8_t)_floatPointBuffer[0]);
}

void ModbusBuffer::writeCRC(int32_t length) {
    uint16_t crc = this->calculateCRC(length);
    _index += 2;
    _buffer[_index - 2] = this->writeByteInstruction((uint8_t)crc);
    _buffer[_index - 1] = this->writeByteInstruction((uint8_t)(crc >> 8));
}

void ModbusBuffer::writeDelay(uint32_t delayMicros) {
    _index += 1;
    _buffer[_index - 1] = delayMicros > 4095 ? (((delayMicros / 1000) + 1) | 0x5000) : (delayMicros | 0x4000);
}

void ModbusBuffer::writeEndOfFrame() {
    _index += 1;
    _buffer[_index - 1] = FIFO_TX_FRAMEEND;
}

void ModbusBuffer::writeSoftwareTrigger() {
    _index += 1;
    _buffer[_index - 1] = FIFO_TX_WAIT_TRIGGER;
}

void ModbusBuffer::writeTimestamp() {
    _index += 1;
    _buffer[_index - 1] = FIFO_TX_TIMESTAMP;
}

void ModbusBuffer::writeTriggerIRQ() {
    _index += 1;
    _buffer[_index - 1] = FIFO_TX_IRQTRIGGER;
}

void ModbusBuffer::writeWaitForRx(uint32_t timeoutMicros) {
    _index += 1;
    _buffer[_index - 1] = timeoutMicros > 4095 ? (((timeoutMicros / 1000) + 1) | FIFO_TX_WAIT_LONG_RX)
                                               : (timeoutMicros | FIFO_TX_WAIT_RX);
}

void ModbusBuffer::pullModbusResponse(uint16_t request, uint64_t& beginTs, uint64_t& endTs,
                                      std::vector<uint8_t>& data) {
    IFPGA::get().writeRequestFIFO(request, 0);

    reset();

    uint16_t reportedLength;
    IFPGA::get().readU16ResponseFIFO(&reportedLength, 1, 20);
    if (reportedLength <= 4) {
        throw std::runtime_error("ModbusBuffer::pullModbusResponse: Timeout on response: " +
                                 std::to_string(reportedLength));
    }

    IFPGA::get().readU16ResponseFIFO(getBuffer(), reportedLength, 10);
    setLength(reportedLength);

    // check for frame ends - should be just one
    int endCount = 0;
    endTs = 0;
    int endTsShift = 0;
    for (int i = 4; i < reportedLength; i++) {
        switch (_buffer[i] & FIFO_INSTRUCTION) {
            case FIFO_RX_READ:
                data.push_back(readInstructionByte(_buffer[i]));
                break;
            case FIFO_RX_TIMESTAMP:
                endTs |= (uint64_t)(_buffer[i] & 0xFF) << endTsShift;
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

    if (_buffer[getLength() - 1] != FIFO_RX_ENDFRAME)
        throw std::runtime_error("ModbusBuffer::pullModbusResponse: last received word isn't ENDFRAME");

    // remove CRC16
    uint16_t crc = data.back();
    data.pop_back();
    crc = (crc << 8) | (uint16_t)data.back();
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
