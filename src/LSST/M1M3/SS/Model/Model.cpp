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
#include <PowerController.h>
#include <AutomaticOperationsController.h>
#include <Gyro.h>
#include <spdlog/spdlog.h>
#include <DigitalInputOutput.h>

using namespace std;

namespace LSST {
namespace M1M3 {
namespace SS {

Model::Model(M1M3SSPublisher* publisher, DigitalInputOutput* digitalInputOutput) {
    spdlog::debug("Model: Model()");
    this->publisher = publisher;
    this->safetyController = NULL;
    this->displacement = NULL;
    this->inclinometer = NULL;
    this->ilc = NULL;
    this->forceController = NULL;
    this->positionController = NULL;
    this->digitalInputOutput = digitalInputOutput;
    this->accelerometer = NULL;
    this->powerController = NULL;
    this->automaticOperationsController = NULL;
    this->gyro = NULL;
    this->cachedTimestamp = 0;
    pthread_mutex_init(&this->mutex, NULL);
    pthread_mutex_lock(&this->mutex);
}

Model::~Model() {
    pthread_mutex_unlock(&this->mutex);
    pthread_mutex_destroy(&this->mutex);

    delete this->safetyController;
    delete this->displacement;
    delete this->inclinometer;
    delete this->ilc;
    delete this->forceController;
    delete this->positionController;
    delete this->accelerometer;
    delete this->powerController;
    delete this->automaticOperationsController;
    delete this->gyro;
}

void Model::loadSettings(std::string settingsToApply) {
    spdlog::info("Model: loadSettings({})", settingsToApply);

    SettingReader& settingReader = SettingReader::get();
    settingReader.configure(settingsToApply);

    this->publisher->getOuterLoopData()->slewFlag = false;

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

    this->populateForceActuatorInfo(forceActuatorApplicationSettings, forceActuatorSettings);
    this->populateHardpointActuatorInfo(hardpointActuatorApplicationSettings, hardpointActuatorSettings,
                                        positionControllerSettings);
    this->populateHardpointMonitorInfo(hardpointMonitorApplicationSettings);

    delete this->safetyController;
    spdlog::info("Model: Creating safety controller");
    this->safetyController = new SafetyController(this->publisher, safetyControllerSettings);

    delete this->displacement;
    spdlog::info("Model: Creating displacement");
    this->displacement = new Displacement(displacementSensorSettings, IFPGA::get().getSupportFPGAData(),
                                          this->publisher, this->safetyController);

    delete this->inclinometer;
    spdlog::info("Model: Creating inclinometer");
    this->inclinometer = new Inclinometer(IFPGA::get().getSupportFPGAData(), this->publisher,
                                          this->safetyController, inclinometerSettings);

    delete this->positionController;
    spdlog::info("Model: Creating position controller");
    this->positionController =
            new PositionController(positionControllerSettings, hardpointActuatorSettings, this->publisher);

    delete this->ilc;
    spdlog::info("Model: Creating ILC");
    this->ilc = new ILC(this->publisher, this->positionController, ilcApplicationSettings,
                        forceActuatorApplicationSettings, forceActuatorSettings,
                        hardpointActuatorApplicationSettings, hardpointActuatorSettings,
                        hardpointMonitorApplicationSettings, this->safetyController);

    delete this->forceController;
    spdlog::info("Model: Creating force controller");
    this->forceController = new ForceController(forceActuatorApplicationSettings, forceActuatorSettings,
                                                pidSettings, this->publisher, this->safetyController);

    spdlog::info("Model: Updating digital input output");
    this->digitalInputOutput->setSafetyController(this->safetyController);

    delete this->accelerometer;
    spdlog::info("Model: Creating accelerometer");
    this->accelerometer = new Accelerometer(accelerometerSettings, this->publisher);

    delete this->powerController;
    spdlog::info("Model: Creating power controller");
    this->powerController = new PowerController(this->publisher, this->safetyController);

    delete this->automaticOperationsController;
    spdlog::info("Model: Creating automatic operations controller");
    this->automaticOperationsController =
            new AutomaticOperationsController(this->positionController, this->forceController,
                                              this->safetyController, this->publisher, this->powerController);

    delete this->gyro;
    spdlog::info("Model: Creating gyro");
    this->gyro = new Gyro(gyroSettings, this->publisher);

    spdlog::info("Model: Settings applied");
}

void Model::queryFPGAData() {}

void Model::publishStateChange(States::Type newState) {
    spdlog::debug("Model: publishStateChange({:d})", newState);
    uint64_t state = (uint64_t)newState;
    double timestamp = this->publisher->getTimestamp();
    MTM1M3_logevent_summaryStateC* summaryStateData = this->publisher->getEventSummaryState();
    // summaryStateData->timestamp = timestamp;
    summaryStateData->summaryState = (int32_t)((state & 0xFFFFFFFF00000000) >> 32);
    this->publisher->logSummaryState();
    MTM1M3_logevent_detailedStateC* detailedStateData = this->publisher->getEventDetailedState();
    detailedStateData->timestamp = timestamp;
    detailedStateData->detailedState = (int32_t)(state & 0x00000000FFFFFFFF);
    this->publisher->logDetailedState();
}

void Model::publishRecommendedSettings() {
    spdlog::debug("Model: publishRecommendedSettings()");
    RecommendedApplicationSettings* recommendedApplicationSettings =
            SettingReader::get().loadRecommendedApplicationSettings();
    MTM1M3_logevent_settingVersionsC* data = this->publisher->getEventSettingVersions();
    data->recommendedSettingsVersion = "";
    for (uint32_t i = 0; i < recommendedApplicationSettings->RecommendedSettings.size(); i++) {
        data->recommendedSettingsVersion += recommendedApplicationSettings->RecommendedSettings[i] + ",";
    }
    this->publisher->logSettingVersions();
}

void Model::publishOuterLoop(double executionTime) {
    spdlog::trace("Model: publishOuterLoop()");
    MTM1M3_outerLoopDataC* data = this->publisher->getOuterLoopData();
    data->executionTime = executionTime;
    this->publisher->putOuterLoopData();
}

void Model::shutdown() { pthread_mutex_unlock(&this->mutex); }

void Model::waitForShutdown() {
    pthread_mutex_lock(&this->mutex);
    pthread_mutex_unlock(&this->mutex);
}

void Model::populateForceActuatorInfo(ForceActuatorApplicationSettings* forceActuatorApplicationSettings,
                                      ForceActuatorSettings* forceActuatorSettings) {
    spdlog::debug("Model: populateForceActuatorInfo()");
    MTM1M3_logevent_forceActuatorInfoC* forceInfo = this->publisher->getEventForceActuatorInfo();
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

void Model::populateHardpointActuatorInfo(
        HardpointActuatorApplicationSettings* hardpointActuatorApplicationSettings,
        HardpointActuatorSettings* hardpointActuatorSettings,
        PositionControllerSettings* positionControllerSettings) {
    spdlog::debug("Model: populateHardpointActuatorInfo()");
    MTM1M3_logevent_hardpointActuatorInfoC* hardpointInfo = this->publisher->getEventHardpointActuatorInfo();
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

void Model::populateHardpointMonitorInfo(
        HardpointMonitorApplicationSettings* hardpointMonitorApplicationSettings) {
    spdlog::debug("Model: populateHardpointMonitorInfo()");
    MTM1M3_logevent_hardpointMonitorInfoC* hardpointMonitorInfo =
            this->publisher->getEventHardpointMonitorInfo();
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
