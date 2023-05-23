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

#ifndef MODEL_H_
#define MODEL_H_

#include <chrono>
#include <mutex>
#include <string>

#include <Accelerometer.h>
#include <MirrorRaiseController.h>
#include <MirrorLowerController.h>
#include <BumpTestController.h>
#include <DigitalInputOutput.h>
#include <Displacement.h>
#include <ForceActuatorApplicationSettings.h>
#include <ForceController.h>
#include <Gyro.h>
#include <HardpointActuatorApplicationSettings.h>
#include <HardpointMonitorApplicationSettings.h>
#include <HardpointTestController.h>
#include <Inclinometer.h>
#include <InclinometerSettings.h>
#include <ILC.h>
#include <PIDSettings.h>
#include <PositionController.h>
#include <PositionControllerSettings.h>
#include <PowerController.h>
#include <ProfileController.h>
#include <SafetyController.h>
#include <StateTypes.h>

namespace LSST {
namespace M1M3 {
namespace SS {

/**
 * @brief Contains pointers to objects communicating with cRIO boards.
 *
 * Singleton. Provides methods to load setting. Populates pointers for cRIO
 * boards.
 */
class Model {
public:
    Model();
    virtual ~Model();

    /**
     * @brief Returns singleton instance.
     *
     * @return singleton instance
     */
    static Model& get();

    inline DigitalInputOutput* getDigitalInputOutput() { return &_digitalInputOutput; }
    inline Displacement* getDisplacement() { return _displacement; }
    inline Inclinometer* getInclinometer() { return _inclinometer; }
    inline ILC* getILC() { return _ilc; }
    inline ForceController* getForceController() { return _forceController; }
    inline SafetyController* getSafetyController() { return _safetyController; }
    inline PositionController* getPositionController() { return _positionController; }
    inline Accelerometer* getAccelerometer() { return _accelerometer; }
    inline PowerController* getPowerController() { return _powerController; }
    inline MirrorRaiseController* getMirrorRaiseController() { return _mirrorRaiseController; }
    inline MirrorLowerController* getMirrorLowerController() { return _mirrorLowerController; }
    inline Gyro* getGyro() { return _gyro; }
    inline ProfileController* getProfileController() { return &_profileController; }
    inline BumpTestController* getBumpTestController() { return &_bumpTestController; }
    inline HardpointTestController* getHardpointTestController() { return _hardpointTestController; }

    void setCachedTimestamp(double timestamp) { this->_cachedTimestamp = timestamp; }
    double getCachedTimestamp() { return _cachedTimestamp; }

    void loadSettings(std::string settingsToApply);

    void queryFPGAData();

    void publishStateChange(States::Type newState);
    void publishRecommendedSettings();
    void publishOuterLoop(std::chrono::nanoseconds executionTime);

    void exitControl();
    void waitForExitControl();

private:
    Model& operator=(const Model&) = delete;
    Model(const Model&) = delete;

    void _populateHardpointActuatorInfo(
            HardpointActuatorApplicationSettings* hardpointActuatorApplicationSettings);
    void _populateHardpointMonitorInfo(
            HardpointMonitorApplicationSettings* hardpointMonitorApplicationSettings);

    DigitalInputOutput _digitalInputOutput;
    Displacement* _displacement;
    Inclinometer* _inclinometer;
    ILC* _ilc;
    ForceController* _forceController;
    SafetyController* _safetyController;
    PositionController* _positionController;
    Accelerometer* _accelerometer;
    PowerController* _powerController;
    MirrorRaiseController* _mirrorRaiseController;
    MirrorLowerController* _mirrorLowerController;
    Gyro* _gyro;
    ProfileController _profileController;
    BumpTestController _bumpTestController;
    HardpointTestController* _hardpointTestController;

    std::mutex _mutex;

    double _cachedTimestamp;
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* MODEL_H_ */
