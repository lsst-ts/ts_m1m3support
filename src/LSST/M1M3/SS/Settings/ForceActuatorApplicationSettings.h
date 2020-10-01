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

#ifndef FORCEACTUATORAPPLICATIONSETTINGS_H_
#define FORCEACTUATORAPPLICATIONSETTINGS_H_

#include <DataTypes.h>
#include <ForceActuatorOrientations.h>
#include <ForceActuatorTypes.h>

namespace LSST {
namespace M1M3 {
namespace SS {

/**
 * Force actuator data. Contains static data, which shall not change and which
 * don't contribute to force calculation. Particularly note that [XYZ]Position
 * is used only to calculate combined forces and moments. Position shall not be
 * used in calculating actuator force and distributing forces among actuator,
 * that's done with configurable matrices.
 */
struct ForceActuatorTableRow {
    int32_t ActuatorID;
    double XPosition;
    double YPosition;
    double ZPosition;
    ForceActuatorTypes Type;
    uint8_t Subnet;
    uint8_t Address;
    ForceActuatorOrientations Orientation;
};

/**
 *
 */
class ForceActuatorApplicationSettings {
public:
    ForceActuatorApplicationSettings();

    int32_t XIndexToZIndex[FA_X_COUNT];
    int32_t YIndexToZIndex[FA_Y_COUNT];
    int32_t SecondaryCylinderIndexToZIndex[FA_X_COUNT + FA_Y_COUNT];
    int32_t ZIndexToXIndex[FA_Z_COUNT];
    int32_t ZIndexToYIndex[FA_Z_COUNT];
    int32_t ZIndexToSecondaryCylinderIndex[FA_Z_COUNT];
    static ForceActuatorTableRow Table[FA_COUNT];
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* FORCEACTUATORAPPLICATIONSETTINGS_H_ */
