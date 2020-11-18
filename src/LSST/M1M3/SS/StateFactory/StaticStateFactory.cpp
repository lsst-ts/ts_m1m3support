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

#include <StaticStateFactory.h>
#include <spdlog/spdlog.h>

namespace LSST {
namespace M1M3 {
namespace SS {

StaticStateFactory::StaticStateFactory()
        : _offlineState(),
          _standbyState(),
          _disabledState(),
          _parkedState(),
          _raisingState(),
          _activeState(),
          _loweringState(),
          _parkedEngineeringState(),
          _raisingEngineeringState(),
          _activeEngineeringState(),
          _loweringEngineeringState(),
          _faultState(),
          _loweringFaultState(),
          _profileHardpointCorrectionState() {
    SPDLOG_DEBUG("StaticStateFactory: StaticStateFactor()");
}

StaticStateFactory& StaticStateFactory::get() {
    static StaticStateFactory stateFactory;
    return stateFactory;
}

State* StaticStateFactory::create(States::Type state) {
    SPDLOG_TRACE("StaticStateFactory: create({:d})", state);
    switch (state) {
        case States::OfflineState:
            return &_offlineState;
        case States::StandbyState:
            return &_standbyState;
        case States::DisabledState:
            return &this->_disabledState;
        case States::ParkedState:
            return &_parkedState;
        case States::RaisingState:
            return &_raisingState;
        case States::ActiveState:
            return &_activeState;
        case States::LoweringState:
            return &_loweringState;
        case States::ParkedEngineeringState:
            return &_parkedEngineeringState;
        case States::RaisingEngineeringState:
            return &_raisingEngineeringState;
        case States::ActiveEngineeringState:
            return &_activeEngineeringState;
        case States::LoweringEngineeringState:
            return &_loweringEngineeringState;
        case States::FaultState:
            return &_faultState;
        case States::LoweringFaultState:
            return &_loweringFaultState;
        case States::ProfileHardpointCorrectionState:
            return &_profileHardpointCorrectionState;
        default:
            return 0;
    }
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
