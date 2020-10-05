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
 * is used only to calculate combined forces and moments (f[xyz], m[xyz], force
 * magnitude). Position shall not be used in calculating actuator force and
 * distributing forces among actuator, that's done with configurable matrices.
 */
struct ForceActuatorTableRow {
    /**
     * Force actuator ID. Three digit number, where the first digit is
     * quadrant.
     */
    int32_t ActuatorID;

    /**
     * Force actuator X (horizontal when mirror is pointed at horizon) position in m. Cartesian system with
     * origin in mirror center.
     */
    double XPosition;

    /**
     * Force actuator Y (vertical when mirror is pointed at horizon) position in m. Cartesian system with
     * origin in mirror center.
     */
    double YPosition;

    //* Force actuator Z (positive toward M2) position in m. Cartesian system with origin in mirror center.
    double ZPosition;

    //* Single or double axis actuator.
    ForceActuatorTypes Type;

    //* Actuator ILC Modbus subnet (bus) number. 1-4.
    uint8_t Subnet;

    //* Actuator ILC address on bus. 10-46.
    uint8_t Address;

    //* Second axis orientation. NA for single axis actuators.
    ForceActuatorOrientations Orientation;
};

/**
 * (Almost) constant force actuator values. Contains array of 156 force actuator
 * values, used to pre-compute other mappings.
 *
 * @note *SSA* Single Axis Actuator
 * @note *DAA* Dual Axis Actuator
 */
class ForceActuatorApplicationSettings {
public:
    /**
     * Constructor. Populates auxiliary mapping arrays from static Table data.
     */
    ForceActuatorApplicationSettings();

    /**
     * Source data. Contains force actuator position, type and orientation. Populated in
     * ForceActuatorApplicationSettings.cpp.
     *
     * @snippet Settings/ForceActuatorApplicationSettings.cpp Table initialization
     *
     * @see ForceActuatorTableRow
     */
    static ForceActuatorTableRow Table[FA_COUNT];

    //* Maps X DAA (lateral support) number (0-11) into Z index (0-155).
    int32_t XIndexToZIndex[FA_X_COUNT];

    //* Maps Y DAA (longitudinal support) number (0-99) into Z index (0-155).
    int32_t YIndexToZIndex[FA_Y_COUNT];

    //* Maps secondary (DAA) index (0-111) into Z index (0-155).
    int32_t SecondaryCylinderIndexToZIndex[FA_X_COUNT + FA_Y_COUNT];

    //* Maps Z index (0-155) to X DAA (lateral support, 0-11).
    int32_t ZIndexToXIndex[FA_Z_COUNT];

    //* Maps Z index (0-155) to Y DAA (longitudinal support, 0-99).
    int32_t ZIndexToYIndex[FA_Z_COUNT];

    //* Maps Z index (0-155) to secondary (DAA) index (0-111).
    int32_t ZIndexToSecondaryCylinderIndex[FA_Z_COUNT];
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* FORCEACTUATORAPPLICATIONSETTINGS_H_ */
