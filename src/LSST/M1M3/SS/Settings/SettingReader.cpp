/*
 * This file is part of LSST M1M3 support system package.
 *
 * Developed for the Vera C. Rubin Telescope and Site System.
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

#include <dirent.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include <yaml-cpp/yaml.h>
#include <spdlog/spdlog.h>

#include <AccelerometerSettings.h>
#include <DisplacementSensorSettings.h>
#include <ExpansionFPGAApplicationSettings.h>
#include <ForceActuatorSettings.h>
#include <GyroSettings.h>
#include <ILCApplicationSettings.h>
#include <InclinometerSettings.h>
#include <HardpointActuatorSettings.h>
#include <PositionControllerSettings.h>
#include <SettingReader.h>

extern const char* CONFIG_SCHEMA_VERSION;

using namespace LSST::M1M3::SS;

auto test_dir = [](std::string dir) {
    struct stat dirstat;
    if (stat(dir.c_str(), &dirstat)) {
        throw std::runtime_error("Directory " + dir + " cannot be accessed: " + strerror(errno));
    }
    if (!(dirstat.st_mode & (S_IFLNK | S_IFDIR))) {
        throw std::runtime_error(dir + " isn't directory or link");
    }
};

void SettingReader::setRootPath(std::string rootPath) {
    if (rootPath[0] != '/') {
        char* cwd = getcwd(NULL, 0);
        rootPath = std::string(cwd) + "/" + rootPath;
        free(cwd);
    }
    SPDLOG_DEBUG("Configuration absolute path: {}", rootPath);

    test_dir(rootPath);

    _rootPath = rootPath;
    _currentSet = "";

    test_dir(_getSetPath(""));
    test_dir(getTablePath(""));
}

std::string SettingReader::getTablePath(std::string filename) {
    if (filename[0] == '/') return filename;
    return _getSetPath("tables/" + filename);
}

std::list<std::string> SettingReader::getAvailableConfigurations() {
    std::list<std::string> ret;
    auto setdir = _getSetPath("");
    auto dirp = opendir(setdir.c_str());
    if (dirp == NULL) {
        throw std::runtime_error("Directory " + setdir + " cannot be opened: " + strerror(errno));
    }
    dirent* de;
    while ((de = readdir(dirp)) != NULL) {
        if ((de->d_type & DT_REG) == DT_REG && de->d_name[0] != '.' && de->d_name[0] != '_') {
            ret.push_back(de->d_name);
        }
    }
    return ret;
}

void SettingReader::configure(std::string settingsToApply) {
    SPDLOG_DEBUG("SettingReader: configure(\"{}\")", settingsToApply);
    _currentSet = settingsToApply;
    if (_currentSet.empty()) {
        throw std::runtime_error("Cannot find configuration for " + settingsToApply + " settings");
    }
}

void SettingReader::load() {
    std::string filename = _getSetPath("_init.yaml");
    try {
        SPDLOG_INFO("Reading configurationf file {}", filename);
        YAML::Node settings = YAML::LoadFile(filename);

        ForceActuatorSettings::instance().load(settings["ForceActuatorSettings"]);
        HardpointActuatorSettings::instance().load(settings["HardpointActuatorSettings"]);
        _safetyControllerSettings.load(settings["SafetyControllerSettings"]);
        PositionControllerSettings::instance().load(settings["PositionControllerSettings"]);
        AccelerometerSettings::instance().load(settings["AccelerometerSettings"]);
        DisplacementSensorSettings::instance().load(settings["DisplacementSensorSettings"]);
        GyroSettings::instance().load(settings["GyroSettings"]);
        ILCApplicationSettings::instance().load(settings["ILCApplicationSettings"]);
        ExpansionFPGAApplicationSettings::instance().load(settings["ExpansionFPGAApplicationSettings"]);

        _slewPID.load(settings["PIDSettings"]["Slewing"]);
        _trackingPID.load(settings["PIDSettings"]["Tracking"]);

        InclinometerSettings::instance().load(settings["InclinometerSettings"]);
    } catch (YAML::Exception& ex) {
        throw runtime_error(fmt::format("YAML Loading {}: {}", filename, ex.what()));
    }
}

PIDSettings& SettingReader::getPIDSettings(bool slew) {
    if (slew) {
        return _slewPID;
    }
    return _trackingPID;
}

std::string SettingReader::_getSetPath(std::string file) {
    return _rootPath + "/" + CONFIG_SCHEMA_VERSION + "/" + file;
}
