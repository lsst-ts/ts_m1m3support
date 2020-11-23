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

#include <ParkedState.h>
#include <ILC.h>
#include <Model.h>
#include <M1M3SSPublisher.h>
#include <SafetyController.h>
#include <ForceController.h>
#include <PositionController.h>
#include <MirrorController.h>
#include <RaiseM1M3Command.h>
#include <DigitalInputOutput.h>
#include <PowerController.h>
#include <spdlog/spdlog.h>

namespace LSST {
namespace M1M3 {
namespace SS {

ParkedState::ParkedState() : EnabledState("ParkedState") {}

States::Type ParkedState::update(UpdateCommand* command) {
    SPDLOG_TRACE("ParkedState: update()");
    sendTelemetry();
    return Model::get().getSafetyController()->checkSafety(States::NoStateTransition);
}

States::Type ParkedState::raiseM1M3(RaiseM1M3Command* command) {
    SPDLOG_INFO("ParkedState: raiseM1M3()");
    if (command->getData()->bypassReferencePosition) {
        M1M3SSPublisher::get().logCommandRejectionWarning(
                "RaiseM1M3",
                "The BypassReferencePosition parameter of the RaiseM1M3 cannot be true in the ParkedState.");
        return Model::get().getSafetyController()->checkSafety(States::NoStateTransition);
    }
    Model::get().getMirrorController()->startRaiseOperation(false);
    return Model::get().getSafetyController()->checkSafety(States::RaisingState);
}

States::Type ParkedState::enterEngineering(EnterEngineeringCommand* command) {
    SPDLOG_INFO("ParkedState: enterEngineering()");
    return Model::get().getSafetyController()->checkSafety(States::ParkedEngineeringState);
}

States::Type ParkedState::disable(DisableCommand* command) {
    SPDLOG_INFO("ParkedState: disable()");
    Model::get().getILC()->writeSetModeDisableBuffer();
    Model::get().getILC()->triggerModbus();
    Model::get().getILC()->waitForAllSubnets(5000);
    Model::get().getILC()->readAll();
    Model::get().getILC()->verifyResponses();
    Model::get().getForceController()->reset();
    // TODO: Uncomment this when its not so hot outside
    // Model::get().getDigitalInputOutput()->turnAirOff();
    Model::get().getPowerController()->setAllAuxPowerNetworks(false);
    return Model::get().getSafetyController()->checkSafety(States::DisabledState);
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
