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

#include <ForceActuatorApplicationSettings.h>
#include <ForceActuatorInfo.h>

using namespace LSST::M1M3::SS;

ForceActuatorInfo::ForceActuatorInfo(token) {
    SPDLOG_DEBUG("Populating ForceActuatorInfo");

    auto &faa_settings = ForceActuatorApplicationSettings::instance();

    for (int i = 0; i < FA_COUNT; i++) {
        ForceActuatorTableRow row = faa_settings.Table[i];
        referenceId[i] = row.ActuatorID;
        auto xIndex = faa_settings.ZIndexToXIndex[i];
        if (xIndex >= 0) {
            xDataReferenceId[xIndex] = row.ActuatorID;
        }
        auto yIndex = faa_settings.ZIndexToYIndex[i];
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

void ForceActuatorInfo::parseServerIDResponse(ModbusBuffer *buffer, const ILCMap &ilc) {
    uint8_t length = buffer->readU8();

    ilcUniqueId[ilc.DataIndex] = buffer->readU48();

    ilcApplicationType[ilc.DataIndex] = buffer->readU8();
    networkNodeType[ilc.DataIndex] = buffer->readU8();

    ilcSelectedOptions[ilc.DataIndex] = buffer->readU8();
    networkNodeOptions[ilc.DataIndex] = buffer->readU8();
    majorRevision[ilc.DataIndex] = buffer->readU8();
    minorRevision[ilc.DataIndex] = buffer->readU8();

    buffer->incIndex(length - 12);
    buffer->skipToNextFrame();
}

void ForceActuatorInfo::parseBoosterValveDCAGains(ModbusBuffer *buffer, const ILCMap &ilc) {
    mezzaninePrimaryCylinderGain[ilc.DataIndex] = buffer->readSGL();
    mezzanineSecondaryCylinderGain[ilc.DataIndex] = buffer->readSGL();

    buffer->skipToNextFrame();
}

void ForceActuatorInfo::parseFAADCScanRate(ModbusBuffer *buffer, const ILCMap &ilc) {
    adcScanRate[ilc.DataIndex] = buffer->readU8();

    buffer->skipToNextFrame();
}

void ForceActuatorInfo::parseFACalibration(ModbusBuffer *buffer, const ILCMap &ilc) {
    mainPrimaryCylinderCoefficient[ilc.DataIndex] = buffer->readSGL();
    mainSecondaryCylinderCoefficient[ilc.DataIndex] = buffer->readSGL();
    buffer->readSGL();  // Main Coefficient K3
    buffer->readSGL();  // Main Coefficient K4
    mainPrimaryCylinderLoadCellOffset[ilc.DataIndex] = buffer->readSGL();
    mainSecondaryCylinderLoadCellOffset[ilc.DataIndex] = buffer->readSGL();
    buffer->readSGL();  // Main Offset Channel 3
    buffer->readSGL();  // Main Offset Channel 4
    mainPrimaryCylinderLoadCellSensitivity[ilc.DataIndex] = buffer->readSGL();
    mainSecondaryCylinderLoadCellSensitivity[ilc.DataIndex] = buffer->readSGL();
    buffer->readSGL();  // Main Sensitivity Channel 3
    buffer->readSGL();  // Main Sensitivity Channel 4
    backupPrimaryCylinderCoefficient[ilc.DataIndex] = buffer->readSGL();
    backupSecondaryCylinderCoefficient[ilc.DataIndex] = buffer->readSGL();
    buffer->readSGL();  // Backup Coefficient K3
    buffer->readSGL();  // Backup Coefficient K4
    backupPrimaryCylinderLoadCellOffset[ilc.DataIndex] = buffer->readSGL();
    backupSecondaryCylinderLoadCellOffset[ilc.DataIndex] = buffer->readSGL();
    buffer->readSGL();  // Backup Offset Channel 3
    buffer->readSGL();  // Backup Offset Channel 4
    backupPrimaryCylinderLoadCellSensitivity[ilc.DataIndex] = buffer->readSGL();
    backupSecondaryCylinderLoadCellSensitivity[ilc.DataIndex] = buffer->readSGL();
    buffer->readSGL();  // Backup Sensitivity Channel 3
    buffer->readSGL();  // Backup Sensitivity Channel 4

    buffer->skipToNextFrame();
}

void ForceActuatorInfo::parseSetDCAID(ModbusBuffer *buffer, const ILCMap &ilc) {
    mezzanineUniqueId[ilc.DataIndex] = buffer->readU48();
    mezzanineFirmwareType[ilc.DataIndex] = buffer->readU8();
    mezzanineMajorRevision[ilc.DataIndex] = buffer->readU8();
    mezzanineMinorRevision[ilc.DataIndex] = buffer->readU8();

    buffer->skipToNextFrame();
}
