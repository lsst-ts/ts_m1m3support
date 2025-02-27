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

#ifndef LSST_FORCEACTUATORWARNING_H
#define LSST_FORCEACTUATORWARNING_H

#include <SAL_MTM1M3.h>

#include <ILCDataTypes.h>
#include <ModbusBuffer.h>

#include <string.h>

namespace LSST {
namespace M1M3 {
namespace SS {

/**
 * Wrapper object for MTM1M3_logevent_forceActuatorWarningC. Keeps track of
 * changes to parsed data, and sends updates only if data changed. Variables
 * inherited from MTM1M3_logevent_forceActuatorWarningC are reset in first pass
 * of various parse* methods.
 */
class ForceActuatorWarning : public MTM1M3_logevent_forceActuatorWarningC {
public:
    /**
     * Construct new ForceActuatorWarning, sets internal variables to 0xFF
     * (actual ILC shall never send those responses).
     */
    ForceActuatorWarning();

    /**
     * Sets logEvent timestamp.
     *
     * @param globalTimestamp actual timestamp
     */
    void setTimestamp(double globalTimestamp) { timestamp = globalTimestamp; }

    /**
     * Parses FA server status (included in response to function code 18).
     *
     * @param buffer receiving buffer
     * @param dataIndex FA index
     */
    void parseFAServerStatusResponse(ModbusBuffer *buffer, int32_t dataIndex);

    /**
     * Parses FA status (included in response to function code 75).
     *
     * @param buffer receiving buffer
     * @param dataIndex FA index
     * @param broadcastCounter actual (expected) value of broadcast counter
     */
    void parseStatus(ModbusBuffer *buffer, int32_t dataIndex, int32_t broadcastCounter);

    /**
     * Parses DCA status (included in response to function code 121).
     *
     * @param buffer receiving buffer
     * @param dataIndex FA index
     */
    void parseDCAStatus(ModbusBuffer *buffer, int32_t dataIndex);

    /**
     * Sends updates through SAL/DDS.
     */
    void log();

private:
    uint16_t _lastFAILCStatusResponse[FA_COUNT];
    uint16_t _lastFAILCFaultsResponse[FA_COUNT];
    uint8_t _lastForceDemandResponse[FA_COUNT];
    uint16_t _lastDCAStatus[FA_COUNT];
    bool _shouldSend;
};

}  // namespace SS
}  // namespace M1M3
}  // namespace LSST

#endif  // LSST_FORCEACTUATORWARNING_H
