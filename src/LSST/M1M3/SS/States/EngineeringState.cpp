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

#include <EngineeringState.h>
#include <Model.h>
#include <DigitalInputOutput.h>
#include <PositionController.h>
#include <ForceController.h>
#include <PowerController.h>
#include <MoveHardpointActuatorsCommand.h>
#include <EnableHardpointChaseCommand.h>
#include <DisableHardpointChaseCommand.h>
#include <ApplyOffsetForcesCommand.h>
#include <ApplyOffsetForcesByMirrorForceCommand.h>
#include <TurnPowerOnCommand.h>
#include <TurnPowerOffCommand.h>
#include <SafetyController.h>
#include <M1M3SSPublisher.h>
#include <ILC.h>
#include <ModbusTransmitCommand.h>
#include <spdlog/spdlog.h>

namespace LSST {
namespace M1M3 {
namespace SS {

EngineeringState::EngineeringState(std::string name) : EnabledState(name) {}

States::Type EngineeringState::turnAirOn(TurnAirOnCommand* command) {
    SPDLOG_INFO("{}: turnAirOn()", this->name);
    Model::get().getDigitalInputOutput()->turnAirOn();
    return Model::get().getSafetyController()->checkSafety(States::NoStateTransition);
}

States::Type EngineeringState::turnAirOff(TurnAirOffCommand* command) {
    SPDLOG_INFO("{}: turnAirOff()", this->name);
    Model::get().getDigitalInputOutput()->turnAirOff();
    return Model::get().getSafetyController()->checkSafety(States::NoStateTransition);
}

States::Type EngineeringState::stopHardpointMotion(StopHardpointMotionCommand* command) {
    SPDLOG_INFO("{}: stopHardpointMotion()", this->name);
    Model::get().getPositionController()->stopMotion();
    return Model::get().getSafetyController()->checkSafety(States::NoStateTransition);
}

States::Type EngineeringState::moveHardpointActuators(MoveHardpointActuatorsCommand* command) {
    SPDLOG_INFO("{}: moveHardpointActuators()", this->name);
    if (!Model::get().getPositionController()->move(command->getData()->steps)) {
        M1M3SSPublisher::get().logCommandRejectionWarning(
                "MoveHardpointActuators",
                "At least one hardpoint actuator commanded to move is already MOVING or CHASING.");
    }
    return Model::get().getSafetyController()->checkSafety(States::NoStateTransition);
}

States::Type EngineeringState::enableHardpointChase(EnableHardpointChaseCommand* command) {
    SPDLOG_INFO("{}: enableHardpointChase()", this->name);
    if (!Model::get().getPositionController()->enableChase(command->getData()->hardpointActuator)) {
        M1M3SSPublisher::get().logCommandRejectionWarning(
                "EnableHardpointChase",
                "At least one hardpoint actuator commanded to chase is already MOVING or CHASING.");
    }
    return Model::get().getSafetyController()->checkSafety(States::NoStateTransition);
}

States::Type EngineeringState::disableHardpointChase(DisableHardpointChaseCommand* command) {
    SPDLOG_INFO("{}: disableHardpointChase()", this->name);
    Model::get().getPositionController()->disableChase(command->getData()->hardpointActuator);
    return Model::get().getSafetyController()->checkSafety(States::NoStateTransition);
}

States::Type EngineeringState::applyOffsetForces(ApplyOffsetForcesCommand* command) {
    SPDLOG_INFO("{}: applyOffsetForces()", this->name);
    Model::get().getForceController()->applyOffsetForces(
            command->getData()->xForces, command->getData()->yForces, command->getData()->zForces);
    Model::get().getForceController()->processAppliedForces();
    return Model::get().getSafetyController()->checkSafety(States::NoStateTransition);
}

States::Type EngineeringState::applyOffsetForcesByMirrorForce(
        ApplyOffsetForcesByMirrorForceCommand* command) {
    SPDLOG_INFO("{}: applyOffsetForcesByMirrorForce()", this->name);
    Model::get().getForceController()->applyOffsetForcesByMirrorForces(
            command->getData()->xForce, command->getData()->yForce, command->getData()->zForce,
            command->getData()->xMoment, command->getData()->yMoment, command->getData()->zMoment);
    Model::get().getForceController()->processAppliedForces();
    return Model::get().getSafetyController()->checkSafety(States::NoStateTransition);
}

States::Type EngineeringState::clearOffsetForces(ClearOffsetForcesCommand* command) {
    SPDLOG_INFO("{}: clearOffsetForces()", this->name);
    Model::get().getForceController()->zeroOffsetForces();
    Model::get().getForceController()->processAppliedForces();
    return Model::get().getSafetyController()->checkSafety(States::NoStateTransition);
}

States::Type EngineeringState::turnLightsOn(TurnLightsOnCommand* command) {
    SPDLOG_INFO("{}: turnLightsOn()", this->name);
    Model::get().getDigitalInputOutput()->turnCellLightsOn();
    return Model::get().getSafetyController()->checkSafety(States::NoStateTransition);
}

States::Type EngineeringState::turnLightsOff(TurnLightsOffCommand* command) {
    SPDLOG_INFO("{}: turnLightsOff()", this->name);
    Model::get().getDigitalInputOutput()->turnCellLightsOff();
    return Model::get().getSafetyController()->checkSafety(States::NoStateTransition);
}

States::Type EngineeringState::turnPowerOn(TurnPowerOnCommand* command) {
    SPDLOG_INFO("{}: turnPowerOn()", this->name);
    if (command->getData()->turnPowerNetworkAOn) {
        Model::get().getPowerController()->setPowerNetworkA(true);
    }
    if (command->getData()->turnPowerNetworkBOn) {
        Model::get().getPowerController()->setPowerNetworkB(true);
    }
    if (command->getData()->turnPowerNetworkCOn) {
        Model::get().getPowerController()->setPowerNetworkC(true);
    }
    if (command->getData()->turnPowerNetworkDOn) {
        Model::get().getPowerController()->setPowerNetworkD(true);
    }
    if (command->getData()->turnAuxPowerNetworkAOn) {
        Model::get().getPowerController()->setAuxPowerNetworkA(true);
    }
    if (command->getData()->turnAuxPowerNetworkBOn) {
        Model::get().getPowerController()->setAuxPowerNetworkB(true);
    }
    if (command->getData()->turnAuxPowerNetworkCOn) {
        Model::get().getPowerController()->setAuxPowerNetworkC(true);
    }
    if (command->getData()->turnAuxPowerNetworkDOn) {
        Model::get().getPowerController()->setAuxPowerNetworkD(true);
    }
    return Model::get().getSafetyController()->checkSafety(States::NoStateTransition);
}

States::Type EngineeringState::turnPowerOff(TurnPowerOffCommand* command) {
    SPDLOG_INFO("{}: turnPowerOff()", this->name);
    if (command->getData()->turnPowerNetworkAOff) {
        Model::get().getPowerController()->setPowerNetworkA(false);
    }
    if (command->getData()->turnPowerNetworkBOff) {
        Model::get().getPowerController()->setPowerNetworkB(false);
    }
    if (command->getData()->turnPowerNetworkCOff) {
        Model::get().getPowerController()->setPowerNetworkC(false);
    }
    if (command->getData()->turnPowerNetworkDOff) {
        Model::get().getPowerController()->setPowerNetworkD(false);
    }
    if (command->getData()->turnAuxPowerNetworkAOff) {
        Model::get().getPowerController()->setAuxPowerNetworkA(false);
    }
    if (command->getData()->turnAuxPowerNetworkBOff) {
        Model::get().getPowerController()->setAuxPowerNetworkB(false);
    }
    if (command->getData()->turnAuxPowerNetworkCOff) {
        Model::get().getPowerController()->setAuxPowerNetworkC(false);
    }
    if (command->getData()->turnAuxPowerNetworkDOff) {
        Model::get().getPowerController()->setAuxPowerNetworkD(false);
    }
    return Model::get().getSafetyController()->checkSafety(States::NoStateTransition);
}

States::Type EngineeringState::modbusTransmit(ModbusTransmitCommand* command) {
    SPDLOG_INFO("{}: modbusTransmit()", this->name);
    Model::get().getILC()->modbusTransmit(command->getData()->actuatorId, command->getData()->functionCode,
                                          command->getData()->dataLength, command->getData()->data);
    return Model::get().getSafetyController()->checkSafety(States::NoStateTransition);
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
