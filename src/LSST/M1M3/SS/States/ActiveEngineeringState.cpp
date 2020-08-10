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

States::Type ActiveEngineeringState::update(UpdateCommand* command) {
    spdlog::trace("ActiveEngineeringState: update()");
    sendTelemetry();
    return Model::get().getSafetyController()->checkSafety(States::NoStateTransition);
}

States::Type ActiveEngineeringState::lowerM1M3(LowerM1M3Command* command) {
    spdlog::info("ActiveEngineeringState: lowerM1M3()");
    Model::get().getForceController()->resetPIDs();
    Model::get().getAutomaticOperationsController()->startLowerOperation();
    return Model::get().getSafetyController()->checkSafety(States::LoweringEngineeringState);
}

States::Type ActiveEngineeringState::exitEngineering(ExitEngineeringCommand* command) {
    spdlog::info("ActiveEngineeringState: exitEngineering()");
    Model::get().getForceController()->resetPIDs();
    Model::get().getDigitalInputOutput()->turnAirOn();
    Model::get().getPositionController()->stopMotion();
    Model::get().getForceController()->zeroAberrationForces();
    Model::get().getForceController()->applyBalanceForces();
    Model::get().getForceController()->zeroOffsetForces();
    Model::get().getForceController()->processAppliedForces();
    Model::get().getDigitalInputOutput()->turnCellLightsOff();
    // TODO: Real problems exist if the user enabled / disabled ILC power...
    Model::get().getPowerController()->setAllPowerNetworks(true);
    return Model::get().getSafetyController()->checkSafety(States::ActiveState);
}

States::Type ActiveEngineeringState::applyAberrationForcesByBendingModes(
        ApplyAberrationForcesByBendingModesCommand* command) {
    spdlog::info("ActiveEngineeringState: applyAberrationForcesByBendingModes()");
    Model::get().getForceController()->applyAberrationForcesByBendingModes(command->getData()->coefficients);
    Model::get().getForceController()->processAppliedForces();
    return Model::get().getSafetyController()->checkSafety(States::NoStateTransition);
}

States::Type ActiveEngineeringState::applyAberrationForces(ApplyAberrationForcesCommand* command) {
    spdlog::info("ActiveEngineeringState: applyAberrationForces()");
    Model::get().getForceController()->applyAberrationForces(command->getData()->zForces);
    Model::get().getForceController()->processAppliedForces();
    return Model::get().getSafetyController()->checkSafety(States::NoStateTransition);
}

States::Type ActiveEngineeringState::clearAberrationForces(ClearAberrationForcesCommand* command) {
    spdlog::info("ActiveEngineeringState: clearAberrationForces()");
    Model::get().getForceController()->zeroAberrationForces();
    Model::get().getForceController()->processAppliedForces();
    return Model::get().getSafetyController()->checkSafety(States::NoStateTransition);
}

States::Type ActiveEngineeringState::applyActiveOpticForcesByBendingModes(
        ApplyActiveOpticForcesByBendingModesCommand* command) {
    spdlog::info("ActiveEngineeringState: applyActiveOpticForcesByBendingModes()");
    Model::get().getForceController()->applyActiveOpticForcesByBendingModes(command->getData()->coefficients);
    Model::get().getForceController()->processAppliedForces();
    return Model::get().getSafetyController()->checkSafety(States::NoStateTransition);
}

States::Type ActiveEngineeringState::applyActiveOpticForces(ApplyActiveOpticForcesCommand* command) {
    spdlog::info("ActiveEngineeringState: applyActiveOpticForces()");
    Model::get().getForceController()->applyActiveOpticForces(command->getData()->zForces);
    Model::get().getForceController()->processAppliedForces();
    return Model::get().getSafetyController()->checkSafety(States::NoStateTransition);
}

States::Type ActiveEngineeringState::clearActiveOpticForces(ClearActiveOpticForcesCommand* command) {
    spdlog::info("ActiveEngineeringState: clearActiveOpticForces()");
    Model::get().getForceController()->zeroActiveOpticForces();
    Model::get().getForceController()->processAppliedForces();
    return Model::get().getSafetyController()->checkSafety(States::NoStateTransition);
}

States::Type ActiveEngineeringState::translateM1M3(TranslateM1M3Command* command) {
    spdlog::info("ActiveEngineeringState: translateM1M3()");
    if (!Model::get().getPositionController()->translate(
                command->getData()->xTranslation, command->getData()->xTranslation,
                command->getData()->zTranslation, command->getData()->xRotation,
                command->getData()->yRotation, command->getData()->zRotation)) {
        M1M3SSPublisher::get().logCommandRejectionWarning(
                "TranslateM1M3",
                "At least one hardpoint actuator commanded to move is already MOVING or CHASING.");
    }
    return Model::get().getSafetyController()->checkSafety(States::NoStateTransition);
}

States::Type ActiveEngineeringState::positionM1M3(PositionM1M3Command* command) {
    spdlog::info("ActiveEngineeringState: positionM1M3()");
    if (!Model::get().getPositionController()->moveToAbsolute(
                command->getData()->xPosition, command->getData()->yPosition, command->getData()->zPosition,
                command->getData()->xRotation, command->getData()->yRotation,
                command->getData()->zRotation)) {
        M1M3SSPublisher::get().logCommandRejectionWarning(
                "PositionM1M3",
                "At least one hardpoint actuator commanded to move is already MOVING or CHASING.");
    }
    return Model::get().getSafetyController()->checkSafety(States::NoStateTransition);
}

States::Type ActiveEngineeringState::enableHardpointCorrections(EnableHardpointCorrectionsCommand* command) {
    spdlog::info("ActiveEngineeringState: enableHardpointCorrections()");
    Model::get().getForceController()->applyBalanceForces();
    return Model::get().getSafetyController()->checkSafety(States::NoStateTransition);
}

States::Type ActiveEngineeringState::disableHardpointCorrections(
        DisableHardpointCorrectionsCommand* command) {
    spdlog::info("ActiveEngineeringState: disableHardpointCorrections()");
    Model::get().getForceController()->zeroBalanceForces();
    return Model::get().getSafetyController()->checkSafety(States::NoStateTransition);
}

States::Type ActiveEngineeringState::runMirrorForceProfile(RunMirrorForceProfileCommand* command) {
    spdlog::info("ActiveEngineeringState: runMirrorForceProfile()");
    Model::get().getProfileController()->setupMirrorForceProfile(
            command->getData()->xForce, command->getData()->yForce, command->getData()->zForce,
            command->getData()->xMoment, command->getData()->yMoment, command->getData()->zMoment);
    return Model::get().getSafetyController()->checkSafety(States::ProfileHardpointCorrectionState);
}

States::Type ActiveEngineeringState::updatePID(UpdatePIDCommand* command) {
    spdlog::info("ActiveEngineeringState: updatePID()");
    PIDParameters parameters;
    parameters.Timestep = command->getData()->timestep;
    parameters.P = command->getData()->p;
    parameters.I = command->getData()->i;
    parameters.D = command->getData()->d;
    parameters.N = command->getData()->n;
    Model::get().getForceController()->updatePID(command->getData()->pid, parameters);
    return Model::get().getSafetyController()->checkSafety(States::NoStateTransition);
}

States::Type ActiveEngineeringState::resetPID(ResetPIDCommand* command) {
    spdlog::info("ActiveEngineeringState: resetPID()");
    Model::get().getForceController()->resetPID(command->getData()->pid);
    return Model::get().getSafetyController()->checkSafety(States::NoStateTransition);
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
