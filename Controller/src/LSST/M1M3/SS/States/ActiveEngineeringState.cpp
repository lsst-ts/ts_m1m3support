/*
 * ActiveEngineeringState.cpp
 *
 *  Created on: Oct 25, 2017
 *      Author: ccontaxis
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
#include <Log.h>

namespace LSST {
namespace M1M3 {
namespace SS {

ActiveEngineeringState::ActiveEngineeringState(M1M3SSPublisher* publisher) : EngineeringState(publisher, "ActiveEngineeringState") { }

States::Type ActiveEngineeringState::update(UpdateCommand* command, Model* model) {
	Log.Trace("ActiveEngineeringState: update()");
	this->startTimer();
	EnabledState::update(command, model);
	this->stopTimer();
	model->publishOuterLoop(this->getTimer());
	return model->getSafetyController()->checkSafety(States::NoStateTransition);
}

States::Type ActiveEngineeringState::lowerM1M3(LowerM1M3Command* command, Model* model) {
	Log.Info("ActiveEngineeringState: lowerM1M3()");
	States::Type newState = States::LoweringEngineeringState;
	model->getForceController()->resetPIDs();
	model->getAutomaticOperationsController()->startLowerOperation();
	return model->getSafetyController()->checkSafety(newState);
}

States::Type ActiveEngineeringState::exitEngineering(ExitEngineeringCommand* command, Model* model) {
	Log.Info("ActiveEngineeringState: exitEngineering()");
	States::Type newState = States::ActiveState;
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
	return model->getSafetyController()->checkSafety(newState);
}

States::Type ActiveEngineeringState::applyAberrationForcesByBendingModes(ApplyAberrationForcesByBendingModesCommand* command, Model* model) {
	Log.Info("ActiveEngineeringState: applyAberrationForcesByBendingModes()");
	model->getForceController()->applyAberrationForcesByBendingModes(command->getData()->Coefficients);
	model->getForceController()->processAppliedForces();
	return model->getSafetyController()->checkSafety(States::NoStateTransition);
}

States::Type ActiveEngineeringState::applyAberrationForces(ApplyAberrationForcesCommand* command, Model* model) {
	Log.Info("ActiveEngineeringState: applyAberrationForces()");
	model->getForceController()->applyAberrationForces(command->getData()->ZForces);
	model->getForceController()->processAppliedForces();
	return model->getSafetyController()->checkSafety(States::NoStateTransition);
}

States::Type ActiveEngineeringState::clearAberrationForces(ClearAberrationForcesCommand* command, Model* model) {
	Log.Info("ActiveEngineeringState: clearAberrationForces()");
	model->getForceController()->zeroAberrationForces();
	model->getForceController()->processAppliedForces();
	return model->getSafetyController()->checkSafety(States::NoStateTransition);
}

States::Type ActiveEngineeringState::applyActiveOpticForcesByBendingModes(ApplyActiveOpticForcesByBendingModesCommand* command, Model* model) {
	Log.Info("ActiveEngineeringState: applyActiveOpticForcesByBendingModes()");
	model->getForceController()->applyActiveOpticForcesByBendingModes(command->getData()->Coefficients);
	model->getForceController()->processAppliedForces();
	return model->getSafetyController()->checkSafety(States::NoStateTransition);
}

States::Type ActiveEngineeringState::applyActiveOpticForces(ApplyActiveOpticForcesCommand* command, Model* model) {
	Log.Info("ActiveEngineeringState: applyActiveOpticForces()");
	model->getForceController()->applyActiveOpticForces(command->getData()->ZForces);
	model->getForceController()->processAppliedForces();
	return model->getSafetyController()->checkSafety(States::NoStateTransition);
}

States::Type ActiveEngineeringState::clearActiveOpticForces(ClearActiveOpticForcesCommand* command, Model* model) {
	Log.Info("ActiveEngineeringState: clearActiveOpticForces()");
	model->getForceController()->zeroActiveOpticForces();
	model->getForceController()->processAppliedForces();
	return model->getSafetyController()->checkSafety(States::NoStateTransition);
}

States::Type ActiveEngineeringState::translateM1M3(TranslateM1M3Command* command, Model* model) {
	Log.Info("ActiveEngineeringState: translateM1M3()");
	if (!model->getPositionController()->translate(command->getData()->XTranslation, command->getData()->YTranslation, command->getData()->ZTranslation,
			command->getData()->XRotation, command->getData()->YRotation, command->getData()->ZRotation)) {
		model->getPublisher()->logCommandRejectionWarning("TranslateM1M3", "At least one hardpoint actuator commanded to move is already MOVING or CHASING.");
	}
	return model->getSafetyController()->checkSafety(States::NoStateTransition);
}

States::Type ActiveEngineeringState::positionM1M3(PositionM1M3Command* command, Model* model) {
	Log.Info("ActiveEngineeringState: positionM1M3()");
	if (!model->getPositionController()->moveToAbsolute(command->getData()->XPosition, command->getData()->YPosition, command->getData()->ZPosition,
			command->getData()->XRotation, command->getData()->YRotation, command->getData()->ZRotation)) {
		model->getPublisher()->logCommandRejectionWarning("PositionM1M3", "At least one hardpoint actuator commanded to move is already MOVING or CHASING.");
	}
	return model->getSafetyController()->checkSafety(States::NoStateTransition);
}

States::Type ActiveEngineeringState::enableHardpointCorrections(EnableHardpointCorrectionsCommand* command, Model* model) {
	Log.Info("ActiveEngineeringState: enableHardpointCorrections()");
	model->getForceController()->applyBalanceForces();
	return model->getSafetyController()->checkSafety(States::NoStateTransition);
}

States::Type ActiveEngineeringState::disableHardpointCorrections(DisableHardpointCorrectionsCommand* command, Model* model) {
	Log.Info("ActiveEngineeringState: disableHardpointCorrections()");
	model->getForceController()->zeroBalanceForces();
	return model->getSafetyController()->checkSafety(States::NoStateTransition);
}

States::Type ActiveEngineeringState::runMirrorForceProfile(RunMirrorForceProfileCommand* command, Model* model) {
	Log.Info("ActiveEngineeringState: runMirrorForceProfile()");
	model->getProfileController()->setupMirrorForceProfile(command->getData()->XForce, command->getData()->YForce, command->getData()->ZForce, command->getData()->XMoment, command->getData()->YMoment, command->getData()->ZMoment);
	return model->getSafetyController()->checkSafety(States::ProfileHardpointCorrectionState);
}

States::Type ActiveEngineeringState::updatePID(UpdatePIDCommand* command, Model* model) {
	Log.Info("ActiveEngineeringState: updatePID()");
	PIDParameters parameters;
	parameters.Timestep = command->getData()->Timestep;
	parameters.P = command->getData()->P;
	parameters.I = command->getData()->I;
	parameters.D = command->getData()->D;
	parameters.N = command->getData()->N;
	model->getForceController()->updatePID(command->getData()->PID, parameters);
	return model->getSafetyController()->checkSafety(States::NoStateTransition);
}

States::Type ActiveEngineeringState::resetPID(ResetPIDCommand* command, Model* model) {
	Log.Info("ActiveEngineeringState: resetPID()");
	model->getForceController()->resetPID(command->getData()->PID);
	return model->getSafetyController()->checkSafety(States::NoStateTransition);
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
