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

#ifndef SETTINGREADER_H_
#define SETTINGREADER_H_

#include <SafetyControllerSettings.h>
#include <AliasApplicationSettings.h>
#include <ForceActuatorApplicationSettings.h>
#include <ForceActuatorSettings.h>
#include <HardpointActuatorApplicationSettings.h>
#include <HardpointActuatorSettings.h>
#include <ILCApplicationSettings.h>
#include <RecommendedApplicationSettings.h>
#include <PositionControllerSettings.h>
#include <AccelerometerSettings.h>
#include <DisplacementSensorSettings.h>
#include <HardpointMonitorApplicationSettings.h>
#include <InterlockApplicationSettings.h>
#include <GyroSettings.h>
#include <ExpansionFPGAApplicationSettings.h>
#include <PIDSettings.h>
#include <InclinometerSettings.h>
#include <string>

namespace LSST {
namespace M1M3 {
namespace SS {

/**
 * Reads M1M3 support system configuration. Singleton, please use setBasePath
 * to configure configuration root directory.
 */
class SettingReader {
public:
    SettingReader() : rootPath("UNDEFINED") {}

    /**
     * Returns SettingReader singleton.
     */
    static SettingReader& get();

    /**
     * Sets root path.
     *
     * @param rootPath new root path
     *
     * @throw runtime_error if directory doesn't exist
     */
    void setRootPath(std::string rootPath);

    std::string getFilePath(std::string filename);

    /**
     * Select given configuration set.
     *
     * @throw runtime_error if configuration is
     */
    void configure(std::string settingsToApply);
    AliasApplicationSettings* loadAliasApplicationSettings();
    ForceActuatorApplicationSettings* loadForceActuatorApplicationSettings();
    ForceActuatorSettings* loadForceActuatorSettings();
    HardpointActuatorApplicationSettings* loadHardpointActuatorApplicationSettings();
    HardpointActuatorSettings* loadHardpointActuatorSettings();
    ILCApplicationSettings* loadILCApplicationSettings();
    RecommendedApplicationSettings* loadRecommendedApplicationSettings();
    SafetyControllerSettings* loadSafetyControllerSettings();
    PositionControllerSettings* loadPositionControllerSettings();
    AccelerometerSettings* loadAccelerometerSettings();
    DisplacementSensorSettings* loadDisplacementSensorSettings();
    HardpointMonitorApplicationSettings* loadHardpointMonitorApplicationSettings();
    InterlockApplicationSettings* loadInterlockApplicationSettings();
    GyroSettings* loadGyroSettings();
    ExpansionFPGAApplicationSettings* loadExpansionFPGAApplicationSettings();
    PIDSettings* loadPIDSettings();
    InclinometerSettings* loadInclinometerSettings();

private:
    std::string getBasePath(std::string file);
    std::string getSetPath(std::string file);

    AliasApplicationSettings aliasApplicationSettings;
    ForceActuatorApplicationSettings forceActuatorApplicationSettings;
    ForceActuatorSettings forceActuatorSettings;
    HardpointActuatorApplicationSettings hardpointActuatorApplicationSettings;
    HardpointActuatorSettings hardpointActuatorSettings;
    ILCApplicationSettings ilcApplicationSettings;
    RecommendedApplicationSettings recommendedApplicationSettings;
    SafetyControllerSettings safetyControllerSettings;
    PositionControllerSettings positionControllerSettings;
    AccelerometerSettings accelerometerSettings;
    DisplacementSensorSettings displacementSensorSettings;
    HardpointMonitorApplicationSettings hardpointMonitorApplicationSettings;
    InterlockApplicationSettings interlockApplicationSettings;
    GyroSettings gyroSettings;
    ExpansionFPGAApplicationSettings expansionFPGAApplicationSettings;
    PIDSettings pidSettings;
    InclinometerSettings inclinometerSettings;

    std::string rootPath;
    std::string currentSet;
    std::string currentVersion;
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* SETTINGREADER_H_ */
