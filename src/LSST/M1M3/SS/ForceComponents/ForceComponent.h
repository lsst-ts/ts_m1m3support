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

#ifndef LSST_M1M3_SS_FORCECONTROLLER_FORCECOMPONENT_H_
#define LSST_M1M3_SS_FORCECONTROLLER_FORCECOMPONENT_H_

#include <string>

#include <cRIO/DataTypes.h>
#include <ForceComponentSettings.h>

namespace LSST {
namespace M1M3 {
namespace SS {

/**
 * Force component states. Only transition from ENABLED to DISABLED requires
 * gradual removal of the component force.
 */
enum ForceComponentState { INITIALISING, DISABLED, ENABLED, DISABLING };

/**
 * Abstract parent class of all force components.
 *
 * Forces acting on mirror are calculated as sum of 6 scalar forces (lateral
 * forces in x,y and z direction and rotational forces (moments) around x,y and
 * z axis). Individual actuators forces are summed in FinalForceComponent.
 * Considered forces and demands are:
 *
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
 * Commanding of the enabled/disabled states and checking if the forces are
 * enabled before being applied is performed in the ForceController class.
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
     * Returns force component name.
     *
     * @return force component name
     */
    const char *getName() { return _name; }

    /**
     * Returns true if the force component is being initialised.
     *
     * @return true if the force component is being initialised
     */
    bool isInitialising() { return _state == INITIALISING; }

    /**
     * Returns true if the force component is enabled.
     *
     * @return true if the force component is enabled
     */
    bool isEnabled() { return _state == ENABLED; }

    /**
     * Returns true if the force component is being disabled.
     *
     * @return true if the force component is being disabled
     */
    bool isDisabling() { return _state == DISABLING; }

    /**
     * Returns true if the force compoment is active - shall be calculated.
     *
     * @return true if the force component is either enabled or disabling (scaling
     * back to 0)
     */
    bool isActive() { return _state == INITIALISING || _state == ENABLED || _state == DISABLING; }

    /**
     * Enable the force component.
     */
    void enable();

    /**
     * Disable the force component. Starts to gradually scales force contribution to
     */
    void disable();

    /**
     * Called after enable/disable changes.
     *
     * Publish a state change for the component Pure virtual, needs to be
     * overriden in children.
     */
    virtual void postEnableDisableActions() = 0;

    void update();

    void reset();

protected:
    /**
     * Called after update to forces.
     *
     * Check for forces that need to be clipped. Update SAL. Pure virtual,
     * needs to be overriden in children.
     */
    virtual void postUpdateActions() = 0;

    /// measured actuator current X force
    float xCurrent[FA_X_COUNT];
    /// measured actuator current Y force
    float yCurrent[FA_Y_COUNT];
    /// measured actuator current Z force
    float zCurrent[FA_Z_COUNT];

    /// target actuator X force
    float xTarget[FA_X_COUNT];
    /// target actuator Y force
    float yTarget[FA_Y_COUNT];
    /// target actuator Z force
    float zTarget[FA_Z_COUNT];

    /// difference (error) between current and target X force
    float xOffset[FA_X_COUNT];
    /// difference (error) between current and target Y force
    float yOffset[FA_Y_COUNT];
    /// difference (error) between current and target Z force
    float zOffset[FA_Z_COUNT];

private:
    const char *_name;
    float _maxRateOfChange;
    float _nearZeroValue;

    ForceComponentState _state;

    /**
     * Zero target forces.
     */
    void _zeroTarget();

    void _zeroAll();
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* LSST_M1M3_SS_FORCECONTROLLER_FORCECOMPONENT_H_ */
