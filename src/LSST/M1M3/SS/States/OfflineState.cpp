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
#include <Model.h>
#include <OfflineState.h>
#include <PowerController.h>
#include <RaisingLoweringInfo.h>
#include <SimulationMode.h>

#include <spdlog/spdlog.h>

namespace LSST {
namespace M1M3 {
namespace SS {

OfflineState::OfflineState() : State("OfflineState") {}

States::Type OfflineState::update(UpdateCommand* command) {
    return rejectCommandInvalidState(command, "Update");
}

States::Type OfflineState::enterControl(EnterControlCommand* command) {
    SPDLOG_INFO("OfflineState: enterControl()");

    RaisingLoweringInfo::instance().sendUpdates(true);

    M1M3SSPublisher::instance().logSoftwareVersions();
#ifdef SIMULATOR
    SimulationMode::instance().setSimulationMode(1);
#else
    SimulationMode::instance().setSimulationMode(0);
#endif
    Model::instance().publishRecommendedSettings();

    auto& digitalInputOutput = DigitalInputOutput::instance();
    digitalInputOutput.turnAirOff();
    digitalInputOutput.turnCellLightsOff();
    return States::StandbyState;
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
