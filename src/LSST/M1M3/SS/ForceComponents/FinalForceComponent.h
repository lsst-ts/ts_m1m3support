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

#ifndef LSST_M1M3_SS_FORCECONTROLLER_FINALFORCECOMPONENT_H_
#define LSST_M1M3_SS_FORCECONTROLLER_FINALFORCECOMPONENT_H_

#include <EnabledForceActuators.h>
#include <ForceComponent.h>
#include <ForceActuatorApplicationSettings.h>
#include <SafetyController.h>
#include <SAL_MTM1M3C.h>

namespace LSST {
namespace M1M3 {
namespace SS {

/**
 * @brief Final force produced as sum of components.
 *
 * Sum component forces stored in SAL events. Apply mirror safety checks.  Log
 * force emirror safety limits are violated.
 */
class FinalForceComponent : public ForceComponent {
public:
    /**
     * @brief Sets internal variables.
     *
     * @param forceActuatorApplicationSettings
     */
    FinalForceComponent(ForceActuatorApplicationSettings* forceActuatorApplicationSettings);

    /**
     * @brief Sums applied forces to target x,y and z forces.
     */
    void applyForcesByComponents();

protected:
    void postEnableDisableActions() override;
    void postUpdateActions() override;

private:
    SafetyController* _safetyController;
    EnabledForceActuators* _enabledForceActuators;
    ForceActuatorApplicationSettings* _forceActuatorApplicationSettings;

    MTM1M3_logevent_forceActuatorStateC* _forceActuatorState;
    MTM1M3_logevent_forceSetpointWarningC* _forceSetpointWarning;
    MTM1M3_appliedForcesC* _appliedForces;
    MTM1M3_logevent_preclippedForcesC* _preclippedForces;

    MTM1M3_appliedAccelerationForcesC* _appliedAccelerationForces;
    MTM1M3_logevent_appliedActiveOpticForcesC* _appliedActiveOpticForces;
    MTM1M3_appliedAzimuthForcesC* _appliedAzimuthForces;
    MTM1M3_appliedBalanceForcesC* _appliedBalanceForces;
    MTM1M3_appliedElevationForcesC* _appliedElevationForces;
    MTM1M3_logevent_appliedOffsetForcesC* _appliedOffsetForces;
    MTM1M3_logevent_appliedStaticForcesC* _appliedStaticForces;
    MTM1M3_appliedThermalForcesC* _appliedThermalForces;
    MTM1M3_appliedVelocityForcesC* _appliedVelocityForces;
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* LSST_M1M3_SS_FORCECONTROLLER_FINALFORCECOMPONENT_H_ */
