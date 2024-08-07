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

#ifndef LSST_FORCECONTROLLERSTATE_H
#define LSST_FORCECONTROLLERSTATE_H

#include <string.h>

#include <SAL_MTM1M3.h>

#include <cRIO/Singleton.h>

#include <M1M3SSPublisher.h>

namespace LSST {
namespace M1M3 {
namespace SS {

/**
 * Wrapper object for MTM1M3_logevent_forceControllerStateC.
 */
class ForceControllerState : public MTM1M3_logevent_forceControllerStateC,
                             public cRIO::Singleton<ForceControllerState> {
public:
    /**
     * Construct new ForceActuatorState
     */
    ForceControllerState(token) {
        slewFlag = false;
        accelerationForcesApplied = false;
        activeOpticForcesApplied = false;
        azimuthForcesApplied = false;
        balanceForcesApplied = false;
        elevationForcesApplied = false;
        offsetForcesApplied = false;
        staticForcesApplied = false;
        thermalForcesApplied = false;
        velocityForcesApplied = false;
    }

    /**
     * Sends updates through SAL/DDS.
     */
    void log() { M1M3SSPublisher::instance().logForceControllerState(this); }

#define appliedSetter(name)         \
    void set_##name(bool applied) { \
        if (name != applied) {      \
            name = applied;         \
            log();                  \
        }                           \
    }

    appliedSetter(slewFlag);
    appliedSetter(accelerationForcesApplied);
    appliedSetter(activeOpticForcesApplied);
    appliedSetter(azimuthForcesApplied);
    appliedSetter(balanceForcesApplied);
    appliedSetter(elevationForcesApplied);
    appliedSetter(offsetForcesApplied);
    appliedSetter(staticForcesApplied);
    appliedSetter(thermalForcesApplied);
    appliedSetter(velocityForcesApplied);
};

}  // namespace SS
}  // namespace M1M3
}  // namespace LSST

#endif  // LSST_FORCECONTROLLERSTATE_H
