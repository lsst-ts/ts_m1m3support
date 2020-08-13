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

#ifndef LSST_M1M3_SS_FORCECONTROLLER_FINALFORCECOMPONENT_H_
#define LSST_M1M3_SS_FORCECONTROLLER_FINALFORCECOMPONENT_H_

#include <ForceComponent.h>
#include <ForceActuatorApplicationSettings.h>
#include <ForceActuatorSettings.h>
#include <SafetyController.h>
#include <SAL_MTM1M3C.h>

namespace LSST {
namespace M1M3 {
namespace SS {

class FinalForceComponent : public ForceComponent {
public:
    FinalForceComponent(SafetyController* safetyController,
                        ForceActuatorApplicationSettings* forceActuatorApplicationSettings,
                        ForceActuatorSettings* forceActuatorSettings);

    void applyForces(float* x, float* y, float* z);
    void applyForcesByComponents();

protected:
    void postEnableDisableActions();
    void postUpdateActions();

private:
    SafetyController* _safetyController;
    ForceActuatorApplicationSettings* _forceActuatorApplicationSettings;
    ForceActuatorSettings* _forceActuatorSettings;

    MTM1M3_logevent_forceActuatorStateC* _forceActuatorState;
    MTM1M3_logevent_forceSetpointWarningC* _forceSetpointWarning;
    MTM1M3_logevent_appliedForcesC* _appliedForces;
    MTM1M3_logevent_rejectedForcesC* _rejectedForces;

    MTM1M3_logevent_appliedAberrationForcesC* _appliedAberrationForces;
    MTM1M3_logevent_appliedAccelerationForcesC* _appliedAccelerationForces;
    MTM1M3_logevent_appliedActiveOpticForcesC* _appliedActiveOpticForces;
    MTM1M3_logevent_appliedAzimuthForcesC* _appliedAzimuthForces;
    MTM1M3_logevent_appliedBalanceForcesC* _appliedBalanceForces;
    MTM1M3_logevent_appliedElevationForcesC* _appliedElevationForces;
    MTM1M3_logevent_appliedOffsetForcesC* _appliedOffsetForces;
    MTM1M3_logevent_appliedStaticForcesC* _appliedStaticForces;
    MTM1M3_logevent_appliedThermalForcesC* _appliedThermalForces;
    MTM1M3_logevent_appliedVelocityForcesC* _appliedVelocityForces;
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* LSST_M1M3_SS_FORCECONTROLLER_FINALFORCECOMPONENT_H_ */
