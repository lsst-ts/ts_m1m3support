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

#ifndef POSITIONCONTROLLER_H_
#define POSITIONCONTROLLER_H_

#include <DataTypes.h>
#include <Units.h>
#include <HardpointActuatorSettings.h>
#include <PositionControllerSettings.h>
#include <SAL_MTM1M3C.h>

namespace LSST {
namespace M1M3 {
namespace SS {

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
    PositionController(PositionControllerSettings* positionControllerSettings,
                       HardpointActuatorSettings* hardpointActuatorSettings);

    /**
     * Returns raise and lower timeout.
     *
     * @return timeout for raise in seconds
     */
    int getRaiseTimeout() { return _positionControllerSettings->raiseTimeout; }
    int getLowerTimeout() { return _positionControllerSettings->lowerTimeout; }

    bool enableChaseAll();
    void disableChaseAll();

    bool forcesInTolerance(bool raise);
    bool motionComplete();

    /**
     * Move HPs by given steps. Switch HP that needs to move into Stepping
     * state. updateSteps then moves HP as needed.
     *
     * @param steps array of 6 HP steps offsets
     *
     * @return false if move cannot be perfomed, as not all actuators are in Standby state
     */
    bool move(int32_t* steps);
    bool moveToEncoder(int32_t* encoderValues);

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
    bool moveToLowerPosition() {
        return moveToAbsolute(0, 0, _positionControllerSettings->lowerPositionOffsetZ * MM2M, 0, 0, 0);
    }
    bool translate(double x, double y, double z, double rX, double rY, double rZ);
    void stopMotion();

    /**
     * Should be run in a loop to command steps updates. Sets stepsCommanded
     * and stepsQueued. What is happening with the actuators is governed by
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
     * @see PositionControllerSettings
     */
    void updateSteps();

private:
    void _convertToSteps(int32_t* steps, double x, double y, double z, double rX, double rY, double rZ);

    PositionControllerSettings* _positionControllerSettings;
    HardpointActuatorSettings* _hardpointActuatorSettings;

    MTM1M3_hardpointActuatorDataC* _hardpointActuatorData;
    MTM1M3_logevent_hardpointActuatorStateC* _hardpointActuatorState;
    MTM1M3_logevent_hardpointActuatorInfoC* _hardpointInfo;

    int32_t _scaledMaxStepsPerLoop[6];
    int32_t _targetEncoderValues[6];
    int32_t _stableEncoderCount[6];
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* POSITIONCONTROLLER_H_ */
