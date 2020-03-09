/*
 * Model.cpp
 *
 *  Created on: Sep 26, 2017
 *      Author: ccontaxis
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
#include <FPGA.h>
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
#include <Log.h>
#include <DigitalInputOutput.h>

using namespace std;

namespace LSST {
namespace M1M3 {
namespace SS {

Model::Model(SettingReader* settingReader, M1M3SSPublisher* publisher, FPGA* fpga, ExpansionFPGA* expansionFPGA, DigitalInputOutput* digitalInputOutput) {
	Log.Debug("Model: Model()");
	this->settingReader = settingReader;
	this->publisher = publisher;
	this->fpga = fpga;
	this->expansionFPGA = expansionFPGA;
	this->safetyController = 0;
	this->displacement = 0;
	this->inclinometer = 0;
	this->ilc = 0;
	this->forceController = 0;
	this->positionController = 0;
	this->digitalInputOutput = digitalInputOutput;
	this->accelerometer = 0;
	this->powerController = 0;
	this->automaticOperationsController = 0;
	this->gyro = 0;
	this->cachedTimestamp = 0;
	pthread_mutex_init(&this->mutex, NULL);
	pthread_mutex_lock(&this->mutex);
}

Model::~Model() {
	pthread_mutex_unlock(&this->mutex);
	pthread_mutex_destroy(&this->mutex);

	if (this->safetyController) {
		delete this->safetyController;
	}
	if (this->displacement) {
		delete this->displacement;
	}
	if (this->inclinometer) {
		delete this->inclinometer;
	}
	if (this->ilc) {
		delete this->ilc;
	}
	if (this->forceController) {
		delete this->forceController;
	}
	if (this->positionController) {
		delete this->positionController;
	}
	if (this->accelerometer) {
		delete this->accelerometer;
	}
	if (this->powerController) {
		delete this->powerController;
	}
	if (this->automaticOperationsController) {
		delete this->automaticOperationsController;
	}
	if (this->gyro) {
		delete this->gyro;
	}
}

void Model::loadSettings(std::string settingsToApply) {
	Log.Info("Model: loadSettings(%s)", settingsToApply.c_str());
	this->settingReader->configure(settingsToApply);

	this->publisher->getOuterLoopData()->slewFlag = false;

	Log.Info("Model: Loading ILC application settings");
	ILCApplicationSettings* ilcApplicationSettings = this->settingReader->loadILCApplicationSettings();
	Log.Info("Model: Loading force actuator application settings");
	ForceActuatorApplicationSettings* forceActuatorApplicationSettings = this->settingReader->loadForceActuatorApplicationSettings();
	Log.Info("Model: Loading force actuator settings");
	ForceActuatorSettings* forceActuatorSettings = this->settingReader->loadForceActuatorSettings();
	Log.Info("Model: Loading hardpoint actuator application settings");
	HardpointActuatorApplicationSettings* hardpointActuatorApplicationSettings = this->settingReader->loadHardpointActuatorApplicationSettings();
	Log.Info("Model: Loading hardpoint actuator settings");
	HardpointActuatorSettings* hardpointActuatorSettings = this->settingReader->loadHardpointActuatorSettings();
	Log.Info("Model: Loading safety controller settings");
	SafetyControllerSettings* safetyControllerSettings = this->settingReader->loadSafetyControllerSettings();
	Log.Info("Model: Loading position controller settings");
	PositionControllerSettings* positionControllerSettings = this->settingReader->loadPositionControllerSettings();
	Log.Info("Model: Loading accelerometer settings");
	AccelerometerSettings* accelerometerSettings = this->settingReader->loadAccelerometerSettings();
	Log.Info("Model: Loading displacement settings");
	DisplacementSensorSettings* displacementSensorSettings = this->settingReader->loadDisplacementSensorSettings();
	Log.Info("Model: Loading hardpoint monitor application settings");
	HardpointMonitorApplicationSettings* hardpointMonitorApplicationSettings = this->settingReader->loadHardpointMonitorApplicationSettings();
	Log.Info("Model: Loading gyro settings");
	GyroSettings* gyroSettings = this->settingReader->loadGyroSettings();
	Log.Info("Model: Loading PID settings");
	PIDSettings* pidSettings = this->settingReader->loadPIDSettings();
	Log.Info("Model: Loading inclinometer settings");
	InclinometerSettings* inclinometerSettings = this->settingReader->loadInclinometerSettings();

	this->populateForceActuatorInfo(forceActuatorApplicationSettings, forceActuatorSettings);
	this->populateHardpointActuatorInfo(hardpointActuatorApplicationSettings, hardpointActuatorSettings, positionControllerSettings);
	this->populateHardpointMonitorInfo(hardpointMonitorApplicationSettings);

	if (this->safetyController) {
		Log.Debug("Model: Deleting safety controller");
		delete this->safetyController;
	}
	Log.Info("Model: Creating safety controller");
	this->safetyController = new SafetyController(this->publisher, safetyControllerSettings);

	if (this->displacement) {
		Log.Debug("Model: Deleting displacement");
		delete this->displacement;
	}
	Log.Info("Model: Creating displacement");
	this->displacement = new Displacement(displacementSensorSettings, this->fpga->getSupportFPGAData(), this->publisher, this->safetyController);

	if (this->inclinometer) {
		Log.Debug("Model: Deleting inclinometer");
		delete this->inclinometer;
	}
	Log.Info("Model: Creating inclinometer");
	this->inclinometer = new Inclinometer(this->fpga->getSupportFPGAData(), this->publisher, this->safetyController, inclinometerSettings);

	if (this->positionController) {
		Log.Debug("Model: Deleting position controller");
		delete this->positionController;
	}
	Log.Info("Model: Creating position controller");
	this->positionController = new PositionController(positionControllerSettings, hardpointActuatorSettings, this->publisher);

	if (this->ilc) {
		Log.Debug("Model: Deleting ILC");
		delete this->ilc;
	}
	Log.Info("Model: Creating ILC");
	this->ilc = new ILC(this->publisher, this->fpga, this->positionController, ilcApplicationSettings, forceActuatorApplicationSettings, forceActuatorSettings, hardpointActuatorApplicationSettings, hardpointActuatorSettings, hardpointMonitorApplicationSettings, this->safetyController);

	if (this->forceController) {
		Log.Debug("Model: Deleting force controller");
		delete this->forceController;
	}
	Log.Info("Model: Creating force controller");
	this->forceController = new ForceController(forceActuatorApplicationSettings, forceActuatorSettings, pidSettings, this->publisher, this->safetyController);

	Log.Info("Model: Updating digital input output");
	this->digitalInputOutput->setSafetyController(this->safetyController);

	if (this->accelerometer) {
		Log.Debug("Model: Deleting accelerometer");
		delete this->accelerometer;
	}
	Log.Info("Model: Creating accelerometer");
	this->accelerometer = new Accelerometer(accelerometerSettings, this->fpga->getSupportFPGAData(), this->publisher);

	if (this->powerController) {
		Log.Debug("Model: Deleting power controller");
		delete this->powerController;
	}
	Log.Info("Model: Creating power controller");
	this->powerController = new PowerController(this->fpga, this->expansionFPGA, this->publisher, this->safetyController);

	if (this->automaticOperationsController) {
		Log.Debug("Model: Deleting automatic operations controller");
		delete this->automaticOperationsController;
	}
	Log.Info("Model: Creating automatic operations controller");
	this->automaticOperationsController = new AutomaticOperationsController(this->positionController, this->forceController, this->safetyController, this->publisher, this->powerController);

	if (this->gyro) {
		Log.Debug("Model: Deleting gyro");
		delete this->gyro;
	}
	Log.Info("Model: Creating gyro");
	this->gyro = new Gyro(gyroSettings, this->fpga, this->publisher);

	Log.Info("Model: Settings applied");
}

void Model::queryFPGAData() {

}

void Model::publishFPGAData() {
	uint16_t response[512];
	this->fpga->writeRequestFIFO(FPGAAddresses::HealthAndStatus, 0);
	this->fpga->readU16ResponseFIFO(response, 64*4, 20);
	/*for(int i = 0; i < 25; i++) {
		cout << U16ArrayUtilities::u64(response, i * 4) << " ";
	}
	cout << endl;*/
}

