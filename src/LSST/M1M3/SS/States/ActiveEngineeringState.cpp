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

#include <ActiveEngineeringState.h>
#include <Model.h>
#include <ILC.h>
#include <Displacement.h>
#include <Inclinometer.h>
#include <ForceController.h>
#include <ApplyOffsetForcesCommand.h>
#include <ApplyAberrationForcesByBendingModesCommand.h>
#include <ApplyAberrationForcesCommand.h>
#include <ClearAberrationForcesCommand.h>
#include <ApplyActiveOpticForcesByBendingModesCommand.h>
#include <ApplyActiveOpticForcesCommand.h>
#include <ClearActiveOpticForcesCommand.h>
#include <SafetyController.h>
#include <DigitalInputOutput.h>
#include <TMAAzimuthSampleCommand.h>
#include <TMAElevationSampleCommand.h>
#include <MoveHardpointActuatorsCommand.h>
#include <TranslateM1M3Command.h>
#include <PositionController.h>
#include <PositionM1M3Command.h>
#include <M1M3SSPublisher.h>
#include <PowerController.h>
#include <TurnPowerOnCommand.h>
#include <TurnPowerOffCommand.h>
#include <AutomaticOperationsController.h>
#include <ApplyOffsetForcesByMirrorForceCommand.h>
#include <RunMirrorForceProfileCommand.h>
#include <PIDParameters.h>
#include <UpdatePIDCommand.h>
#include <ResetPIDCommand.h>
#include <unistd.h>
#include <spdlog/spdlog.h>

