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

#ifndef FIRMWAREUPDATE_H_
#define FIRMWAREUPDATE_H_

#include <string>
#include <vector>
#include <DataTypes.h>

namespace LSST {
namespace M1M3 {
namespace SS {

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
public:
    FirmwareUpdate(ILCSubnetData* subnetData);

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

private:
    std::vector<unsigned char> _appData;
    std::vector<IntelHexLine> _hexData;
    ILCApplicationStats _appStats;
    ILCSubnetData* _subnetData;
    int _desiredState;
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* FIRMWAREUPDATE_H_ */
