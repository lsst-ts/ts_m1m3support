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

#ifndef LSST_M1M3_SS_FORCECONTROLLER_STATICFORCECOMPONENT_H_
#define LSST_M1M3_SS_FORCECONTROLLER_STATICFORCECOMPONENT_H_

#include <vector>

#include <SAL_MTM1M3C.h>

#include "ForceComponent.h"
#include "PreclippedForces.h"
#include "SafetyController.h"

namespace LSST {
namespace M1M3 {
namespace SS {

class StaticForceComponent : public ForceComponent {
public:
    StaticForceComponent();

    /**
     * Apply static forces. Enable static forces if those were disabled.
     *
     * @param x x static forces
     * @param y y static forces
     * @param z z static forces
     */
    void applyStaticForces(std::vector<float>* x, std::vector<float>* y, std::vector<float>* z);

    void postEnableDisableActions() override;

protected:
    void postUpdateActions() override;

private:
    SafetyController* _safetyController;

    MTM1M3_logevent_forceSetpointWarningC* _forceSetpointWarning;
    MTM1M3_logevent_appliedStaticForcesC* _appliedStaticForces;
    PreclippedForces<MTM1M3_logevent_preclippedStaticForcesC> _preclipped_static_forces;
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* LSST_M1M3_SS_FORCECONTROLLER_STATICFORCECOMPONENT_H_ */
