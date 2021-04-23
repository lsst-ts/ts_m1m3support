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
 * Wrapper object for MTM1M3_logevent_forceActuatorWarningC.
 */
class ForceActuatorWarning : public MTM1M3_logevent_forceActuatorWarningC {
public:
    ForceActuatorWarning() {
        memset(lastFAServerStatusResponse, 0xFF, sizeof(lastFAServerStatusResponse));
        memset(lastForceDemandResponse, 0xFF, sizeof(lastForceDemandResponse));
        memset(lastDCAStatus, 0xFF, sizeof(lastDCAStatus));
        shouldSend = false;
    }
    void setTimestamp(double globalTimestamp) { timestamp = globalTimestamp; }

    void parseFAServerStatusResponse(ModbusBuffer *buffer, int32_t dataIndex);
    void parseStatus(ModbusBuffer *buffer, int32_t dataIndex, const DDS::Short broadcastCounter);
    void parseDCAStatus(ModbusBuffer *buffer, int32_t dataIndex);

    void log();

private:
    uint16_t lastFAServerStatusResponse[FA_COUNT];
    uint8_t lastForceDemandResponse[FA_COUNT];
    uint16_t lastDCAStatus[FA_COUNT];
    bool shouldSend;
};

}  // namespace SS
}  // namespace M1M3
}  // namespace LSST

#endif  // LSST_FORCEACTUATORWARNING_H
