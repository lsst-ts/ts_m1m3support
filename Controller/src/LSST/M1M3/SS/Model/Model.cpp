/*
 * Model.cpp
 *
 *  Created on: Sep 26, 2017
 *      Author: ccontaxis
 */

#include <Model.h>
#include <ISettingReader.h>
#include <IPublisher.h>
#include <RS232.h>
#include <ILC.h>
#include <Timestamp.h>
#include <ILCApplicationSettings.h>
#include <iostream>
#include <IFPGA.h>

using namespace std;

namespace LSST {
namespace M1M3 {
namespace SS {

Model::Model(ISettingReader* settingReader, IPublisher* publisher, IFPGA* fpga) {
	this->settingReader = settingReader;
	this->publisher = publisher;
	this->fpga = fpga;
	this->rs232 = 0;
	this->ilc = 0;
	pthread_mutex_init(&this->mutex, NULL);
	pthread_mutex_lock(&this->mutex);
}

Model::~Model() {
	pthread_mutex_unlock(&this->mutex);
	pthread_mutex_destroy(&this->mutex);
	if (this->rs232) {
		delete this->rs232;
	}
	if (this->ilc) {
		delete this->ilc;
	}
}

ISettingReader* Model::getSettingReader() {
	return this->settingReader;
}

IPublisher* Model::getPublisher() {
	return this->publisher;
}

IFPGA* Model::getFPGA() {
	return this->fpga;
}

IRS232* Model::getRS232() {
	return this->rs232;
}

IILC* Model::getILC() {
	return this->ilc;
}

void Model::loadSettings(std::string settingsToApply) {
	this->settingReader->configure(settingsToApply);
	if (this->rs232) {
		delete this->rs232;
	}
	this->rs232 = new RS232(this->fpga, this->publisher);
	ILCApplicationSettings ilcApplicationSettings = this->settingReader->loadILCApplicationSettings();
	ForceActuatorApplicationSettings forceActuatorApplicationSettings = this->settingReader->loadForceActuatorApplicationSettings();
	HardpointActuatorApplicationSettings hardpointActuatorApplicationSettings = this->settingReader->loadHardpointActuatorApplicationSettings();
	if (this->ilc) {
		delete this->ilc;
	}
	this->ilc = new ILC(this->publisher, this->fpga, ilcApplicationSettings, forceActuatorApplicationSettings, hardpointActuatorApplicationSettings);
}

void Model::queryFPGAData() {
	uint64_t buffer[] = { 2, 0 };
	this->fpga->writeHealthAndStatusFIFO(buffer, 2, 0);
}

void Model::publishFPGAData() {
	int32_t available = this->fpga->readHealthAndStatusFIFOAvailable();
	if (available > 0) {
		uint64_t buffer[available];
		this->fpga->readHealthAndStatusFIFO(buffer, available, 0);
	}
	else {
		cout << "No FPGA Data Available" << endl;
	}
}

void Model::publishStateChange(States::Type newState) {
	m1m3_logevent_SummaryStateC data;
	data.Timestamp = Timestamp::currentTime();
	data.SummaryState = newState;
	this->publisher->logStateChange(&data);
}

void Model::publishRecommendedSettings() {
	RecommendedApplicationSettings recommendedApplicationSettings = this->settingReader->loadRecommendedApplicationSettings();
	m1m3_logevent_SettingVersionsC data;
	data.Timestamp = Timestamp::currentTime();
	data.RecommendedSettingsVersion = "";
	for(uint32_t i = 0; i < recommendedApplicationSettings.RecommendedSettings.size(); i++) {
		data.RecommendedSettingsVersion += recommendedApplicationSettings.RecommendedSettings[i] + ",";
	}
	this->publisher->logSettingVersions(&data);
}

void Model::shutdown() {
	pthread_mutex_unlock(&this->mutex);
}

void Model::waitForShutdown() {
	pthread_mutex_lock(&this->mutex);
	pthread_mutex_unlock(&this->mutex);
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
