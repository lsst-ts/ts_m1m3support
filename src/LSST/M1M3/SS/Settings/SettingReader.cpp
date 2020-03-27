/*
 * SettingReader.cpp
 *
 *  Created on: Oct 3, 2017
 *      Author: ccontaxis
 */

#include <SettingReader.h>
#include <boost/tokenizer.hpp>
#include <Log.h>

#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

namespace LSST {
namespace M1M3 {
namespace SS {

SettingReader& SettingReader::get() {
    static SettingReader instance;
    return instance;
}

void SettingReader::setRootPath(std::string rootPath) {
    Log.Debug("SettingReader: setRootPath(\"%s\")", rootPath.c_str());

    auto test_dir = [rootPath](std::string dir) {
        struct stat dirstat;
        if (stat(dir.c_str(), &dirstat)) {
            throw std::runtime_error("Directory " + rootPath + "doesn't exist: " + strerror(errno));
        }
        if (!(dirstat.st_mode & (S_IFLNK | S_IFDIR))) {
            throw std::runtime_error(rootPath + " isn't directory or link");
        }
    };

    test_dir(rootPath);

    this->rootPath = rootPath;

    test_dir(getBasePath(""));
    test_dir(getSetPath(""));

    this->currentSet = "";
    this->currentVersion = "";
}

std::string SettingReader::getFilePath(std::string filename) {
    if (filename[0] == '/') return filename;
    return this->rootPath + "/" + filename;
}

void SettingReader::configure(std::string settingsToApply) {
    Log.Debug("SettingReader: configure(\"%s\")", settingsToApply.c_str());
    if (settingsToApply.find(',') != std::string::npos) {
        typedef boost::tokenizer<boost::escaped_list_separator<char> > tokenizer;
        tokenizer tokenize(settingsToApply);
        tokenizer::iterator token = tokenize.begin();
        this->currentSet = *token;
        ++token;
        this->currentVersion = *token;
    } else {
        AliasApplicationSettings* aliasApplicationSettings = loadAliasApplicationSettings();
        for (uint32_t i = 0; i != aliasApplicationSettings->Aliases.size(); ++i) {
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
    Log.Debug("SettingReader: loadAliasApplicationSettings()");
    this->aliasApplicationSettings.load(this->getBasePath("AliasApplicationSettings.xml").c_str());
    return &this->aliasApplicationSettings;
}

ForceActuatorApplicationSettings* SettingReader::loadForceActuatorApplicationSettings() {
    Log.Debug("SettingReader: loadForceActuatorApplicationSettings()");
    this->forceActuatorApplicationSettings.load(
            this->getBasePath("ForceActuatorApplicationSettings.xml").c_str());
    return &this->forceActuatorApplicationSettings;
}

ForceActuatorSettings* SettingReader::loadForceActuatorSettings() {
    Log.Debug("SettingReader: loadForceActuatorSettings()");
    this->forceActuatorSettings.load(this->getSetPath("ForceActuatorSettings.xml").c_str());
    return &this->forceActuatorSettings;
}

HardpointActuatorApplicationSettings* SettingReader::loadHardpointActuatorApplicationSettings() {
    Log.Debug("SettingReader: loadHardpointActuatorApplicationSettings()");
    this->hardpointActuatorApplicationSettings.load(
            this->getBasePath("HardpointActuatorApplicationSettings.xml").c_str());
    return &this->hardpointActuatorApplicationSettings;
}

HardpointActuatorSettings* SettingReader::loadHardpointActuatorSettings() {
    Log.Debug("SettingReader: loadHardpointActuatorSettings()");
    this->hardpointActuatorSettings.load(this->getSetPath("HardpointActuatorSettings.xml").c_str());
    return &this->hardpointActuatorSettings;
}

ILCApplicationSettings* SettingReader::loadILCApplicationSettings() {
    Log.Debug("SettingReader: loadILCApplicationSettings()");
    this->ilcApplicationSettings.load(this->getBasePath("ILCApplicationSettings.xml").c_str());
    return &this->ilcApplicationSettings;
}

RecommendedApplicationSettings* SettingReader::loadRecommendedApplicationSettings() {
    Log.Debug("SettingReader: loadRecommendedApplicationSettings()");
    this->recommendedApplicationSettings.load(
            this->getBasePath("RecommendedApplicationSettings.xml").c_str());
    return &this->recommendedApplicationSettings;
}

SafetyControllerSettings* SettingReader::loadSafetyControllerSettings() {
    Log.Debug("SettingReader: loadSafetyControllerSettings()");
    this->safetyControllerSettings.load(this->getSetPath("SafetyControllerSettings.xml").c_str());
    return &this->safetyControllerSettings;
}

PositionControllerSettings* SettingReader::loadPositionControllerSettings() {
    Log.Debug("SettingReader: loadPositionControllerSettings()");
    this->positionControllerSettings.load(this->getSetPath("PositionControllerSettings.xml").c_str());
    return &this->positionControllerSettings;
}

AccelerometerSettings* SettingReader::loadAccelerometerSettings() {
    Log.Debug("SettingReader: loadAccelerometerSettings()");
    this->accelerometerSettings.load(this->getSetPath("AccelerometerSettings.xml").c_str());
    return &this->accelerometerSettings;
}

DisplacementSensorSettings* SettingReader::loadDisplacementSensorSettings() {
    Log.Debug("SettingReader: loadDisplacementSensorSettings()");
    this->displacementSensorSettings.load(this->getSetPath("DisplacementSensorSettings.xml").c_str());
    return &this->displacementSensorSettings;
}

HardpointMonitorApplicationSettings* SettingReader::loadHardpointMonitorApplicationSettings() {
    Log.Debug("SettingReader: loadHardpointMonitorApplicationSettings()");
    this->hardpointMonitorApplicationSettings.load(
            this->getBasePath("HardpointMonitorApplicationSettings.xml").c_str());
    return &this->hardpointMonitorApplicationSettings;
}

InterlockApplicationSettings* SettingReader::loadInterlockApplicationSettings() {
    Log.Debug("SettingReader: loadInterlockApplicationSettings()");
    this->interlockApplicationSettings.load(this->getBasePath("InterlockApplicationSettings.xml").c_str());
    return &this->interlockApplicationSettings;
}

GyroSettings* SettingReader::loadGyroSettings() {
    Log.Debug("SettingReader: loadGyroSettings()");
    this->gyroSettings.load(this->getSetPath("GyroSettings.xml").c_str());
    return &this->gyroSettings;
}

ExpansionFPGAApplicationSettings* SettingReader::loadExpansionFPGAApplicationSettings() {
    Log.Debug("SettingReader: loadExpansionFPGAApplicationSettings()");
    this->expansionFPGAApplicationSettings.load(
            this->getBasePath("ExpansionFPGAApplicationSettings.xml").c_str());
    return &this->expansionFPGAApplicationSettings;
}

PIDSettings* SettingReader::loadPIDSettings() {
    Log.Debug("SettingReader: loadPIDSettings()");
    this->pidSettings.load(this->getSetPath("PIDSettings.xml"));
    return &this->pidSettings;
}

InclinometerSettings* SettingReader::loadInclinometerSettings() {
    Log.Debug("SettingReader: loadInclinometerSettings()");
    this->inclinometerSettings.load(this->getSetPath("InclinometerSettings.xml"));
    return &this->inclinometerSettings;
}

std::string SettingReader::getBasePath(std::string file) { return this->rootPath + "/Base/" + file; }

std::string SettingReader::getSetPath(std::string file) {
    return this->rootPath + "/Sets/" + this->currentSet + "/" + this->currentVersion + "/" + file;
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
