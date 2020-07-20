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
#include <spdlog/spdlog.h>

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
    spdlog::debug("SettingReader: setRootPath(\"{}\")", rootPath);

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
    spdlog::debug("SettingReader: configure(\"{}\")", settingsToApply);
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
    spdlog::debug("SettingReader: loadAliasApplicationSettings()");
    _aliasApplicationSettings.load(_getBasePath("AliasApplicationSettings.xml").c_str());
    return &_aliasApplicationSettings;
}

ForceActuatorApplicationSettings* SettingReader::loadForceActuatorApplicationSettings() {
    spdlog::debug("SettingReader: loadForceActuatorApplicationSettings()");
    _forceActuatorApplicationSettings.load(_getBasePath("ForceActuatorApplicationSettings.xml").c_str());
    return &_forceActuatorApplicationSettings;
}

ForceActuatorSettings* SettingReader::loadForceActuatorSettings() {
    spdlog::debug("SettingReader: loadForceActuatorSettings()");
    _forceActuatorSettings.load(_getSetPath("ForceActuatorSettings.xml").c_str());
    return &_forceActuatorSettings;
}

HardpointActuatorApplicationSettings* SettingReader::loadHardpointActuatorApplicationSettings() {
    spdlog::debug("SettingReader: loadHardpointActuatorApplicationSettings()");
    _hardpointActuatorApplicationSettings.load(
            _getBasePath("HardpointActuatorApplicationSettings.xml").c_str());
    return &_hardpointActuatorApplicationSettings;
}

HardpointActuatorSettings* SettingReader::loadHardpointActuatorSettings() {
    spdlog::debug("SettingReader: loadHardpointActuatorSettings()");
    _hardpointActuatorSettings.load(_getSetPath("HardpointActuatorSettings.xml").c_str());
    return &_hardpointActuatorSettings;
}

ILCApplicationSettings* SettingReader::loadILCApplicationSettings() {
    spdlog::debug("SettingReader: loadILCApplicationSettings()");
    _ilcApplicationSettings.load(_getBasePath("ILCApplicationSettings.xml").c_str());
    return &_ilcApplicationSettings;
}

RecommendedApplicationSettings* SettingReader::loadRecommendedApplicationSettings() {
    spdlog::debug("SettingReader: loadRecommendedApplicationSettings()");
    _recommendedApplicationSettings.load(_getBasePath("RecommendedApplicationSettings.xml").c_str());
    return &_recommendedApplicationSettings;
}

SafetyControllerSettings* SettingReader::loadSafetyControllerSettings() {
    spdlog::debug("SettingReader: loadSafetyControllerSettings()");
    _safetyControllerSettings.load(_getSetPath("SafetyControllerSettings.xml").c_str());
    return &_safetyControllerSettings;
}

PositionControllerSettings* SettingReader::loadPositionControllerSettings() {
    spdlog::debug("SettingReader: loadPositionControllerSettings()");
    _positionControllerSettings.load(_getSetPath("PositionControllerSettings.xml").c_str());
    return &_positionControllerSettings;
}

AccelerometerSettings* SettingReader::loadAccelerometerSettings() {
    spdlog::debug("SettingReader: loadAccelerometerSettings()");
    _accelerometerSettings.load(_getSetPath("AccelerometerSettings.xml").c_str());
    return &_accelerometerSettings;
}

DisplacementSensorSettings* SettingReader::loadDisplacementSensorSettings() {
    spdlog::debug("SettingReader: loadDisplacementSensorSettings()");
    _displacementSensorSettings.load(_getSetPath("DisplacementSensorSettings.xml").c_str());
    return &_displacementSensorSettings;
}

HardpointMonitorApplicationSettings* SettingReader::loadHardpointMonitorApplicationSettings() {
    spdlog::debug("SettingReader: loadHardpointMonitorApplicationSettings()");
    _hardpointMonitorApplicationSettings.load(
            _getBasePath("HardpointMonitorApplicationSettings.xml").c_str());
    return &_hardpointMonitorApplicationSettings;
}

InterlockApplicationSettings* SettingReader::loadInterlockApplicationSettings() {
    spdlog::debug("SettingReader: loadInterlockApplicationSettings()");
    _interlockApplicationSettings.load(_getBasePath("InterlockApplicationSettings.xml").c_str());
    return &_interlockApplicationSettings;
}

GyroSettings* SettingReader::loadGyroSettings() {
    spdlog::debug("SettingReader: loadGyroSettings()");
    _gyroSettings.load(_getSetPath("GyroSettings.xml").c_str());
    return &_gyroSettings;
}

ExpansionFPGAApplicationSettings* SettingReader::loadExpansionFPGAApplicationSettings() {
    spdlog::debug("SettingReader: loadExpansionFPGAApplicationSettings()");
    _expansionFPGAApplicationSettings.load(_getBasePath("ExpansionFPGAApplicationSettings.xml").c_str());
    return &_expansionFPGAApplicationSettings;
}

PIDSettings* SettingReader::loadPIDSettings() {
    spdlog::debug("SettingReader: loadPIDSettings()");
    _pidSettings.load(_getSetPath("PIDSettings.xml"));
    return &_pidSettings;
}

InclinometerSettings* SettingReader::loadInclinometerSettings() {
    spdlog::debug("SettingReader: loadInclinometerSettings()");
    _inclinometerSettings.load(_getSetPath("InclinometerSettings.xml"));
    return &_inclinometerSettings;
}

std::string SettingReader::_getBasePath(std::string file) { return _rootPath + "/Base/" + file; }

std::string SettingReader::_getSetPath(std::string file) {
    return _rootPath + "/Sets/" + _currentSet + "/" + _currentVersion + "/" + file;
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
