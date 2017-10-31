/*
 * FaultState.cpp
 *
 *  Created on: Oct 26, 2017
 *      Author: ccontaxis
 */

#include <FaultState.h>
#include <IModel.h>
#include <IILC.h>
#include <IDisplacement.h>
#include <IInclinometer.h>
#include <unistd.h>

namespace LSST {
namespace M1M3 {
namespace SS {

States::Type FaultState::update(UpdateCommand* command, IModel* model) {
	model->getILC()->writeFreezeSensorListBuffer();
	model->getILC()->triggerModbus();
	model->getDisplacement()->writeDataRequest();
	model->getInclinometer()->writeDataRequest();
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
	//model->getAirController()->checkStatus();
	return States::Ignore;
}

States::Type FaultState::standby(StandbyCommand* command, IModel* model) {
	States::Type newState = States::StandbyState;
	model->getILC()->writeSetModeStandbyBuffer();
	model->getILC()->triggerModbus();
	model->getILC()->waitForAllSubnets(5000);
	model->getILC()->readAll();
	model->getILC()->verifyResponses();
	return newState;
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
