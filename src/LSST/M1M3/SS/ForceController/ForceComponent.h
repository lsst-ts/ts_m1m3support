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
 * Abstract parent class of all force components.
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
    /**
     * Construct ForceComponent.
     *
     * @param name force component name
     */
    ForceComponent(const char *name);
    virtual ~ForceComponent();

    /**
     * Returns true if force component is enabled.
     *
     * @return true if the force component is enabled
     */
    bool isEnabled();
    bool isDisabling();

    /**
     * Enable force component.
     */
    void enable();

    /**
     * Disable force component.
     */
    void disable();

    void update();

    void reset();

protected:
    /**
     * Called after enable/disable changes.
     *
     * Publish a state change for the component Pure virtual, needs to be
     * overriden in children.
     */
    virtual void postEnableDisableActions() = 0;

    /**
     * Called after update to forces.
     *
     * Check for forces that need to be clipped. Update SAL. Pure virtual,
     * needs to be overriden in children.
     */
    virtual void postUpdateActions() = 0;

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

private:
    const char *_name;
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* LSST_M1M3_SS_FORCECONTROLLER_FORCECOMPONENT_H_ */
