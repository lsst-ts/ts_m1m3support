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

#include <cRIO/Join.h>

#include <Model.h>
#include <SettingReader.h>
#include <M1M3SSPublisher.h>
#include <Displacement.h>
#include <Inclinometer.h>
#include <ILC.h>
#include <Timestamp.h>
#include <ILCApplicationSettings.h>
#include <U16ArrayUtilities.h>
#include <iostream>

#include <IFPGA.h>
#include <FPGAAddresses.h>
#include <ForceController.h>
#include <ForceActuatorSettings.h>
#include <SafetyController.h>
#include <PositionController.h>
#include <Accelerometer.h>
#include <ForceActuatorApplicationSettings.h>
#include <HardpointActuatorApplicationSettings.h>
#include <HardpointMonitorApplicationSettings.h>
#include <PowerController.h>
#include <MirrorRaiseController.h>
#include <MirrorLowerController.h>
#include <Gyro.h>
#include <spdlog/spdlog.h>
#include <DigitalInputOutput.h>

using namespace std;

using namespace LSST::M1M3::SS;

Model::Model() {
    SPDLOG_DEBUG("Model: Model()");
    _safetyController = NULL;
    _displacement = NULL;
    _inclinometer = NULL;
    _ilc = NULL;
    _forceController = NULL;
    _positionController = NULL;
    _accelerometer = NULL;
    _powerController = NULL;
    _mirrorRaiseController = NULL;
    _mirrorLowerController = NULL;
    _hardpointTestController = NULL;
    _gyro = NULL;
    _cachedTimestamp = 0;
    _mutex.lock();
}

Model::~Model() {
    _mutex.unlock();

    delete _safetyController;
    delete _displacement;
    delete _inclinometer;
    delete _ilc;
    delete _forceController;
    delete _positionController;
    delete _accelerometer;
    delete _powerController;
    delete _mirrorRaiseController;
    delete _mirrorLowerController;
    delete _gyro;
}

Model& Model::get() {
    static Model model;
    return model;
}

