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

#include <spdlog/spdlog.h>

#include <ForceActuatorInfo.h>

using namespace LSST::M1M3::SS;

ForceActuatorInfo::ForceActuatorInfo(token) {}

void ForceActuatorInfo::populate(ForceActuatorApplicationSettings* forceActuatorApplicationSettings) {
    SPDLOG_DEBUG("Populating ForceActuatorInfo");
    for (int i = 0; i < FA_COUNT; i++) {
        ForceActuatorTableRow row = forceActuatorApplicationSettings->Table[i];
        referenceId[i] = row.ActuatorID;
        auto xIndex = forceActuatorApplicationSettings->ZIndexToXIndex[i];
        if (xIndex >= 0) {
            xDataReferenceId[xIndex] = row.ActuatorID;
        }
        auto yIndex = forceActuatorApplicationSettings->ZIndexToYIndex[i];
        if (yIndex >= 0) {
            yDataReferenceId[yIndex] = row.ActuatorID;
        }
        zDataReferenceId[i] = row.ActuatorID;
        modbusSubnet[i] = row.Subnet;
        modbusAddress[i] = row.Address;
        actuatorType[i] = row.Type;
        actuatorOrientation[i] = row.Orientation;
        xPosition[i] = row.XPosition;
        yPosition[i] = row.YPosition;
        zPosition[i] = row.ZPosition;
    }
}

void ForceActuatorInfo::serverIDResponse(int32_t dataIndex, uint64_t _ilcUniqueId,
                                         uint8_t _ilcApplicationType, uint8_t _networkNodeType,
                                         uint8_t _ilcSelectedOptions, uint8_t _networkNodeOptions,
                                         uint8_t _majorRevision, uint8_t _minorRevision) {
    ilcUniqueId[dataIndex] = _ilcUniqueId;
    ilcApplicationType[dataIndex] = _ilcApplicationType;
    networkNodeType[dataIndex] = _networkNodeType;
    ilcSelectedOptions[dataIndex] = _ilcSelectedOptions;
    networkNodeOptions[dataIndex] = _networkNodeOptions;
    majorRevision[dataIndex] = _majorRevision;
    minorRevision[dataIndex] = _minorRevision;
}

void ForceActuatorInfo::boosterValveDCAGains(int32_t dataIndex, float _mezzaninePrimaryCylinderGain,
                                             float _mezzanineSecondaryCylinderGain) {
    mezzaninePrimaryCylinderGain[dataIndex] = _mezzaninePrimaryCylinderGain;
    mezzanineSecondaryCylinderGain[dataIndex] = _mezzanineSecondaryCylinderGain;
}

void ForceActuatorInfo::setFACalibration(int32_t dataIndex, uint8_t _adcScanRate) {
    adcScanRate[dataIndex] = _adcScanRate;
}

void ForceActuatorInfo::parseFAADCScanRate(int32_t dataIndex, ModbusBuffer* buffer) {
    mainPrimaryCylinderCoefficient[dataIndex] = buffer->readSGL();
    mainSecondaryCylinderCoefficient[dataIndex] = mainPrimaryCylinderCoefficient[dataIndex];
    buffer->readSGL();  // Main Coefficient K2
    buffer->readSGL();  // Main Coefficient K3
    buffer->readSGL();  // Main Coefficient K4
    mainPrimaryCylinderLoadCellOffset[dataIndex] = buffer->readSGL();
    mainSecondaryCylinderLoadCellOffset[dataIndex] = buffer->readSGL();
    buffer->readSGL();  // Main Offset Channel 3
    buffer->readSGL();  // Main Offset Channel 4
    mainPrimaryCylinderLoadCellSensitivity[dataIndex] = buffer->readSGL();
    mainSecondaryCylinderLoadCellSensitivity[dataIndex] = buffer->readSGL();
    buffer->readSGL();  // Main Sensitivity Channel 3
    buffer->readSGL();  // Main Sensitivity Channel 4
    backupPrimaryCylinderCoefficient[dataIndex] = buffer->readSGL();
    backupSecondaryCylinderCoefficient[dataIndex] = backupPrimaryCylinderCoefficient[dataIndex];
    buffer->readSGL();  // Backup Coefficient K2
    buffer->readSGL();  // Backup Coefficient K3
    buffer->readSGL();  // Backup Coefficient K4
    backupPrimaryCylinderLoadCellOffset[dataIndex] = buffer->readSGL();
    backupSecondaryCylinderLoadCellOffset[dataIndex] = buffer->readSGL();
    buffer->readSGL();  // Backup Offset Channel 3
    buffer->readSGL();  // Backup Offset Channel 4
    backupPrimaryCylinderLoadCellSensitivity[dataIndex] = buffer->readSGL();
    backupSecondaryCylinderLoadCellSensitivity[dataIndex] = buffer->readSGL();
    buffer->readSGL();  // Backup Sensitivity Channel 3
    buffer->readSGL();  // Backup Sensitivity Channel 4
}

void ForceActuatorInfo::setDCAID(int32_t dataIndex, uint32_t _mezzanineUniqueId,
                                 uint8_t _mezzanineFirmwareType, uint8_t _mezzanineMajorRevision,
                                 uint8_t _mezzanineMinorRevision) {
    mezzanineUniqueId[dataIndex] = _mezzanineUniqueId;
    mezzanineFirmwareType[dataIndex] = _mezzanineFirmwareType;
    mezzanineMajorRevision[dataIndex] = _mezzanineMajorRevision;
    mezzanineMinorRevision[dataIndex] = _mezzanineMinorRevision;
}
