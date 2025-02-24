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

#ifndef SSILCS_H_
#define SSILCS_H_

#include <SAL_MTM1M3C.h>

#include <ActiveBusList.h>
#include <ChangeILCModeBusList.h>
#include <ForceActuatorApplicationSettings.h>
#include <ForceActuatorSettings.h>
#include <FreezeSensorBusList.h>
#include <HardpointActuatorApplicationSettings.h>
#include <HardpointActuatorSettings.h>
#include <HardpointMonitorApplicationSettings.h>
#include <ILCDataTypes.h>
#include <ILCMessageFactory.h>
#include <ILCResponseParser.h>
#include <ILCSubnetData.h>
#include <ModbusBuffer.h>
#include <PositionController.h>
#include <RaisedBusList.h>
#include <ReadBoostValveDCAGainBusList.h>
#include <ReadCalibrationBusList.h>
#include <ReportADCScanRateBusList.h>
#include <ReportDCAIDBusList.h>
#include <ReportDCAStatusBusList.h>
#include <ReportServerIDBusList.h>
#include <ReportServerStatusBusList.h>
#include <ResetBustList.h>
#include <SafetyController.h>
#include <SetADCChanneOffsetAndSensitivityBusList.h>
#include <SetADCScanRateBusList.h>
#include <SetBoostValveDCAGainBusList.h>

namespace LSST {
namespace M1M3 {
namespace SS {

/*!
 * The SSILCs class used to communicate with the M1M3's 5 subnets. Uses BusList
 * subclasses to send queries to FPGA.
 *
 * @see BusList
 */
class SSILCs {
public:
    SSILCs(PositionController *positionController,
           ForceActuatorApplicationSettings *forceActuatorApplicationSettings,
           HardpointActuatorApplicationSettings *hardpointActuatorApplicationSettings,
           HardpointMonitorApplicationSettings *hardpointMonitorApplicationSettings,
           SafetyController *safetyController);
    virtual ~SSILCs();

    /**
     * (Re)-build all bus lists.
     */
    void buildBusLists();

    void writeCalibrationDataBuffer();
    void writeSetADCScanRateBuffer();
    void writeSetBoostValveDCAGainBuffer();
    void writeResetBuffer();
    void writeReportServerIDBuffer();
    void writeReportServerStatusBuffer();
    void writeReportADCScanRateBuffer();
    void writeReadCalibrationDataBuffer();
    void writeReadBoostValveDCAGainBuffer();
    void writeReportDCAIDBuffer();
    void writeReportDCAStatusBuffer();
    void writeSetModeDisableBuffer();
    void writeSetModeEnableBuffer();
    void writeSetModeStandbyBuffer();
    void writeSetModeClearFaultsBuffer();
    void writeFreezeSensorListBuffer();

    /**
     * Called in enabled state (parked, raised,..).
     */
    void writeRaisedListBuffer();

    /**
     * Called in enabled state (parked, raised,..).
     */
    void writeActiveListBuffer();

    /**
     * Called in enabled state. Calls once writeRaisedListBuffer and twice (to
     * get more data) writeActiveListBuffer.
     */
    void writeControlListBuffer();

    void triggerModbus();

    /**
     * Wait for Modbus IRQs.
     *
     * @param realtime_loop if true, realtime loop timeouts are used. When
     * false, config timeouts are used.
     */
    void waitForAllSubnets(bool realtime_loop);

    void read(uint8_t subnet);
    void readAll();
    void flush(uint8_t subnet);
    void flushAll();

    void calculateHPPostion();
    void calculateHPMirrorForces();
    void calculateFAMirrorForces();
    void clearResponses();
    void verifyResponses();

    void publishForceActuatorInfo();
    void publishForceActuatorStatus();
    void publishHardpointActuatorInfo();
    void publishHardpointStatus();
    void publishHardpointData();
    void publishHardpointMonitorInfo();
    void publishHardpointMonitorStatus();
    void publishHardpointMonitorData();

    /**
     * Disable given FA.
     *
     * @param actuatorId actutor ID (101..443) to disable
     */
    void disableFA(uint32_t actuatorId);

    /**
     * Enables given FA.
     *
     * @param actuatorId actuator ID (101..443) to enable
     */
    void enableFA(uint32_t actuatorId);

    /**
     * Enables all force actuators.
     */
    void enableAllFA();

    /**
     * Check if given actuator is disabled.
     *
     * @param actuatorId Actuator ID (101..443)
     *
     * @return true when given actuator is disabled, false otherwise
     */
    bool isDisabled(uint32_t actuatorId) { return _subnetData.getMap(actuatorId).Disabled; }

    /**
     * Check if any far neighbor of an actuator with given index is disabled.
     *
     * @param actuatorIndex actuator index (0-155) of FA to check
     *
     * @return disabled actuator ID (101..) or 0 when no disabled actuator was
     * found
     */
    uint32_t hasDisabledFarNeighbor(uint32_t actuatorIndex);

private:
    SafetyController *_safetyController;
    ILCSubnetData _subnetData;
    ILCMessageFactory _ilcMessageFactory;
    ILCResponseParser _responseParser;

    SetADCChanneOffsetAndSensitivityBusList _busListSetADCChannelOffsetAndSensitivity;
    SetADCScanRateBusList _busListSetADCScanRate;
    SetBoostValveDCAGainBusList _busListSetBoostValveDCAGains;
    ResetBustList _busListReset;
    ReportServerIDBusList _busListReportServerID;
    ReportServerStatusBusList _busListReportServerStatus;
    ReportADCScanRateBusList _busListReportADCScanRate;
    ReadCalibrationBusList _busListReadCalibration;
    ReadBoostValveDCAGainBusList _busListReadBoostValveDCAGains;
    ReportDCAIDBusList _busListReportDCAID;
    ReportDCAStatusBusList _busListReportDCAStatus;
    ChangeILCModeBusList _busListChangeILCModeDisabled;
    ChangeILCModeBusList _busListChangeILCModeEnabled;
    ChangeILCModeBusList _busListChangeILCModeStandby;
    ChangeILCModeBusList _busListChangeILCModeClearFaults;
    FreezeSensorBusList _busListFreezeSensor;
    RaisedBusList _busListRaised;
    ActiveBusList _busListActive;

    HardpointActuatorSettings *_hardpointActuatorSettings;
    MTM1M3_hardpointActuatorDataC *_hardpointActuatorData;
    ForceActuatorApplicationSettings *_forceActuatorApplicationSettings;
    MTM1M3_logevent_hardpointActuatorInfoC *_hardpointActuatorInfo;
    PositionController *_positionController;

    int8_t _hpStepCommand[6];
    int32_t _hpSteps[6];

    ModbusBuffer _rxBuffer;

    int32_t _controlListToggle;

    uint8_t _subnetToRxAddress(uint8_t subnet);
    uint8_t _subnetToTxAddress(uint8_t subnet);

    void _writeBusList(BusList *busList);

    void _updateHPSteps();
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* SSILCS_H_ */
