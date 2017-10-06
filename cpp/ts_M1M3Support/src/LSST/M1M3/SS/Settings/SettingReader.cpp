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
		AliasApplicationSettings aliasApplicationSettings = this->loadAliasApplicationSettings();
		for(uint32_t i = 0; i != aliasApplicationSettings.Aliases.size(); ++i) {
			Alias alias = aliasApplicationSettings.Aliases[i];
			if (alias.Name.compare(settingsToApply) == 0) {
				this->currentSet = alias.Set;
				this->currentVersion = alias.Version;
				break;
			}
		}
	}
}

AliasApplicationSettings SettingReader::loadAliasApplicationSettings() {
	AliasApplicationSettings aliasApplicationSettings;
	aliasApplicationSettings.load(this->getBasePath("AliasApplicationSettings.xml").c_str());
	return aliasApplicationSettings;
}

RecommendedApplicationSettings SettingReader::loadRecommendedApplicationSettings() {
	RecommendedApplicationSettings recommendedApplicationSettings;
	recommendedApplicationSettings.load(this->getBasePath("RecommendedApplicationSettings.xml").c_str());
	return recommendedApplicationSettings;
}

ILCApplicationSettings SettingReader::loadILCApplicationSettings() {
	ILCApplicationSettings ilcApplicationSettings;
	ilcApplicationSettings.load(this->getBasePath("ILCApplicationSettings.xml").c_str());
	return ilcApplicationSettings;
}

ForceActuatorApplicationSettings SettingReader::loadForceActuatorApplicationSettings() {
	ForceActuatorApplicationSettings forceActuatorApplicationSettings;
	forceActuatorApplicationSettings.load(this->getBasePath("ForceActuatorApplicationSettings.xml").c_str());
	return forceActuatorApplicationSettings;
}

HardpointActuatorApplicationSettings SettingReader::loadHardpointActuatorApplicationSettings() {
	HardpointActuatorApplicationSettings hardpointActuatorApplicationSettings;
	hardpointActuatorApplicationSettings.load(this->getBasePath("HardpointActuatorApplicationSettings.xml").c_str());
	return hardpointActuatorApplicationSettings;
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
