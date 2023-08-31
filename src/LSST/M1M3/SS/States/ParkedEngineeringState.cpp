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
#include <unistd.h>

#include <ApplyOffsetForcesCommand.h>
#include <BoosterValveStatus.h>
#include <DigitalInputOutput.h>
#include <DisableHardpointChaseCommand.h>
#include <Displacement.h>
#include <EnableHardpointChaseCommand.h>
#include <ForceController.h>
#include <ILC.h>
#include <Inclinometer.h>
#include <Model.h>
#include <MoveHardpointActuatorsCommand.h>
#include <ParkedEngineeringState.h>
#include <PositionController.h>
#include <PowerController.h>
#include <RaiseM1M3Command.h>
#include <SafetyController.h>
#include <TurnPowerOnCommand.h>
#include <TurnPowerOffCommand.h>

using namespace LSST::M1M3::SS;

ParkedEngineeringState::ParkedEngineeringState() : EnabledState("ParkedEngineeringState") {}

States::Type ParkedEngineeringState::update(UpdateCommand* command) {
    SPDLOG_TRACE("ParkedEngineeringState: update()");
    sendTelemetry();

    // check & run tests
    Model::instance().getBumpTestController()->runLoop();
    Model::instance().getHardpointTestController()->runLoop();

    return Model::instance().getSafetyController()->checkSafety(States::NoStateTransition);
}

States::Type ParkedEngineeringState::raiseM1M3(RaiseM1M3Command* command) {
    SPDLOG_INFO("ParkedEngineeringState: raiseM1M3()");

    Model::instance().getBumpTestController()->stopAll(true);
    Model::instance().getHardpointTestController()->killHardpointTest(-1);
    BoosterValveStatus::instance().setUserTriggered(false);

    Model::instance().getMirrorRaiseController()->start(command->getData()->bypassReferencePosition);
    return Model::instance().getSafetyController()->checkSafety(States::RaisingEngineeringState);
}

States::Type ParkedEngineeringState::exitEngineering(ExitEngineeringCommand* command) {
    SPDLOG_INFO("ParkedEngineeringState: exitEngineering()");

    Model::instance().getBumpTestController()->stopAll(true);
    Model::instance().getHardpointTestController()->killHardpointTest(-1);

    DigitalInputOutput::instance().turnAirOn();
    Model::instance().getPositionController()->stopMotion();
    Model::instance().getForceController()->zeroOffsetForces();
    Model::instance().getForceController()->processAppliedForces();
    DigitalInputOutput::instance().turnCellLightsOff();
    // TODO: Real problems exist if the user enabled / disabled ILC power...
    Model::instance().getPowerController()->setAllPowerNetworks(true);
    BoosterValveStatus::instance().setUserTriggered(false);
    return Model::instance().getSafetyController()->checkSafety(States::ParkedState);
}

States::Type ParkedEngineeringState::disable(DisableCommand* command) {
    SPDLOG_INFO("ParkedEngineeringState: disable()");

    Model::instance().getBumpTestController()->stopAll(true);
    Model::instance().getHardpointTestController()->killHardpointTest(-1);

    // Stop any existing motion (chase and move commands)
    Model::instance().getPositionController()->stopMotion();
    Model::instance().getForceController()->reset();

    return EnabledState::disableMirror();
}

States::Type ParkedEngineeringState::forceActuatorBumpTest(ForceActuatorBumpTestCommand* command) {
    SPDLOG_INFO("ParkedEngineeringState: forceActuatorBumpTest({}, {}, {})", command->getData()->actuatorId,
                command->getData()->testPrimary, command->getData()->testSecondary);
    Model::instance().getBumpTestController()->setBumpTestActuator(command->getData()->actuatorId,
                                                                   command->getData()->testPrimary,
                                                                   command->getData()->testSecondary);
    return Model::instance().getSafetyController()->checkSafety(States::NoStateTransition);
}

States::Type ParkedEngineeringState::killForceActuatorBumpTest(KillForceActuatorBumpTestCommand* command) {
    SPDLOG_INFO("ParkedEngineeringState: killForceActuatorBumpTest()");
    Model::instance().getBumpTestController()->stopAll(false);
    return Model::instance().getSafetyController()->checkSafety(States::NoStateTransition);
}

States::Type ParkedEngineeringState::testHardpoint(TestHardpointCommand* command) {
    SPDLOG_INFO("ParkedEngineeringState: testHardpoint({})", command->getData()->hardpointActuator);
    Model::instance().getHardpointTestController()->startHardpointTest(command->getData()->hardpointActuator -
                                                                       1);
    return Model::instance().getSafetyController()->checkSafety(States::NoStateTransition);
}

States::Type ParkedEngineeringState::killHardpointTest(KillHardpointTestCommand* command) {
    SPDLOG_INFO("ParkedEngineeringState: KillHardpointTest({})", command->getData()->hardpointActuator);
    Model::instance().getHardpointTestController()->killHardpointTest(command->getData()->hardpointActuator -
                                                                      1);
    return Model::instance().getSafetyController()->checkSafety(States::NoStateTransition);
}
