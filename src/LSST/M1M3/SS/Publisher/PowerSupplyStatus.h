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

#ifndef LSST_POWERSUPPLYSTATUS_H
#define LSST_POWERSUPPLYSTATUS_H

#include <SAL_MTM1M3.h>

#include <ILCDataTypes.h>
#include <ModbusBuffer.h>

#include <string.h>

namespace LSST {
namespace M1M3 {
namespace SS {

/**
 * Wrapper object for MTM1M3_logevent_powerSupplyStatusC. Keeps track of
 * changes to parsed data, and sends updates only if data changed. Variables
 * inherited from MTM1M3_logevent_powerSupplyStatusC are reset in first pass
 * of various parse* methods.
 */
class PowerSupplyStatus : public MTM1M3_logevent_powerSupplyStatusC {
public:
    /**
     * Construct new PowerSupplyStatus, sets internal variables to 0xFF
     * (actual ILC shall never send those responses).
     */
    PowerSupplyStatus();

    /**
     * Sets logEvent timestamp.
     *
     * @param globalTimestamp actual timestamp
     */
    void setTimestamp(double globalTimestamp) { timestamp = globalTimestamp; }

    /**
     * Parses powerSupply status, sends updates when updated.
     *
     * @param powerStatus Slot 2 value of the Expansion FPGA
     */
    void setPowerSupplyStatus(uint32_t powerStatus);

private:
    uint32_t lastStatus;
};

}  // namespace SS
}  // namespace M1M3
}  // namespace LSST

#endif  // LSST_POWERSUPPLYSTATUS_H
