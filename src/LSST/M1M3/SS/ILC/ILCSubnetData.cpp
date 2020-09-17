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

#include <ILCSubnetData.h>
#include <ForceActuatorApplicationSettings.h>
#include <ForceActuatorSettings.h>
#include <HardpointActuatorApplicationSettings.h>
#include <HardpointMonitorApplicationSettings.h>
#include <spdlog/spdlog.h>

namespace LSST {
namespace M1M3 {
namespace SS {

ILCSubnetData::ILCSubnetData(ForceActuatorApplicationSettings* forceActuatorApplicationSettings,
                             ForceActuatorSettings* forceActuatorSettings,
                             HardpointActuatorApplicationSettings* hardpointActuatorApplicationSettings,
                             HardpointMonitorApplicationSettings* hardpointMonitorApplicationSettings) {
    spdlog::debug("ILCSubnetData::ILCSubnetData()");
    for (int subnetIndex = 0; subnetIndex < SUBNET_COUNT; subnetIndex++) {
        this->subnetData[subnetIndex].FACount = 0;
        this->subnetData[subnetIndex].HPCount = 0;
        this->subnetData[subnetIndex].HMCount = 0;
    }
    for (int i = 0; i < FA_COUNT; i++) {
        ForceActuatorTableRow row = forceActuatorApplicationSettings->Table[i];
        int32_t subnetIndex = row.Subnet - 1;
        this->subnetData[subnetIndex].ILCDataFromAddress[row.Address].Type = ILCTypes::FA;
        this->subnetData[subnetIndex].ILCDataFromAddress[row.Address].Subnet = row.Subnet;
        this->subnetData[subnetIndex].ILCDataFromAddress[row.Address].Address = row.Address;
        this->subnetData[subnetIndex].ILCDataFromAddress[row.Address].ActuatorId = row.ActuatorID;
        this->subnetData[subnetIndex].ILCDataFromAddress[row.Address].DataIndex = row.Index;
        this->subnetData[subnetIndex].ILCDataFromAddress[row.Address].XDataIndex =
                forceActuatorApplicationSettings->ZIndexToXIndex[row.Index];
        this->subnetData[subnetIndex].ILCDataFromAddress[row.Address].YDataIndex =
                forceActuatorApplicationSettings->ZIndexToYIndex[row.Index];
        this->subnetData[subnetIndex].ILCDataFromAddress[row.Address].SecondaryDataIndex =
                forceActuatorApplicationSettings->ZIndexToSecondaryCylinderIndex[row.Index];
        this->subnetData[subnetIndex].ILCDataFromAddress[row.Address].Disabled =
                forceActuatorSettings->IsActuatorDisabled(row.ActuatorID);
        ILCMap map = this->subnetData[subnetIndex].ILCDataFromAddress[row.Address];
        this->subnetData[subnetIndex].FAIndex.push_back(map);
        this->subnetData[subnetIndex].FACount += 1;
    }
    for (int i = 0; i < HP_COUNT; i++) {
        HardpointActuatorTableRow row = hardpointActuatorApplicationSettings->Table[i];
        int32_t subnetIndex = row.Subnet - 1;
        this->subnetData[subnetIndex].ILCDataFromAddress[row.Address].Type = ILCTypes::HP;
        this->subnetData[subnetIndex].ILCDataFromAddress[row.Address].Subnet = row.Subnet;
        this->subnetData[subnetIndex].ILCDataFromAddress[row.Address].Address = row.Address;
        this->subnetData[subnetIndex].ILCDataFromAddress[row.Address].ActuatorId = row.ActuatorID;
        this->subnetData[subnetIndex].ILCDataFromAddress[row.Address].DataIndex = row.Index;
        this->subnetData[subnetIndex].ILCDataFromAddress[row.Address].XDataIndex = -1;
        this->subnetData[subnetIndex].ILCDataFromAddress[row.Address].YDataIndex = -1;
        this->subnetData[subnetIndex].ILCDataFromAddress[row.Address].SecondaryDataIndex = -1;
        this->subnetData[subnetIndex].ILCDataFromAddress[row.Address].Disabled = false;
        ILCMap map = this->subnetData[subnetIndex].ILCDataFromAddress[row.Address];
        this->subnetData[subnetIndex].HPIndex.push_back(map);
        this->subnetData[subnetIndex].HPCount += 1;
    }
    for (int i = 0; i < HP_COUNT; i++) {
        HardpointMonitorTableRow row = hardpointMonitorApplicationSettings->Table[i];
        int32_t subnetIndex = row.Subnet - 1;
        this->subnetData[subnetIndex].ILCDataFromAddress[row.Address].Type = ILCTypes::HM;
        this->subnetData[subnetIndex].ILCDataFromAddress[row.Address].Subnet = row.Subnet;
        this->subnetData[subnetIndex].ILCDataFromAddress[row.Address].Address = row.Address;
        this->subnetData[subnetIndex].ILCDataFromAddress[row.Address].ActuatorId = row.ActuatorID;
        this->subnetData[subnetIndex].ILCDataFromAddress[row.Address].DataIndex = row.Index;
        this->subnetData[subnetIndex].ILCDataFromAddress[row.Address].XDataIndex = -1;
        this->subnetData[subnetIndex].ILCDataFromAddress[row.Address].YDataIndex = -1;
        this->subnetData[subnetIndex].ILCDataFromAddress[row.Address].SecondaryDataIndex = -1;
        this->subnetData[subnetIndex].ILCDataFromAddress[row.Address].Disabled = false;
        ILCMap map = this->subnetData[subnetIndex].ILCDataFromAddress[row.Address];
        this->subnetData[subnetIndex].HMIndex.push_back(map);
        this->subnetData[subnetIndex].HMCount += 1;
    }
}

ILCMap ILCSubnetData::getMap(int32_t actuatorId) {
    for (int subnetIndex = 0; subnetIndex < 5; ++subnetIndex) {
        Container container = this->subnetData[subnetIndex];
        for (int i = 0; i < container.HPCount; ++i) {
            if (container.HPIndex[i].ActuatorId == actuatorId) {
                return container.HPIndex[i];
            }
        }
        for (int i = 0; i < container.FACount; ++i) {
            if (container.FAIndex[i].ActuatorId == actuatorId) {
                return container.FAIndex[i];
            }
        }
        for (int i = 0; i < container.HMCount; ++i) {
            if (container.HMIndex[i].ActuatorId == actuatorId) {
                return container.HMIndex[i];
            }
        }
    }
    ILCMap none;
    none.Address = 255;
    none.Subnet = 255;
    return none;
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
