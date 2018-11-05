/*
 * FirmwareUpdate.h
 *
 *  Created on: May 8, 2018
 *      Author: ccontaxis
 */

#ifndef FIRMWAREUPDATE_H_
#define FIRMWAREUPDATE_H_

#include <string>
#include <vector>
#include <DataTypes.h>

namespace LSST {
namespace M1M3 {
namespace SS {

class IFPGA;
class ModbusBuffer;
class ILCSubnetData;

struct IntelRecordType {
	enum Types {
		Data = 0,
		EndOfFile = 1,
		ExtendedSegmentAddress = 2,
		StartSegmentAddress = 3,
		ExtendedLinearAddress = 4,
		StartLinearAddress = 5
	};
};

struct IntelHexLine {
	char StartCode;
	char ByteCount;
	unsigned short Address;
	IntelRecordType::Types RecordType;
	std::vector<char> Data;
	char Checksum;
};

struct ILCApplicationStats {
	unsigned short DataCRC;
	unsigned short StartAddress;
	unsigned short DataLength;
	unsigned short StatsCRC;
};

class FirmwareUpdate {
private:
	std::vector<char> appData;
	std::vector<IntelHexLine> hexData;
	ILCApplicationStats appStats;
	IFPGA* fpga;
	ILCSubnetData* subnetData;
	int desiredState;

public:
	FirmwareUpdate(IFPGA* fpga, ILCSubnetData* subnetData);

	bool Program(int actuatorId, std::string filePath);

	bool CreateAppData();
	bool ProcessFile(std::string filePath);
	bool ProcessLine(const char* line, IntelHexLine* hexLine);
	bool UpdateAppData();
	bool UpdateAppDataCRC();
	bool UpdateAppStatCRC();
	bool EnterFirmwareUpdate(int subnet, int address);
	bool ClearFaults(int subnet, int address);
	bool EraseILCApplication(int subnet, int address);
	bool WriteApplication(int subnet, int address);
	bool WriteApplicationPage(int subnet, int address, uint16_t startAddress, uint16_t length, uint8_t* data);
	bool WriteApplicationStats(int subnet, int address, ILCApplicationStats stats);
	bool WriteVerifyApplication(int subnet, int address);
	bool RestartApplication(int subnet, int address);
	bool EnterDisable(int subnet, int address);

	bool ProcessResponse(ModbusBuffer* buffer, int subnet);
	bool ValidateCRC(ModbusBuffer* buffer, uint16_t* length, double* timestamp);
	bool ProcessStateChange(ModbusBuffer* buffer);
	bool ProcessEraseILCApplication(ModbusBuffer* buffer);
	bool ProcessWriteApplicationPage(ModbusBuffer* buffer);
	bool ProcessWriteApplicationStats(ModbusBuffer* buffer);
	bool ProcessVerifyUserApplication(ModbusBuffer* buffer);
	bool ProcessExceptionCode(ModbusBuffer* buffer, int functionCode);

	void SetupBuffer(ModbusBuffer* buffer, int subnet);
	int SubnetTxToFPGAAddress(int subnet);
	int SubnetRxToFPGAAddress(int subnet);
	void EndBuffer(ModbusBuffer* buffer);

	bool PrintBuffer(ModbusBuffer* buffer, std::string text);
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* FIRMWAREUPDATE_H_ */
