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

StaticStateFactory::StaticStateFactory(M1M3SSPublisher* publisher)
        : offlineState(publisher),
          standbyState(publisher),
          disabledState(publisher),
          enabledState(publisher),
          parkedState(publisher),
          raisingState(publisher),
          activeState(publisher),
          loweringState(publisher),
          engineeringState(publisher),
          parkedEngineeringState(publisher),
          raisingEngineeringState(publisher),
          activeEngineeringState(publisher),
          loweringEngineeringState(publisher),
          faultState(publisher),
          loweringFaultState(publisher),
          profileHardpointCorrectionState(publisher) {
    spdlog::debug("StaticStateFactory: StaticStateFactor()");
}

State* StaticStateFactory::create(States::Type state) {
    spdlog::trace("StaticStateFactory: create({:d})", state);
    switch (state) {
        case States::OfflineState:
            return &this->offlineState;
        case States::StandbyState:
            return &this->standbyState;
        case States::DisabledState:
            return &this->disabledState;
        case States::EnabledState:
            return &this->enabledState;
        case States::ParkedState:
            return &this->parkedState;
        case States::RaisingState:
            return &this->raisingState;
        case States::ActiveState:
            return &this->activeState;
        case States::LoweringState:
            return &this->loweringState;
        case States::EngineeringState:
            return &this->engineeringState;
        case States::ParkedEngineeringState:
            return &this->parkedEngineeringState;
        case States::RaisingEngineeringState:
            return &this->raisingEngineeringState;
        case States::ActiveEngineeringState:
            return &this->activeEngineeringState;
        case States::LoweringEngineeringState:
            return &this->loweringEngineeringState;
        case States::FaultState:
            return &this->faultState;
        case States::LoweringFaultState:
            return &this->loweringFaultState;
        case States::ProfileHardpointCorrectionState:
            return &this->profileHardpointCorrectionState;
        default:
            return 0;
    }
}

void StaticStateFactory::destroy(State* state) { spdlog::trace("StaticStateFactory: destroy()"); }

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
