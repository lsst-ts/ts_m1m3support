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

#include <ActiveEngineeringState.h>
#include <ApplyActiveOpticForcesCommand.h>
#include <ApplyOffsetForcesByMirrorForceCommand.h>
#include <ApplyOffsetForcesCommand.h>
#include <BoosterValveStatus.h>
#include <ClearActiveOpticForcesCommand.h>
#include <DigitalInputOutput.h>
#include <Displacement.h>
#include <ForceController.h>
#include <ILC.h>
#include <Inclinometer.h>
#include <M1M3SSPublisher.h>
#include <Model.h>
#include <MoveHardpointActuatorsCommand.h>
#include <PIDParameters.h>
#include <PositionController.h>
#include <PositionM1M3Command.h>
#include <PowerController.h>
#include <ResetPIDCommand.h>
#include <RunMirrorForceProfileCommand.h>
#include <SafetyController.h>
#include <TMAAzimuthSampleCommand.h>
#include <TMAElevationSampleCommand.h>
#include <TranslateM1M3Command.h>
#include <TurnPowerOnCommand.h>
#include <TurnPowerOffCommand.h>
#include <UpdatePIDCommand.h>

using namespace LSST::M1M3::SS;

ActiveEngineeringState::ActiveEngineeringState() : EnabledState("ActiveEngineeringState") {}

States::Type ActiveEngineeringState::update(UpdateCommand* command) {
    SPDLOG_TRACE("ActiveEngineeringState: update()");
    EngineeringState::sendTelemetry();
    return Model::instance().getSafetyController()->checkSafety(States::NoStateTransition);
}

States::Type ActiveEngineeringState::setSlewFlag(SetSlewFlagCommand* command) {
    SPDLOG_INFO("ActiveEngineeringState: setSlewFlag()");
    Model::instance().getSlewController()->enterSlew();
    return Model::instance().getSafetyController()->checkSafety(States::NoStateTransition);
}

States::Type ActiveEngineeringState::clearSlewFlag(ClearSlewFlagCommand* command) {
    SPDLOG_INFO("ActiveEngineeringState: clearSlewFlag()");
    Model::instance().getSlewController()->exitSlew();
    return Model::instance().getSafetyController()->checkSafety(States::NoStateTransition);
}

States::Type ActiveEngineeringState::exitEngineering(ExitEngineeringCommand* command) {
    SPDLOG_INFO("ActiveEngineeringState: exitEngineering()");
    Model::instance().getForceController()->resetPIDs();
    Model::instance().getDigitalInputOutput()->turnAirOn();
    Model::instance().getPositionController()->stopMotion();
    Model::instance().getForceController()->applyBalanceForces();
    Model::instance().getForceController()->zeroOffsetForces();
    Model::instance().getForceController()->processAppliedForces();
    Model::instance().getDigitalInputOutput()->turnCellLightsOff();
    // TODO: Real problems exist if the user enabled / disabled ILC power...
    Model::instance().getPowerController()->setAllPowerNetworks(true);
    BoosterValveStatus::instance().setUserTriggered(false);
    return Model::instance().getSafetyController()->checkSafety(States::ActiveState);
}

States::Type ActiveEngineeringState::lowerM1M3(LowerM1M3Command* command) {
    BoosterValveStatus::instance().setUserTriggered(false);
    return EnabledActiveState::lowerM1M3(command);
}

States::Type ActiveEngineeringState::translateM1M3(TranslateM1M3Command* command) {
    SPDLOG_INFO("ActiveEngineeringState: translateM1M3()");
    if (!Model::instance().getPositionController()->translate(
                command->getData()->xTranslation, command->getData()->yTranslation,
                command->getData()->zTranslation, command->getData()->xRotation,
                command->getData()->yRotation, command->getData()->zRotation)) {
        M1M3SSPublisher::instance().logCommandRejectionWarning(
                "TranslateM1M3",
                "At least one hardpoint actuator commanded to move is already MOVING or CHASING.");
    }
    return Model::instance().getSafetyController()->checkSafety(States::NoStateTransition);
}

States::Type ActiveEngineeringState::positionM1M3(PositionM1M3Command* command) {
    SPDLOG_INFO("ActiveEngineeringState: positionM1M3()");
    if (!Model::instance().getPositionController()->moveToAbsolute(
                command->getData()->xPosition, command->getData()->yPosition, command->getData()->zPosition,
                command->getData()->xRotation, command->getData()->yRotation,
                command->getData()->zRotation)) {
        M1M3SSPublisher::instance().logCommandRejectionWarning(
                "PositionM1M3",
                "At least one hardpoint actuator commanded to move is already MOVING or CHASING.");
    }
    return Model::instance().getSafetyController()->checkSafety(States::NoStateTransition);
}

States::Type ActiveEngineeringState::runMirrorForceProfile(RunMirrorForceProfileCommand* command) {
    SPDLOG_INFO("ActiveEngineeringState: runMirrorForceProfile()");
    Model::instance().getProfileController()->setupMirrorForceProfile(
            command->getData()->xForce, command->getData()->yForce, command->getData()->zForce,
            command->getData()->xMoment, command->getData()->yMoment, command->getData()->zMoment);
    return Model::instance().getSafetyController()->checkSafety(States::ProfileHardpointCorrectionState);
}

States::Type ActiveEngineeringState::updatePID(UpdatePIDCommand* command) {
    SPDLOG_INFO("ActiveEngineeringState: updatePID()");
    auto data = command->getData();
    PIDParameters parameters(data->timestep, data->p, data->i, data->d, data->n);
    Model::instance().getForceController()->updatePID(command->getData()->pid, parameters);
    return Model::instance().getSafetyController()->checkSafety(States::NoStateTransition);
}

States::Type ActiveEngineeringState::resetPID(ResetPIDCommand* command) {
    SPDLOG_INFO("ActiveEngineeringState: resetPID()");
    Model::instance().getForceController()->resetPID(command->getData()->pid);
    return Model::instance().getSafetyController()->checkSafety(States::NoStateTransition);
}

States::Type ActiveEngineeringState::enableDisableForceComponent(
        EnableDisableForceComponentCommand* command) {
    SPDLOG_INFO("EnableDisableForceComponentCommand forceComponent {}, enable {}",
                command->getData()->forceComponent, command->getData()->enable ? "true" : "false");
    Model::instance().getForceController()->enableDisableForceComponent(command->getData()->forceComponent,
                                                                        command->getData()->enable);
    return Model::instance().getSafetyController()->checkSafety(States::NoStateTransition);
}
