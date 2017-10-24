/*
 * ParkedEngineeringState.cpp
 *
 *  Created on: Oct 17, 2017
 *      Author: ccontaxis
 */

#include <ParkedEngineeringState.h>
#include <IModel.h>
#include <IAirController.h>
#include <IRS232.h>
#include <IILC.h>
#include <unistd.h>

namespace LSST {
namespace M1M3 {
namespace SS {

States::Type ParkedEngineeringState::disable(DisableCommand* command, IModel* model) {
	States::Type newState = States::DisabledState;
	model->publishStateChange(newState);
	return newState;
}

States::Type ParkedEngineeringState::update(UpdateCommand* command, IModel* model) {
	model->getILC()->writeRaisedListBuffer();
	model->getILC()->triggerModbus();
	model->getRS232()->writeDisplacementRequest();
	model->getRS232()->writeInclinometerRequest();
	model->getAirController()->checkStatus();
	model->getILC()->waitForAllSubnets(5000);
	model->getILC()->readAll();
	model->getRS232()->readDisplacementResponse();
	model->getRS232()->readInclinometerResponse();
	model->getILC()->verifyResponses();
	usleep(50000);
	model->queryFPGAData();
	usleep(10000);
	model->publishFPGAData();
	model->getILC()->publishForceActuatorStatus();
	model->getILC()->publishForceActuatorData();
	model->getILC()->publishHardpointStatus();
	model->getILC()->publishHardpointData();
	model->calculateForces();
	return States::Ignore;
}

States::Type ParkedEngineeringState::turnAirOn(TurnAirOnCommand* command, IModel* model) {
	model->getAirController()->turnAirOn();
	return States::Ignore;
}

States::Type ParkedEngineeringState::turnAirOff(TurnAirOffCommand* command, IModel* model) {
	model->getAirController()->turnAirOff();
	return States::Ignore;
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
