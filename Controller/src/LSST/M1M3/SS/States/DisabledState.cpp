/*
 * DisabledState.cpp
 *
 *  Created on: Sep 27, 2017
 *      Author: ccontaxis
 */

#include <DisabledState.h>
#include <IAccelerometer.h>
#include <IAirController.h>
#include <IDisplacement.h>
#include <IILC.h>
#include <IInclinometer.h>
#include <IInterlockController.h>
#include <IModel.h>
#include <ISafetyController.h>
#include <IPowerController.h>

#include <iostream>
using namespace std;

namespace LSST {
namespace M1M3 {
namespace SS {

States::Type DisabledState::update(UpdateCommand* command, IModel* model) {
//	this->startTimer();
	model->getILC()->writeFreezeSensorListBuffer();
	model->getILC()->triggerModbus();
	model->getDisplacement()->writeDataRequest();
	model->getInclinometer()->writeDataRequest();
	model->getAccelerometer()->sampleData();
	model->getILC()->waitForAllSubnets(5000);
	model->getDisplacement()->readDataResponse();
	model->getInclinometer()->readDataResponse();
	model->getILC()->readAll();
	model->getILC()->verifyResponses();
	model->getILC()->publishForceActuatorStatus();
	model->getILC()->publishForceActuatorData();
	model->getILC()->publishHardpointStatus();
	model->getILC()->publishHardpointData();
	model->getInterlockController()->tryToggleHeartbeat();
//	this->stopTimer();
//	cout << "Time: " << this->getTimer() << endl;
	return model->getSafetyController()->checkSafety(States::NoStateTransition);
}

States::Type DisabledState::enable(EnableCommand* command, IModel* model) {
	States::Type newState = States::ParkedState;
	model->getILC()->writeSetModeEnableBuffer();
	model->getILC()->triggerModbus();
	model->getILC()->waitForAllSubnets(5000);
	model->getILC()->readAll();
	model->getILC()->verifyResponses();
	return model->getSafetyController()->checkSafety(newState);
}

States::Type DisabledState::standby(StandbyCommand* command, IModel* model) {
	States::Type newState = States::StandbyState;
	model->getILC()->writeSetModeStandbyBuffer();
	model->getILC()->triggerModbus();
	model->getILC()->waitForAllSubnets(5000);
	model->getILC()->readAll();
	model->getILC()->verifyResponses();
	model->getPowerController()->setAllPowerNetworks(false);
	return model->getSafetyController()->checkSafety(newState);
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
