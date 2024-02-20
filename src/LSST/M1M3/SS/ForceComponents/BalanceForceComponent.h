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

#ifndef LSST_M1M3_SS_FORCECONTROLLER_BALANCEFORCECOMPONENT_H_
#define LSST_M1M3_SS_FORCECONTROLLER_BALANCEFORCECOMPONENT_H_

#include <SAL_MTM1M3C.h>

#include <ForceActuatorApplicationSettings.h>
#include <ForceComponent.h>
#include <PID.h>
#include <SafetyController.h>

namespace LSST {
namespace M1M3 {
namespace SS {

/**
 * Distributes balance forces. Only active when enabledHardpointChase command
 * was called. Takes forces and moments measured on hardpoints (transformed to
 * XYZ space from hardpoints load cells measurements), feed them through PIDs,
 * and calculates force offsets (12*X, 100*Y, 156*Z). Applied balance forces
 * are published in appliedBalanceForces SAL/DDS event.
 *
 * The purpose of this is to see almost 0 forces acting on hardpoints.
 * Hardpoints task is not to actively steer the mirror, their purpose is to
 * hold mirror in position. Mirror is being steered by applying offsets
 * (moving) hardpoints, which are immediately (as hardpoints moves and forces
 * are being measured on hardpoints load cells) offloaded to 156 mirror force
 * actuators (assuming hardpoints chase is enabled).
 *
 * @see LSST::M1M3::SS::PID
 */
class BalanceForceComponent : public ForceComponent {
public:
    BalanceForceComponent(ForceActuatorApplicationSettings* forceActuatorApplicationSettings);

    void applyBalanceForces(std::vector<float> x, std::vector<float> y, std::vector<float> z);

    /**
     * Called from ForceController::updateAppliedForces. Feeds in hardpoint
     * force and moments measured from the hardpoint load cells and transformed
     * into xyz forces and moments.
     *
     * @param xForce X projection of force acting on hardpoints
     * @param yForce Y projection of force acting on hardpoints
     * @param zForce Z projection of force acting on hardpoints
     * @param xMoment X projection of moment acting on hardpoints
     * @param yMoment Y projection of moment acting on hardpoints
     * @param zMoment Z projection of moment acting on hardpoints
     */
    void applyBalanceForcesByMirrorForces(float xForce, float yForce, float zForce, float xMoment,
                                          float yMoment, float zMoment);

    void updatePID(int id, PIDParameters parameters);
    void resetPID(int id);
    void resetPIDs();

    void postEnableDisableActions() override;

protected:
    void postUpdateActions() override;

private:
    PID* _idToPID(int id);

    SafetyController* _safetyController;
    ForceActuatorApplicationSettings* _forceActuatorApplicationSettings;

    PID _fx;
    PID _fy;
    PID _fz;
    PID _mx;
    PID _my;
    PID _mz;

    MTM1M3_logevent_forceSetpointWarningC* _forceSetpointWarning;
    MTM1M3_appliedBalanceForcesC* _appliedBalanceForces;
    MTM1M3_logevent_preclippedBalanceForcesC* _preclippedBalanceForces;
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* LSST_M1M3_SS_FORCECONTROLLER_BALANCEFORCECOMPONENT_H_ */
