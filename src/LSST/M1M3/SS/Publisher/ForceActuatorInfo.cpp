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

void ForceActuatorInfo::populate(ForceActuatorApplicationSettings *forceActuatorApplicationSettings) {
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

void ForceActuatorInfo::parseServerIDResponse(ModbusBuffer *buffer, int32_t dataIndex) {
    uint8_t length = buffer->readU8();

    ilcUniqueId[dataIndex] = buffer->readU48();

    ilcApplicationType[dataIndex] = buffer->readU8();
    networkNodeType[dataIndex] = buffer->readU8();

    ilcSelectedOptions[dataIndex] = buffer->readU8();
    networkNodeOptions[dataIndex] = buffer->readU8();
    majorRevision[dataIndex] = buffer->readU8();
    minorRevision[dataIndex] = buffer->readU8();

    buffer->incIndex(length - 12);
    buffer->skipToNextFrame();
}

void ForceActuatorInfo::parseBoosterValveDCAGains(ModbusBuffer *buffer, int32_t dataIndex) {
    mezzaninePrimaryCylinderGain[dataIndex] = buffer->readSGL();
    mezzanineSecondaryCylinderGain[dataIndex] = buffer->readSGL();

    buffer->skipToNextFrame();
}

void ForceActuatorInfo::setFACalibration(ModbusBuffer *buffer, int32_t dataIndex) {
    adcScanRate[dataIndex] = buffer->readU8();

    buffer->skipToNextFrame();
}

void ForceActuatorInfo::parseFAADCScanRate(ModbusBuffer *buffer, int32_t dataIndex) {
    mainPrimaryCylinderCoefficient[dataIndex] = buffer->readSGL();
    mainSecondaryCylinderCoefficient[dataIndex] = buffer->readSGL();
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
    backupSecondaryCylinderCoefficient[dataIndex] = buffer->readSGL();
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

    buffer->skipToNextFrame();
}

void ForceActuatorInfo::parseSetDCAID(ModbusBuffer *buffer, int32_t dataIndex) {
    mezzanineUniqueId[dataIndex] = buffer->readU48();
    mezzanineFirmwareType[dataIndex] = buffer->readU8();
    mezzanineMajorRevision[dataIndex] = buffer->readU8();
    mezzanineMinorRevision[dataIndex] = buffer->readU8();

    buffer->skipToNextFrame();
}
