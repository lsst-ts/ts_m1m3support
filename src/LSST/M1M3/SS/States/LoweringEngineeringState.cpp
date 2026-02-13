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

#include <LoweringEngineeringState.h>
#include <Model.h>
#include <ModelPublisher.h>
#include <SafetyController.h>
#include <spdlog/spdlog.h>

using namespace LSST::M1M3::SS;

LoweringEngineeringState::LoweringEngineeringState() : EnabledState("LoweringEngineeringState") {}

States::Type LoweringEngineeringState::update(UpdateCommand* command) {
    ModelPublisher publishIt{};
    SPDLOG_TRACE("LoweringEngineeringState: update()");
    Model::instance().getMirrorLowerController()->runLoop();
    runLoop();
    return Model::instance().getSafetyController()->checkSafety(
            lowerCompleted() ? States::ParkedEngineeringState : States::NoStateTransition);
}

States::Type LoweringEngineeringState::pauseM1M3RaisingLowering(PauseM1M3RaisingLoweringCommand* command) {
    SPDLOG_INFO("Pausing M1M3 lowering in engineering state");
    Model::instance().getMirrorLowerController()->pauseM1M3Lowering();
    return Model::instance().getSafetyController()->checkSafety(States::PausedLoweringEngineeringState);
}
