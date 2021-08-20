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

#include <SettingReader.h>
#include <boost/tokenizer.hpp>
#include <yaml-cpp/yaml.h>
#include <spdlog/spdlog.h>

#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

using namespace LSST::M1M3::SS;

SettingReader& SettingReader::get() {
    static SettingReader instance;
    return instance;
}

void SettingReader::setRootPath(std::string rootPath) {
    SPDLOG_DEBUG("SettingReader: setRootPath(\"{}\")", rootPath);

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

    _rootPath = rootPath;

    test_dir(_getBasePath(""));
    test_dir(_getSetPath(""));

    _currentSet = "";
    _currentVersion = "";
}

std::string SettingReader::getFilePath(std::string filename) {
    if (filename[0] == '/') return filename;
    return _rootPath + "/" + filename;
}

void SettingReader::configure(std::string settingsToApply) {
    SPDLOG_DEBUG("SettingReader: configure(\"{}\")", settingsToApply);
    _currentSet = "";
    _currentVersion = "";
    if (settingsToApply.find(',') != std::string::npos) {
        typedef boost::tokenizer<boost::escaped_list_separator<char> > tokenizer;
        tokenizer tokenize(settingsToApply);
        tokenizer::iterator token = tokenize.begin();
        _currentSet = *token;
        ++token;
        _currentVersion = *token;
    } else {
        AliasApplicationSettings* aliasApplicationSettings = loadAliasApplicationSettings();
        for (uint32_t i = 0; i != aliasApplicationSettings->Aliases.size(); ++i) {
            Alias alias = aliasApplicationSettings->Aliases[i];
            if (alias.Name.compare(settingsToApply) == 0) {
                _currentSet = alias.Set;
                _currentVersion = alias.Version;
                break;
            }
        }
    }
    if (_currentSet.empty()) {
        throw std::runtime_error("Cannot find configuration for " + settingsToApply + " settings");
    }

    if (_currentVersion.empty()) {
        throw std::runtime_error("Empty version for " + settingsToApply + " settings");
    }
}

AliasApplicationSettings* SettingReader::loadAliasApplicationSettings() {
    SPDLOG_DEBUG("SettingReader: loadAliasApplicationSettings()");
    _aliasApplicationSettings.load(_getBasePath("AliasApplicationSettings.yaml"));
    return &_aliasApplicationSettings;
}

ForceActuatorSettings* SettingReader::loadForceActuatorSettings() {
    SPDLOG_DEBUG("SettingReader: loadForceActuatorSettings()");
    _forceActuatorSettings.load(_getSetPath("ForceActuatorSettings.yaml"));
    return &_forceActuatorSettings;
}

HardpointActuatorApplicationSettings* SettingReader::loadHardpointActuatorApplicationSettings() {
    SPDLOG_DEBUG("SettingReader: loadHardpointActuatorApplicationSettings()");
    _hardpointActuatorApplicationSettings.load(
            _getBasePath("HardpointActuatorApplicationSettings.xml").c_str());
    return &_hardpointActuatorApplicationSettings;
}

HardpointActuatorSettings* SettingReader::loadHardpointActuatorSettings() {
    SPDLOG_DEBUG("SettingReader: loadHardpointActuatorSettings()");
    _hardpointActuatorSettings.load(_getSetPath("HardpointActuatorSettings.yaml"));
    return &_hardpointActuatorSettings;
}

ILCApplicationSettings* SettingReader::loadILCApplicationSettings() {
    SPDLOG_DEBUG("SettingReader: loadILCApplicationSettings()");
    _ilcApplicationSettings.load(_getBasePath("ILCApplicationSettings.yaml"));
    return &_ilcApplicationSettings;
}

RecommendedApplicationSettings* SettingReader::loadRecommendedApplicationSettings() {
    SPDLOG_DEBUG("SettingReader: loadRecommendedApplicationSettings()");
    _recommendedApplicationSettings.load(_getBasePath("RecommendedApplicationSettings.xml").c_str());
    return &_recommendedApplicationSettings;
}

SafetyControllerSettings* SettingReader::loadSafetyControllerSettings() {
    SPDLOG_DEBUG("SettingReader: loadSafetyControllerSettings()");
    _safetyControllerSettings.load(_getSetPath("SafetyControllerSettings.yaml"));
    return &_safetyControllerSettings;
}

PositionControllerSettings* SettingReader::loadPositionControllerSettings() {
    SPDLOG_DEBUG("SettingReader: loadPositionControllerSettings()");
    _positionControllerSettings.load(_getSetPath("PositionControllerSettings.yaml"));
    return &_positionControllerSettings;
}

AccelerometerSettings* SettingReader::loadAccelerometerSettings() {
    SPDLOG_DEBUG("SettingReader: loadAccelerometerSettings()");
    _accelerometerSettings.load(_getSetPath("AccelerometerSettings.yaml"));
    return &_accelerometerSettings;
}

DisplacementSensorSettings* SettingReader::loadDisplacementSensorSettings() {
    SPDLOG_DEBUG("SettingReader: loadDisplacementSensorSettings()");
    _displacementSensorSettings.load(_getSetPath("DisplacementSensorSettings.yaml"));
    return &_displacementSensorSettings;
}

HardpointMonitorApplicationSettings* SettingReader::loadHardpointMonitorApplicationSettings() {
    SPDLOG_DEBUG("SettingReader: loadHardpointMonitorApplicationSettings()");
    _hardpointMonitorApplicationSettings.load(_getBasePath("HardpointMonitorApplicationSettings.yaml"));
    return &_hardpointMonitorApplicationSettings;
}

GyroSettings* SettingReader::loadGyroSettings() {
    SPDLOG_DEBUG("SettingReader: loadGyroSettings()");
    _gyroSettings.load(_getSetPath("GyroSettings.yaml"));
    return &_gyroSettings;
}

ExpansionFPGAApplicationSettings* SettingReader::loadExpansionFPGAApplicationSettings() {
    SPDLOG_DEBUG("SettingReader: loadExpansionFPGAApplicationSettings()");
    _expansionFPGAApplicationSettings.load(_getBasePath("ExpansionFPGAApplicationSettings.yaml"));
    return &_expansionFPGAApplicationSettings;
}

PIDSettings* SettingReader::loadPIDSettings() {
    SPDLOG_DEBUG("SettingReader: loadPIDSettings()");
    _pidSettings.load(_getSetPath("PIDSettings.yaml"));
    return &_pidSettings;
}

InclinometerSettings* SettingReader::loadInclinometerSettings() {
    SPDLOG_DEBUG("SettingReader: loadInclinometerSettings()");
    _inclinometerSettings.load(_getSetPath("InclinometerSettings.yaml"));
    return &_inclinometerSettings;
}

std::string SettingReader::_getBasePath(std::string file) { return _rootPath + "/Base/" + file; }

std::string SettingReader::_getSetPath(std::string file) {
    return _rootPath + "/Sets/" + _currentSet + "/" + _currentVersion + "/" + file;
}
