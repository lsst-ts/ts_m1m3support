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

#include <ForceActuatorWarning.h>
#include <M1M3SSPublisher.h>

namespace LSST {
namespace M1M3 {
namespace SS {

ForceActuatorWarning::ForceActuatorWarning() {
    memset(_lastFAServerStatusResponse, 0xFF, sizeof(_lastFAServerStatusResponse));
    memset(_lastForceDemandResponse, 0xFF, sizeof(_lastForceDemandResponse));
    memset(_lastDCAStatus, 0xFF, sizeof(_lastDCAStatus));
    _shouldSend = false;
}

void ForceActuatorWarning::parseFAServerStatusResponse(ModbusBuffer* buffer, int32_t dataIndex) {
    uint16_t ilcStatus = buffer->readU16();
    if (_lastFAServerStatusResponse[dataIndex] == ilcStatus) {
        return;
    }
    majorFault[dataIndex] = (ilcStatus & 0x0001) != 0;
    minorFault[dataIndex] = (ilcStatus & 0x0002) != 0;
    // 0x0004 is reserved
    faultOverride[dataIndex] = (ilcStatus & 0x0008) != 0;
    mainCalibrationError[dataIndex] = (ilcStatus & 0x0010) != 0;
    backupCalibrationError[dataIndex] = (ilcStatus & 0x0020) != 0;
    // 0x0040 is reserved
    // 0x0080 is reserved
    // 0x0100 is limit switch (HP only)
    // 0x0200 is limit switch (HP only)
    // 0x0400 is reserved
    // 0x0800 is reserved
    // 0x1000 is reserved
    mezzanineError[dataIndex] = (ilcStatus & 0x2000) != 0;
    mezzanineBootloaderActive[dataIndex] = (ilcStatus & 0x4000) != 0;
    // 0x8000 is reserved
    uint16_t ilcFaults = buffer->readU16();
    uniqueIdCRCError[dataIndex] = (ilcFaults & 0x0001) != 0;
    applicationTypeMismatch[dataIndex] = (ilcFaults & 0x0002) != 0;
    applicationMissing[dataIndex] = (ilcFaults & 0x0004) != 0;
    applicationCRCMismatch[dataIndex] = (ilcFaults & 0x0008) != 0;
    oneWireMissing[dataIndex] = (ilcFaults & 0x0010) != 0;
    oneWire1Mismatch[dataIndex] = (ilcFaults & 0x0020) != 0;
    oneWire2Mismatch[dataIndex] = (ilcFaults & 0x0040) != 0;
    // 0x0080 is reserved
    watchdogReset[dataIndex] = (ilcFaults & 0x0100) != 0;
    brownOut[dataIndex] = (ilcFaults & 0x0200) != 0;
    eventTrapReset[dataIndex] = (ilcFaults & 0x0400) != 0;
    // 0x0800 is Motor Driver (HP only)
    ssrPowerFault[dataIndex] = (ilcFaults & 0x1000) != 0;
    auxPowerFault[dataIndex] = (ilcFaults & 0x2000) != 0;
    // 0x4000 is SMC Power (HP only)
    // 0x8000 is reserved
    _lastFAServerStatusResponse[dataIndex] = ilcStatus;
    _shouldSend = true;
}

void ForceActuatorWarning::parseStatus(ModbusBuffer* buffer, const int32_t dataIndex,
                                       DDS::Short broadcastCounter) {
    uint8_t ilcStatus = buffer->readU8();
    bool brCntWarning = broadcastCounter != ((ilcStatus & 0xF0) >> 4);
    // bit 0x10 becomes brCntWarning
    ilcStatus = (ilcStatus & !0xF0) | (brCntWarning ? 0x10 : 0x00);
    if (_lastForceDemandResponse[dataIndex] == ilcStatus) {
        return;
    }
    ilcFault[dataIndex] = (ilcStatus & 0x01) != 0;
    mezzanineError[dataIndex] = (ilcStatus & 0x02) != 0;
    // 0x04 is reserved
    // 0x08 is reserved
    broadcastCounterWarning[dataIndex] = brCntWarning;
    _lastForceDemandResponse[dataIndex] = ilcStatus;
    _shouldSend = true;
}

void ForceActuatorWarning::parseDCAStatus(ModbusBuffer* buffer, int32_t dataIndex) {
    uint16_t dcaStatus = buffer->readU16();
    if (_lastDCAStatus[dataIndex] == dcaStatus) {
        return;
    }

    // 0x0001 is DCA Outputs Enabled (wont report)
    mezzaninePowerFault[dataIndex] = (dcaStatus & 0x0002) != 0;
    mezzanineCurrentAmp1Fault[dataIndex] = (dcaStatus & 0x0004) != 0;
    mezzanineCurrentAmp2Fault[dataIndex] = (dcaStatus & 0x0008) != 0;
    mezzanineUniqueIdCRCError[dataIndex] = (dcaStatus & 0x0010) != 0;
    // 0x0020 is reserved
    mezzanineMainCalibrationError[dataIndex] = (dcaStatus & 0x0040) != 0;
    mezzanineBackupCalibrationError[dataIndex] = (dcaStatus & 0x0080) != 0;
    mezzanineEventTrapReset[dataIndex] = (dcaStatus & 0x0100) != 0;
    // 0x0200 is reserved
    // 0x0400 is reserved
    // 0x0800 is reserved
    mezzanineApplicationMissing[dataIndex] = (dcaStatus & 0x1000) != 0;
    mezzanineApplicationCRCMismatch[dataIndex] = (dcaStatus & 0x2000) != 0;
    // 0x4000 is reserved
    mezzanineBootloaderActive[dataIndex] = (dcaStatus & 0x8000) != 0;
    _lastDCAStatus[dataIndex] = dcaStatus;
    _shouldSend = true;
}

void ForceActuatorWarning::log() {
    if (_shouldSend == false) {
        return;
    }
    anyMajorFault = false;
    anyMinorFault = false;
    anyFaultOverride = false;
    anyMainCalibrationError = false;
    anyBackupCalibrationError = false;
    anyMezzanineError = false;
    anyMezzanineBootloaderActive = false;
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
    anySSRPowerFault = false;
    anyAuxPowerFault = false;
    anyMezzaninePowerFault = false;
    anyMezzanineCurrentAmp1Fault = false;
    anyMezzanineCurrentAmp2Fault = false;
    anyMezzanineUniqueIdCRCError = false;
    anyMezzanineMainCalibrationError = false;
    anyMezzanineBackupCalibrationError = false;
    anyMezzanineEventTrapReset = false;
    anyMezzanineApplicationMissing = false;
    anyMezzanineApplicationCRCMismatch = false;
    anyILCFault = false;
    anyBroadcastCounterWarning = false;
    for (int i = 0; i < FA_COUNT; ++i) {
        anyMajorFault = anyMajorFault || majorFault[i];
        anyMinorFault = anyMinorFault || minorFault[i];
        anyFaultOverride = anyFaultOverride || faultOverride[i];
        anyMainCalibrationError = anyMainCalibrationError || mainCalibrationError[i];
        anyBackupCalibrationError = anyBackupCalibrationError || backupCalibrationError[i];
        anyMezzanineError = anyMezzanineError || mezzanineError[i];
        anyMezzanineBootloaderActive = anyMezzanineBootloaderActive || mezzanineBootloaderActive[i];
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
        anySSRPowerFault = anySSRPowerFault || ssrPowerFault[i];
        anyAuxPowerFault = anyAuxPowerFault || auxPowerFault[i];
        anyMezzaninePowerFault = anyMezzaninePowerFault || mezzaninePowerFault[i];
        anyMezzanineCurrentAmp1Fault = anyMezzanineCurrentAmp1Fault || mezzanineCurrentAmp1Fault[i];
        anyMezzanineCurrentAmp2Fault = anyMezzanineCurrentAmp2Fault || mezzanineCurrentAmp2Fault[i];
        anyMezzanineUniqueIdCRCError = anyMezzanineUniqueIdCRCError || mezzanineUniqueIdCRCError[i];
        anyMezzanineMainCalibrationError =
                anyMezzanineMainCalibrationError || mezzanineMainCalibrationError[i];
        anyMezzanineBackupCalibrationError =
                anyMezzanineBackupCalibrationError || mezzanineBackupCalibrationError[i];
        anyMezzanineEventTrapReset = anyMezzanineEventTrapReset || mezzanineEventTrapReset[i];
        anyMezzanineApplicationMissing = anyMezzanineApplicationMissing || mezzanineApplicationMissing[i];
        anyMezzanineApplicationCRCMismatch =
                anyMezzanineApplicationCRCMismatch || mezzanineApplicationCRCMismatch[i];
        anyILCFault = anyILCFault || ilcFault[i];
        anyBroadcastCounterWarning = anyBroadcastCounterWarning || broadcastCounterWarning[i];
    }
    anyWarning = anyMajorFault || anyMinorFault || anyFaultOverride || anyMainCalibrationError ||
                 anyBackupCalibrationError || anyMezzanineError || anyMezzanineBootloaderActive ||
                 anyUniqueIdCRCError || anyApplicationTypeMismatch || anyApplicationMissing ||
                 anyApplicationCRCMismatch || anyOneWireMissing || anyOneWire1Mismatch ||
                 anyOneWire2Mismatch || anyWatchdogReset || anyBrownOut || anyEventTrapReset ||
                 anySSRPowerFault || anyAuxPowerFault || anyMezzaninePowerFault ||
                 anyMezzanineCurrentAmp1Fault || anyMezzanineCurrentAmp2Fault ||
                 anyMezzanineUniqueIdCRCError || anyMezzanineMainCalibrationError ||
                 anyMezzanineBackupCalibrationError || anyMezzanineEventTrapReset ||
                 anyMezzanineApplicationMissing || anyMezzanineApplicationCRCMismatch || anyILCFault ||
                 anyBroadcastCounterWarning;
    M1M3SSPublisher::get().logForceActuatorWarning(this);
    _shouldSend = false;
}

}  // namespace SS
}  // namespace M1M3
}  // namespace LSST
