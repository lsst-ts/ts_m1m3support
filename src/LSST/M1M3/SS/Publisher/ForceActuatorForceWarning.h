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

#ifndef LSST_FORCEACTUATORFORCEWARNING_H
#define LSST_FORCEACTUATORFORCEWARNING_H

#include <string.h>

#include <SAL_MTM1M3.h>

#include <cRIO/Singleton.h>

namespace LSST {
namespace M1M3 {
namespace SS {

/**
 * Wrapper object for MTM1M3_logevent_forceActuatorForceWarningC. The check
 * done on forces are done on with actual measured forces. Forces might be
 * clipped before being applied to the FAs - look on various preclipped events
 * for the clipping, and
 */
class ForceActuatorForceWarning : public MTM1M3_logevent_forceActuatorForceWarningC,
                                  public cRIO::Singleton<ForceActuatorForceWarning> {
public:
    /**
     * Construct new ForceActuatorForceWarning.
     */
    ForceActuatorForceWarning(token);

    void reset();
    void resetSend();

    void setTimestamp(double globalTimestamp);

    /**
     * Check primary force, Check for following error and measured force.
     * Returns true if the force is out of limit and system shall fault the
     * mirror.
     *
     * @param dataIndex FA index
     * @param actuatorId FA ID
     * @param primaryForce force measured on the force actuator
     * @param primarySetpoint FA setpoint - target force
     */
    void checkPrimary(int dataIndex, int actuatorId, float primaryForce, float primarySetpoint);

    /**
     * Check secondary force. Check for following error and measured force.
     * Returns true if the force is out of limit and system shall fault the
     * mirror.
     *
     * @param dataIndex FA index
     * @param actuatorId FA ID
     * @param secondaryForce force measured on the force actuator
     * @param secondarySetpoint FA setpoint = target force
     *
     * @return true if mirror shall fault (measured force exceed allowable limits)
     */
    void checkSecondary(int dataIndex, int actuatorId, float secondaryForce, float secondarySetpoint);

    bool checkXMeasuredForce(int xIndex, int actuatorId, float xForce);
    bool checkYMeasuredForce(int yIndex, int actuatorId, float yForce);
    bool checkZMeasuredForce(int zIndex, int actuatorId, float zForce);

    /**
     * Sends updates through SAL/DDS.
     */
    void send();

private:
    float _measuredForceWarningRatio;

    bool _shouldSend;
};

}  // namespace SS
}  // namespace M1M3
}  // namespace LSST

#endif  // LSST_FORCEACTUATORFORCEWARNING_H
