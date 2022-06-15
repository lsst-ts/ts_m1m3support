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

#include <EnabledActiveState.h>
#include <Model.h>
#include <spdlog/spdlog.h>

namespace LSST {
namespace M1M3 {
namespace SS {

States::Type EnabledActiveState::lowerM1M3(LowerM1M3Command* command) {
    SPDLOG_INFO("EnabledActiveState: lowerM1M3()");
    Model::get().getForceController()->resetPIDs();
    Model::get().getMirrorLowerController()->start();
    return Model::get().getSafetyController()->checkSafety(getLoweringState());
}

States::Type EnabledActiveState::applyAberrationForces(ApplyAberrationForcesCommand* command) {
    SPDLOG_INFO("ActiveEngineeringState: applyAberrationForces()");
    Model::get().getForceController()->applyAberrationForces(command->getData()->zForces);
    Model::get().getForceController()->processAppliedForces();
    return Model::get().getSafetyController()->checkSafety(States::NoStateTransition);
}

States::Type EnabledActiveState::clearAberrationForces(ClearAberrationForcesCommand* command) {
    SPDLOG_INFO("EnabledActiveState: clearAberrationForces()");
    Model::get().getForceController()->zeroAberrationForces();
    Model::get().getForceController()->processAppliedForces();
    return Model::get().getSafetyController()->checkSafety(States::NoStateTransition);
}

States::Type EnabledActiveState::applyActiveOpticForces(ApplyActiveOpticForcesCommand* command) {
    SPDLOG_INFO("ActiveEngineeringState: applyActiveOpticForces()");
    Model::get().getForceController()->applyActiveOpticForces(command->getData()->zForces);
    Model::get().getForceController()->processAppliedForces();
    return Model::get().getSafetyController()->checkSafety(States::NoStateTransition);
}

States::Type EnabledActiveState::clearActiveOpticForces(ClearActiveOpticForcesCommand* command) {
    SPDLOG_INFO("EnabledActiveState: clearActiveOpticForces()");
    Model::get().getForceController()->zeroActiveOpticForces();
    Model::get().getForceController()->processAppliedForces();
    return Model::get().getSafetyController()->checkSafety(States::NoStateTransition);
}

States::Type EnabledActiveState::enableHardpointCorrections(EnableHardpointCorrectionsCommand* command) {
    SPDLOG_INFO("EnabledActiveState: enableHardpointCorrections()");
    Model::get().getForceController()->applyBalanceForces();
    return Model::get().getSafetyController()->checkSafety(States::NoStateTransition);
}

States::Type EnabledActiveState::disableHardpointCorrections(DisableHardpointCorrectionsCommand* command) {
    SPDLOG_INFO("EnabledActiveState: disableHardpointCorrections()");
    Model::get().getForceController()->zeroBalanceForces();
    return Model::get().getSafetyController()->checkSafety(States::NoStateTransition);
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
