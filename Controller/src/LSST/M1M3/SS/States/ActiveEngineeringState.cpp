/*
 * ActiveEngineeringState.cpp
 *
 *  Created on: Oct 25, 2017
 *      Author: ccontaxis
 */

#include <ActiveEngineeringState.h>
#include <IModel.h>
#include <IILC.h>
#include <IRS232.h>
#include <IAirController.h>
#include <ForceCalculator.h>
#include <ApplyOffsetForcesCommand.h>
#include <unistd.h>

namespace LSST {
namespace M1M3 {
namespace SS {

States::Type ActiveEngineeringState::update(UpdateCommand* command, IModel* model) {
	model->getILC()->writeRaisedListBuffer();
	model->getILC()->triggerModbus();
	model->getRS232()->writeDisplacementRequest();
	model->getRS232()->writeInclinometerRequest();
	model->getAirController()->checkStatus();
	model->getForceCalculator()->updateAppliedForces();
	model->getForceCalculator()->processAppliedForces();
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

States::Type ActiveEngineeringState::applyOffsetForces(ApplyOffsetForcesCommand* command, IModel* model) {
	model->getForceCalculator()->applyOffsetForces(command->getData()->XForces, command->getData()->YForces, command->getData()->ZForces);
	model->getForceCalculator()->processAppliedForces();
	return States::Ignore;
}

States::Type ActiveEngineeringState::clearOffsetForces(ClearOffsetForcesCommand* command, IModel* model) {
	model->getForceCalculator()->zeroOffsetForces();
	model->getForceCalculator()->processAppliedForces();
	return States::Ignore;
}

States::Type ActiveEngineeringState::lowerM1M3(LowerM1M3Command* command, IModel* model) {
	States::Type newState = States::ParkedEngineeringState;
	model->getForceCalculator()->zeroElevationForces();
	model->getForceCalculator()->processAppliedForces();
	model->publishStateChange(newState);
	return newState;
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
