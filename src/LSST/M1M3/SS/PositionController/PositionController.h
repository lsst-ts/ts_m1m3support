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
#include <SAL_MTM1M3C.h>

namespace LSST {
namespace M1M3 {
namespace SS {

class PositionControllerSettings;
class HardpointActuatorSettings;
class M1M3SSPublisher;

class PositionController {
private:
    PositionControllerSettings* positionControllerSettings;
    HardpointActuatorSettings* hardpointActuatorSettings;
    M1M3SSPublisher* publisher;

    MTM1M3_hardpointActuatorDataC* hardpointActuatorData;
    MTM1M3_logevent_hardpointActuatorStateC* hardpointActuatorState;
    MTM1M3_logevent_hardpointActuatorInfoC* hardpointInfo;

    int32_t scaledMaxStepsPerLoop[6];
    int32_t targetEncoderValues[6];
    int32_t stableEncoderCount[6];

public:
    PositionController(PositionControllerSettings* positionControllerSettings,
                       HardpointActuatorSettings* hardpointActuatorSettings, M1M3SSPublisher* publisher);

    double getRaiseLowerTimeout();

    bool enableChase(int32_t actuatorID);
    void disableChase(int32_t actuatorID);
    bool enableChaseAll();
    void disableChaseAll();

    bool forcesInTolerance();
    bool motionComplete();

    bool move(int32_t* steps);
    bool moveToEncoder(int32_t* encoderValues);
    bool moveToAbsolute(double x, double y, double z, double rX, double rY, double rZ);
    bool moveToReferencePosition();
    bool translate(double x, double y, double z, double rX, double rY, double rZ);
    void stopMotion();

    void updateSteps();

private:
    void convertToSteps(int32_t* steps, double x, double y, double z, double rX, double rY, double rZ);
    int32_t abs(int32_t x);
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* POSITIONCONTROLLER_H_ */
