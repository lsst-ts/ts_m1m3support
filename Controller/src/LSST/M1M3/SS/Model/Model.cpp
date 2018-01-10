/*
 * Model.cpp
 *
 *  Created on: Sep 26, 2017
 *      Author: ccontaxis
 */

#include <Model.h>
#include <ISettingReader.h>
#include <IPublisher.h>
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
#include <InterlockController.h>
#include <Accelerometer.h>
#include <AirController.h>
#include <ForceActuatorApplicationSettings.h>
#include <HardpointActuatorApplicationSettings.h>
#include <HardpointMonitorApplicationSettings.h>
#include <PowerController.h>
#include <AutomaticOperationsController.h>
#include <Gyro.h>

using namespace std;

namespace LSST {
namespace M1M3 {
namespace SS {

Model::Model(ISettingReader* settingReader, IPublisher* publisher, IFPGA* fpga, IExpansionFPGA* expansionFPGA, IInterlockController* interlockController) {
	this->settingReader = settingReader;
	this->publisher = publisher;
	this->fpga = fpga;
	this->expansionFPGA = expansionFPGA;
	this->safetyController = 0;
	this->displacement = 0;
	this->inclinometer = 0;
	this->ilc = 0;
	this->airController = 0;
	this->forceController = 0;
	this->positionController = 0;
	this->interlockController = interlockController;
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
	if (this->airController) {
		delete this->airController;
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
	this->settingReader->configure(settingsToApply);

	ILCApplicationSettings* ilcApplicationSettings = this->settingReader->loadILCApplicationSettings();
	ForceActuatorApplicationSettings* forceActuatorApplicationSettings = this->settingReader->loadForceActuatorApplicationSettings();
	ForceActuatorSettings* forceActuatorSettings = this->settingReader->loadForceActuatorSettings();
	HardpointActuatorApplicationSettings* hardpointActuatorApplicationSettings = this->settingReader->loadHardpointActuatorApplicationSettings();
	HardpointActuatorSettings* hardpointActuatorSettings = this->settingReader->loadHardpointActuatorSettings();
	SafetyControllerSettings* safetyControllerSettings = this->settingReader->loadSafetyControllerSettings();
	PositionControllerSettings* positionControllerSettings = this->settingReader->loadPositionControllerSettings();
	AccelerometerSettings* accelerometerSettings = this->settingReader->loadAccelerometerSettings();
	DisplacementSensorSettings* displacementSensorSettings = this->settingReader->loadDisplacementSensorSettings();
	HardpointMonitorApplicationSettings* hardpointMonitorApplicationSettings = this->settingReader->loadHardpointMonitorApplicationSettings();
	GyroSettings* gyroSettings = this->settingReader->loadGyroSettings();

	this->populateForceActuatorInfo(forceActuatorApplicationSettings, forceActuatorSettings);
	this->populateHardpointActuatorInfo(hardpointActuatorApplicationSettings, hardpointActuatorSettings);
	this->populateHardpointMonitorInfo(hardpointMonitorApplicationSettings);

	if (this->safetyController) {
		delete this->safetyController;
	}
	this->safetyController = new SafetyController(this->publisher, safetyControllerSettings);

	if (this->displacement) {
		delete this->displacement;
	}
	this->displacement = new Displacement(displacementSensorSettings, this->publisher, this->safetyController, this->fpga);

	if (this->inclinometer) {
		delete this->inclinometer;
	}
	this->inclinometer = new Inclinometer(this->publisher, this->safetyController, this->fpga);

	if (this->ilc) {
		delete this->ilc;
	}
	this->ilc = new ILC(this->publisher, this->fpga, ilcApplicationSettings, forceActuatorApplicationSettings, forceActuatorSettings, hardpointActuatorApplicationSettings, hardpointActuatorSettings, hardpointMonitorApplicationSettings);

	if (this->forceController) {
		delete this->forceController;
	}
	this->forceController = new ForceController(forceActuatorApplicationSettings, forceActuatorSettings, this->publisher, this->safetyController);

	if (this->airController) {
		delete this->airController;
	}
	this->airController = new AirController(this->publisher, this->safetyController, this->fpga);

	if (this->positionController) {
		delete this->positionController;
	}
	this->positionController = new PositionController(positionControllerSettings, hardpointActuatorSettings, this->publisher);

	this->interlockController->setSafetyController(this->safetyController);

	if (this->accelerometer) {
		delete this->accelerometer;
	}
	this->accelerometer = new Accelerometer(this->publisher, this->fpga, accelerometerSettings);

	if (this->powerController) {
		delete this->powerController;
	}
	this->powerController = new PowerController(this->publisher, this->fpga, this->expansionFPGA, this->safetyController);

	if (this->automaticOperationsController) {
		delete this->automaticOperationsController;
	}
	this->automaticOperationsController = new AutomaticOperationsController(this->positionController, this->forceController, this->interlockController, this->safetyController, this->publisher, this->powerController);

	if (this->gyro) {
		delete this->gyro;
	}
	this->gyro = new Gyro(gyroSettings, this->fpga, this->publisher);
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
	m1m3_logevent_SummaryStateC* data = this->publisher->getEventSummaryState();
	data->Timestamp = this->publisher->getTimestamp();
	data->SummaryState = newState;
	this->publisher->logSummaryState();
}

void Model::publishRecommendedSettings() {
	RecommendedApplicationSettings* recommendedApplicationSettings = this->settingReader->loadRecommendedApplicationSettings();
	m1m3_logevent_SettingVersionsC* data = this->publisher->getEventSettingVersions();
	data->Timestamp = this->publisher->getTimestamp();
	data->RecommendedSettingsVersion = "";
	for(uint32_t i = 0; i < recommendedApplicationSettings->RecommendedSettings.size(); i++) {
		data->RecommendedSettingsVersion += recommendedApplicationSettings->RecommendedSettings[i] + ",";
	}
	this->publisher->logSettingVersions();
}

void Model::shutdown() {
	pthread_mutex_unlock(&this->mutex);
}

void Model::waitForShutdown() {
	pthread_mutex_lock(&this->mutex);
	pthread_mutex_unlock(&this->mutex);
}

void Model::populateForceActuatorInfo(ForceActuatorApplicationSettings* forceActuatorApplicationSettings, ForceActuatorSettings* forceActuatorSettings) {
	m1m3_logevent_ForceActuatorInfoC* forceInfo = this->publisher->getEventForceActuatorInfo();
	for(int i = 0; i < FA_COUNT; i++) {
		ForceActuatorTableRow row = forceActuatorApplicationSettings->Table[i];
		forceInfo->ReferenceId[row.Index] = row.ActuatorID;
		forceInfo->ModbusSubnet[row.Index] = row.Subnet;
		forceInfo->ModbusAddress[row.Index] = row.Address;
		forceInfo->ActuatorType[row.Index] = row.Type;
		forceInfo->ActuatorOrientation[row.Index] = row.Orientation;
		forceInfo->XPosition[row.Index] = row.XPosition;
		forceInfo->YPosition[row.Index] = row.YPosition;
		forceInfo->ZPosition[row.Index] = row.ZPosition;
		forceInfo->PrimaryCylinderSensorOffset[row.Index] = row.PrimaryAxisSensorOffset;
		forceInfo->PrimaryCylinderSensorSensitivity[row.Index] = row.PrimaryAxisSensorSensitivity;
		forceInfo->SecondaryCylinderSensorOffset[row.Index] = row.SecondaryAxisSensorOffset;
		forceInfo->SecondaryCylinderSensorSensitivity[row.Index] = row.SecondaryAxisSensorSensitivity;
		forceInfo->StaticXSetpoint[row.Index] = forceActuatorSettings->StaticXForces[row.Index];
		forceInfo->StaticYSetpoint[row.Index] = forceActuatorSettings->StaticYForces[row.Index];
		forceInfo->StaticZSetpoint[row.Index] = forceActuatorSettings->StaticZForces[row.Index];
	}
}

void Model::populateHardpointActuatorInfo(HardpointActuatorApplicationSettings* hardpointActuatorApplicationSettings, HardpointActuatorSettings* hardpointActuatorSettings) {
	m1m3_logevent_HardpointActuatorInfoC* hardpointInfo = this->publisher->getEventHardpointActuatorInfo();
	for(int i = 0; i < HP_COUNT; i++) {
		HardpointActuatorTableRow row = hardpointActuatorApplicationSettings->Table[i];
		hardpointInfo->ReferenceId[row.Index] = row.ActuatorID;
		hardpointInfo->ModbusSubnet[row.Index] = row.Subnet;
		hardpointInfo->ModbusAddress[row.Index] = row.Address;
		hardpointInfo->XPosition[row.Index] = row.XPosition;
		hardpointInfo->YPosition[row.Index] = row.YPosition;
		hardpointInfo->ZPosition[row.Index] = row.ZPosition;
		hardpointInfo->SensorOffset[row.Index] = row.SensorOffset;
		hardpointInfo->SensorSensitivity[row.Index] = row.SensorSensitivity;
	}
}

void Model::populateHardpointMonitorInfo(HardpointMonitorApplicationSettings* hardpointMonitorApplicationSettings) {
	m1m3_logevent_HardpointMonitorInfoC* hardpointMonitorInfo = this->publisher->getEventHardpointMonitorInfo();
	for(int i = 0; i < HM_COUNT; i++) {
		HardpointMonitorTableRow row = hardpointMonitorApplicationSettings->Table[i];
		hardpointMonitorInfo->ReferenceId[row.Index] = row.ActuatorID;
		hardpointMonitorInfo->ModbusSubnet[row.Index] = row.Subnet;
		hardpointMonitorInfo->ModbusAddress[row.Index] = row.Address;
	}
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
