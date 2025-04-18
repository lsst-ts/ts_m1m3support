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
#ifndef LSST_DETAILEDSTATE_H
#define LSST_DETAILEDSTATE_H

#include <SAL_MTM1M3.h>

#include <cRIO/Singleton.h>

namespace LSST {
namespace M1M3 {
namespace SS {

/**
 * Manipulates DetailedState event fields.
 */
class DetailedState : public MTM1M3_logevent_detailedStateC, public cRIO::Singleton<DetailedState> {
public:
    DetailedState(token);

    /**
     * Sends updates to DetailedState event.
     */
    void send();

    void set(int new_state);

    inline bool is_raising() {
        switch (detailedState) {
            case MTM1M3::MTM1M3_shared_DetailedStates_RaisingState:
            case MTM1M3::MTM1M3_shared_DetailedStates_RaisingEngineeringState:
                return true;
            default:
                return false;
        }
    }

    inline bool is_lowering() {
        switch (detailedState) {
            case MTM1M3::MTM1M3_shared_DetailedStates_LoweringState:
            case MTM1M3::MTM1M3_shared_DetailedStates_LoweringEngineeringState:
                return true;
            default:
                return false;
        }
    }

    inline bool is_parked() {
        switch (detailedState) {
            case MTM1M3::MTM1M3_shared_DetailedStates_DisabledState:
            case MTM1M3::MTM1M3_shared_DetailedStates_ParkedState:
            case MTM1M3::MTM1M3_shared_DetailedStates_ParkedEngineeringState:
                return true;
            default:
                return false;
        }
    }

    inline bool is_active() {
        switch (detailedState) {
            case MTM1M3::MTM1M3_shared_DetailedStates_ActiveState:
            case MTM1M3::MTM1M3_shared_DetailedStates_ActiveEngineeringState:
                return true;
            default:
                return false;
        }
    }
};

}  // namespace SS
}  // namespace M1M3
}  // namespace LSST

#endif  // !LSST_DETAILEDSTATE_H
