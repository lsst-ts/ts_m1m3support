/*
 * ParkedEngineeringState.cpp
 *
 *  Created on: Oct 17, 2017
 *      Author: ccontaxis
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
#include <M1M3SSPublisher.h>
#include <PowerController.h>
#include <TurnPowerOnCommand.h>
#include <TurnPowerOffCommand.h>
#include <AutomaticOperationsController.h>
#include <RaiseM1M3Command.h>
#include <Log.h>

namespace LSST {
namespace M1M3 {
namespace SS {

ParkedEngineeringState::ParkedEngineeringState(M1M3SSPublisher* publisher) : EngineeringState(publisher, "ParkedEngineeringState") { }

States::Type ParkedEngineeringState::update(UpdateCommand* command, Model* model) {
	Log.Trace("ParkedEngineeringState: update()");
	this->startTimer();
	EnabledState::update(command, model);
	this->stopTimer();
	model->publishOuterLoop(this->getTimer());
	return model->getSafetyController()->checkSafety(States::NoStateTransition);
}

States::Type ParkedEngineeringState::raiseM1M3(RaiseM1M3Command* command, Model* model) {
	Log.Info("ParkedEngineeringState: raiseM1M3()");
	States::Type newState = States::RaisingEngineeringState;
	model->getAutomaticOperationsController()->startRaiseOperation(command->getData()->BypassReferencePosition);
	return model->getSafetyController()->checkSafety(newState);
}

States::Type ParkedEngineeringState::exitEngineering(ExitEngineeringCommand* command, Model* model) {
	Log.Info("ParkedEngineeringState: exitEngineering()");
	States::Type newState = States::ParkedState;
	return model->getSafetyController()->checkSafety(newState);
}

States::Type ParkedEngineeringState::disable(DisableCommand* command, Model* model) {
	Log.Info("ParkedEngineeringState: disable()");
	States::Type newState = States::DisabledState;
	// Make sure the air is off
	model->getDigitalInputOutput()->turnAirOff();
	// Stop any existing motion (chase and move commands)
	model->getPositionController()->stopMotion();
	// Clear any offset force
	model->getForceController()->zeroOffsetForces();
	model->getForceController()->processAppliedForces();
	// Perform ILC state transition
	model->getILC()->writeSetModeDisableBuffer();
	model->getILC()->triggerModbus();
	model->getILC()->waitForAllSubnets(5000);
	model->getILC()->readAll();
	model->getILC()->verifyResponses();
	return model->getSafetyController()->checkSafety(newState);
}

States::Type ParkedEngineeringState::turnAirOn(TurnAirOnCommand* command, Model* model) {
	Log.Info("ParkedEngineeringState: turnAirOn()");
	model->getDigitalInputOutput()->turnAirOn();
	return model->getSafetyController()->checkSafety(States::NoStateTransition);
}

States::Type ParkedEngineeringState::turnAirOff(TurnAirOffCommand* command, Model* model) {
	Log.Info("ParkedEngineeringState: turnAirOff()");
	model->getDigitalInputOutput()->turnAirOff();
	return model->getSafetyController()->checkSafety(States::NoStateTransition);
}

States::Type ParkedEngineeringState::stopHardpointMotion(StopHardpointMotionCommand* command, Model* model) {
	Log.Info("ParkedEngineeringState: stopHardpointMotion()");
	model->getPositionController()->stopMotion();
	return model->getSafetyController()->checkSafety(States::NoStateTransition);
}

States::Type ParkedEngineeringState::moveHardpointActuators(MoveHardpointActuatorsCommand* command, Model* model) {
	Log.Info("ParkedEngineeringState: moveHardpointActuators()");
	if (!model->getPositionController()->move(command->getData()->Steps)) {
		model->getPublisher()->logCommandRejectionWarning("MoveHardpointActuators", "At least one hardpoint actuator commanded to move is already MOVING or CHASING.");
	}
	return model->getSafetyController()->checkSafety(States::NoStateTransition);
}

States::Type ParkedEngineeringState::enableHardpointChase(EnableHardpointChaseCommand* command, Model* model) {
	Log.Info("ParkedEngineeringState: enableHardpointChase()");
	if (!model->getPositionController()->enableChase(command->getData()->HardpointActuator)) {
		model->getPublisher()->logCommandRejectionWarning("EnableHardpointChase", "At least one hardpoint actuator commanded to chase is already MOVING or CHASING.");
	}
	return model->getSafetyController()->checkSafety(States::NoStateTransition);
}

States::Type ParkedEngineeringState::disableHardpointChase(DisableHardpointChaseCommand* command, Model* model) {
	Log.Info("ParkedEngineeringState: disableHardpointChase()");
	model->getPositionController()->disableChase(command->getData()->HardpointActuator);
	return model->getSafetyController()->checkSafety(States::NoStateTransition);
}

States::Type ParkedEngineeringState::applyOffsetForces(ApplyOffsetForcesCommand* command, Model* model) {
	Log.Info("ParkedEngineeringState: applyOffsetForces()");
	model->getForceController()->applyOffsetForces(command->getData()->XForces, command->getData()->YForces, command->getData()->ZForces);
	model->getForceController()->processAppliedForces();
	return model->getSafetyController()->checkSafety(States::NoStateTransition);
}

States::Type ParkedEngineeringState::clearOffsetForces(ClearOffsetForcesCommand* command, Model* model) {
	Log.Info("ParkedEngineeringState: clearOffsetForces()");
	model->getForceController()->zeroOffsetForces();
	model->getForceController()->processAppliedForces();
	return model->getSafetyController()->checkSafety(States::NoStateTransition);
}

States::Type ParkedEngineeringState::turnLightsOn(TurnLightsOnCommand* command, Model* model) {
	Log.Info("ParkedEngineeringState: turnLightsOn()");
	model->getDigitalInputOutput()->turnCellLightsOn();
	return model->getSafetyController()->checkSafety(States::NoStateTransition);
}

States::Type ParkedEngineeringState::turnLightsOff(TurnLightsOffCommand* command, Model* model) {
	Log.Info("ParkedEngineeringState: turnLightsOff()");
	model->getDigitalInputOutput()->turnCellLightsOff();
	return model->getSafetyController()->checkSafety(States::NoStateTransition);
}

States::Type ParkedEngineeringState::turnPowerOn(TurnPowerOnCommand* command, Model* model) {
	Log.Info("ParkedEngineeringState: turnPowerOn()");
	if (command->getData()->TurnPowerNetworkAOn) {
		model->getPowerController()->setPowerNetworkA(true);
	}
	if (command->getData()->TurnPowerNetworkBOn) {
		model->getPowerController()->setPowerNetworkB(true);
	}
	if (command->getData()->TurnPowerNetworkCOn) {
		model->getPowerController()->setPowerNetworkC(true);
	}
	if (command->getData()->TurnPowerNetworkDOn) {
		model->getPowerController()->setPowerNetworkD(true);
	}
	if (command->getData()->TurnAuxPowerNetworkAOn) {
		model->getPowerController()->setAuxPowerNetworkA(true);
	}
	if (command->getData()->TurnAuxPowerNetworkBOn) {
		model->getPowerController()->setAuxPowerNetworkB(true);
	}
	if (command->getData()->TurnAuxPowerNetworkCOn) {
		model->getPowerController()->setAuxPowerNetworkC(true);
	}
	if (command->getData()->TurnAuxPowerNetworkDOn) {
		model->getPowerController()->setAuxPowerNetworkD(true);
	}
	return model->getSafetyController()->checkSafety(States::NoStateTransition);
}

States::Type ParkedEngineeringState::turnPowerOff(TurnPowerOffCommand* command, Model* model) {
	Log.Info("ParkedEngineeringState: turnPowerOff()");
	if (command->getData()->TurnPowerNetworkAOff) {
		model->getPowerController()->setPowerNetworkA(false);
	}
	if (command->getData()->TurnPowerNetworkBOff) {
		model->getPowerController()->setPowerNetworkB(false);
	}
	if (command->getData()->TurnPowerNetworkCOff) {
		model->getPowerController()->setPowerNetworkC(false);
	}
	if (command->getData()->TurnPowerNetworkDOff) {
		model->getPowerController()->setPowerNetworkD(false);
	}
	if (command->getData()->TurnAuxPowerNetworkAOff) {
		model->getPowerController()->setAuxPowerNetworkA(false);
	}
	if (command->getData()->TurnAuxPowerNetworkBOff) {
		model->getPowerController()->setAuxPowerNetworkB(false);
	}
	if (command->getData()->TurnAuxPowerNetworkCOff) {
		model->getPowerController()->setAuxPowerNetworkC(false);
	}
	if (command->getData()->TurnAuxPowerNetworkDOff) {
		model->getPowerController()->setAuxPowerNetworkD(false);
	}
	return model->getSafetyController()->checkSafety(States::NoStateTransition);
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
