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

ActiveEngineeringState::ActiveEngineeringState() : EnabledState("ActiveEngineeringState") {}

States::Type ActiveEngineeringState::update(UpdateCommand* command) {
    SPDLOG_TRACE("ActiveEngineeringState: update()");
    EngineeringState::sendTelemetry();
    return Model::get().getSafetyController()->checkSafety(States::NoStateTransition);
}

States::Type ActiveEngineeringState::exitEngineering(ExitEngineeringCommand* command) {
    SPDLOG_INFO("ActiveEngineeringState: exitEngineering()");
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
    SPDLOG_INFO("EnabledActiveState: applyAberrationForcesByBendingModes()");
    Model::get().getForceController()->applyAberrationForcesByBendingModes(command->getData()->coefficients);
    Model::get().getForceController()->processAppliedForces();
    return Model::get().getSafetyController()->checkSafety(States::NoStateTransition);
}

States::Type ActiveEngineeringState::applyActiveOpticForcesByBendingModes(
        ApplyActiveOpticForcesByBendingModesCommand* command) {
    SPDLOG_INFO("EnabledActiveState: applyActiveOpticForcesByBendingModes()");
    Model::get().getForceController()->applyActiveOpticForcesByBendingModes(command->getData()->coefficients);
    Model::get().getForceController()->processAppliedForces();
    return Model::get().getSafetyController()->checkSafety(States::NoStateTransition);
}

States::Type ActiveEngineeringState::translateM1M3(TranslateM1M3Command* command) {
    SPDLOG_INFO("ActiveEngineeringState: translateM1M3()");
    if (!Model::get().getPositionController()->translate(
                command->getData()->xTranslation, command->getData()->yTranslation,
                command->getData()->zTranslation, command->getData()->xRotation,
                command->getData()->yRotation, command->getData()->zRotation)) {
        M1M3SSPublisher::get().logCommandRejectionWarning(
                "TranslateM1M3",
                "At least one hardpoint actuator commanded to move is already MOVING or CHASING.");
    }
    return Model::get().getSafetyController()->checkSafety(States::NoStateTransition);
}

States::Type ActiveEngineeringState::positionM1M3(PositionM1M3Command* command) {
    SPDLOG_INFO("ActiveEngineeringState: positionM1M3()");
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

States::Type ActiveEngineeringState::runMirrorForceProfile(RunMirrorForceProfileCommand* command) {
    SPDLOG_INFO("ActiveEngineeringState: runMirrorForceProfile()");
    Model::get().getProfileController()->setupMirrorForceProfile(
            command->getData()->xForce, command->getData()->yForce, command->getData()->zForce,
            command->getData()->xMoment, command->getData()->yMoment, command->getData()->zMoment);
    return Model::get().getSafetyController()->checkSafety(States::ProfileHardpointCorrectionState);
}

States::Type ActiveEngineeringState::updatePID(UpdatePIDCommand* command) {
    SPDLOG_INFO("ActiveEngineeringState: updatePID()");
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
    SPDLOG_INFO("ActiveEngineeringState: resetPID()");
    Model::get().getForceController()->resetPID(command->getData()->pid);
    return Model::get().getSafetyController()->checkSafety(States::NoStateTransition);
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
