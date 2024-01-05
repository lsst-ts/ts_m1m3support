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

#ifndef ILCDATATYPES_H_
#define ILCDATATYPES_H_

#include <cRIO/DataTypes.h>
#include <vector>
#include <string>

namespace LSST {
namespace M1M3 {
namespace SS {

struct ILCTypes {
    enum Type { Unknown = 0, HP = 1, FA = 2, HM = 3 };
};

/**
 * ILC data. Contains subnet, address and more of an ILC on subnet.
 */
struct ILCMap {
    ILCTypes::Type Type;
    uint8_t Subnet;
    uint8_t Address;
    int32_t ActuatorId;          /// Actuator ID. 100-443 for FA, 0-5 for HP
    int32_t DataIndex;           /// Z index. 0-155
    int32_t XDataIndex;          /// X index. -1 for single axis FA, 0-99
    int32_t YDataIndex;          /// Y index, -1 for single axis FA, 0-11
    int32_t SecondaryDataIndex;  /// secondary index, -1 for single axis, 0-111

    /**
     * When true, ILC communication is disabled (ILC is not queried for any
     * data).
     */
    bool Disabled;
};

struct HPSettings {
    uint8_t subnet[6];
    uint8_t address[6];
    int16_t id[6];
    float xPosition[6];
    float yPosition[6];
    float zPosition[6];
    float loadCellOffset[6];
    float loadCellSensitivity[6];
};

struct ILCScanRates {
    enum Type {
        Hz50 = 0,
        Hz60 = 1,
        Hz100 = 2,
        Hz120 = 3,
        Hz200 = 4,
        Hz240 = 5,
        Hz300 = 6,
        Hz400 = 7,
        Hz480 = 8,
        Hz600 = 9,
        Hz1200 = 10,
        Hz2400 = 11,
        Hz4800 = 12,
        NoChange = 255
    };
};

struct ILCModes {
    enum Type {
        Standby = 0,
        Disabled = 1,
        Enabled = 2,
        FirmwareUpdate = 3,
        Fault = 4,
        ClearFaults = 5,
        NoChange = 65535
    };
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* ILCDATATYPES_H_ */