void Model::publishStateChange(States::Type newState) {
	Log.Debug("Model: publishStateChange(%d)", newState);
	uint64_t state = (uint64_t)newState;
	double timestamp = this->publisher->getTimestamp();
	MTM1M3_logevent_summaryStateC* summaryStateData = this->publisher->getEventSummaryState();
	summaryStateData->timestamp = timestamp;
	summaryStateData->summaryState = (int32_t)((state & 0xFFFFFFFF00000000) >> 32);
	this->publisher->logSummaryState();
	MTM1M3_logevent_detailedStateC* detailedStateData = this->publisher->getEventDetailedState();
	detailedStateData->timestamp = timestamp;
	detailedStateData->detailedState = (int32_t)(state & 0x00000000FFFFFFFF);
	this->publisher->logDetailedState();
}

void Model::publishRecommendedSettings() {
	Log.Debug("Model: publishRecommendedSettings()");
	RecommendedApplicationSettings* recommendedApplicationSettings = this->settingReader->loadRecommendedApplicationSettings();
	MTM1M3_logevent_settingVersionsC* data = this->publisher->getEventSettingVersions();
	data->timestamp = this->publisher->getTimestamp();
	data->recommendedSettingsVersion = "";
	for(uint32_t i = 0; i < recommendedApplicationSettings->RecommendedSettings.size(); i++) {
		data->recommendedSettingsVersion += recommendedApplicationSettings->RecommendedSettings[i] + ",";
	}
	this->publisher->logSettingVersions();
}