void Model::loadSettings(std::string settingsToApply) {
    SPDLOG_INFO("Model: loadSettings({})", settingsToApply);

    SettingReader::instance().configure(settingsToApply);

    M1M3SSPublisher::get().reset();

    SPDLOG_INFO("Model: Loading ILC application settings");
    ILCApplicationSettings* ilcApplicationSettings = SettingReader::instance().loadILCApplicationSettings();
    SPDLOG_INFO("Model: Loading force actuator application settings");
    ForceActuatorApplicationSettings* forceActuatorApplicationSettings =
            SettingReader::instance().getForceActuatorApplicationSettings();
    SPDLOG_INFO("Model: Loading force actuator settings");
    ForceActuatorSettings* forceActuatorSettings = SettingReader::instance().loadForceActuatorSettings();
    SPDLOG_INFO("Model: Loading hardpoint actuator application settings");
    HardpointActuatorApplicationSettings* hardpointActuatorApplicationSettings =
            SettingReader::instance().loadHardpointActuatorApplicationSettings();
    SPDLOG_INFO("Model: Loading hardpoint actuator settings");
    HardpointActuatorSettings* hardpointActuatorSettings =
            SettingReader::instance().loadHardpointActuatorSettings();
    SPDLOG_INFO("Model: Loading safety controller settings");
    SafetyControllerSettings* safetyControllerSettings =
            SettingReader::instance().loadSafetyControllerSettings();
    SPDLOG_INFO("Model: Loading position controller settings");
    PositionControllerSettings* positionControllerSettings =
            SettingReader::instance().loadPositionControllerSettings();
    SPDLOG_INFO("Model: Loading accelerometer settings");
    AccelerometerSettings* accelerometerSettings = SettingReader::instance().loadAccelerometerSettings();
    SPDLOG_INFO("Model: Loading displacement settings");
    DisplacementSensorSettings* displacementSensorSettings =
            SettingReader::instance().loadDisplacementSensorSettings();
    SPDLOG_INFO("Model: Loading hardpoint monitor application settings");
    HardpointMonitorApplicationSettings* hardpointMonitorApplicationSettings =
            SettingReader::instance().loadHardpointMonitorApplicationSettings();
    SPDLOG_INFO("Model: Loading gyro settings");
    GyroSettings* gyroSettings = SettingReader::instance().loadGyroSettings();
    SPDLOG_INFO("Model: Loading PID settings");
    PIDSettings* pidSettings = SettingReader::instance().loadPIDSettings();
    SPDLOG_INFO("Model: Loading inclinometer settings");
    InclinometerSettings* inclinometerSettings = SettingReader::instance().loadInclinometerSettings();

    _populateForceActuatorInfo(forceActuatorApplicationSettings, forceActuatorSettings);
    _populateHardpointActuatorInfo(hardpointActuatorApplicationSettings, hardpointActuatorSettings,
                                   positionControllerSettings);
    _populateHardpointMonitorInfo(hardpointMonitorApplicationSettings);

    delete _safetyController;
    SPDLOG_INFO("Model: Creating safety controller");
    _safetyController = new SafetyController(safetyControllerSettings);

    delete _displacement;
    SPDLOG_INFO("Model: Creating displacement");
    _displacement = new Displacement(displacementSensorSettings, IFPGA::get().getSupportFPGAData(),
                                     _safetyController);

    delete _inclinometer;
    SPDLOG_INFO("Model: Creating inclinometer");
    _inclinometer =
            new Inclinometer(IFPGA::get().getSupportFPGAData(), _safetyController, inclinometerSettings);

    delete _positionController;
    SPDLOG_INFO("Model: Creating position controller");
    _positionController = new PositionController(positionControllerSettings, hardpointActuatorSettings);

    delete _ilc;
    SPDLOG_INFO("Model: Creating ILC");
    _ilc = new ILC(_positionController, ilcApplicationSettings, forceActuatorApplicationSettings,
                   forceActuatorSettings, hardpointActuatorApplicationSettings, hardpointActuatorSettings,
                   hardpointMonitorApplicationSettings, _safetyController);

    delete _forceController;
    SPDLOG_INFO("Model: Creating force controller");
    _forceController =
            new ForceController(forceActuatorApplicationSettings, forceActuatorSettings, pidSettings);

    SPDLOG_INFO("Model: Updating digital input output");
    _digitalInputOutput.setSafetyController(_safetyController);

    delete _accelerometer;
    SPDLOG_INFO("Model: Creating accelerometer");
    _accelerometer = new Accelerometer(accelerometerSettings);

    delete _powerController;
    SPDLOG_INFO("Model: Creating power controller");
    _powerController = new PowerController(_safetyController);

    delete _mirrorLowerController;
    SPDLOG_INFO("Model: Creating mirror lower controller");
    _mirrorLowerController = new MirrorLowerController(_positionController, _forceController,
                                                       _safetyController, _powerController);

    delete _mirrorRaiseController;
    SPDLOG_INFO("Model: Creating mirror raise controller");
    _mirrorRaiseController = new MirrorRaiseController(_positionController, _forceController,
                                                       _safetyController, _powerController);

    delete _hardpointTestController;
    SPDLOG_INFO("Model: Creating hardpoint test controller");
    _hardpointTestController = new HardpointTestController(_positionController, hardpointActuatorSettings);

    delete _gyro;
    SPDLOG_INFO("Model: Creating gyro");
    _gyro = new Gyro(gyroSettings);

    // apply disabled FA from setting
    for (int i = 0; i < FA_COUNT; i++) {
        if (forceActuatorSettings->isActuatorDisabled(i)) {
            _ilc->disableFA(forceActuatorApplicationSettings->ZIndexToActuatorId(i));
        }
    }

    SPDLOG_INFO("Model: Settings applied");
}

void Model::queryFPGAData() {}

void Model::publishStateChange(States::Type newState) {
    SPDLOG_DEBUG("Model: publishStateChange({:d})", newState);
    uint64_t state = (uint64_t)newState;
    double timestamp = M1M3SSPublisher::get().getTimestamp();
    MTM1M3_logevent_summaryStateC* summaryStateData = M1M3SSPublisher::get().getEventSummaryState();
    // summaryStateData->timestamp = timestamp;
    summaryStateData->summaryState = (int32_t)((state & 0xFFFFFFFF00000000) >> 32);
    M1M3SSPublisher::get().logSummaryState();
    MTM1M3_logevent_detailedStateC* detailedStateData = M1M3SSPublisher::get().getEventDetailedState();
    detailedStateData->timestamp = timestamp;
    detailedStateData->detailedState = (int32_t)(state & 0x00000000FFFFFFFF);
    M1M3SSPublisher::get().logDetailedState();
}

