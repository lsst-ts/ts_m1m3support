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
#include <ForceCalculator.h>
#include <ApplyOffsetForcesCommand.h>

namespace LSST {
namespace M1M3 {
namespace SS {

States::Type ParkedEngineeringState::disable(DisableCommand* command, IModel* model) {
	States::Type newState = States::DisabledState;
	model->publishStateChange(newState);
	return newState;
}

States::Type ParkedEngineeringState::update(UpdateCommand* command, IModel* model) {
	model->getILC()->writeFreezeSensorListBuffer();
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

States::Type ParkedEngineeringState::raiseM1M3(RaiseM1M3Command* command, IModel* model) {
	States::Type newState = States::ActiveEngineeringState;
	model->getForceCalculator()->applyElevationForces();
	model->publishStateChange(newState);
	return newState;
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
