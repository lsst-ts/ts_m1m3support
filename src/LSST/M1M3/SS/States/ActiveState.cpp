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

#include <spdlog/spdlog.h>

#include <SAL_MTM1M3_enums.h>

#include <ActiveState.h>
#include <BoosterValveController.h>
#include <DigitalInputOutput.h>
#include <ForceController.h>
#include <M1M3SSPublisher.h>
#include <Model.h>
#include <SafetyController.h>

namespace LSST {
namespace M1M3 {
namespace SS {

ActiveState::ActiveState() : EnabledState("ActiveState") {}

States::Type ActiveState::update(UpdateCommand *command) {
    SPDLOG_TRACE("ActiveState: update()");
    DigitalInputOutput::instance().toggleMirrorRaisedHB(0, true);
    sendTelemetry();
    DigitalInputOutput::instance().toggleMirrorRaisedHB(1, true);
    return Model::instance().getSafetyController()->checkSafety(States::NoStateTransition);
}

States::Type ActiveState::setSlewFlag(SetSlewFlagCommand *command) {
    SPDLOG_INFO("ActiveState: setSlewFlag()");
    Model::instance().getSlewController()->enterSlew();
    return Model::instance().getSafetyController()->checkSafety(States::NoStateTransition);
}

States::Type ActiveState::clearSlewFlag(ClearSlewFlagCommand *command) {
    SPDLOG_INFO("ActiveState: clearSlewFlag()");
    Model::instance().getSlewController()->exitSlew();
    return Model::instance().getSafetyController()->checkSafety(States::NoStateTransition);
}

States::Type ActiveState::enterEngineering(EnterEngineeringCommand *command) {
    SPDLOG_INFO("ActiveState: enterEngineering()");
    return Model::instance().getSafetyController()->checkSafety(States::ActiveEngineeringState);
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
