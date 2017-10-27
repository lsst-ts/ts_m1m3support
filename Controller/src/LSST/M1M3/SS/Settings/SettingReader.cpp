/*
 * SettingReader.cpp
 *
 *  Created on: Oct 3, 2017
 *      Author: ccontaxis
 */

#include <SettingReader.h>
#include <boost/tokenizer.hpp>

namespace LSST {
namespace M1M3 {
namespace SS {

SettingReader::SettingReader(std::string basePath, std::string setsPath) {
	this->basePath = basePath;
	this->setsPath = setsPath;
	this->currentSet = "";
	this->currentVersion = "";
}

void SettingReader::configure(std::string settingsToApply) {
	if (settingsToApply.find(',') != std::string::npos) {
		typedef boost::tokenizer< boost::escaped_list_separator<char> > tokenizer;
		tokenizer tokenize(settingsToApply);
		tokenizer::iterator token = tokenize.begin();
		this->currentSet = *token;
		++token;
		this->currentVersion = *token;
	}
	else {
		AliasApplicationSettings* aliasApplicationSettings = this->loadAliasApplicationSettings();
		for(uint32_t i = 0; i != aliasApplicationSettings->Aliases.size(); ++i) {
			Alias alias = aliasApplicationSettings->Aliases[i];
			if (alias.Name.compare(settingsToApply) == 0) {
				this->currentSet = alias.Set;
				this->currentVersion = alias.Version;
				break;
			}
		}
	}
}

AliasApplicationSettings* SettingReader::loadAliasApplicationSettings() {
	this->aliasApplicationSettings.load(this->getBasePath("AliasApplicationSettings.xml").c_str());
	return &this->aliasApplicationSettings;
}

ForceActuatorApplicationSettings* SettingReader::loadForceActuatorApplicationSettings() {
	this->forceActuatorApplicationSettings.load(this->getBasePath("ForceActuatorApplicationSettings.xml").c_str());
	return &this->forceActuatorApplicationSettings;
}

ForceActuatorSettings* SettingReader::loadForceActuatorSettings() {
	this->forceActuatorSettings.load(this->getSetPath("ForceActuatorSettings.xml").c_str());
	return &this->forceActuatorSettings;
}

HardpointActuatorApplicationSettings* SettingReader::loadHardpointActuatorApplicationSettings() {
	this->hardpointActuatorApplicationSettings.load(this->getBasePath("HardpointActuatorApplicationSettings.xml").c_str());
	return &this->hardpointActuatorApplicationSettings;
}

HardpointActuatorSettings* SettingReader::loadHardpointActuatorSettings() {
	return &this->hardpointActuatorSettings;
}

ILCApplicationSettings* SettingReader::loadILCApplicationSettings() {
	this->ilcApplicationSettings.load(this->getBasePath("ILCApplicationSettings.xml").c_str());
	return &this->ilcApplicationSettings;
}

RecommendedApplicationSettings* SettingReader::loadRecommendedApplicationSettings() {
	this->recommendedApplicationSettings.load(this->getBasePath("RecommendedApplicationSettings.xml").c_str());
	return &this->recommendedApplicationSettings;
}

SafetyControllerSettings* SettingReader::loadSafetyControllerSettings() {
	this->safetyControllerSettings.load(this->getSetPath("SafetyControllerSettings.xml").c_str());
	return &this->safetyControllerSettings;
}

std::string SettingReader::getBasePath(std::string file) {
	return this->basePath + file;
}

std::string SettingReader::getSetPath(std::string file) {
	return this->setsPath + this->currentSet + "/" + this->currentVersion + "/" + file;
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