void Model::publishRecommendedSettings() {
    SPDLOG_DEBUG("Model: publishRecommendedSettings()");
    MTM1M3_logevent_configurationsAvailableC* data = M1M3SSPublisher::get().getEventConfigurationsAvailable();
    data->overrides = LSST::cRIO::join(SettingReader::instance().getAvailableConfigurations());
    data->version = GIT_HASH;
    data->url = "https://github.com/lsst-ts/ts_m1m3support";
    data->schemaVersion = "v1";
    M1M3SSPublisher::get().logConfigurationsAvailable();
}

void Model::publishOuterLoop(std::chrono::nanoseconds executionTime) {
    SPDLOG_TRACE("Model: publishOuterLoop()");
    MTM1M3_outerLoopDataC* data = M1M3SSPublisher::get().getOuterLoopData();
    data->executionTime = executionTime.count() / 1000000000.0;
    M1M3SSPublisher::get().putOuterLoopData();
}

void Model::exitControl() { _mutex.unlock(); }

void Model::waitForExitControl() {
    _mutex.lock();
    _mutex.unlock();
}

void Model::_populateForceActuatorInfo(ForceActuatorApplicationSettings* forceActuatorApplicationSettings,
                                       ForceActuatorSettings* forceActuatorSettings) {
    SPDLOG_DEBUG("Model: populateForceActuatorInfo()");
    MTM1M3_logevent_forceActuatorInfoC* forceInfo = M1M3SSPublisher::get().getEventForceActuatorInfo();
    for (int i = 0; i < FA_COUNT; i++) {
        ForceActuatorTableRow row = forceActuatorApplicationSettings->Table[i];
        forceInfo->referenceId[i] = row.ActuatorID;
        forceInfo->modbusSubnet[i] = row.Subnet;
        forceInfo->modbusAddress[i] = row.Address;
        forceInfo->actuatorType[i] = row.Type;
        forceInfo->actuatorOrientation[i] = row.Orientation;
        forceInfo->xPosition[i] = row.XPosition;
        forceInfo->yPosition[i] = row.YPosition;
        forceInfo->zPosition[i] = row.ZPosition;
    }
}

void Model::_populateHardpointActuatorInfo(
        HardpointActuatorApplicationSettings* hardpointActuatorApplicationSettings,
        HardpointActuatorSettings* hardpointActuatorSettings,
        PositionControllerSettings* positionControllerSettings) {
    SPDLOG_DEBUG("Model: populateHardpointActuatorInfo()");
    MTM1M3_logevent_hardpointActuatorInfoC* hardpointInfo =
            M1M3SSPublisher::get().getEventHardpointActuatorInfo();
    for (int i = 0; i < HP_COUNT; i++) {
        HardpointActuatorTableRow row = hardpointActuatorApplicationSettings->Table[i];
        hardpointInfo->referenceId[row.Index] = row.ActuatorID;
        hardpointInfo->modbusSubnet[row.Index] = row.Subnet;
        hardpointInfo->modbusAddress[row.Index] = row.Address;
        hardpointInfo->xPosition[row.Index] = row.XPosition;
        hardpointInfo->yPosition[row.Index] = row.YPosition;
        hardpointInfo->zPosition[row.Index] = row.ZPosition;
        hardpointInfo->referencePosition[i] = positionControllerSettings->referencePosition[i];
    }
}

void Model::_populateHardpointMonitorInfo(
        HardpointMonitorApplicationSettings* hardpointMonitorApplicationSettings) {
    SPDLOG_DEBUG("Model: populateHardpointMonitorInfo()");
    MTM1M3_logevent_hardpointMonitorInfoC* hardpointMonitorInfo =
            M1M3SSPublisher::get().getEventHardpointMonitorInfo();
    for (int i = 0; i < HP_COUNT; i++) {
        HardpointMonitorTableRow row = hardpointMonitorApplicationSettings->Table[i];
        hardpointMonitorInfo->referenceId[row.Index] = row.ActuatorID;
        hardpointMonitorInfo->modbusSubnet[row.Index] = row.Subnet;
        hardpointMonitorInfo->modbusAddress[row.Index] = row.Address;
    }
}
