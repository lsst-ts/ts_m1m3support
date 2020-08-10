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
#include <RecommendedApplicationSettings.h>
#include <ForceActuatorSettings.h>
#include <SafetyController.h>
#include <PositionController.h>
#include <Accelerometer.h>
#include <ForceActuatorApplicationSettings.h>
#include <HardpointActuatorApplicationSettings.h>
#include <HardpointMonitorApplicationSettings.h>
#include <InterlockApplicationSettings.h>
#include <PowerController.h>
#include <AutomaticOperationsController.h>
#include <Gyro.h>
#include <spdlog/spdlog.h>
#include <DigitalInputOutput.h>

using namespace std;

namespace LSST {
namespace M1M3 {
namespace SS {

Model::Model() {
    spdlog::debug("Model: Model()");
    _safetyController = NULL;
    _digitalInputOutput = NULL;
    _displacement = NULL;
    _inclinometer = NULL;
    _ilc = NULL;
    _forceController = NULL;
    _positionController = NULL;
    _accelerometer = NULL;
    _powerController = NULL;
    _automaticOperationsController = NULL;
    _gyro = NULL;
    _cachedTimestamp = 0;
    pthread_mutex_init(&_mutex, NULL);
    pthread_mutex_lock(&_mutex);
}

Model::~Model() {
    pthread_mutex_unlock(&_mutex);
    pthread_mutex_destroy(&_mutex);

    delete _safetyController;
    delete _digitalInputOutput;
    delete _displacement;
    delete _inclinometer;
    delete _ilc;
    delete _forceController;
    delete _positionController;
    delete _accelerometer;
    delete _powerController;
    delete _automaticOperationsController;
    delete _gyro;
}

Model& Model::get() {
    static Model model;
    return model;
}

void Model::loadSettings(std::string settingsToApply) {
    spdlog::info("Model: loadSettings({})", settingsToApply);

    SettingReader& settingReader = SettingReader::get();
    settingReader.configure(settingsToApply);

    M1M3SSPublisher::get().getOuterLoopData()->slewFlag = false;

    spdlog::info("Model: Loading ILC application settings");
    ILCApplicationSettings* ilcApplicationSettings = settingReader.loadILCApplicationSettings();
    spdlog::info("Model: Loading force actuator application settings");
    ForceActuatorApplicationSettings* forceActuatorApplicationSettings =
            settingReader.loadForceActuatorApplicationSettings();
    spdlog::info("Model: Loading force actuator settings");
    ForceActuatorSettings* forceActuatorSettings = settingReader.loadForceActuatorSettings();
    spdlog::info("Model: Loading hardpoint actuator application settings");
    HardpointActuatorApplicationSettings* hardpointActuatorApplicationSettings =
            settingReader.loadHardpointActuatorApplicationSettings();
    spdlog::info("Model: Loading hardpoint actuator settings");
    HardpointActuatorSettings* hardpointActuatorSettings = settingReader.loadHardpointActuatorSettings();
    spdlog::info("Model: Loading safety controller settings");
    SafetyControllerSettings* safetyControllerSettings = settingReader.loadSafetyControllerSettings();
    spdlog::info("Model: Loading position controller settings");
    PositionControllerSettings* positionControllerSettings = settingReader.loadPositionControllerSettings();
    spdlog::info("Model: Loading accelerometer settings");
    AccelerometerSettings* accelerometerSettings = settingReader.loadAccelerometerSettings();
    spdlog::info("Main: Load interlock application settings");
    InterlockApplicationSettings* interlockApplicationSettings =
            SettingReader::get().loadInterlockApplicationSettings();
    spdlog::info("Model: Loading displacement settings");
    DisplacementSensorSettings* displacementSensorSettings = settingReader.loadDisplacementSensorSettings();
    spdlog::info("Model: Loading hardpoint monitor application settings");
    HardpointMonitorApplicationSettings* hardpointMonitorApplicationSettings =
            settingReader.loadHardpointMonitorApplicationSettings();
    spdlog::info("Model: Loading gyro settings");
    GyroSettings* gyroSettings = settingReader.loadGyroSettings();
    spdlog::info("Model: Loading PID settings");
    PIDSettings* pidSettings = settingReader.loadPIDSettings();
    spdlog::info("Model: Loading inclinometer settings");
    InclinometerSettings* inclinometerSettings = settingReader.loadInclinometerSettings();

    _populateForceActuatorInfo(forceActuatorApplicationSettings, forceActuatorSettings);
    _populateHardpointActuatorInfo(hardpointActuatorApplicationSettings, hardpointActuatorSettings,
                                   positionControllerSettings);
    _populateHardpointMonitorInfo(hardpointMonitorApplicationSettings);

    delete _safetyController;
    spdlog::info("Model: Creating safety controller");
    _safetyController = new SafetyController(safetyControllerSettings);

    delete _digitalInputOutput;
    spdlog::info("Model: Creating digital input output");
    _digitalInputOutput = new DigitalInputOutput(interlockApplicationSettings);

    delete _displacement;
    spdlog::info("Model: Creating displacement");
    _displacement = new Displacement(displacementSensorSettings, IFPGA::get().getSupportFPGAData(),
                                     _safetyController);

    delete _inclinometer;
    spdlog::info("Model: Creating inclinometer");
    _inclinometer =
            new Inclinometer(IFPGA::get().getSupportFPGAData(), _safetyController, inclinometerSettings);

    delete _positionController;
    spdlog::info("Model: Creating position controller");
    _positionController = new PositionController(positionControllerSettings, hardpointActuatorSettings);

    delete _ilc;
    spdlog::info("Model: Creating ILC");
    _ilc = new ILC(_positionController, ilcApplicationSettings, forceActuatorApplicationSettings,
                   forceActuatorSettings, hardpointActuatorApplicationSettings, hardpointActuatorSettings,
                   hardpointMonitorApplicationSettings, _safetyController);

    delete _forceController;
    spdlog::info("Model: Creating force controller");
    _forceController = new ForceController(forceActuatorApplicationSettings, forceActuatorSettings,
                                           pidSettings, _safetyController);

    spdlog::info("Model: Updating digital input output");
    _digitalInputOutput->setSafetyController(_safetyController);

    delete _accelerometer;
    spdlog::info("Model: Creating accelerometer");
    _accelerometer = new Accelerometer(accelerometerSettings);

    delete _powerController;
    spdlog::info("Model: Creating power controller");
    _powerController = new PowerController(_safetyController);

    delete _automaticOperationsController;
    spdlog::info("Model: Creating automatic operations controller");
    _automaticOperationsController = new AutomaticOperationsController(_positionController, _forceController,
                                                                       _safetyController, _powerController);

    delete _gyro;
    spdlog::info("Model: Creating gyro");
    _gyro = new Gyro(gyroSettings);

    spdlog::info("Model: Settings applied");
}

void Model::queryFPGAData() {}

void Model::publishStateChange(States::Type newState) {
    spdlog::debug("Model: publishStateChange({:d})", newState);
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
    spdlog::debug("Model: publishRecommendedSettings()");
    RecommendedApplicationSettings* recommendedApplicationSettings =
            SettingReader::get().loadRecommendedApplicationSettings();
    MTM1M3_logevent_settingVersionsC* data = M1M3SSPublisher::get().getEventSettingVersions();
    data->recommendedSettingsVersion = "";
    for (uint32_t i = 0; i < recommendedApplicationSettings->RecommendedSettings.size(); i++) {
        data->recommendedSettingsVersion += recommendedApplicationSettings->RecommendedSettings[i] + ",";
    }
    M1M3SSPublisher::get().logSettingVersions();
}

void Model::publishOuterLoop(std::chrono::nanoseconds executionTime) {
    spdlog::trace("Model: publishOuterLoop()");
    MTM1M3_outerLoopDataC* data = M1M3SSPublisher::get().getOuterLoopData();
    data->executionTime = executionTime.count() / 1000000000.0;
    M1M3SSPublisher::get().putOuterLoopData();
}

void Model::exitControl() { pthread_mutex_unlock(&_mutex); }

void Model::waitForExitControl() {
    pthread_mutex_lock(&_mutex);
    pthread_mutex_unlock(&_mutex);
}

void Model::_populateForceActuatorInfo(ForceActuatorApplicationSettings* forceActuatorApplicationSettings,
                                       ForceActuatorSettings* forceActuatorSettings) {
    spdlog::debug("Model: populateForceActuatorInfo()");
    MTM1M3_logevent_forceActuatorInfoC* forceInfo = M1M3SSPublisher::get().getEventForceActuatorInfo();
    for (int i = 0; i < FA_COUNT; i++) {
        ForceActuatorTableRow row = forceActuatorApplicationSettings->Table[i];
        forceInfo->referenceId[row.Index] = row.ActuatorID;
        forceInfo->modbusSubnet[row.Index] = row.Subnet;
        forceInfo->modbusAddress[row.Index] = row.Address;
        forceInfo->actuatorType[row.Index] = row.Type;
        forceInfo->actuatorOrientation[row.Index] = row.Orientation;
        forceInfo->xPosition[row.Index] = row.XPosition;
        forceInfo->yPosition[row.Index] = row.YPosition;
        forceInfo->zPosition[row.Index] = row.ZPosition;
    }
}

void Model::_populateHardpointActuatorInfo(
        HardpointActuatorApplicationSettings* hardpointActuatorApplicationSettings,
        HardpointActuatorSettings* hardpointActuatorSettings,
        PositionControllerSettings* positionControllerSettings) {
    spdlog::debug("Model: populateHardpointActuatorInfo()");
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
    }
    hardpointInfo->referencePosition[0] = positionControllerSettings->ReferencePositionEncoder1;
    hardpointInfo->referencePosition[1] = positionControllerSettings->ReferencePositionEncoder2;
    hardpointInfo->referencePosition[2] = positionControllerSettings->ReferencePositionEncoder3;
    hardpointInfo->referencePosition[3] = positionControllerSettings->ReferencePositionEncoder4;
    hardpointInfo->referencePosition[4] = positionControllerSettings->ReferencePositionEncoder5;
    hardpointInfo->referencePosition[5] = positionControllerSettings->ReferencePositionEncoder6;
}

void Model::_populateHardpointMonitorInfo(
        HardpointMonitorApplicationSettings* hardpointMonitorApplicationSettings) {
    spdlog::debug("Model: populateHardpointMonitorInfo()");
    MTM1M3_logevent_hardpointMonitorInfoC* hardpointMonitorInfo =
            M1M3SSPublisher::get().getEventHardpointMonitorInfo();
    for (int i = 0; i < HM_COUNT; i++) {
        HardpointMonitorTableRow row = hardpointMonitorApplicationSettings->Table[i];
        hardpointMonitorInfo->referenceId[row.Index] = row.ActuatorID;
        hardpointMonitorInfo->modbusSubnet[row.Index] = row.Subnet;
        hardpointMonitorInfo->modbusAddress[row.Index] = row.Address;
    }
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
