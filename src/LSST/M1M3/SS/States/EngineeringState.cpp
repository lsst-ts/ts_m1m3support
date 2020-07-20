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

EngineeringState::EngineeringState(M1M3SSPublisher* publisher)
        : EnabledState(publisher, "EngineeringState") {}
EngineeringState::EngineeringState(M1M3SSPublisher* publisher, std::string name)
        : EnabledState(publisher, name) {}

States::Type EngineeringState::turnAirOn(TurnAirOnCommand* command, Model* model) {
    spdlog::info("{}: turnAirOn()", this->name);
    model->getDigitalInputOutput()->turnAirOn();
    return model->getSafetyController()->checkSafety(States::NoStateTransition);
}

States::Type EngineeringState::turnAirOff(TurnAirOffCommand* command, Model* model) {
    spdlog::info("{}: turnAirOff()", this->name);
    model->getDigitalInputOutput()->turnAirOff();
    return model->getSafetyController()->checkSafety(States::NoStateTransition);
}

States::Type EngineeringState::stopHardpointMotion(StopHardpointMotionCommand* command, Model* model) {
    spdlog::info("{}: stopHardpointMotion()", this->name);
    model->getPositionController()->stopMotion();
    return model->getSafetyController()->checkSafety(States::NoStateTransition);
}

States::Type EngineeringState::moveHardpointActuators(MoveHardpointActuatorsCommand* command, Model* model) {
    spdlog::info("{}: moveHardpointActuators()", this->name);
    if (!model->getPositionController()->move(command->getData()->steps)) {
        model->getPublisher()->logCommandRejectionWarning(
                "MoveHardpointActuators",
                "At least one hardpoint actuator commanded to move is already MOVING or CHASING.");
    }
    return model->getSafetyController()->checkSafety(States::NoStateTransition);
}

States::Type EngineeringState::enableHardpointChase(EnableHardpointChaseCommand* command, Model* model) {
    spdlog::info("{}: enableHardpointChase()", this->name);
    if (!model->getPositionController()->enableChase(command->getData()->hardpointActuator)) {
        model->getPublisher()->logCommandRejectionWarning(
                "EnableHardpointChase",
                "At least one hardpoint actuator commanded to chase is already MOVING or CHASING.");
    }
    return model->getSafetyController()->checkSafety(States::NoStateTransition);
}

States::Type EngineeringState::disableHardpointChase(DisableHardpointChaseCommand* command, Model* model) {
    spdlog::info("{}: disableHardpointChase()", this->name);
    model->getPositionController()->disableChase(command->getData()->hardpointActuator);
    return model->getSafetyController()->checkSafety(States::NoStateTransition);
}

States::Type EngineeringState::applyOffsetForces(ApplyOffsetForcesCommand* command, Model* model) {
    spdlog::info("{}: applyOffsetForces()", this->name);
    model->getForceController()->applyOffsetForces(command->getData()->xForces, command->getData()->yForces,
                                                   command->getData()->zForces);
    model->getForceController()->processAppliedForces();
    return model->getSafetyController()->checkSafety(States::NoStateTransition);
}

States::Type EngineeringState::applyOffsetForcesByMirrorForce(ApplyOffsetForcesByMirrorForceCommand* command,
                                                              Model* model) {
    spdlog::info("{}: applyOffsetForcesByMirrorForce()", this->name);
    model->getForceController()->applyOffsetForcesByMirrorForces(
            command->getData()->xForce, command->getData()->yForce, command->getData()->zForce,
            command->getData()->xMoment, command->getData()->yMoment, command->getData()->zMoment);
    model->getForceController()->processAppliedForces();
    return model->getSafetyController()->checkSafety(States::NoStateTransition);
}

States::Type EngineeringState::clearOffsetForces(ClearOffsetForcesCommand* command, Model* model) {
    spdlog::info("{}: clearOffsetForces()", this->name);
    model->getForceController()->zeroOffsetForces();
    model->getForceController()->processAppliedForces();
    return model->getSafetyController()->checkSafety(States::NoStateTransition);
}

States::Type EngineeringState::turnLightsOn(TurnLightsOnCommand* command, Model* model) {
    spdlog::info("{}: turnLightsOn()", this->name);
    model->getDigitalInputOutput()->turnCellLightsOn();
    return model->getSafetyController()->checkSafety(States::NoStateTransition);
}

States::Type EngineeringState::turnLightsOff(TurnLightsOffCommand* command, Model* model) {
    spdlog::info("{}: turnLightsOff()", this->name);
    model->getDigitalInputOutput()->turnCellLightsOff();
    return model->getSafetyController()->checkSafety(States::NoStateTransition);
}

States::Type EngineeringState::turnPowerOn(TurnPowerOnCommand* command, Model* model) {
    spdlog::info("{}: turnPowerOn()", this->name);
    if (command->getData()->turnPowerNetworkAOn) {
        model->getPowerController()->setPowerNetworkA(true);
    }
    if (command->getData()->turnPowerNetworkBOn) {
        model->getPowerController()->setPowerNetworkB(true);
    }
    if (command->getData()->turnPowerNetworkCOn) {
        model->getPowerController()->setPowerNetworkC(true);
    }
    if (command->getData()->turnPowerNetworkDOn) {
        model->getPowerController()->setPowerNetworkD(true);
    }
    if (command->getData()->turnAuxPowerNetworkAOn) {
        model->getPowerController()->setAuxPowerNetworkA(true);
    }
    if (command->getData()->turnAuxPowerNetworkBOn) {
        model->getPowerController()->setAuxPowerNetworkB(true);
    }
    if (command->getData()->turnAuxPowerNetworkCOn) {
        model->getPowerController()->setAuxPowerNetworkC(true);
    }
    if (command->getData()->turnAuxPowerNetworkDOn) {
        model->getPowerController()->setAuxPowerNetworkD(true);
    }
    return model->getSafetyController()->checkSafety(States::NoStateTransition);
}

States::Type EngineeringState::turnPowerOff(TurnPowerOffCommand* command, Model* model) {
    spdlog::info("{}: turnPowerOff()", this->name);
    if (command->getData()->turnPowerNetworkAOff) {
        model->getPowerController()->setPowerNetworkA(false);
    }
    if (command->getData()->turnPowerNetworkBOff) {
        model->getPowerController()->setPowerNetworkB(false);
    }
    if (command->getData()->turnPowerNetworkCOff) {
        model->getPowerController()->setPowerNetworkC(false);
    }
    if (command->getData()->turnPowerNetworkDOff) {
        model->getPowerController()->setPowerNetworkD(false);
    }
    if (command->getData()->turnAuxPowerNetworkAOff) {
        model->getPowerController()->setAuxPowerNetworkA(false);
    }
    if (command->getData()->turnAuxPowerNetworkBOff) {
        model->getPowerController()->setAuxPowerNetworkB(false);
    }
    if (command->getData()->turnAuxPowerNetworkCOff) {
        model->getPowerController()->setAuxPowerNetworkC(false);
    }
    if (command->getData()->turnAuxPowerNetworkDOff) {
        model->getPowerController()->setAuxPowerNetworkD(false);
    }
    return model->getSafetyController()->checkSafety(States::NoStateTransition);
}

States::Type EngineeringState::modbusTransmit(ModbusTransmitCommand* command, Model* model) {
    spdlog::info("{}: modbusTransmit()", this->name);
    model->getILC()->modbusTransmit(command->getData()->actuatorId, command->getData()->functionCode,
                                    command->getData()->dataLength, command->getData()->data);
    return model->getSafetyController()->checkSafety(States::NoStateTransition);
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
