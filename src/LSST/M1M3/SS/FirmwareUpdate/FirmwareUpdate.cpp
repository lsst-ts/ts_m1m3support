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

#include <FirmwareUpdate.h>
#include <cstring>
#include <spdlog/spdlog.h>
#include <fstream>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>
#include <CRC.h>
#include <IFPGA.h>
#include <ModbusBuffer.h>
#include <DataTypes.h>
#include <FPGAAddresses.h>
#include <ILCDataTypes.h>
#include <ILCSubnetData.h>

namespace LSST {
namespace M1M3 {
namespace SS {

FirmwareUpdate::FirmwareUpdate(ILCSubnetData* subnetData) {
    this->subnetData = subnetData;
    this->desiredState = 255;
}

bool FirmwareUpdate::Program(int actuatorId, std::string filePath) {
    ILCMap map = this->subnetData->getMap(actuatorId);
    int subnet = map.Subnet;
    int address = map.Address;
    if (subnet == 255 || address == 255) {
        spdlog::error("FirmwareUpdate: Unknown actuator {}", actuatorId);
        return false;
    }
    if (!this->CreateAppData()) {
        spdlog::error("FirmwareUpdate: Failed to create application data for actuator {}", actuatorId);
        return false;
    }
    if (!this->ProcessFile(filePath)) {
        spdlog::error("FirmwareUpdate: Failed to process intel hex file for actuator {}", actuatorId);
        return false;
    }
    if (!this->UpdateAppData()) {
        spdlog::error("FirmwareUpdate: Failed to load intel hex file into application data for actuator {:d}",
                      actuatorId);
        return false;
    }
    if (!this->UpdateAppDataCRC()) {
        spdlog::error("FirmwareUpdate: Failed to calculate application data CRC for actuator {}", actuatorId);
        return false;
    }
    if (!this->UpdateAppStatCRC()) {
        spdlog::error("FirmwareUpdate: Failed to calculate application statistics CRC for actuator {}",
                      actuatorId);
        return false;
    }
    if (!this->RestartApplication(subnet, address)) {
        spdlog::error(
                "FirmwareUpdate: Failed to restart ILC application for actuator {} before loading firmware",
                actuatorId);
        return false;
    }
    if (!this->EnterFirmwareUpdate(subnet, address)) {
        spdlog::error("FirmwareUpdate: Failed to start transition to firmware update state for actuator {}",
                      actuatorId);
        return false;
    }
    if (!this->ClearFaults(subnet, address)) {
        spdlog::error("FirmwareUpdate: Failed to clear faults to enter firmware update state for actuator {}",
                      actuatorId);
        return false;
    }
    if (!this->EraseILCApplication(subnet, address)) {
        spdlog::error("FirmwareUpdate: Failed to erase ILC application for actuator {}", actuatorId);
        return false;
    }
    if (!this->WriteApplication(subnet, address)) {
        spdlog::error("FirmwareUpdate: Failed to write ILC application for actuator {}", actuatorId);
        return false;
    }
    if (!this->WriteApplicationStats(subnet, address, this->appStats)) {
        spdlog::error("FirmwareUpdate: Failed to write ILC application stats for actuator {}", actuatorId);
        return false;
    }
    if (!this->WriteVerifyApplication(subnet, address)) {
        // ILC seems to produce a CRC error
        spdlog::warn("FirmwareUpdate: Failed to verify ILC application for actuator {}", actuatorId);
    }
    if (!this->RestartApplication(subnet, address)) {
        // ILC doesn't seem to restart the application as intended
        spdlog::warn(
                "FirmwareUpdate: Failed to restart ILC application for actuator {} after loading firmware",
                actuatorId);
    }
    if (!this->EnterDisable(subnet, address)) {
        // ILC doesn't seem to restart the application as intended
        spdlog::warn("FirmwareUpdate: Failed to enter disabled state for actuator {}", actuatorId);
    }
    return true;
}

bool FirmwareUpdate::CreateAppData() {
    this->appData.clear();
    for (int i = 0; i < 16384; ++i) {
        appData.push_back(0xFF);
        appData.push_back(0xFF);
        appData.push_back(0xFF);
        appData.push_back(0x00);
    }
    return true;
}

bool FirmwareUpdate::ProcessFile(std::string filePath) {
    this->hexData.clear();
    std::ifstream inputStream(filePath.c_str());
    std::string lineText;
    bool allZero = true;
    bool ok = true;
    bool ignoreData = false;
    while (std::getline(inputStream, lineText)) {
        boost::trim_right(lineText);
        IntelHexLine hexLine;
        if (this->ProcessLine(lineText.c_str(), &hexLine)) {
            switch (hexLine.RecordType) {
                case IntelRecordType::Data:
                    if (!ignoreData) {
                        this->hexData.push_back(hexLine);
                    }
                    break;
                case IntelRecordType::ExtendedLinearAddress:
                    // Basically if the data is non-zero that means we are skipping a bunch of address which
                    // means we are at the end of the file and what we are skipping is a bunch of filler so
                    // lets not bother doing anything and just ignore all of the data
                    allZero = true;
                    for (unsigned int i = 0; i < hexLine.Data.size() && allZero; ++i) {
                        allZero = hexLine.Data[i] == 0;
                    }
                    ignoreData = !allZero;
                    break;
                default:
                    break;
            }
        } else {
            ok = false;
            break;
        }
    }
    inputStream.close();
    return ok;
}

bool FirmwareUpdate::ProcessLine(const char* line, IntelHexLine* hexLine) {
    bool ok = true;
    hexLine->StartCode = line[0];
    int offset = 1;
    if (hexLine->StartCode == ':') {
        unsigned int byteCount = 0;
        unsigned int address = 0;
        unsigned int recordType = 0;
        int returnCode = sscanf(line + offset, "%2X%4X%2X", &byteCount, &address, &recordType);
        hexLine->ByteCount = (char)byteCount;
        hexLine->Address = (unsigned short)address;
        hexLine->RecordType = (IntelRecordType::Types)recordType;
        offset += 8;
        if (returnCode >= 0) {
            for (int i = 0; i < hexLine->ByteCount; ++i) {
                unsigned int value = 0;
                returnCode = sscanf(line + offset, "%2X", &value);
                offset += 2;
                if (returnCode >= 0) {
                    hexLine->Data.push_back((char)value);
                } else {
                    spdlog::error("FirmwareUpdate: Unable to parse DataByte {} for Address {}", i,
                                  (int)hexLine->Address);
                    ok = false;
                    break;
                }
            }
            if (ok) {
                unsigned int expectedChecksum = 0;
                returnCode = sscanf(line + offset, "%2X", &expectedChecksum);
                hexLine->Checksum = expectedChecksum;
                if (returnCode >= 0) {
                    char checksum = 0;
                    checksum += hexLine->ByteCount;
                    checksum += (char)((hexLine->Address & 0xFF00) >> 8);
                    checksum += (char)(hexLine->Address & 0x00FF);
                    checksum += hexLine->RecordType;
                    for (unsigned int i = 0; i < hexLine->Data.size(); ++i) {
                        checksum += hexLine->Data[i];
                    }
                    checksum = ~checksum;
                    checksum += 1;
                    if (checksum != hexLine->Checksum) {
                        spdlog::error(
                                "FirmwareUpdate: Checksum mismatch for Address 0x{:04X}, expecting 0x{:02X} "
                                "got 0x{:02X}",
                                (int)hexLine->Address, (int)hexLine->Checksum, checksum);
                        ok = false;
                    }
                } else {
                    spdlog::error("FirmwareUpdate: Unable to parse Checksum for Address 0x{:04X}",
                                  (int)hexLine->Address);
                    ok = false;
                }
            }
        } else {
            spdlog::error("FirmwareUpdate: Unable to Parse ByteCount, Address, and RecordType for line.");
            ok = false;
        }
    } else {
        spdlog::error("FirmwareUpdate: Invalid IntelHexLine StartCode '{}' expecting '{}'",
                      (int)hexLine->StartCode, (int)':');
        ok = false;
    }
    return ok;
}

bool FirmwareUpdate::UpdateAppData() {
    bool ok = true;
    this->appStats.DataCRC = 0;
    this->appStats.StartAddress = 65535;
    this->appStats.DataLength = 0;
    this->appStats.StatsCRC = 0;
    unsigned short endAddress = 0;
    for (unsigned int i = 0; i < this->hexData.size(); ++i) {
        IntelHexLine line = this->hexData[i];
        switch (line.RecordType) {
            case IntelRecordType::Data:
                if (line.Address < this->appStats.StartAddress) {
                    this->appStats.StartAddress = line.Address;
                }
                if ((line.Address + line.ByteCount) > endAddress) {
                    endAddress = (line.Address + line.ByteCount);
                }
                for (unsigned int j = 0; j < line.Data.size(); ++j) {
                    unsigned int address = line.Address + j;
                    this->appData[address] = line.Data[j];
                }
                break;
            default:
                break;
        }
    }
    this->appStats.DataLength = endAddress - this->appStats.StartAddress;
    return ok;
}

bool FirmwareUpdate::UpdateAppDataCRC() {
    bool ok = true;
    unsigned char buffer[65536];
    for (unsigned int i = 0; i < 65536; ++i) {
        if (i < this->appData.size()) {
            buffer[i] = this->appData[i];
        }
    }
    this->appStats.DataCRC =
            CRC::modbus(buffer, (int)this->appStats.StartAddress, (int)this->appStats.DataLength);
    return ok;
}

bool FirmwareUpdate::UpdateAppStatCRC() {
    bool ok = true;
    unsigned char buffer[12];
    buffer[1] = (unsigned char)((this->appStats.DataCRC >> 8) & 0xFF);
    buffer[0] = (unsigned char)(this->appStats.DataCRC & 0xFF);
    buffer[2] = 0;
    buffer[3] = 0;
    buffer[5] = (unsigned char)((this->appStats.StartAddress >> 8) & 0xFF);
    buffer[4] = (unsigned char)(this->appStats.StartAddress & 0xFF);
    buffer[6] = 0;
    buffer[7] = 0;
    buffer[9] = (unsigned char)((this->appStats.DataLength >> 8) & 0xFF);
    buffer[8] = (unsigned char)(this->appStats.DataLength & 0xFF);
    buffer[10] = 0;
    buffer[11] = 0;
    this->appStats.StatsCRC = CRC::modbus(buffer, 0, 12);
    return ok;
}

bool FirmwareUpdate::EnterFirmwareUpdate(int subnet, int address) {
    this->desiredState = 4;
    ModbusBuffer buffer;
    this->SetupBuffer(&buffer, subnet);
    buffer.writeU8((uint8_t)address);
    buffer.writeU8(65);
    buffer.writeU8(0);
    buffer.writeU8(3);
    buffer.writeCRC(4);
    buffer.writeEndOfFrame();
    buffer.writeWaitForRx(100000);
    this->EndBuffer(&buffer);
    IFPGA::get().writeCommandFIFO(buffer.getBuffer(), buffer.getLength(), 0);
    return this->ProcessResponse(&buffer, subnet);
}

bool FirmwareUpdate::ClearFaults(int subnet, int address) {
    this->desiredState = 3;
    ModbusBuffer buffer;
    this->SetupBuffer(&buffer, subnet);
    buffer.writeU8((uint8_t)address);
    buffer.writeU8(65);
    buffer.writeU8(0);
    buffer.writeU8(5);
    buffer.writeCRC(4);
    buffer.writeEndOfFrame();
    buffer.writeWaitForRx(10000);
    this->EndBuffer(&buffer);
    IFPGA::get().writeCommandFIFO(buffer.getBuffer(), buffer.getLength(), 0);
    return this->ProcessResponse(&buffer, subnet);
}

bool FirmwareUpdate::EraseILCApplication(int subnet, int address) {
    ModbusBuffer buffer;
    this->SetupBuffer(&buffer, subnet);
    buffer.writeU8((uint8_t)address);
    buffer.writeU8(101);
    buffer.writeCRC(2);
    buffer.writeEndOfFrame();
    buffer.writeWaitForRx(500000);
    this->EndBuffer(&buffer);
    IFPGA::get().writeCommandFIFO(buffer.getBuffer(), buffer.getLength(), 0);
    return this->ProcessResponse(&buffer, subnet);
}

bool FirmwareUpdate::WriteApplication(int subnet, int address) {
    for (unsigned int i = this->appStats.StartAddress;
         i < (this->appStats.StartAddress + this->appStats.DataLength);) {
        unsigned int startAddress = i;
        uint8_t buffer[192];
        for (int j = 0; j < 64; ++j) {
            buffer[j * 3 + 0] = this->appData[i + 0];
            buffer[j * 3 + 1] = this->appData[i + 1];
            buffer[j * 3 + 2] = this->appData[i + 2];
            i += 4;
        }
        if (!this->WriteApplicationPage(subnet, address, startAddress, 192, buffer)) {
            spdlog::error("FirmwareUpdate: Failed to write application page address {}", startAddress);
            return false;
        }
    }
    return true;
}

bool FirmwareUpdate::WriteApplicationPage(int subnet, int address, uint16_t startAddress, uint16_t length,
                                          uint8_t* data) {
    ModbusBuffer buffer;
    this->SetupBuffer(&buffer, subnet);
    buffer.writeU8((uint8_t)address);
    buffer.writeU8(102);
    buffer.writeU16(startAddress);
    buffer.writeU16(length);
    for (int i = 0; i < length; ++i) {
        buffer.writeU8(data[i]);
    }
    buffer.writeCRC(6 + length);
    buffer.writeEndOfFrame();
    buffer.writeWaitForRx(500000);
    this->EndBuffer(&buffer);
    IFPGA::get().writeCommandFIFO(buffer.getBuffer(), buffer.getLength(), 0);
    return this->ProcessResponse(&buffer, subnet);
}

bool FirmwareUpdate::WriteApplicationStats(int subnet, int address, ILCApplicationStats stats) {
    ModbusBuffer buffer;
    this->SetupBuffer(&buffer, subnet);
    buffer.writeU8((uint8_t)address);
    buffer.writeU8(100);
    buffer.writeU16(stats.DataCRC);
    buffer.writeU16(stats.StartAddress);
    buffer.writeU16(stats.DataLength);
    buffer.writeU16(stats.StatsCRC);
    buffer.writeCRC(10);
    buffer.writeEndOfFrame();
    buffer.writeWaitForRx(500000);
    this->EndBuffer(&buffer);
    IFPGA::get().writeCommandFIFO(buffer.getBuffer(), buffer.getLength(), 0);
    return this->ProcessResponse(&buffer, subnet);
}

bool FirmwareUpdate::WriteVerifyApplication(int subnet, int address) {
    ModbusBuffer buffer;
    this->SetupBuffer(&buffer, subnet);
    buffer.writeU8((uint8_t)address);
    buffer.writeU8(103);
    buffer.writeCRC(2);
    buffer.writeEndOfFrame();
    buffer.writeWaitForRx(500000);
    this->EndBuffer(&buffer);
    IFPGA::get().writeCommandFIFO(buffer.getBuffer(), buffer.getLength(), 0);
    return this->ProcessResponse(&buffer, subnet);
}

bool FirmwareUpdate::RestartApplication(int subnet, int address) {
    this->desiredState = 0;
    ModbusBuffer buffer;
    this->SetupBuffer(&buffer, subnet);
    buffer.writeU8((uint8_t)address);
    buffer.writeU8(65);
    buffer.writeU8(0);
    buffer.writeU8(0);
    buffer.writeCRC(4);
    buffer.writeEndOfFrame();
    buffer.writeWaitForRx(500000);
    this->EndBuffer(&buffer);
    IFPGA::get().writeCommandFIFO(buffer.getBuffer(), buffer.getLength(), 0);
    return this->ProcessResponse(&buffer, subnet);
}

bool FirmwareUpdate::EnterDisable(int subnet, int address) {
    this->desiredState = 1;
    ModbusBuffer buffer;
    this->SetupBuffer(&buffer, subnet);
    buffer.writeU8((uint8_t)address);
    buffer.writeU8(65);
    buffer.writeU8(0);
    buffer.writeU8(1);
    buffer.writeCRC(4);
    buffer.writeEndOfFrame();
    buffer.writeWaitForRx(10000);
    this->EndBuffer(&buffer);
    IFPGA::get().writeCommandFIFO(buffer.getBuffer(), buffer.getLength(), 0);
    return this->ProcessResponse(&buffer, subnet);
}

bool FirmwareUpdate::ProcessResponse(ModbusBuffer* buffer, int subnet) {
    IFPGA::get().writeCommandFIFO(FPGAAddresses::ModbusSoftwareTrigger, 0);
    IFPGA::get().waitForModbusIRQ(subnet, 2000);
    IFPGA::get().ackModbusIRQ(subnet);
    buffer->setIndex(0);
    buffer->setLength(0);
    IFPGA::get().writeRequestFIFO(this->SubnetRxToFPGAAddress(subnet), 0);
    IFPGA::get().readU16ResponseFIFO(buffer->getBuffer(), 1, 2000);
    uint16_t reportedLength = buffer->readLength();
    if (reportedLength > 0) {
        buffer->setIndex(0);
        IFPGA::get().readU16ResponseFIFO(buffer->getBuffer(), reportedLength, 2000);
        buffer->setLength(reportedLength);
        // Read U64 Global Timestamp (UNUSED)
        buffer->readLength();
        buffer->readLength();
        buffer->readLength();
        buffer->readLength();
        while (!buffer->endOfBuffer()) {
            uint16_t length = 0;
            double timestamp = 0;
            if (this->ValidateCRC(buffer, &length, &timestamp)) {
                if (subnet >= 1 && subnet <= 5) {
                    buffer->readU8();  // Address
                    uint8_t function = buffer->readU8();
                    bool ok = true;
                    switch (function) {
                        case 65:
                            ok = this->ProcessStateChange(buffer);
                            break;
                        case 100:
                            ok = this->ProcessWriteApplicationStats(buffer);
                            break;
                        case 101:
                            ok = this->ProcessEraseILCApplication(buffer);
                            break;
                        case 102:
                            ok = this->ProcessWriteApplicationPage(buffer);
                            break;
                        case 103:
                            ok = this->ProcessVerifyUserApplication(buffer);
                            break;
                        case 193:
                        case 228:
                        case 229:
                        case 238:
                        case 231:
                            ok = this->ProcessExceptionCode(buffer, function);
                            break;
                        default:
                            spdlog::error("FirmwareUpdate: Unknown function {} on subnet {}", function,
                                          subnet);
                            return false;
                    }
                    if (!ok) {
                        return false;
                    }
                } else {
                    spdlog::warn("FirmwareUpdate: Unknown subnet {}", subnet);
                    return false;
                }
            } else {
                spdlog::warn("FirmwareUpdate: Invalid CRC {}", subnet);
                return false;
            }
        }
    } else {
        spdlog::error("FirmwareUpdate: No response");
        return false;
    }
    return true;
}

bool FirmwareUpdate::ValidateCRC(ModbusBuffer* buffer, uint16_t* length, double* timestamp) {
    int32_t addressIndex = buffer->getIndex();
    while (!buffer->endOfFrame() && !buffer->endOfBuffer() && (buffer->readLength() & 0xF000) != 0xB000) {
    }
    int32_t crcIndex = buffer->getIndex() - 3;
    (*length) = crcIndex - addressIndex + 11;
    buffer->setIndex(crcIndex);
    uint16_t calculatedCRC = buffer->calculateCRC(crcIndex - addressIndex);
    uint16_t crc = buffer->readCRC();
    (*timestamp) = buffer->readTimestamp();
    if (crc == calculatedCRC) {
        buffer->setIndex(addressIndex);
        return true;
    } else {
        buffer->readEndOfFrame();
        return false;
    }
}

bool FirmwareUpdate::ProcessStateChange(ModbusBuffer* buffer) {
    uint16_t state = buffer->readU16();
    buffer->skipToNextFrame();
    return (int)state == this->desiredState;
}

bool FirmwareUpdate::ProcessEraseILCApplication(ModbusBuffer* buffer) {
    buffer->skipToNextFrame();
    return true;
}

bool FirmwareUpdate::ProcessWriteApplicationPage(ModbusBuffer* buffer) {
    buffer->skipToNextFrame();
    return true;
}

bool FirmwareUpdate::ProcessWriteApplicationStats(ModbusBuffer* buffer) {
    buffer->skipToNextFrame();
    return true;
}

bool FirmwareUpdate::ProcessVerifyUserApplication(ModbusBuffer* buffer) {
    uint16_t status = buffer->readU16();
    switch (status) {
        case 0x0000:
            return true;
        case 0x00FF:
            spdlog::error("FirmwareUpdate: Application Stats Error");
            return false;
        case 0xFF00:
            spdlog::error("FirmwareUpdate: Application Error");
            return false;
        case 0xFFFF:
            spdlog::error("FirmwareUpdate: Application Stats and Application Error");
            return false;
        default:
            spdlog::error("FirmwareUpdate: Unknown Status");
            return false;
    }
}

bool FirmwareUpdate::ProcessExceptionCode(ModbusBuffer* buffer, int functionCode) {
    uint8_t exceptionCode = buffer->readU8();
    buffer->skipToNextFrame();
    spdlog::error("FirmwareUpdate: Function code {} got exception code {}", functionCode, exceptionCode);
    return false;
}

void FirmwareUpdate::SetupBuffer(ModbusBuffer* buffer, int subnet) {
    buffer->setIndex(0);
    buffer->setLength(0);
    buffer->writeSubnet((uint8_t)this->SubnetTxToFPGAAddress(subnet));
    buffer->writeLength(0);
    buffer->writeSoftwareTrigger();
}

int FirmwareUpdate::SubnetTxToFPGAAddress(int subnet) {
    switch (subnet) {
        case 1:
            return FPGAAddresses::ModbusSubnetATx;
        case 2:
            return FPGAAddresses::ModbusSubnetBTx;
        case 3:
            return FPGAAddresses::ModbusSubnetCTx;
        case 4:
            return FPGAAddresses::ModbusSubnetDTx;
        case 5:
            return FPGAAddresses::ModbusSubnetETx;
        default:
            return 255;
    }
}

int FirmwareUpdate::SubnetRxToFPGAAddress(int subnet) {
    switch (subnet) {
        case 1:
            return FPGAAddresses::ModbusSubnetARx;
        case 2:
            return FPGAAddresses::ModbusSubnetBRx;
        case 3:
            return FPGAAddresses::ModbusSubnetCRx;
        case 4:
            return FPGAAddresses::ModbusSubnetDRx;
        case 5:
            return FPGAAddresses::ModbusSubnetERx;
        default:
            return 255;
    }
}

void FirmwareUpdate::EndBuffer(ModbusBuffer* buffer) {
    buffer->writeTriggerIRQ();
    buffer->set(1, buffer->getIndex() - 2);
    buffer->setLength(buffer->getIndex());
}

bool FirmwareUpdate::PrintBuffer(ModbusBuffer* buffer, std::string text) {
    buffer->setIndex(3);
    for (int i = 3; i < buffer->getLength() - 3; ++i) {
        text = text + " " + boost::lexical_cast<std::string>((int)buffer->readU8());
    }
    spdlog::info("{}", text.c_str());
    return true;
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
