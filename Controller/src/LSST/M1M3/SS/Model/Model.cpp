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

using namespace std;

namespace LSST {
namespace M1M3 {
namespace SS {

Model::Model(ISettingReader* settingReader, IPublisher* publisher, IFPGA* fpga) {
	this->settingReader = settingReader;
	this->publisher = publisher;
	this->fpga = fpga;
	this->safetyController = 0;
	this->displacement = 0;
	this->inclinometer = 0;
	this->ilc = 0;
	this->airController = 0;
	this->forceController = 0;
	this->positionController = 0;
	this->interlockController = 0;
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
	if (this->interlockController) {
		delete this->interlockController;
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

	this->populateForceActuatorInfo(forceActuatorApplicationSettings, forceActuatorSettings);
	this->populateHardpointActuatorInfo(hardpointActuatorApplicationSettings, hardpointActuatorSettings);

	if (this->safetyController) {
		delete this->safetyController;
	}
	this->safetyController = new SafetyController(this->publisher, safetyControllerSettings);

	if (this->displacement) {
		delete this->displacement;
	}
	this->displacement = new Displacement(this->publisher, this->safetyController, this->fpga);

	if (this->inclinometer) {
		delete this->inclinometer;
	}
	this->inclinometer = new Inclinometer(this->publisher, this->safetyController, this->fpga);

	if (this->ilc) {
		delete this->ilc;
	}
	this->ilc = new ILC(this->publisher, this->fpga, ilcApplicationSettings, forceActuatorApplicationSettings, forceActuatorSettings, hardpointActuatorApplicationSettings, hardpointActuatorSettings);

	if (this->forceController) {
		delete this->forceController;
	}
	this->forceController = new ForceController(forceActuatorApplicationSettings, forceActuatorSettings, this->publisher);

	if (this->airController) {
		delete this->airController;
	}
	this->airController = new AirController(this->publisher, this->safetyController, this->fpga);

	if (this->positionController) {
		delete this->positionController;
	}
	this->positionController = new PositionController(positionControllerSettings, this->publisher);

	if (this->interlockController) {
		delete this->interlockController;
	}
	this->interlockController = new InterlockController(this->publisher, this->safetyController, this->fpga);
}

void Model::queryFPGAData() {

}

void Model::publishFPGAData() {
	uint16_t response[512];
	this->fpga->writeRequestFIFO(FPGAAddresses::HealthAndStatus, 0);
	this->fpga->readU16ResponseFIFO(response, 64*4, 20);
	for(int i = 0; i < 25; i++) {
		cout << U16ArrayUtilities::u64(response, i * 4) << " ";
	}
	cout << endl;
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
	data->Timestamp = Timestamp::currentTime();
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

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
