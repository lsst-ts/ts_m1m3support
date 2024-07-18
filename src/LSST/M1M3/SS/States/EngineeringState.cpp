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

#include <ApplyOffsetForcesByMirrorForceCommand.h>
#include <ApplyOffsetForcesCommand.h>
#include <BoosterValveStatus.h>
#include <DigitalInputOutput.h>
#include <DisableHardpointChaseCommand.h>
#include <EnableHardpointChaseCommand.h>
#include <EngineeringState.h>
#include <ForceController.h>
#include <M1M3SSPublisher.h>
#include <Model.h>
#include <MoveHardpointActuatorsCommand.h>
#include <PositionController.h>
#include <PowerController.h>
#include <SafetyController.h>
#include <SlewControllerSettings.h>
#include <TurnPowerOffCommand.h>
#include <TurnPowerOnCommand.h>

using namespace LSST::M1M3::SS;

States::Type EngineeringState::turnAirOn(TurnAirOnCommand *command) {
    SPDLOG_INFO("{}: turnAirOn()", this->name);
    DigitalInputOutput::instance().turnAirOn();
    return Model::instance().getSafetyController()->checkSafety(States::NoStateTransition);
}

States::Type EngineeringState::turnAirOff(TurnAirOffCommand *command) {
    SPDLOG_INFO("{}: turnAirOff()", this->name);
    DigitalInputOutput::instance().turnAirOff();
    return Model::instance().getSafetyController()->checkSafety(States::NoStateTransition);
}

States::Type EngineeringState::boosterValveOpen(BoosterValveOpenCommand *command) {
    SPDLOG_INFO("{}: boosterValveOpen", name);
    BoosterValveStatus::instance().setUserTriggered(true);
    return Model::instance().getSafetyController()->checkSafety(States::NoStateTransition);
}

States::Type EngineeringState::boosterValveClose(BoosterValveCloseCommand *command) {
    SPDLOG_INFO("{}: boosterValveClose", name);
    BoosterValveStatus::instance().setUserTriggered(false);
    return Model::instance().getSafetyController()->checkSafety(States::NoStateTransition);
}

States::Type EngineeringState::stopHardpointMotion(StopHardpointMotionCommand *command) {
    SPDLOG_INFO("{}: stopHardpointMotion()", this->name);
    Model::instance().getPositionController()->stopMotion();
    return Model::instance().getSafetyController()->checkSafety(States::NoStateTransition);
}

States::Type EngineeringState::moveHardpointActuators(MoveHardpointActuatorsCommand *command) {
    SPDLOG_INFO("{}: moveHardpointActuators()", this->name);
    if (!Model::instance().getPositionController()->move(command->getData()->steps)) {
        M1M3SSPublisher::instance().logCommandRejectionWarning("MoveHardpointActuators",
                                                               "At least one hardpoint actuator commanded "
                                                               "to move is already MOVING or CHASING.");
    }
    return Model::instance().getSafetyController()->checkSafety(States::NoStateTransition);
}

States::Type EngineeringState::enableHardpointChase(EnableHardpointChaseCommand *command) {
    SPDLOG_INFO("{}: enableHardpointChase()", this->name);
    if (!Model::instance().getPositionController()->enableChaseAll()) {
        M1M3SSPublisher::instance().logCommandRejectionWarning("EnableHardpointChase",
                                                               "At least one hardpoint actuator commanded to "
                                                               "chase is already MOVING or CHASING.");
    }
    return Model::instance().getSafetyController()->checkSafety(States::NoStateTransition);
}

States::Type EngineeringState::disableHardpointChase(DisableHardpointChaseCommand *command) {
    SPDLOG_INFO("{}: disableHardpointChase()", this->name);
    Model::instance().getPositionController()->disableChaseAll();
    return Model::instance().getSafetyController()->checkSafety(States::NoStateTransition);
}

States::Type EngineeringState::applyOffsetForces(ApplyOffsetForcesCommand *command) {
    SPDLOG_INFO("{}: applyOffsetForces()", this->name);
    Model::instance().getForceController()->applyOffsetForces(
            command->getData()->xForces, command->getData()->yForces, command->getData()->zForces);
    Model::instance().getForceController()->processAppliedForces();
    return Model::instance().getSafetyController()->checkSafety(States::NoStateTransition);
}

States::Type EngineeringState::applyOffsetForcesByMirrorForce(
        ApplyOffsetForcesByMirrorForceCommand *command) {
    SPDLOG_INFO("{}: applyOffsetForcesByMirrorForce()", this->name);
    Model::instance().getForceController()->applyOffsetForcesByMirrorForces(
            command->getData()->xForce, command->getData()->yForce, command->getData()->zForce,
            command->getData()->xMoment, command->getData()->yMoment, command->getData()->zMoment);
    Model::instance().getForceController()->processAppliedForces();
    return Model::instance().getSafetyController()->checkSafety(States::NoStateTransition);
}

States::Type EngineeringState::clearOffsetForces(ClearOffsetForcesCommand *command) {
    SPDLOG_INFO("{}: clearOffsetForces()", this->name);
    Model::instance().getForceController()->zeroOffsetForces();
    Model::instance().getForceController()->processAppliedForces();
    return Model::instance().getSafetyController()->checkSafety(States::NoStateTransition);
}

