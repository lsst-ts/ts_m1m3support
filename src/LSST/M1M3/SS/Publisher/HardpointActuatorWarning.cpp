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

#include "HardpointActuatorWarning.h"

using namespace LSST::M1M3::SS;

HardpointActuatorWarning::HardpointActuatorWarning(token) {
    timestamp = NAN;
    _updated = false;

    for (int i = 0; i < HP_COUNT; i++) {
        ilcFault[i] = false;

        broadcastCounterWarning[i] = false;

        majorFault[i] = false;
        minorFault[i] = false;
        faultOverride[i] = false;
        mainCalibrationError[i] = false;
        backupCalibrationError[i] = false;
        limitSwitch1Operated[i] = false;
        limitSwitch2Operated[i] = false;

        uniqueIdCRCError[i] = false;
        applicationTypeMismatch[i] = false;
        applicationMissing[i] = false;
        applicationCRCMismatch[i] = false;
        oneWireMissing[i] = false;
        oneWire1Mismatch[i] = false;
        oneWire2Mismatch[i] = false;
        watchdogReset[i] = false;
        brownOut[i] = false;
        eventTrapReset[i] = false;
        motorDriverFault[i] = false;
        ssrPowerFault[i] = false;
        auxPowerFault[i] = false;
        smcPowerFault[i] = false;

        lowProximityWarning[i] = false;
        highProximityWarning[i] = false;

        _ilcOldStatus[i] = 0xFFFF;
        _ilcOldFaults[i] = 0xFFFF;

        lowAirPressureFault[i] = false;
        highAirPressureFault[i] = false;
    }
}

void HardpointActuatorWarning::send() {
    if (_updated == false) {
        return;
    }

    anyMajorFault = false;
    anyMinorFault = false;
    anyFaultOverride = false;
    anyMainCalibrationError = false;
    anyBackupCalibrationError = false;
    anyLimitSwitch1Operated = false;
    anyLimitSwitch2Operated = false;
    anyUniqueIdCRCError = false;
    anyApplicationTypeMismatch = false;
    anyApplicationMissing = false;
    anyApplicationCRCMismatch = false;
    anyOneWireMissing = false;
    anyOneWire1Mismatch = false;
    anyOneWire2Mismatch = false;
    anyWatchdogReset = false;
    anyBrownOut = false;
    anyEventTrapReset = false;
    anyMotorDriverFault = false;
    anySSRPowerFault = false;
    anyAuxPowerFault = false;
    anySMCPowerFault = false;
    anyILCFault = false;
    anyBroadcastCounterWarning = false;
    anyLowProximityWarning = false;
    anyHighProximityWarning = false;
    anyLowAirPressureFault = false;
    anyHighAirPressureFault = false;

    for (int i = 0; i < HP_COUNT; ++i) {
        anyMajorFault = anyMajorFault || majorFault[i];
        anyMinorFault = anyMinorFault || minorFault[i];
        anyFaultOverride = anyFaultOverride || faultOverride[i];
        anyMainCalibrationError = anyMainCalibrationError || mainCalibrationError[i];
        anyBackupCalibrationError = anyBackupCalibrationError || backupCalibrationError[i];
        anyLimitSwitch1Operated = anyLimitSwitch1Operated || limitSwitch1Operated[i];
        anyLimitSwitch2Operated = anyLimitSwitch2Operated || limitSwitch2Operated[i];
        anyUniqueIdCRCError = anyUniqueIdCRCError || uniqueIdCRCError[i];
        anyApplicationTypeMismatch = anyApplicationTypeMismatch || applicationTypeMismatch[i];
        anyApplicationMissing = anyApplicationMissing || applicationMissing[i];
        anyApplicationCRCMismatch = anyApplicationCRCMismatch || applicationCRCMismatch[i];
        anyOneWireMissing = anyOneWireMissing || oneWireMissing[i];
        anyOneWire1Mismatch = anyOneWire1Mismatch || oneWire1Mismatch[i];
        anyOneWire2Mismatch = anyOneWire2Mismatch || oneWire2Mismatch[i];
        anyWatchdogReset = anyWatchdogReset || watchdogReset[i];
        anyBrownOut = anyBrownOut || brownOut[i];
        anyEventTrapReset = anyEventTrapReset || eventTrapReset[i];
        anyMotorDriverFault = anyMotorDriverFault || motorDriverFault[i];
        anySSRPowerFault = anySSRPowerFault || ssrPowerFault[i];
        anyAuxPowerFault = anyAuxPowerFault || auxPowerFault[i];
        anySMCPowerFault = anySMCPowerFault || smcPowerFault[i];
        anyILCFault = anyILCFault || ilcFault[i];
        anyBroadcastCounterWarning = anyBroadcastCounterWarning || broadcastCounterWarning[i];
        anyLowProximityWarning = anyLowProximityWarning || lowProximityWarning[i];
        anyHighProximityWarning = anyHighProximityWarning || highProximityWarning[i];
        anyLowAirPressureFault = anyLowAirPressureFault || lowAirPressureFault[i];
        anyHighAirPressureFault = anyHighAirPressureFault || highAirPressureFault[i];
    }
    anyWarning = anyMajorFault || anyMinorFault || anyFaultOverride || anyMainCalibrationError ||
                 anyBackupCalibrationError || anyLimitSwitch1Operated || anyLimitSwitch2Operated ||
                 anyUniqueIdCRCError || anyApplicationTypeMismatch || anyApplicationMissing ||
                 anyApplicationCRCMismatch || anyOneWireMissing || anyOneWire1Mismatch ||
                 anyOneWire2Mismatch || anyWatchdogReset || anyBrownOut || anyEventTrapReset ||
                 anyMotorDriverFault || anySSRPowerFault || anyAuxPowerFault || anySMCPowerFault ||
                 anyILCFault || anyBroadcastCounterWarning || anyLowProximityWarning ||
                 anyHighProximityWarning || anyLowAirPressureFault || anyHighAirPressureFault;

    M1M3SSPublisher::instance().logHardpointActuatorWarning(this);

    _updated = false;
}

