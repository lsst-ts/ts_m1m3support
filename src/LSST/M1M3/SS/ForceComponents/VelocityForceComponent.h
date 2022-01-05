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

#ifndef LSST_M1M3_SS_FORCECONTROLLER_VELOCITYFORCECOMPONENT_H_
#define LSST_M1M3_SS_FORCECONTROLLER_VELOCITYFORCECOMPONENT_H_

#include <ForceComponent.h>
#include <ForceActuatorApplicationSettings.h>
#include <ForceActuatorSettings.h>
#include <SafetyController.h>
#include <SAL_MTM1M3C.h>

namespace LSST {
namespace M1M3 {
namespace SS {

class VelocityForceComponent : public ForceComponent {
public:
    VelocityForceComponent(ForceActuatorApplicationSettings* forceActuatorApplicationSettings,
                           ForceActuatorSettings* forceActuatorSettings);

    void applyVelocityForces(float* x, float* y, float* z);
    void applyVelocityForcesByAngularVelocity(float angularVelocityX, float angularVelocityY,
                                              float angularVelocityZ);

protected:
    void postEnableDisableActions() override;
    void postUpdateActions() override;

private:
    SafetyController* _safetyController;
    ForceActuatorApplicationSettings* _forceActuatorApplicationSettings;
    ForceActuatorSettings* _forceActuatorSettings;

    MTM1M3_logevent_forceActuatorStateC* _forceActuatorState;
    MTM1M3_logevent_forceSetpointWarningC* _forceSetpointWarning;
    MTM1M3_logevent_appliedVelocityForcesC* _appliedVelocityForces;
    MTM1M3_logevent_preclippedVelocityForcesC* _preclippedVelocityForces;
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* LSST_M1M3_SS_FORCECONTROLLER_VELOCITYFORCECOMPONENT_H_ */