namespace LSST {
namespace M1M3 {
namespace SS {

ActiveEngineeringState::ActiveEngineeringState() : EngineeringState("ActiveEngineeringState") {}

States::Type ActiveEngineeringState::update(UpdateCommand* command, Model* model) {
    spdlog::trace("ActiveEngineeringState: update()");
    sendTelemetry(model);
    return model->getSafetyController()->checkSafety(States::NoStateTransition);
}

States::Type ActiveEngineeringState::lowerM1M3(LowerM1M3Command* command, Model* model) {
    spdlog::info("ActiveEngineeringState: lowerM1M3()");
    model->getForceController()->resetPIDs();
    model->getAutomaticOperationsController()->startLowerOperation();
    return model->getSafetyController()->checkSafety(States::LoweringEngineeringState);
}

States::Type ActiveEngineeringState::exitEngineering(ExitEngineeringCommand* command, Model* model) {
    spdlog::info("ActiveEngineeringState: exitEngineering()");
    model->getForceController()->resetPIDs();
    model->getDigitalInputOutput()->turnAirOn();
    model->getPositionController()->stopMotion();
    model->getForceController()->zeroAberrationForces();
    model->getForceController()->applyBalanceForces();
    model->getForceController()->zeroOffsetForces();
    model->getForceController()->processAppliedForces();
    model->getDigitalInputOutput()->turnCellLightsOff();
    // TODO: Real problems exist if the user enabled / disabled ILC power...
    model->getPowerController()->setAllPowerNetworks(true);
    return model->getSafetyController()->checkSafety(States::ActiveState);
}

States::Type ActiveEngineeringState::applyAberrationForcesByBendingModes(
        ApplyAberrationForcesByBendingModesCommand* command, Model* model) {
    spdlog::info("ActiveEngineeringState: applyAberrationForcesByBendingModes()");
    model->getForceController()->applyAberrationForcesByBendingModes(command->getData()->coefficients);
    model->getForceController()->processAppliedForces();
    return model->getSafetyController()->checkSafety(States::NoStateTransition);
}

States::Type ActiveEngineeringState::applyAberrationForces(ApplyAberrationForcesCommand* command,
                                                           Model* model) {
    spdlog::info("ActiveEngineeringState: applyAberrationForces()");
    model->getForceController()->applyAberrationForces(command->getData()->zForces);
    model->getForceController()->processAppliedForces();
    return model->getSafetyController()->checkSafety(States::NoStateTransition);
}

States::Type ActiveEngineeringState::clearAberrationForces(ClearAberrationForcesCommand* command,
                                                           Model* model) {
    spdlog::info("ActiveEngineeringState: clearAberrationForces()");
    model->getForceController()->zeroAberrationForces();
    model->getForceController()->processAppliedForces();
    return model->getSafetyController()->checkSafety(States::NoStateTransition);
}

States::Type ActiveEngineeringState::applyActiveOpticForcesByBendingModes(
        ApplyActiveOpticForcesByBendingModesCommand* command, Model* model) {
    spdlog::info("ActiveEngineeringState: applyActiveOpticForcesByBendingModes()");
    model->getForceController()->applyActiveOpticForcesByBendingModes(command->getData()->coefficients);
    model->getForceController()->processAppliedForces();
    return model->getSafetyController()->checkSafety(States::NoStateTransition);
}

States::Type ActiveEngineeringState::applyActiveOpticForces(ApplyActiveOpticForcesCommand* command,
                                                            Model* model) {
    spdlog::info("ActiveEngineeringState: applyActiveOpticForces()");
    model->getForceController()->applyActiveOpticForces(command->getData()->zForces);
    model->getForceController()->processAppliedForces();
    return model->getSafetyController()->checkSafety(States::NoStateTransition);
}

States::Type ActiveEngineeringState::clearActiveOpticForces(ClearActiveOpticForcesCommand* command,
                                                            Model* model) {
    spdlog::info("ActiveEngineeringState: clearActiveOpticForces()");
    model->getForceController()->zeroActiveOpticForces();
    model->getForceController()->processAppliedForces();
    return model->getSafetyController()->checkSafety(States::NoStateTransition);
}

States::Type ActiveEngineeringState::translateM1M3(TranslateM1M3Command* command, Model* model) {
    spdlog::info("ActiveEngineeringState: translateM1M3()");
    if (!model->getPositionController()->translate(
                command->getData()->xTranslation, command->getData()->xTranslation,
                command->getData()->zTranslation, command->getData()->xRotation,
                command->getData()->yRotation, command->getData()->zRotation)) {
        M1M3SSPublisher::get().logCommandRejectionWarning(
                "TranslateM1M3",
                "At least one hardpoint actuator commanded to move is already MOVING or CHASING.");
    }
    return model->getSafetyController()->checkSafety(States::NoStateTransition);
}

States::Type ActiveEngineeringState::positionM1M3(PositionM1M3Command* command, Model* model) {
    spdlog::info("ActiveEngineeringState: positionM1M3()");
    if (!model->getPositionController()->moveToAbsolute(
                command->getData()->xPosition, command->getData()->yPosition, command->getData()->zPosition,
                command->getData()->xRotation, command->getData()->yRotation,
                command->getData()->zRotation)) {
        M1M3SSPublisher::get().logCommandRejectionWarning(
                "PositionM1M3",
                "At least one hardpoint actuator commanded to move is already MOVING or CHASING.");
    }
    return model->getSafetyController()->checkSafety(States::NoStateTransition);
}

States::Type ActiveEngineeringState::enableHardpointCorrections(EnableHardpointCorrectionsCommand* command,
                                                                Model* model) {
    spdlog::info("ActiveEngineeringState: enableHardpointCorrections()");
    model->getForceController()->applyBalanceForces();
    return model->getSafetyController()->checkSafety(States::NoStateTransition);
}

States::Type ActiveEngineeringState::disableHardpointCorrections(DisableHardpointCorrectionsCommand* command,
                                                                 Model* model) {
    spdlog::info("ActiveEngineeringState: disableHardpointCorrections()");
    model->getForceController()->zeroBalanceForces();
    return model->getSafetyController()->checkSafety(States::NoStateTransition);
}

States::Type ActiveEngineeringState::runMirrorForceProfile(RunMirrorForceProfileCommand* command,
                                                           Model* model) {
    spdlog::info("ActiveEngineeringState: runMirrorForceProfile()");
    model->getProfileController()->setupMirrorForceProfile(
            command->getData()->xForce, command->getData()->yForce, command->getData()->zForce,
            command->getData()->xMoment, command->getData()->yMoment, command->getData()->zMoment);
    return model->getSafetyController()->checkSafety(States::ProfileHardpointCorrectionState);
}

States::Type ActiveEngineeringState::updatePID(UpdatePIDCommand* command, Model* model) {
    spdlog::info("ActiveEngineeringState: updatePID()");
    PIDParameters parameters;
    parameters.Timestep = command->getData()->timestep;
    parameters.P = command->getData()->p;
    parameters.I = command->getData()->i;
    parameters.D = command->getData()->d;
    parameters.N = command->getData()->n;
    model->getForceController()->updatePID(command->getData()->pid, parameters);
    return model->getSafetyController()->checkSafety(States::NoStateTransition);
}

States::Type ActiveEngineeringState::resetPID(ResetPIDCommand* command, Model* model) {
    spdlog::info("ActiveEngineeringState: resetPID()");
    model->getForceController()->resetPID(command->getData()->pid);
    return model->getSafetyController()->checkSafety(States::NoStateTransition);
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