void HardpointActuatorWarning::setStatus(int32_t hpIndex, double _timestamp, uint8_t status,
                                         int broadcastCounter) {
    timestamp = timestamp;
    ilcFault[hpIndex] = (status & 0x01) != 0;
    // 0x02 is reserved
    limitSwitch1Operated[hpIndex] = (status & 0x04) != 0;
    limitSwitch2Operated[hpIndex] = (status & 0x08) != 0;
    broadcastCounterWarning[hpIndex] = broadcastCounter != ((status & 0xF0) >> 4);
}

void HardpointActuatorWarning::parseIlcStatus(ModbusBuffer *buffer, int32_t hpIndex) {
    uint16_t ilcStatus = buffer->readU16();
    uint16_t ilcFaults = buffer->readU16();

    if (_ilcOldStatus[hpIndex] == ilcStatus && _ilcOldFaults[hpIndex] == ilcFaults) {
        return;
    }

    _updated = true;

    _ilcOldStatus[hpIndex] = ilcStatus;

    majorFault[hpIndex] = (ilcStatus & 0x0001) != 0;
    minorFault[hpIndex] = (ilcStatus & 0x0002) != 0;
    // 0x0004 is reserved
    faultOverride[hpIndex] = (ilcStatus & 0x0008) != 0;
    mainCalibrationError[hpIndex] = (ilcStatus & 0x0010) != 0;
    backupCalibrationError[hpIndex] = (ilcStatus & 0x0020) != 0;
    // 0x0040 is reserved
    // 0x0080 is reserved
    limitSwitch1Operated[hpIndex] = (ilcStatus & 0x0100) != 0;
    limitSwitch2Operated[hpIndex] = (ilcStatus & 0x0200) != 0;
    // 0x0400 is reserved
    // 0x0800 is reserved
    // 0x1000 is reserved
    // 0x2000 is DCA (FA only)
    // 0x4000 is DCA (FA only)
    // 0x8000 is reserved

    _ilcOldFaults[hpIndex] = ilcFaults;

    uniqueIdCRCError[hpIndex] = (ilcFaults & 0x0001) != 0;
    applicationTypeMismatch[hpIndex] = (ilcFaults & 0x0002) != 0;
    applicationMissing[hpIndex] = (ilcFaults & 0x0004) != 0;
    applicationCRCMismatch[hpIndex] = (ilcFaults & 0x0008) != 0;
    oneWireMissing[hpIndex] = (ilcFaults & 0x0010) != 0;
    oneWire1Mismatch[hpIndex] = (ilcFaults & 0x0020) != 0;
    oneWire2Mismatch[hpIndex] = (ilcFaults & 0x0040) != 0;
    // 0x0080 is reserved
    watchdogReset[hpIndex] = (ilcFaults & 0x0100) != 0;
    brownOut[hpIndex] = (ilcFaults & 0x0200) != 0;
    eventTrapReset[hpIndex] = (ilcFaults & 0x0400) != 0;
    motorDriverFault[hpIndex] = (ilcFaults & 0x0800) != 0;
    ssrPowerFault[hpIndex] = (ilcFaults & 0x1000) != 0;
    auxPowerFault[hpIndex] = (ilcFaults & 0x2000) != 0;
    smcPowerFault[hpIndex] = (ilcFaults & 0x4000) != 0;
}

void HardpointActuatorWarning::setProximityWarning(int32_t hpIndex, bool lowWarning, bool highWarning) {
    if (lowProximityWarning[hpIndex] != lowWarning) {
        _updated = true;
        if (lowWarning) {
            SPDLOG_WARN("HP #{} encoder position is below low proximity warning.", hpIndex + 1);
        } else {
            SPDLOG_INFO("HP #{} encoder moved above low proximity warning.", hpIndex + 1);
        }
        lowProximityWarning[hpIndex] = lowWarning;
    }
    if (highProximityWarning[hpIndex] != highWarning) {
        _updated = true;
        if (highWarning) {
            SPDLOG_WARN("HP #{} encoder position is above high proximity warning.", hpIndex + 1);
        } else {
            SPDLOG_INFO("HP #{} encoder moved below high proximity warning.", hpIndex + 1);
        }
        highProximityWarning[hpIndex] = highWarning;
    }
}

void HardpointActuatorWarning::setAirPressure(int32_t hpIndex, bool lowFault, bool highFault,
                                              float airPressure) {
    if (lowAirPressureFault[hpIndex] != lowFault) {
        _updated = true;
        lowAirPressureFault[hpIndex] = lowFault;
    }
    if (highAirPressureFault[hpIndex] != highFault) {
        _updated = true;
        highAirPressureFault[hpIndex] = highFault;
    }
}
