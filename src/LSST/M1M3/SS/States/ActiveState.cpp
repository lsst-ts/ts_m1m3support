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
#include <AutomaticOperationsController.h>
#include <ForceController.h>
#include <spdlog/spdlog.h>

namespace LSST {
namespace M1M3 {
namespace SS {

ActiveState::ActiveState(M1M3SSPublisher* publisher) : EnabledState(publisher, "ActiveState") {}

States::Type ActiveState::update(UpdateCommand* command, Model* model) {
    spdlog::trace("ActiveState: update()");
    sendTelemetry(model);
    return model->getSafetyController()->checkSafety(States::NoStateTransition);
}

States::Type ActiveState::enterEngineering(EnterEngineeringCommand* command, Model* model) {
    spdlog::info("ActiveState: enterEngineering()");
    return model->getSafetyController()->checkSafety(States::ActiveEngineeringState);
}

States::Type ActiveState::lowerM1M3(LowerM1M3Command* command, Model* model) {
    spdlog::info("ActiveState: lowerM1M3()");
    model->getAutomaticOperationsController()->startLowerOperation();
    return model->getSafetyController()->checkSafety(States::LoweringState);
}

States::Type ActiveState::enableHardpointCorrections(EnableHardpointCorrectionsCommand* command,
                                                     Model* model) {
    spdlog::info("ActiveState: enableHardpointCorrections()");
    model->getForceController()->applyBalanceForces();
    return model->getSafetyController()->checkSafety(States::NoStateTransition);
}

States::Type ActiveState::disableHardpointCorrections(DisableHardpointCorrectionsCommand* command,
                                                      Model* model) {
    spdlog::info("ActiveState: disableHardpointCorrections()");
    model->getForceController()->zeroBalanceForces();
    return model->getSafetyController()->checkSafety(States::NoStateTransition);
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
