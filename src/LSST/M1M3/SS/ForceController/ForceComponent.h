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

#include <ForceComponentSettings.h>
#include <string>

namespace LSST {
namespace M1M3 {
namespace SS {

/**
 * Force component states. Only transition from ENABLED to DISABLED requires
 * gradual removal of the component force.
 */
enum ForceComponentState { DISABLED, ENABLED, DISABLING };

/**
 * Abstract parent class of all force components.
 *
 * Forces acting on mirror are calculated as sum of 6 scalar forces (lateral
 * forces in x,y and z direction and rotational forces (moments) around x,y and
 * z axis). Individual actuators forces are summed in FinalForceComponent.
 * Considered forces and demands are:
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
 *
 * Force component can be enabled or disabled. When it is disabled, its
 * contribution is scaled linearly to zero to prevent overstressing the mirror.
 * Handling of the enabled/disabled states is performed in ForceController
 * class.
 */
class ForceComponent {
public:
    /**
     * Construct ForceComponent.
     *
     * @param name force component name
     * @param forceComponentSettings
     */
    ForceComponent(const char *name, const ForceComponentSettings &forceComponentSettings);
    virtual ~ForceComponent();

    /**
     * Returns true if force component is enabled.
     *
     * @return true if the force component is enabled
     */
    bool isEnabled() { return _state == ENABLED; }

    /**
     * Force component is being disabled.
     *
     * @return true if force component is being disabled
     */
    bool isDisabling() { return _state == DISABLING; }

    /**
     * Enable force component.
     */
    void enable();

    /**
     * Disable force component. Starts to gradually scales force contribution to
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

    /// measured actuator current X force
    float xCurrent[12];
    /// measured actuator current Y force
    float yCurrent[100];
    /// measured actuator current Z force
    float zCurrent[156];

    /// target actuator X force
    float xTarget[12];
    /// target actuator Y force
    float yTarget[100];
    /// target actuator Z force
    float zTarget[156];

    /// difference (error) between current and target X force
    float xOffset[12];
    /// difference (error) between current and target Y force
    float yOffset[100];
    /// difference (error) between current and target Z force
    float zOffset[156];

private:
    const char *_name;
    float _maxRateOfChange;
    float _nearZeroValue;

    ForceComponentState _state;
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* LSST_M1M3_SS_FORCECONTROLLER_FORCECOMPONENT_H_ */
