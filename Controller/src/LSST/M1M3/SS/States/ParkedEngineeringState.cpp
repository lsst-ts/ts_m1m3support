/*
 * ParkedEngineeringState.cpp
 *
 *  Created on: Oct 17, 2017
 *      Author: ccontaxis
 */

#include <ParkedEngineeringState.h>
#include <IModel.h>
#include <IAirController.h>
#include <IDisplacement.h>
#include <IInclinometer.h>
#include <IILC.h>
#include <unistd.h>
#include <IForceController.h>
#include <ApplyOffsetForcesCommand.h>
#include <ISafetyController.h>
#include <IPositionController.h>
#include <MoveHardpointActuatorsCommand.h>
#include <EnableHardpointChaseCommand.h>
#include <DisableHardpointChaseCommand.h>
#include <IInterlockController.h>
#include <IPublisher.h>

namespace LSST {
namespace M1M3 {
namespace SS {

States::Type ParkedEngineeringState::update(UpdateCommand* command, IModel* model) {
	EnabledState::update(command, model);
	return model->getSafetyController()->checkSafety(States::NoStateTransition);
}

States::Type ParkedEngineeringState::raiseM1M3(RaiseM1M3Command* command, IModel* model) {
	States::Type newState = States::RaisingEngineeringState;
	model->getPositionController()->enableChaseAll();
	model->getForceController()->applyElevationForces();
	model->getForceController()->zeroStaticForces();
	model->getForceController()->zeroAzimuthForces();
	model->getForceController()->zeroTemperatureForces();
	model->getForceController()->zeroDynamicForces();
	model->getForceController()->zeroOffsetForces();
	model->getForceController()->zeroAberration();
	model->getForceController()->zeroAOSCorrection();
	model->getForceController()->zeroSupportPercentage();
	model->getInterlockController()->setMirrorParked(false);
	model->getInterlockController()->setMirrorLoweringRaising(true);
	model->setCachedTimestamp(model->getPublisher()->getTimestamp());
	return model->getSafetyController()->checkSafety(newState);
}

States::Type ParkedEngineeringState::exitEngineering(ExitEngineeringCommand* command, IModel* model) {
	States::Type newState = States::ParkedState;
	return model->getSafetyController()->checkSafety(newState);
}

States::Type ParkedEngineeringState::disable(DisableCommand* command, IModel* model) {
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

States::Type ParkedEngineeringState::turnAirOn(TurnAirOnCommand* command, IModel* model) {
	model->getAirController()->turnAirOn();
	return model->getSafetyController()->checkSafety(States::NoStateTransition);
}

States::Type ParkedEngineeringState::turnAirOff(TurnAirOffCommand* command, IModel* model) {
	model->getAirController()->turnAirOff();
	return model->getSafetyController()->checkSafety(States::NoStateTransition);
}

States::Type ParkedEngineeringState::stopHardpointMotion(StopHardpointMotionCommand* command, IModel* model) {
	model->getPositionController()->stopMotion();
	return model->getSafetyController()->checkSafety(States::NoStateTransition);
}

States::Type ParkedEngineeringState::moveHardpointActuators(MoveHardpointActuatorsCommand* command, IModel* model) {
	model->getPositionController()->move(command->getData()->Steps);
	return model->getSafetyController()->checkSafety(States::NoStateTransition);
}

States::Type ParkedEngineeringState::enableHardpointChase(EnableHardpointChaseCommand* command, IModel* model) {
	model->getPositionController()->enableChase(command->getData()->ActuatorId);
	return model->getSafetyController()->checkSafety(States::NoStateTransition);
}

States::Type ParkedEngineeringState::disableHardpointChase(DisableHardpointChaseCommand* command, IModel* model) {
	model->getPositionController()->disableChase(command->getData()->ActuatorId);
	return model->getSafetyController()->checkSafety(States::NoStateTransition);
}

States::Type ParkedEngineeringState::applyOffsetForces(ApplyOffsetForcesCommand* command, IModel* model) {
	model->getForceController()->applyOffsetForces(command->getData()->XForces, command->getData()->YForces, command->getData()->ZForces);
	model->getForceController()->processAppliedForces();
	return model->getSafetyController()->checkSafety(States::NoStateTransition);
}

States::Type ParkedEngineeringState::clearOffsetForces(ClearOffsetForcesCommand* command, IModel* model) {
	model->getForceController()->zeroOffsetForces();
	model->getForceController()->processAppliedForces();
	return model->getSafetyController()->checkSafety(States::NoStateTransition);
}

States::Type ParkedEngineeringState::turnLightsOn(TurnLightsOnCommand* command, IModel* model) {
	model->getInterlockController()->setCellLightsOn(true);
	return model->getSafetyController()->checkSafety(States::NoStateTransition);
}

States::Type ParkedEngineeringState::turnLightsOff(TurnLightsOffCommand* command, IModel* model) {
	model->getInterlockController()->setCellLightsOn(false);
	return model->getSafetyController()->checkSafety(States::NoStateTransition);
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
