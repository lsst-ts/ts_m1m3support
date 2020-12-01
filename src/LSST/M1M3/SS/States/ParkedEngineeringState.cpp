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

#include <ParkedEngineeringState.h>
#include <Model.h>
#include <Displacement.h>
#include <Inclinometer.h>
#include <ILC.h>
#include <unistd.h>
#include <ForceController.h>
#include <ApplyOffsetForcesCommand.h>
#include <SafetyController.h>
#include <PositionController.h>
#include <MoveHardpointActuatorsCommand.h>
#include <EnableHardpointChaseCommand.h>
#include <DisableHardpointChaseCommand.h>
#include <DigitalInputOutput.h>
#include <PowerController.h>
#include <TurnPowerOnCommand.h>
#include <TurnPowerOffCommand.h>
#include <RaiseM1M3Command.h>
#include <spdlog/spdlog.h>

namespace LSST {
namespace M1M3 {
namespace SS {

ParkedEngineeringState::ParkedEngineeringState() : EngineeringState("ParkedEngineeringState") {}

States::Type ParkedEngineeringState::update(UpdateCommand* command) {
    SPDLOG_TRACE("ParkedEngineeringState: update()");
    sendTelemetry();

    // check & run tests
    Model::get().getBumpTestController()->runLoop();

    return Model::get().getSafetyController()->checkSafety(States::NoStateTransition);
}

States::Type ParkedEngineeringState::raiseM1M3(RaiseM1M3Command* command) {
    SPDLOG_INFO("ParkedEngineeringState: raiseM1M3()");

    Model::get().getBumpTestController()->stopAll();

    Model::get().getMirrorRaiseController()->start(command->getData()->bypassReferencePosition);
    return Model::get().getSafetyController()->checkSafety(States::RaisingEngineeringState);
}

States::Type ParkedEngineeringState::exitEngineering(ExitEngineeringCommand* command) {
    SPDLOG_INFO("ParkedEngineeringState: exitEngineering()");

    Model::get().getBumpTestController()->stopAll();

    Model::get().getDigitalInputOutput()->turnAirOn();
    Model::get().getPositionController()->stopMotion();
    Model::get().getForceController()->zeroOffsetForces();
    Model::get().getForceController()->processAppliedForces();
    Model::get().getDigitalInputOutput()->turnCellLightsOff();
    // TODO: Real problems exist if the user enabled / disabled ILC power...
    Model::get().getPowerController()->setAllPowerNetworks(true);
    return Model::get().getSafetyController()->checkSafety(States::ParkedState);
}

States::Type ParkedEngineeringState::disable(DisableCommand* command) {
    SPDLOG_INFO("ParkedEngineeringState: disable()");

    Model::get().getBumpTestController()->stopAll();

    // Stop any existing motion (chase and move commands)
    Model::get().getPositionController()->stopMotion();
    Model::get().getForceController()->reset();
    // Perform ILC state transition
    Model::get().getILC()->writeSetModeDisableBuffer();
    Model::get().getILC()->triggerModbus();
    Model::get().getILC()->waitForAllSubnets(5000);
    Model::get().getILC()->readAll();
    Model::get().getILC()->verifyResponses();
    // TODO: Uncomment this later when its not so hot outside
    // Model::get().getDigitalInputOutput()->turnAirOff();
    Model::get().getPowerController()->setAllAuxPowerNetworks(false);
    return Model::get().getSafetyController()->checkSafety(States::DisabledState);
}

States::Type ParkedEngineeringState::forceActuatorBumpTest(ForceActuatorBumpTestCommand* command) {
    SPDLOG_INFO("ParkedEngineeringState: forceActuatorBumpTest({}, {}, {})", command->getData()->actuatorId,
                command->getData()->testPrimary, command->getData()->testSecondary);
    Model::get().getBumpTestController()->setBumpTestActuator(command->getData()->actuatorId,
                                                              command->getData()->testPrimary,
                                                              command->getData()->testSecondary);
    return Model::get().getSafetyController()->checkSafety(States::NoStateTransition);
}

States::Type ParkedEngineeringState::killForceActuatorBumpTest(KillForceActuatorBumpTestCommand* command) {
    SPDLOG_INFO("ParkedEngineeringState: killForceActuatorBumpTest()");
    Model::get().getBumpTestController()->stopAll();
    return Model::get().getSafetyController()->checkSafety(States::NoStateTransition);
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
