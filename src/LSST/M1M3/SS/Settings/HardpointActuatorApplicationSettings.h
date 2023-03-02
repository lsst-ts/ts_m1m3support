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

#ifndef HARDPOINTACTUATORAPPLICATIONSETTINGS_H_
#define HARDPOINTACTUATORAPPLICATIONSETTINGS_H_

#include <cstdint>

#include <DataTypes.h>

namespace LSST {
namespace M1M3 {
namespace SS {

struct HardpointActuatorTableRow {
    int32_t Index;
    int32_t ActuatorID;
    double XPosition;
    double YPosition;
    double ZPosition;
    uint8_t Subnet;
    uint8_t Address;
    float SensorOffset;
    float SensorSensitivity;
};

/**
 * Loads Hardpoint Settings. Settings are stored in csv file, pointed from Base
 * XML file.
 */
class HardpointActuatorApplicationSettings {
public:
    /**
     * Source data. Contains hardpoint actuator position and addresses. Populated in
     * HardpointActuatorApplicationSettings.cpp.
     *
     * @snippet Settings/HardpointActuatorApplicationSettings.cpp HarpointActuatorTableRow initialization
     *
     * @see ForceActuatorTableRow
     */
    static HardpointActuatorTableRow Table[HP_COUNT];
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* HARDPOINTACTUATORAPPLICATIONSETTINGS_H_ */