States::Type EngineeringState::turnLightsOn(TurnLightsOnCommand *command) {
    SPDLOG_INFO("{}: turnLightsOn()", this->name);
    DigitalInputOutput::instance().turnCellLightsOn();
    return Model::instance().getSafetyController()->checkSafety(States::NoStateTransition);
}

States::Type EngineeringState::turnLightsOff(TurnLightsOffCommand *command) {
    SPDLOG_INFO("{}: turnLightsOff()", this->name);
    DigitalInputOutput::instance().turnCellLightsOff();
    return Model::instance().getSafetyController()->checkSafety(States::NoStateTransition);
}

States::Type EngineeringState::turnPowerOn(TurnPowerOnCommand *command) {
    SPDLOG_INFO("{}: turnPowerOn()", this->name);
    if (command->getData()->turnPowerNetworkAOn) {
        Model::instance().getPowerController()->setPowerNetworkA(true);
    }
    if (command->getData()->turnPowerNetworkBOn) {
        Model::instance().getPowerController()->setPowerNetworkB(true);
    }
    if (command->getData()->turnPowerNetworkCOn) {
        Model::instance().getPowerController()->setPowerNetworkC(true);
    }
    if (command->getData()->turnPowerNetworkDOn) {
        Model::instance().getPowerController()->setPowerNetworkD(true);
    }
    if (command->getData()->turnAuxPowerNetworkAOn) {
        Model::instance().getPowerController()->setAuxPowerNetworkA(true);
    }
    if (command->getData()->turnAuxPowerNetworkBOn) {
        Model::instance().getPowerController()->setAuxPowerNetworkB(true);
    }
    if (command->getData()->turnAuxPowerNetworkCOn) {
        Model::instance().getPowerController()->setAuxPowerNetworkC(true);
    }
    if (command->getData()->turnAuxPowerNetworkDOn) {
        Model::instance().getPowerController()->setAuxPowerNetworkD(true);
    }
    return Model::instance().getSafetyController()->checkSafety(States::NoStateTransition);
}

States::Type EngineeringState::turnPowerOff(TurnPowerOffCommand *command) {
    SPDLOG_INFO("{}: turnPowerOff()", this->name);
    if (command->getData()->turnPowerNetworkAOff) {
        Model::instance().getPowerController()->setPowerNetworkA(false);
    }
    if (command->getData()->turnPowerNetworkBOff) {
        Model::instance().getPowerController()->setPowerNetworkB(false);
    }
    if (command->getData()->turnPowerNetworkCOff) {
        Model::instance().getPowerController()->setPowerNetworkC(false);
    }
    if (command->getData()->turnPowerNetworkDOff) {
        Model::instance().getPowerController()->setPowerNetworkD(false);
    }
    if (command->getData()->turnAuxPowerNetworkAOff) {
        Model::instance().getPowerController()->setAuxPowerNetworkA(false);
    }
    if (command->getData()->turnAuxPowerNetworkBOff) {
        Model::instance().getPowerController()->setAuxPowerNetworkB(false);
    }
    if (command->getData()->turnAuxPowerNetworkCOff) {
        Model::instance().getPowerController()->setAuxPowerNetworkC(false);
    }
    if (command->getData()->turnAuxPowerNetworkDOff) {
        Model::instance().getPowerController()->setAuxPowerNetworkD(false);
    }
    return Model::instance().getSafetyController()->checkSafety(States::NoStateTransition);
}

States::Type EngineeringState::disableForceActuator(DisableForceActuatorCommand *command) {
    SPDLOG_INFO("{}: disableForceActuator({}, {})", name, command->actuatorId, command->actuatorIndex);
    Model::instance().getILC()->disableFA(command->actuatorId);
    return Model::instance().getSafetyController()->checkSafety(States::NoStateTransition);
}

States::Type EngineeringState::enableForceActuator(EnableForceActuatorCommand *command) {
    SPDLOG_INFO("{}: enableForceActuator({}, {})", name, command->actuatorId, command->actuatorIndex);
    Model::instance().getILC()->enableFA(command->actuatorId);
    return Model::instance().getSafetyController()->checkSafety(States::NoStateTransition);
}

States::Type EngineeringState::enableAllForceActuators(EnableAllForceActuatorsCommand *command) {
    SPDLOG_INFO("{}: enableAllForceActuators()", name);
    Model::instance().getILC()->enableAllFA();
    return Model::instance().getSafetyController()->checkSafety(States::NoStateTransition);
}

States::Type EngineeringState::setSlewControllerSettings(SetSlewControllerSettingsCommand *command) {
    SPDLOG_INFO("{}: slewControllerSettings {}, enable {}", name, command->getData()->slewSettings,
                command->getData()->enableSlewManagement);
    SlewControllerSettings::instance().set(command->getData()->slewSettings,
                                           command->getData()->enableSlewManagement);
    return Model::instance().getSafetyController()->checkSafety(States::NoStateTransition);
}
