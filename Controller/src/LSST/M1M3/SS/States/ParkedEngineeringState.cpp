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
	model->getDigitalInputOutput()->turnAirOn();
	model->getPositionController()->stopMotion();
	model->getForceController()->zeroOffsetForces();
	model->getForceController()->processAppliedForces();
	model->getDigitalInputOutput()->turnCellLightsOff();
	// TODO: Real problems exist if the user enabled / disabled ILC power...
	model->getPowerController()->setAllPowerNetworks(true);
	return model->getSafetyController()->checkSafety(newState);
}

States::Type ParkedEngineeringState::disable(DisableCommand* command, Model* model) {
	Log.Info("ParkedEngineeringState: disable()");
	States::Type newState = States::DisabledState;
	// Stop any existing motion (chase and move commands)
	model->getPositionController()->stopMotion();
	model->getForceController()->reset();
	// Perform ILC state transition
	model->getILC()->writeSetModeDisableBuffer();
	model->getILC()->triggerModbus();
	model->getILC()->waitForAllSubnets(5000);
	model->getILC()->readAll();
	model->getILC()->verifyResponses();
	// TODO: Uncomment this later when its not so hot outside
	//model->getDigitalInputOutput()->turnAirOff();
	model->getPowerController()->setAllAuxPowerNetworks(false);
	return model->getSafetyController()->checkSafety(newState);
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
