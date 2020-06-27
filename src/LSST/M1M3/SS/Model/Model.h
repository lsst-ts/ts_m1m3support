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

#ifndef MODEL_H_
#define MODEL_H_

#include <ProfileController.h>
#include <StateTypes.h>
#include <pthread.h>
#include <string>

namespace LSST {
namespace M1M3 {
namespace SS {

class M1M3SSPublisher;
class Displacement;
class Inclinometer;
class ILC;
class ForceController;
class SafetyController;
class PositionController;
class Accelerometer;
class PowerController;
class AutomaticOperationsController;
class Gyro;
class ForceCalculator;
class ForceActuatorApplicationSettings;
class ForceActuatorSettings;
class HardpointActuatorApplicationSettings;
class HardpointActuatorSettings;
class HardpointMonitorApplicationSettings;
class PIDSettings;
class DigitalInputOutput;
class InclinometerSettings;
class PositionControllerSettings;

class Model {
public:
    Model(M1M3SSPublisher* publisher, DigitalInputOutput* digitalInputOutput);
    virtual ~Model();

    inline M1M3SSPublisher* getPublisher() { return _publisher; }
    inline Displacement* getDisplacement() { return _displacement; }
    inline Inclinometer* getInclinometer() { return _inclinometer; }
    inline ILC* getILC() { return _ilc; }
    inline ForceController* getForceController() { return _forceController; }
    inline SafetyController* getSafetyController() { return _safetyController; }
    inline PositionController* getPositionController() { return _positionController; }
    inline DigitalInputOutput* getDigitalInputOutput() { return _digitalInputOutput; }
    inline Accelerometer* getAccelerometer() { return _accelerometer; }
    inline PowerController* getPowerController() { return _powerController; }
    inline AutomaticOperationsController* getAutomaticOperationsController() {
        return _automaticOperationsController;
    }
    inline Gyro* getGyro() { return _gyro; }
    inline ProfileController* getProfileController() { return &_profileController; }

    void setCachedTimestamp(double timestamp) { this->_cachedTimestamp = timestamp; }
    double getCachedTimestamp() { return _cachedTimestamp; }

    void loadSettings(std::string settingsToApply);

    void queryFPGAData();

    void publishStateChange(States::Type newState);
    void publishRecommendedSettings();
    void publishOuterLoop(double executionTime);

    void shutdown();
    void waitForShutdown();

private:
    void _populateForceActuatorInfo(ForceActuatorApplicationSettings* forceActuatorApplicationSettings,
                                   ForceActuatorSettings* forceActuatorSettings);
    void _populateHardpointActuatorInfo(
            HardpointActuatorApplicationSettings* hardpointActuatorApplicationSettings,
            HardpointActuatorSettings* hardpointActuatorSettings,
            PositionControllerSettings* positionControllerSettings);
    void _populateHardpointMonitorInfo(
            HardpointMonitorApplicationSettings* hardpointMonitorApplicationSettings);

    M1M3SSPublisher* _publisher;
    Displacement* _displacement;
    Inclinometer* _inclinometer;
    ILC* _ilc;
    ForceController* _forceController;
    SafetyController* _safetyController;
    PositionController* _positionController;
    Accelerometer* _accelerometer;
    PowerController* _powerController;
    AutomaticOperationsController* _automaticOperationsController;
    Gyro* _gyro;
    ProfileController _profileController;
    DigitalInputOutput* _digitalInputOutput;

    pthread_mutex_t _mutex;

    double _cachedTimestamp;
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* MODEL_H_ */