void Model::publishOuterLoop(double executionTime) {
	Log.Trace("Model: publishOuterLoop()");
	MTM1M3_outerLoopDataC* data = this->publisher->getOuterLoopData();
	data->timestamp = this->publisher->getTimestamp();
	data->executionTime = executionTime;
	this->publisher->putOuterLoopData();
}

void Model::shutdown() {
	pthread_mutex_unlock(&this->mutex);
}

void Model::waitForShutdown() {
	pthread_mutex_lock(&this->mutex);
	pthread_mutex_unlock(&this->mutex);
}

void Model::populateForceActuatorInfo(ForceActuatorApplicationSettings* forceActuatorApplicationSettings, ForceActuatorSettings* forceActuatorSettings) {
	Log.Debug("Model: populateForceActuatorInfo()");
	MTM1M3_logevent_forceActuatorInfoC* forceInfo = this->publisher->getEventForceActuatorInfo();
	for(int i = 0; i < FA_COUNT; i++) {
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

void Model::populateHardpointActuatorInfo(HardpointActuatorApplicationSettings* hardpointActuatorApplicationSettings, HardpointActuatorSettings* hardpointActuatorSettings, PositionControllerSettings* positionControllerSettings) {
	Log.Debug("Model: populateHardpointActuatorInfo()");
	MTM1M3_logevent_hardpointActuatorInfoC* hardpointInfo = this->publisher->getEventHardpointActuatorInfo();
	for(int i = 0; i < HP_COUNT; i++) {
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

void Model::populateHardpointMonitorInfo(HardpointMonitorApplicationSettings* hardpointMonitorApplicationSettings) {
	Log.Debug("Model: populateHardpointMonitorInfo()");
	MTM1M3_logevent_hardpointMonitorInfoC* hardpointMonitorInfo = this->publisher->getEventHardpointMonitorInfo();
	for(int i = 0; i < HM_COUNT; i++) {
		HardpointMonitorTableRow row = hardpointMonitorApplicationSettings->Table[i];
		hardpointMonitorInfo->referenceId[row.Index] = row.ActuatorID;
		hardpointMonitorInfo->modbusSubnet[row.Index] = row.Subnet;
		hardpointMonitorInfo->modbusAddress[row.Index] = row.Address;
	}
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
