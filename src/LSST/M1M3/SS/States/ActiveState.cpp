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

#include <ActiveState.h>
#include <Model.h>
#include <Publisher.h>
#include <SafetyController.h>
#include <MirrorController.h>
#include <ForceController.h>
#include <spdlog/spdlog.h>

namespace LSST {
namespace M1M3 {
namespace SS {

ActiveState::ActiveState() : EnabledState("ActiveState") {}

States::Type ActiveState::update(UpdateCommand* command) {
    SPDLOG_TRACE("ActiveState: update()");
    sendTelemetry();
    return Model::get().getSafetyController()->checkSafety(States::NoStateTransition);
}

States::Type ActiveState::enterEngineering(EnterEngineeringCommand* command) {
    SPDLOG_INFO("ActiveState: enterEngineering()");
    return Model::get().getSafetyController()->checkSafety(States::ActiveEngineeringState);
}

States::Type ActiveState::lowerM1M3(LowerM1M3Command* command) {
    SPDLOG_INFO("ActiveState: lowerM1M3()");
    Model::get().getMirrorController()->startLowerOperation();
    return Model::get().getSafetyController()->checkSafety(States::LoweringState);
}

States::Type ActiveState::enableHardpointCorrections(EnableHardpointCorrectionsCommand* command) {
    SPDLOG_INFO("ActiveState: enableHardpointCorrections()");
    Model::get().getForceController()->applyBalanceForces();
    return Model::get().getSafetyController()->checkSafety(States::NoStateTransition);
}

States::Type ActiveState::disableHardpointCorrections(DisableHardpointCorrectionsCommand* command) {
    SPDLOG_INFO("ActiveState: disableHardpointCorrections()");
    Model::get().getForceController()->zeroBalanceForces();
    return Model::get().getSafetyController()->checkSafety(States::NoStateTransition);
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
