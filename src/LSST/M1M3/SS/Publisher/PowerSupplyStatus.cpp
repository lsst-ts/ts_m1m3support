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

#include <PowerSupplyStatus.h>
#include <M1M3SSPublisher.h>

namespace LSST {
namespace M1M3 {
namespace SS {

PowerSupplyStatus::PowerSupplyStatus() {
    // power supply status uses only 24 bits, as topmost 8 channels aren't connected
    lastStatus = 0xFFFFFFFF;
}

void PowerSupplyStatus::setPowerSupplyStatus(uint32_t powerStatus) {
    // don't care when value isn't updated
    if (lastStatus == powerStatus) {
        return;
    }

    rcpMirrorCellUtility220VAC1Status = powerStatus & 0x000001;
    rcpMirrorCellUtility220VAC2Status = powerStatus & 0x000002;
    rcpMirrorCellUtility220VAC3Status = powerStatus & 0x000004;
    rcpCabinetUtility220VACStatus = powerStatus & 0x000008;
    rcpExternalEquipment220VACStatus = powerStatus & 0x000010;
    powerNetworkARedundancyControlStatus = powerStatus & 0x000020;
    powerNetworkBRedundancyControlStatus = powerStatus & 0x000040;
    powerNetworkCRedundancyControlStatus = powerStatus & 0x000080;
    powerNetworkDRedundancyControlStatus = powerStatus & 0x000100;
    controlsPowerNetworkRedundancyControlStatus = powerStatus & 0x000200;

    powerNetworkAStatus = powerStatus & 0x000800;
    powerNetworkARedundancyControlStatus = powerStatus & 0x001000;
    powerNetworkBStatus = powerStatus & 0x002000;
    powerNetworkBRedundancyControlStatus = powerStatus & 0x004000;
    powerNetworkCStatus = powerStatus & 0x008000;
    powerNetworkCRedundancyControlStatus = powerStatus & 0x010000;
    powerNetworkDStatus = powerStatus & 0x020000;
    powerNetworkDRedundancyControlStatus = powerStatus & 0x040000;
    controlsPowerNetworkStatus = powerStatus & 0x080000;
    controlsPowerNetworkRedundantStatus = powerStatus & 0x100000;
    lightPowerNetworkStatus = powerStatus & 0x200000;
    externalEquipmentPowerNetworkStatus = powerStatus & 0x400000;
    laserTrackerPowerNetworkStatus = powerStatus & 0x800000;

    M1M3SSPublisher::instance().logPowerSupplyStatus(this);
    lastStatus = powerStatus;
}

}  // namespace SS
}  // namespace M1M3
}  // namespace LSST
