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

#ifndef SETTINGREADER_H_
#define SETTINGREADER_H_

#include <list>
#include <string>

#include <cRIO/Singleton.h>

#include <SafetyControllerSettings.h>
#include <AliasApplicationSettings.h>
#include <ForceActuatorApplicationSettings.h>
#include <HardpointActuatorApplicationSettings.h>
#include <HardpointActuatorSettings.h>
#include <ILCApplicationSettings.h>
#include <RecommendedApplicationSettings.h>
#include <PositionControllerSettings.h>
#include <AccelerometerSettings.h>
#include <DisplacementSensorSettings.h>
#include <HardpointMonitorApplicationSettings.h>
#include <GyroSettings.h>
#include <ExpansionFPGAApplicationSettings.h>
#include <PIDSettings.h>
#include <InclinometerSettings.h>

namespace LSST {
namespace M1M3 {
namespace SS {

/**
 * Reads M1M3 support system configuration. Singleton, please use setRootPath
 * to configure configuration root directory.
 */
class SettingReader : public cRIO::Singleton<SettingReader> {
public:
    SettingReader(token) : _rootPath("UNDEFINED") {}

    /**
     * Sets root path.
     *
     * @param rootPath new root path
     *
     * @throw runtime_error if directory doesn't exist
     */
    void setRootPath(std::string rootPath);

    std::string getFilePath(std::string filename);

    std::string getSettingsVersion() { return _currentSet + ":" + _currentVersion; }

    /**
     * Returns available configurations.
     */
    std::list<std::string> getAvailableConfigurations();

    /**
     * Select given configuration set.
     *
     * @throw runtime_error if configuration is
     */
    void configure(std::string settingsToApply);
    AliasApplicationSettings* loadAliasApplicationSettings();

    ForceActuatorApplicationSettings* getForceActuatorApplicationSettings() {
        return &_forceActuatorApplicationSettings;
    }

    void loadForceActuatorSettings();

    HardpointActuatorSettings* getHardpointActuatorSettings() { return &_hardpointActuatorSettings; }
    HardpointActuatorApplicationSettings* getHardpointActuatorApplicationSettings() {
        return &_hardpointActuatorApplicationSettings;
    }
    HardpointMonitorApplicationSettings* getHardpointMonitorApplicationSettings() {
        return &_hardpointMonitorApplicationSettings;
    }

    HardpointActuatorSettings* loadHardpointActuatorSettings();
    ILCApplicationSettings* loadILCApplicationSettings();
    RecommendedApplicationSettings* loadRecommendedApplicationSettings();
    SafetyControllerSettings* loadSafetyControllerSettings();
    SafetyControllerSettings* getSafetyControllerSettings() { return &_safetyControllerSettings; }
    PositionControllerSettings* loadPositionControllerSettings();
    AccelerometerSettings* loadAccelerometerSettings();
    DisplacementSensorSettings* loadDisplacementSensorSettings();
    GyroSettings* loadGyroSettings();
    ExpansionFPGAApplicationSettings* loadExpansionFPGAApplicationSettings();
    PIDSettings* loadPIDSettings();
    InclinometerSettings* loadInclinometerSettings();

private:
    SettingReader& operator=(const SettingReader&) = delete;
    SettingReader(const SettingReader&) = delete;

    std::string _getBasePath(std::string file);
    std::string _getSetPath(std::string file);

    AliasApplicationSettings _aliasApplicationSettings;
    ForceActuatorApplicationSettings _forceActuatorApplicationSettings;
    HardpointActuatorApplicationSettings _hardpointActuatorApplicationSettings;
    HardpointActuatorSettings _hardpointActuatorSettings;
    ILCApplicationSettings _ilcApplicationSettings;
    RecommendedApplicationSettings _recommendedApplicationSettings;
    SafetyControllerSettings _safetyControllerSettings;
    PositionControllerSettings _positionControllerSettings;
    AccelerometerSettings _accelerometerSettings;
    DisplacementSensorSettings _displacementSensorSettings;
    HardpointMonitorApplicationSettings _hardpointMonitorApplicationSettings;
    GyroSettings _gyroSettings;
    ExpansionFPGAApplicationSettings _expansionFPGAApplicationSettings;
    PIDSettings _pidSettings;
    InclinometerSettings _inclinometerSettings;

    std::string _rootPath;
    std::string _currentSet;
    std::string _currentVersion;
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* SETTINGREADER_H_ */
