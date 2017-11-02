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

namespace LSST {
namespace M1M3 {
namespace SS {

States::Type ParkedEngineeringState::disable(DisableCommand* command, IModel* model) {
	States::Type newState = States::DisabledState;
	return model->getSafetyController()->checkSafety(newState);
}

States::Type ParkedEngineeringState::update(UpdateCommand* command, IModel* model) {
	model->getPositionController()->updateSteps();
	model->getILC()->writeRaisedListBuffer();
	model->getILC()->triggerModbus();
	model->getDisplacement()->writeDataRequest();
	model->getInclinometer()->writeDataRequest();
	model->getAirController()->checkStatus();
	model->getILC()->waitForAllSubnets(5000);
	model->getILC()->readAll();
	model->getDisplacement()->readDataResponse();
	model->getInclinometer()->readDataResponse();
	model->getILC()->verifyResponses();
	usleep(50000);
	model->queryFPGAData();
	usleep(10000);
	model->publishFPGAData();
	model->getILC()->publishForceActuatorStatus();
	model->getILC()->publishForceActuatorData();
	model->getILC()->publishHardpointStatus();
	model->getILC()->publishHardpointData();
	return model->getSafetyController()->checkSafety(States::Ignore);
}

States::Type ParkedEngineeringState::turnAirOn(TurnAirOnCommand* command, IModel* model) {
	model->getAirController()->turnAirOn();
	return model->getSafetyController()->checkSafety(States::Ignore);
}

States::Type ParkedEngineeringState::turnAirOff(TurnAirOffCommand* command, IModel* model) {
	model->getAirController()->turnAirOff();
	return model->getSafetyController()->checkSafety(States::Ignore);
}

States::Type ParkedEngineeringState::raiseM1M3(RaiseM1M3Command* command, IModel* model) {
	States::Type newState = States::ActiveEngineeringState;
	model->getInterlockController()->setMirrorParked(false);
	model->getForceController()->applyStaticForces();
	model->getForceController()->applyElevationForces();
	model->getForceController()->zeroOffsetForces();
	model->getForceController()->zeroAberration();
	model->getForceController()->zeroAOSCorrection();
	return model->getSafetyController()->checkSafety(newState);
}

States::Type ParkedEngineeringState::stopHardpointMotion(StopHardpointMotionCommand* command, IModel* model) {
	model->getPositionController()->stopMotion();
	return model->getSafetyController()->checkSafety(States::Ignore);
}

States::Type ParkedEngineeringState::moveHardpointActuators(MoveHardpointActuatorsCommand* command, IModel* model) {
	model->getPositionController()->move(command->getData()->Steps);
	return model->getSafetyController()->checkSafety(States::Ignore);
}

States::Type ParkedEngineeringState::enableHardpointChase(EnableHardpointChaseCommand* command, IModel* model) {
	model->getPositionController()->enableChase(command->getData()->ActuatorId);
	return model->getSafetyController()->checkSafety(States::Ignore);
}

States::Type ParkedEngineeringState::disableHardpointChase(DisableHardpointChaseCommand* command, IModel* model) {
	model->getPositionController()->disableChase(command->getData()->ActuatorId);
	return model->getSafetyController()->checkSafety(States::Ignore);
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
