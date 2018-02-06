/*
 * ParkedEngineeringState.cpp
 *
 *  Created on: Oct 17, 2017
 *      Author: ccontaxis
 */

#include <ParkedEngineeringState.h>
#include <Model.h>
#include <AirController.h>
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
#include <InterlockController.h>
#include <M1M3SSPublisher.h>
#include <PowerController.h>
#include <TurnPowerOnCommand.h>
#include <TurnPowerOffCommand.h>
#include <AutomaticOperationsController.h>

namespace LSST {
namespace M1M3 {
namespace SS {

ParkedEngineeringState::ParkedEngineeringState(M1M3SSPublisher* publisher) : EngineeringState(publisher, "ParkedEngineeringState") { }

States::Type ParkedEngineeringState::update(UpdateCommand* command, Model* model) {
	EnabledState::update(command, model);
	return model->getSafetyController()->checkSafety(States::NoStateTransition);
}

States::Type ParkedEngineeringState::raiseM1M3(RaiseM1M3Command* command, Model* model) {
	States::Type newState = States::RaisingEngineeringState;
	model->getAutomaticOperationsController()->startRaiseOperation();
	return model->getSafetyController()->checkSafety(newState);
}

States::Type ParkedEngineeringState::exitEngineering(ExitEngineeringCommand* command, Model* model) {
	States::Type newState = States::ParkedState;
	return model->getSafetyController()->checkSafety(newState);
}

States::Type ParkedEngineeringState::disable(DisableCommand* command, Model* model) {
	States::Type newState = States::DisabledState;
	// Make sure the air is off
	model->getAirController()->turnAirOff();
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
	model->getAirController()->turnAirOn();
	return model->getSafetyController()->checkSafety(States::NoStateTransition);
}

States::Type ParkedEngineeringState::turnAirOff(TurnAirOffCommand* command, Model* model) {
	model->getAirController()->turnAirOff();
	return model->getSafetyController()->checkSafety(States::NoStateTransition);
}

States::Type ParkedEngineeringState::stopHardpointMotion(StopHardpointMotionCommand* command, Model* model) {
	model->getPositionController()->stopMotion();
	return model->getSafetyController()->checkSafety(States::NoStateTransition);
}

States::Type ParkedEngineeringState::moveHardpointActuators(MoveHardpointActuatorsCommand* command, Model* model) {
	if (!model->getPositionController()->move(command->getData()->Steps)) {
		model->getPublisher()->logCommandRejectionWarning("MoveHardpointActuators", "At least one hardpoint actuator commanded to move is already MOVING or CHASING.");
	}
	return model->getSafetyController()->checkSafety(States::NoStateTransition);
}

States::Type ParkedEngineeringState::enableHardpointChase(EnableHardpointChaseCommand* command, Model* model) {
	if (!model->getPositionController()->enableChase(command->getData()->ActuatorId)) {
		model->getPublisher()->logCommandRejectionWarning("EnableHardpointChase", "At least one hardpoint actuator commanded to chase is already MOVING or CHASING.");
	}
	return model->getSafetyController()->checkSafety(States::NoStateTransition);
}

States::Type ParkedEngineeringState::disableHardpointChase(DisableHardpointChaseCommand* command, Model* model) {
	model->getPositionController()->disableChase(command->getData()->ActuatorId);
	return model->getSafetyController()->checkSafety(States::NoStateTransition);
}

States::Type ParkedEngineeringState::applyOffsetForces(ApplyOffsetForcesCommand* command, Model* model) {
	model->getForceController()->applyOffsetForces(command->getData()->XForces, command->getData()->YForces, command->getData()->ZForces);
	model->getForceController()->processAppliedForces();
	return model->getSafetyController()->checkSafety(States::NoStateTransition);
}

States::Type ParkedEngineeringState::clearOffsetForces(ClearOffsetForcesCommand* command, Model* model) {
	model->getForceController()->zeroOffsetForces();
	model->getForceController()->processAppliedForces();
	return model->getSafetyController()->checkSafety(States::NoStateTransition);
}

States::Type ParkedEngineeringState::turnLightsOn(TurnLightsOnCommand* command, Model* model) {
	model->getInterlockController()->setCellLightsOn(true);
	return model->getSafetyController()->checkSafety(States::NoStateTransition);
}

States::Type ParkedEngineeringState::turnLightsOff(TurnLightsOffCommand* command, Model* model) {
	model->getInterlockController()->setCellLightsOn(false);
	return model->getSafetyController()->checkSafety(States::NoStateTransition);
}

States::Type ParkedEngineeringState::turnPowerOn(TurnPowerOnCommand* command, Model* model) {
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
