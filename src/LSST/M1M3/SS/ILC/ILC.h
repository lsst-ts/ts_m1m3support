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

#ifndef ILC_H_
#define ILC_H_

#include <ILCDataTypes.h>
#include <ILCSubnetData.h>
#include <ModbusBuffer.h>
#include <ILCMessageFactory.h>
#include <SetADCChanneOffsetAndSensitivityBusList.h>
#include <ChangeILCModeBusList.h>
#include <ReadBoostValveDCAGainBusList.h>
#include <ReadCalibrationBusList.h>
#include <ReportADCScanRateBusList.h>
#include <ReportDCAIDBusList.h>
#include <ReportDCAStatusBusList.h>
#include <ReportServerIDBusList.h>
#include <ReportServerStatusBusList.h>
#include <ResetBustList.h>
#include <SetADCScanRateBusList.h>
#include <SetBoostValveDCAGainBusList.h>
#include <FreezeSensorBusList.h>
#include <RaisedBusList.h>
#include <ActiveBusList.h>
#include <ILCResponseParser.h>
#include <SAL_MTM1M3C.h>
#include <FirmwareUpdate.h>
#include <ILCApplicationSettings.h>
#include <ForceActuatorApplicationSettings.h>
#include <ForceActuatorSettings.h>
#include <HardpointActuatorApplicationSettings.h>
#include <HardpointActuatorSettings.h>
#include <HardpointMonitorApplicationSettings.h>
#include <PositionController.h>
#include <SafetyController.h>

namespace LSST {
namespace M1M3 {
namespace SS {

/*!
 * The ILC class used to communicate with the M1M3's 5 subnets. Uses BusList
 * subclasses to send queries to FPGA.
 *
 * @see BusList
 */
class ILC {
public:
    ILC(PositionController* positionController, ILCApplicationSettings* ilcApplicationSettings,
        ForceActuatorApplicationSettings* forceActuatorApplicationSettings,
        ForceActuatorSettings* forceActuatorSettings,
        HardpointActuatorApplicationSettings* hardpointActuatorApplicationSettings,
        HardpointActuatorSettings* hardpointActuatorSettings,
        HardpointMonitorApplicationSettings* hardpointMonitorApplicationSettings,
        SafetyController* safetyController);
    virtual ~ILC();

    /**
     * (Re)-build all bus lists.
     */
    void buildBusLists();

    void programILC(int32_t actuatorId, std::string filePath);
    void modbusTransmit(int32_t actuatorId, int32_t functionCode, int32_t dataLength, int16_t* data);

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
    void writeRaisedListBuffer();
    void writeActiveListBuffer();

    /**
     * Called in enabled state. Calls once writeRaisedListBuffer and twice (to
     * get more data) writeActiveListBuffer.
     */
    void writeControlListBuffer();

    void triggerModbus();

    void waitForSubnet(int32_t subnet, int32_t timeout);
    void waitForAllSubnets(int32_t timeout);

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
    void publishForceActuatorData();
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
     * @return disabled actuator ID (101..) or 0 when no disabled actuator was found
     */
    uint32_t hasDisabledFarNeighbor(uint32_t actuatorIndex);

private:
    SafetyController* _safetyController;
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
    FirmwareUpdate _firmwareUpdate;

    HardpointActuatorSettings* _hardpointActuatorSettings;
    MTM1M3_hardpointActuatorDataC* _hardpointActuatorData;
    ForceActuatorApplicationSettings* _forceActuatorApplicationSettings;
    ForceActuatorSettings* _forceActuatorSettings;
    MTM1M3_forceActuatorDataC* _forceActuatorData;
    MTM1M3_logevent_hardpointActuatorInfoC* _hardpointActuatorInfo;
    PositionController* _positionController;

    int8_t _hpStepCommand[6];
    int32_t _hpSteps[6];

    uint16_t _u16Buffer[1];
    ModbusBuffer _rxBuffer;

    int32_t _controlListToggle;

    uint8_t _subnetToRxAddress(uint8_t subnet);
    uint8_t _subnetToTxAddress(uint8_t subnet);

    void _writeBusList(BusList* busList);

    void _updateHPSteps();
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* ILC_H_ */
