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

#include <DigitalInputOutput.h>
#include <ForceController.h>
#include <M1M3SSPublisher.h>
#include <Model.h>
#include <ParkedState.h>
#include <PositionController.h>
#include <PowerController.h>
#include <RaiseM1M3Command.h>
#include <SafetyController.h>
#include <spdlog/spdlog.h>

namespace LSST {
namespace M1M3 {
namespace SS {

ParkedState::ParkedState() : EnabledState("ParkedState") {}

States::Type ParkedState::update(UpdateCommand* command) {
    SPDLOG_TRACE("ParkedState: update()");
    sendTelemetry();
    return Model::instance().getSafetyController()->checkSafety(States::NoStateTransition);
}

States::Type ParkedState::raiseM1M3(RaiseM1M3Command* command) {
    SPDLOG_INFO("ParkedState: raiseM1M3()");
    if (command->getData()->bypassReferencePosition) {
        M1M3SSPublisher::instance().logCommandRejectionWarning(
                "RaiseM1M3",
                "The BypassReferencePosition parameter of the RaiseM1M3 "
                "cannot be true in the ParkedState.");
        return Model::instance().getSafetyController()->checkSafety(States::NoStateTransition);
    }
    Model::instance().getMirrorRaiseController()->start(false);
    return Model::instance().getSafetyController()->checkSafety(States::RaisingState);
}

States::Type ParkedState::enterEngineering(EnterEngineeringCommand* command) {
    SPDLOG_INFO("ParkedState: enterEngineering()");
    return Model::instance().getSafetyController()->checkSafety(States::ParkedEngineeringState);
}

States::Type ParkedState::disable(DisableCommand* command) {
    SPDLOG_INFO("ParkedState: disable()");
    return EnabledState::disableMirror();
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
