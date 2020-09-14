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

#ifndef LSST_M1M3_SS_FORCECONTROLLER_FORCECOMPONENT_H_
#define LSST_M1M3_SS_FORCECONTROLLER_FORCECOMPONENT_H_

#include <string>

namespace LSST {
namespace M1M3 {
namespace SS {

/**
 * @brief Abstract parent class of all force compoments.
 *
 * Forces acting on mirror are calculated as sum of 6 scalar forces (lateral
 * forces in x,y and z direction and rotational forces (moments) around x,y and
 * z axis). Individual forces are summed in FinalForceComponent. Considered
 * forces and demands are:
 *
 * * AberrationForceComponent (Z direction only)
 * * AccelerationForceComponent
 * * ActiveOpticForceComponent (Z direction only)
 * * AzimuthForceComponent
 * * BalanceForceComponent
 * * ElevationForceComponent
 * * OffsetForceComponent
 * * StaticForceComponent
 * * ThermalForceComponent
 * * VelocityForceComponent
 */
class ForceComponent {
public:
    ForceComponent();
    virtual ~ForceComponent();

    bool isEnabled();
    bool isDisabling();

    void enable();
    void disable();

    void update();

    void reset();

protected:
    virtual void postEnableDisableActions() = 0;
    virtual void postUpdateActions() = 0;

    std::string name;

    bool enabled;
    bool disabling;
    float maxRateOfChange;
    float nearZeroValue;

    float xCurrent[12];
    float yCurrent[100];
    float zCurrent[156];

    float xTarget[12];
    float yTarget[100];
    float zTarget[156];

    float xOffset[12];
    float yOffset[100];
    float zOffset[156];
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* LSST_M1M3_SS_FORCECONTROLLER_FORCECOMPONENT_H_ */
