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

#ifndef POSITIONCONTROLLER_H_
#define POSITIONCONTROLLER_H_

#include <SAL_MTM1M3C.h>

#include <HardpointActuatorSettings.h>
#include <PositionControllerSettings.h>
#include <SafetyController.h>
#include <Units.h>
#include <cRIO/DataTypes.h>

namespace LSST {
namespace M1M3 {
namespace SS {

typedef enum { NO_WAIT, CAN_WAIT, WAITING, ALREADY_WAITED } wait_tension_t;

/**
 * Controls M1M3 position. This controller is used in active state to command
 * M1M3 hardpoints, which in turn moves M1M3 platform position and tip/tilt.
 *
 * Hardpoints are (for this application) linear actuators forming hexapod. They
 * are driven by a stepper motor. The position of the actuator is sensed by SSI
 * encoder. Force (=weight) applied on actuator is sensed by a load cell,
 * similar to the force actuator load cells. Loop between encoder and stepper
 * motor is closed by M1M3 CsC (=this application).
 *
 * Steppers are commanded via ILC code 66 in small increments. Encoders are
 * read back, and according to difference between commanded and target
 * position, HP is either in Standby (not moving), Chasing (moving with the
 * mirror so measured actuator force is close to 0), Quick and Fine positioning
 * (moving to target position).
 *
 * Small increments send to the ILC are stored in SAL/DDS
 * MTM1M3_hardpointActuatorDataC stepsCommanded. Target steps are stored in
 * stepsQueued.
 */
class PositionController {
public:
    PositionController();

    /**
     * Returns raise and lower timeout.
     *
     * @return timeout for raise in seconds
     */
    int getRaiseTimeout() { return _positionControllerSettings->raiseTimeout; }
    int getLowerTimeout() { return _positionControllerSettings->lowerTimeout; }

    bool enableChaseAll();
    void disableChaseAll();

    void startRaise();
    void startLower();

    /**
     * Checks that hardpoint forces are in tolerance for mirror raising and
     * lowering. Records raise/lowering forces status in
     * RaisingLoweringInfo.waitHardpoint field.
     *
     * @param raise true if mirror is being raised - raising limits are then used
     *
     * @return true if all hardpoint forces are within tolerance
     */
    bool hpRaiseLowerForcesInTolerance(bool raise);
    bool motionComplete();

    /**
     * Move single hardpoint specified number of steps.
     *
     * @param steps step offset
     * @param hpIndex index of the hardpoint to move
     *
     * @return
     */
    bool moveHardpoint(int32_t steps, int hpIndex);

    /**
     * Move HPs by given steps. Switch HP that needs to move into Stepping
     * state. updateSteps then moves HP as needed.
     *
     * @param steps array of 6 HP steps offsets
     *
     * @return false if move cannot be perfomed, as not all actuators are in
     * Standby state
     */
    bool move(const std::vector<int> &steps);
    bool moveToEncoder(const std::vector<int> &encoderValues);

    /**
     * Commands mirror to move to new position.
     *
     * @param x target x position (m)
     * @param y target y position (m)
     * @param z target z position (m)
     * @param rX target x rotation (deg)
     * @param rY target y rotation (deg)
     * @param rZ target z rotation (deg)
     *
     * @return false when move cannot be performed
     */
    bool moveToAbsolute(double x, double y, double z, double rX, double rY, double rZ);
    bool moveToReferencePosition();

    /**
     * Moves mirror to position ideal for lowering. This position depends on
     * telescope elevation. It is position defined in
     * PositionControllerSettings/Lower/PositionOffset, multiplied by
     * sin(elevation) for Z and cos(elevation) for Y. The multiplication tries
     * to position mirror opposite to its gravity vectory. It is exactly what
     * LBTO uses for off-zenith mirror lowering.
     *
     * @return false when move cannot be performed
     */
    bool moveToLowerPosition();

    bool translate(double x, double y, double z, double rX, double rY, double rZ);

    /**
     * Stops HP motion. If index is negative, all hardpoints are stopped.
     *
     * @param hardpointIndex index of the hardpoint to stop, or negative value to
     * stop all hardpoints, Defaults to -1.
     */
    void stopMotion(int hardpointIndex = -1);

    /**
     * Called in any enabled state (raised, parked, ..). Sets stepsCommanded
     * and stepsQueued. What is happening with the actuators is governed by its
     * state.
     *
     * * **Standby**: both stepsCommanded and stepsQueued are set to 0.
     * * **Chasing**: MTM1M3_hardpointActuatorDataC measuredForce is multiplied
     * by PositionControllerSettings::ForceToStepsCoefficient and coerced into
     * ±PositionControllerSettings::MaxStepsPerLoop.
     * * **Stepping**: executes steps so stepsQueued are decreased, ultimately
     * equal to 0. MTM1M3_hardpointActuatorDataC stepsQueued are coerced into
     * ±_scaledMaxStepsPerLoop[HP] and send to HP. stepsQueued are decreased by
     * stepsCommanded. If coerced value == 0, HP is transitioned into Standby
     * state.
     * * **QuickPositioning**: same as **Stepping**, but transition into
     * FinePositioning state when finished.
     * * **FinePositioning**: finish movement by removing any residual between
     * HP encoder (measured) value and _targetEncoderValues. Difference between
     * _targetEncoderValues and encoder values > 2 are commanded. Transition to
     * Standby state if the difference remains <= ±2 for two loop runs.
     *
     * This loop also monitors if encoder is following expected trajectory. If
     * relative following error is outside prescribed range for configured loop,
     * HardpointActuatorFollowingError is triggered.
     *
     * @see PositionControllerSettings
     */
    void updateSteps();

    /**
     * Check the hardpoint doesn't try to move past limits.
     */
    void checkLimits(int hp);

private:
    void _convertToSteps(int32_t *steps, double x, double y, double z, double rX, double rY, double rZ);

    void _checkFollowingError(int hp);

    void _resetWaitTension();

    PositionControllerSettings *_positionControllerSettings;
    HardpointActuatorSettings *_hardpointActuatorSettings;

    MTM1M3_hardpointActuatorDataC *_hardpointActuatorData;
    MTM1M3_logevent_hardpointActuatorStateC *_hardpointActuatorState;
    MTM1M3_logevent_hardpointActuatorInfoC *_hardpointInfo;

    int32_t _scaledMaxStepsPerLoop[HP_COUNT];
    int32_t _targetEncoderValues[HP_COUNT];
    int32_t _stableEncoderCount[HP_COUNT];
    int32_t _unstableEncoderCount[HP_COUNT];

    int32_t _lastEncoderCount[HP_COUNT];
    wait_tension_t _waitTension[HP_COUNT];
    uint16_t _raisingLoweringInRangeSamples[HP_COUNT];

    SafetyController *_safetyController;
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* POSITIONCONTROLLER_H_ */
