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

#ifndef LSST_ENABLEDFORCEACTUATORS_H
#define LSST_ENABLEDFORCEACTUATORS_H

#include <SAL_MTM1M3.h>

#include <ILCDataTypes.h>
#include <ModbusBuffer.h>

#include <string.h>

namespace LSST {
namespace M1M3 {
namespace SS {

/**
 * Wrapper object for MTM1M3_logevent_enabledForceActuatorsC. Keeps track of
 * changes to parsed data, and sends updates only if data changed. Variables
 * inherited from MTM1M3_logevent_enabledForceActuatorsC in first pass
 * of various parse* methods.
 */
class EnabledForceActuators : public MTM1M3_logevent_enabledForceActuatorsC {
public:
    /**
     * Construct new EnabledForceActuators, sets all enabled.
     */
    EnabledForceActuators();

    /**
     * Sets logEvent timestamp.
     *
     * @param globalTimestamp actual timestamp
     */
    void setTimestamp(double globalTimestamp) { timestamp = globalTimestamp; }

    void setEnabled(int32_t actuatorId, bool enabled);
    void setEnabledAll();

    /**
     * Sends updates through SAL/DDS.
     */
    void log();

private:
    bool _shouldSend;
};

}  // namespace SS
}  // namespace M1M3
}  // namespace LSST

#endif  // LSST_ENABLEDFORCEACTUATORS_H
