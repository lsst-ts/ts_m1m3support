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

#ifndef LSST_M1M3_SS_FORCECONTROLLER_BALANCEFORCECOMPONENT_H_
#define LSST_M1M3_SS_FORCECONTROLLER_BALANCEFORCECOMPONENT_H_

#include <ForceComponent.h>
#include <PID.h>
#include <SAL_MTM1M3C.h>

namespace LSST {
namespace M1M3 {
namespace SS {

class M1M3SSPublisher;
class SafetyController;
class ForceActuatorApplicationSettings;
class ForceActuatorSettings;
class PIDSettings;

class BalanceForceComponent : public ForceComponent {
public:
    BalanceForceComponent(M1M3SSPublisher* publisher, SafetyController* safetyController,
                          ForceActuatorApplicationSettings* forceActuatorApplicationSettings,
                          ForceActuatorSettings* forceActuatorSettings, PIDSettings* pidSettings);

    void applyBalanceForces(float* x, float* y, float* z);
    void applyBalanceForcesByMirrorForces(float xForce, float yForce, float zForce, float xMoment,
                                          float yMoment, float zMoment);

    void updatePID(int id, PIDParameters parameters);
    void resetPID(int id);
    void resetPIDs();

protected:
    void postEnableDisableActions();
    void postUpdateActions();

private:
    PID* _idToPID(int id);

    M1M3SSPublisher* _publisher;
    SafetyController* _safetyController;
    ForceActuatorApplicationSettings* _forceActuatorApplicationSettings;
    ForceActuatorSettings* _forceActuatorSettings;
    PIDSettings* _pidSettings;

    PID _fx;
    PID _fy;
    PID _fz;
    PID _mx;
    PID _my;
    PID _mz;

    MTM1M3_logevent_forceActuatorStateC* _forceActuatorState;
    MTM1M3_logevent_forceSetpointWarningC* _forceSetpointWarning;
    MTM1M3_logevent_appliedBalanceForcesC* _appliedBalanceForces;
    MTM1M3_logevent_rejectedBalanceForcesC* _rejectedBalanceForces;
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* LSST_M1M3_SS_FORCECONTROLLER_BALANCEFORCECOMPONENT_H_ */
