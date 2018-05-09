/*
 * FirmwareUpdate.cpp
 *
 *  Created on: May 8, 2018
 *      Author: ccontaxis
 */

#include <FirmwareUpdate.h>
#include <cstring>
#include <Log.h>
#include <fstream>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>
#include <CRC.h>
#include <FPGA.h>
#include <ModbusBuffer.h>
#include <DataTypes.h>
#include <FPGAAddresses.h>
#include <ILCDataTypes.h>
#include <ILCSubnetData.h>

namespace LSST {
namespace M1M3 {
namespace SS {

FirmwareUpdate::FirmwareUpdate(FPGA* fpga, ILCSubnetData* subnetData) {
	this->fpga = fpga;
	this->subnetData = subnetData;
	this->desiredState = 255;
}

bool FirmwareUpdate::Program(int actuatorId, std::string filePath) {
	ILCMap map = this->subnetData->getMap(actuatorId);
	int subnet = map.Subnet;
	int address = map.Address;
	if (subnet == 255 || address == 255) {
		Log.Error("FirmwareUpdate: Unknown actuator %d", actuatorId);
		return false;
	}
	if(!this->CreateAppData()) {
		Log.Error("FirmwareUpdate: Failed to create application data for actuator %d", actuatorId);
		return false;
	}
	if (!this->ProcessFile(filePath)) {
		Log.Error("FirmwareUpdate: Failed to process intel hex file for actuator %d", actuatorId);
		return false;
	}
	if (!this->UpdateAppData()) {
		Log.Error("FirmwareUpdate: Failed to load intel hex file into application data for actuator %d", actuatorId);
		return false;
	}
	if (!this->UpdateAppDataCRC()) {
		Log.Error("FirmwareUpdate: Failed to calculate application data CRC for actuator %d", actuatorId);
		return false;
	}
	if (!this->UpdateAppStatCRC()) {
		Log.Error("FirmwareUpdate: Failed to calculate application statistics CRC for actuator %d", actuatorId);
		return false;
	}
	if (!this->EnterFirmwareUpdate(subnet, address)) {
		Log.Error("FirmwareUpdate: Failed to start transition to firmware update state for actuator %d", actuatorId);
		return false;
	}
	if (!this->ClearFaults(subnet, address)) {
		Log.Error("FirmwareUpdate: Failed to clear faults to enter firmware update state for actuator %d", actuatorId);
		return false;
	}
	if (!this->EraseILCApplication(subnet, address)) {
		Log.Error("FirmwareUpdate: Failed to erase ILC application for actuator %d", actuatorId);
		return false;
	}
	if (!this->WriteApplication(subnet, address)) {
		Log.Error("FirmwareUpdate: Failed to write ILC application for actuator %d", actuatorId);
		return false;
	}
	if (!this->WriteApplicationStats(subnet, address, this->appStats)) {
		Log.Error("FirmwareUpdate: Failed to write ILC application stats for actuator %d", actuatorId);
		return false;
	}
	if (!this->WriteVerifyApplication(subnet, address)) {
		Log.Error("FirmwareUpdate: Failed to verify ILC application for actuator %d", actuatorId);
		return false;
	}
	if (!this->RestartApplication(subnet, address)) {
		Log.Error("FirmwareUpdate: Failed to restart ILC application for actuator %d", actuatorId);
		return false;
	}
	return true;
}

bool FirmwareUpdate::CreateAppData() {
	this->appData.clear();
	for(int i = 0; i < 16384; ++i) {
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
	bool ok = true;
	while(std::getline(inputStream, lineText)) {
		boost::trim_right(lineText);
		IntelHexLine hexLine;
		if (this->ProcessLine(lineText.c_str(), &hexLine)) {
			this->hexData.push_back(hexLine);
		}
		else {
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
			for(int i = 0; i < hexLine->ByteCount; ++i) {
				unsigned int value = 0;
				returnCode = sscanf(line + offset, "%2X", &value);
				offset += 2;
				if (returnCode >= 0) {
					hexLine->Data.push_back((char)value);
				}
				else {
					Log.Error("FirmwareUpdate: Unable to parse DataByte %d for Address %d", i, (int)hexLine->Address);
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
					checksum += hexLine->Address;
					checksum += hexLine->RecordType;
					for(unsigned int i = 0; i < hexLine->Data.size(); ++i) {
						checksum += hexLine->Data[i];
					}
					checksum = checksum & 0xFF;
					checksum = ~checksum;
					checksum += 1;
					if (checksum != hexLine->Checksum) {
						Log.Error("FirmwareUpdate: Checksum mismatch for Address %d, expecting %d got %d", (int)hexLine->Address, (int)hexLine->Checksum, checksum);
						ok = false;
					}
				}
				else {
					Log.Error("FirmwareUpdate: Unable to parse Checksum for Address %d", (int)hexLine->Address);
					ok = false;
				}
			}
		}
		else {
			Log.Error("FirmwareUpdate: Unable to Parse ByteCount, Address, and RecordType for line.");
			ok = false;
		}
	}
	else {
		Log.Error("FirmwareUpdate: Invalid IntelHexLine StartCode '%d' expecting '%d'", (int)hexLine->StartCode, (int)':');
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
	for(unsigned int i = 0; i < this->hexData.size(); ++i) {
		IntelHexLine line = this->hexData[i];
		if (line.Address < this->appStats.StartAddress) {
			this->appStats.StartAddress = line.Address;
		}
		if (line.Address > endAddress) {
			endAddress = line.Address;
		}
		for(unsigned int j = 0; j < line.Data.size(); ++j) {
			unsigned int address = line.Address + j;
			this->appData[address] = line.Data[j];
		}
	}
	this->appStats.DataLength = endAddress - this->appStats.StartAddress + 1;
	return ok;
}

bool FirmwareUpdate::UpdateAppDataCRC() {
	bool ok = true;
	unsigned char buffer[65536];
	for(unsigned int i = 0; i < 65536; ++i) {
		if (i < this->appData.size()) {
			buffer[i] = this->appData[i];
		}
	}
	this->appStats.DataCRC = CRC::modbus(buffer, (int)this->appStats.StartAddress, (int)this->appStats.DataLength);
	return ok;
}

bool FirmwareUpdate::UpdateAppStatCRC() {
	bool ok = true;
	unsigned char buffer[6];
	buffer[0] = (unsigned char)((this->appStats.DataCRC >> 8) & 0xFF);
	buffer[1] = (unsigned char)(this->appStats.DataCRC & 0xFF);
	buffer[2] = (unsigned char)((this->appStats.StartAddress >> 8) & 0xFF);
	buffer[3] = (unsigned char)(this->appStats.StartAddress & 0xFF);
	buffer[4] = (unsigned char)((this->appStats.DataLength >> 8) & 0xFF);
	buffer[5] = (unsigned char)(this->appStats.DataLength & 0xFF);
	this->appStats.StatsCRC = CRC::modbus(buffer, 0, 6);
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
	buffer.writeWaitForRx(10000);
	this->EndBuffer(&buffer);
	return this->PrintBuffer(&buffer, "EnterFirmware:");
	this->fpga->writeCommandFIFO(buffer.getBuffer(), buffer.getLength(), 0);
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
	return this->PrintBuffer(&buffer, "ClearFaults:");
	this->fpga->writeCommandFIFO(buffer.getBuffer(), buffer.getLength(), 0);
	return this->ProcessResponse(&buffer, subnet);
}

bool FirmwareUpdate::EraseILCApplication(int subnet, int address) {
	ModbusBuffer buffer;
	this->SetupBuffer(&buffer, subnet);
	buffer.writeU8((uint8_t)address);
	buffer.writeU8(101);
	buffer.writeCRC(2);
	buffer.writeEndOfFrame();
	buffer.writeWaitForRx(10000);
	this->EndBuffer(&buffer);
	return this->PrintBuffer(&buffer, "EraseILCApplication:");
	this->fpga->writeCommandFIFO(buffer.getBuffer(), buffer.getLength(), 0);
	return this->ProcessResponse(&buffer, subnet);
}

bool FirmwareUpdate::WriteApplication(int subnet, int address) {
	for(unsigned int i = 0; i < this->appStats.DataLength;) {
		uint8_t buffer[192];
		for(int j = 0; j < 64; ++j) {
			buffer[j * 3 + 0] = this->appData[i + 0];
			buffer[j * 3 + 1] = this->appData[i + 1];
			buffer[j * 3 + 2] = this->appData[i + 2];
			i += 4;
		}
		uint16_t startAddress = this->appStats.StartAddress + i;
		if (!this->WriteApplicationPage(subnet, address, startAddress, 192, buffer)) {
			Log.Error("FirmwareUpdate: Failed to write application page address %d", startAddress);
			return false;
		}
	}
	return true;
}

bool FirmwareUpdate::WriteApplicationPage(int subnet, int address, uint16_t startAddress, uint16_t length, uint8_t* data) {
	ModbusBuffer buffer;
	this->SetupBuffer(&buffer, subnet);
	buffer.writeU8((uint8_t)address);
	buffer.writeU8(102);
	buffer.writeU16(startAddress);
	buffer.writeU16(length);
	for(int i = 0; i < length; ++i) {
		buffer.writeU8(data[i]);
	}
	buffer.writeCRC(6 + length);
	buffer.writeEndOfFrame();
	buffer.writeWaitForRx(10000);
	this->EndBuffer(&buffer);
	return this->PrintBuffer(&buffer, "WriteApplicationPage:");
	this->fpga->writeCommandFIFO(buffer.getBuffer(), buffer.getLength(), 0);
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
	buffer.writeWaitForRx(10000);
	this->EndBuffer(&buffer);
	return this->PrintBuffer(&buffer, "WriteApplicationStats:");
	this->fpga->writeCommandFIFO(buffer.getBuffer(), buffer.getLength(), 0);
	return this->ProcessResponse(&buffer, subnet);
}

bool FirmwareUpdate::WriteVerifyApplication(int subnet, int address) {
	ModbusBuffer buffer;
	this->SetupBuffer(&buffer, subnet);
	buffer.writeU8((uint8_t)address);
	buffer.writeU8(103);
	buffer.writeCRC(2);
	buffer.writeEndOfFrame();
	buffer.writeWaitForRx(10000);
	this->EndBuffer(&buffer);
	return this->PrintBuffer(&buffer, "WriteVerifyApplication:");
	this->fpga->writeCommandFIFO(buffer.getBuffer(), buffer.getLength(), 0);
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
	buffer.writeWaitForRx(10000);
	this->EndBuffer(&buffer);
	return this->PrintBuffer(&buffer, "RestartApplication:");
	this->fpga->writeCommandFIFO(buffer.getBuffer(), buffer.getLength(), 0);
	return this->ProcessResponse(&buffer, subnet);
}

bool FirmwareUpdate::ProcessResponse(ModbusBuffer* buffer, int subnet) {
	this->fpga->waitForModbusIRQ(subnet, 200);
	this->fpga->ackModbusIRQ(subnet);
	buffer->setIndex(0);
	buffer->setLength(0);
	this->fpga->writeRequestFIFO(this->SubnetToFPGAAddress(subnet), 200);
	this->fpga->readU16ResponseFIFO(buffer->getBuffer(), 1, 10);
	uint16_t reportedLength = buffer->readLength();
	if (reportedLength > 0) {
		buffer->setIndex(0);
		if (this->fpga->readU16ResponseFIFO(buffer->getBuffer(), reportedLength, 10)) {
			Log.Error("FirmwareUpdate: Failed to read all %d words", reportedLength);
			return false;
		}
		buffer->setLength(reportedLength);
		while(!buffer->endOfBuffer()) {
			uint16_t length = 0;
			double timestamp = 0;
			if (this->ValidateCRC(buffer, &length, &timestamp)) {
				if(subnet >= 1 && subnet <= 5) {
					buffer->readU8(); // Address
					uint8_t function = buffer->readU8();
					bool ok = true;
					switch(function) {
					case 65: ok = this->ProcessStateChange(buffer); break;
					case 100: ok = this->ProcessWriteApplicationStats(buffer); break;
					case 101: ok = this->ProcessEraseILCApplication(buffer); break;
					case 102: ok = this->ProcessWriteApplicationPage(buffer); break;
					case 103: ok = this->ProcessVerifyUserApplication(buffer); break;
					case 193:
					case 228:
					case 229:
					case 238:
					case 231: ok = this->ProcessExceptionCode(buffer, function); break;
					default: Log.Error("FirmwareUpdate: Unknown function %d on subnet %d", function, subnet); return false;
					}
					if (!ok) {
						return false;
					}
				}
				else {
					Log.Warn("FirmwareUpdate: Unknown subnet %d", subnet);
					return false;
				}
			}
			else {
				Log.Warn("FirmwareUpdate: Invalid CRC %d", subnet);
				return false;
			}
		}
	}
	else {
		Log.Error("FirmwareUpdate: Erase ILC Application had no response");
		return false;
	}
	return true;
}

bool FirmwareUpdate::ValidateCRC(ModbusBuffer* buffer, uint16_t* length, double* timestamp) {
	int32_t addressIndex = buffer->getIndex();
	while(!buffer->endOfFrame() && !buffer->endOfBuffer() && (buffer->readLength() & 0xF000) != 0xB000) { }
	int32_t crcIndex = buffer->getIndex() - 3;
	(*length) = crcIndex - addressIndex + 11;
	buffer->setIndex(crcIndex);
	uint16_t calculatedCRC = buffer->calculateCRC(crcIndex - addressIndex);
	uint16_t crc = buffer->readCRC();
	(*timestamp) = buffer->readTimestamp();
	if (crc == calculatedCRC) {
		buffer->setIndex(addressIndex);
		return true;
	}
	else {
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
	switch(status) {
	case 0x0000: return true;
	case 0x00FF: Log.Error("FirmwareUpdate: Application Stats Error"); return false;
	case 0xFF00: Log.Error("FirmwareUpdate: Application Error"); return false;
	case 0xFFFF: Log.Error("FirmwareUpdate: Application Stats and Application Error"); return false;
	default: Log.Error("FirmwareUpdate: Unknown Status"); return false;
	}
}

bool FirmwareUpdate::ProcessExceptionCode(ModbusBuffer* buffer, int functionCode) {
	uint8_t exceptionCode = buffer->readU8();
	buffer->skipToNextFrame();
	Log.Error("FirmwareUpdate: Function code %d got exception code %d", functionCode, exceptionCode);
	return false;
}

void FirmwareUpdate::SetupBuffer(ModbusBuffer* buffer, int subnet) {
	buffer->writeSubnet((uint8_t)this->SubnetToFPGAAddress(subnet));
	buffer->writeLength(0);
}

int FirmwareUpdate::SubnetToFPGAAddress(int subnet) {
	switch(subnet) {
	case 1: subnet = FPGAAddresses::ModbusSubnetATx; break;
	case 2: subnet = FPGAAddresses::ModbusSubnetBTx; break;
	case 3: subnet = FPGAAddresses::ModbusSubnetCTx; break;
	case 4: subnet = FPGAAddresses::ModbusSubnetDTx; break;
	case 5: subnet = FPGAAddresses::ModbusSubnetETx; break;
	default: subnet = 255; break;
	}
	return subnet;
}

void FirmwareUpdate::EndBuffer(ModbusBuffer* buffer) {
	buffer->writeTriggerIRQ();
	buffer->set(1, buffer->getIndex() - 2);
}

bool FirmwareUpdate::PrintBuffer(ModbusBuffer* buffer, std::string text) {
	buffer->setIndex(0);
	for(int i = 0; i < buffer->getLength(); ++i) {
		text = text + " " + boost::lexical_cast<std::string>(buffer->readU8());
	}
	Log.Info(text.c_str());
	return true;
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

