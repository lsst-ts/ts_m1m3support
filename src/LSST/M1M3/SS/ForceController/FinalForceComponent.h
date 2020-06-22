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
#include <SAL_MTM1M3C.h>

namespace LSST {
namespace M1M3 {
namespace SS {

class M1M3SSPublisher;
class SafetyController;
class ForceActuatorApplicationSettings;
class ForceActuatorSettings;

class FinalForceComponent : public ForceComponent {
private:
    M1M3SSPublisher* publisher;
    SafetyController* safetyController;
    ForceActuatorApplicationSettings* forceActuatorApplicationSettings;
    ForceActuatorSettings* forceActuatorSettings;

    MTM1M3_logevent_forceActuatorStateC* forceActuatorState;
    MTM1M3_logevent_forceSetpointWarningC* forceSetpointWarning;
    MTM1M3_logevent_appliedForcesC* appliedForces;
    MTM1M3_logevent_rejectedForcesC* rejectedForces;

    MTM1M3_logevent_appliedAberrationForcesC* appliedAberrationForces;
    MTM1M3_logevent_appliedAccelerationForcesC* appliedAccelerationForces;
    MTM1M3_logevent_appliedActiveOpticForcesC* appliedActiveOpticForces;
    MTM1M3_logevent_appliedAzimuthForcesC* appliedAzimuthForces;
    MTM1M3_logevent_appliedBalanceForcesC* appliedBalanceForces;
    MTM1M3_logevent_appliedElevationForcesC* appliedElevationForces;
    MTM1M3_logevent_appliedOffsetForcesC* appliedOffsetForces;
    MTM1M3_logevent_appliedStaticForcesC* appliedStaticForces;
    MTM1M3_logevent_appliedThermalForcesC* appliedThermalForces;
    MTM1M3_logevent_appliedVelocityForcesC* appliedVelocityForces;

public:
    FinalForceComponent(M1M3SSPublisher* publisher, SafetyController* safetyController,
                        ForceActuatorApplicationSettings* forceActuatorApplicationSettings,
                        ForceActuatorSettings* forceActuatorSettings);

    void applyForces(float* x, float* y, float* z);
    void applyForcesByComponents();

protected:
    void postEnableDisableActions();
    void postUpdateActions();
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* LSST_M1M3_SS_FORCECONTROLLER_FINALFORCECOMPONENT_H_ */
