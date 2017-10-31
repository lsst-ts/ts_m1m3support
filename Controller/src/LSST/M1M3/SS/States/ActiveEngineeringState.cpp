/*
 * ActiveEngineeringState.cpp
 *
 *  Created on: Oct 25, 2017
 *      Author: ccontaxis
 */

#include <ActiveEngineeringState.h>
#include <IModel.h>
#include <IILC.h>
#include <IDisplacement.h>
#include <IInclinometer.h>
#include <IAirController.h>
#include <IForceController.h>
#include <ApplyOffsetForcesCommand.h>
#include <ApplyAberrationByBendingModesCommand.h>
#include <ApplyAberrationByForcesCommand.h>
#include <ClearAberrationCommand.h>
#include <ApplyAOSCorrectionByBendingModesCommand.h>
#include <ApplyAOSCorrectionByForcesCommand.h>
#include <ClearAOSCorrectionCommand.h>
#include <ISafetyController.h>
#include <unistd.h>

namespace LSST {
namespace M1M3 {
namespace SS {

States::Type ActiveEngineeringState::update(UpdateCommand* command, IModel* model) {
	model->getILC()->writeRaisedListBuffer();
	model->getILC()->triggerModbus();
	model->getDisplacement()->writeDataRequest();
	model->getInclinometer()->writeDataRequest();
	model->getAirController()->checkStatus();
	model->getForceController()->updateAppliedForces();
	model->getForceController()->processAppliedForces();
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

States::Type ActiveEngineeringState::applyOffsetForces(ApplyOffsetForcesCommand* command, IModel* model) {
	model->getForceController()->applyOffsetForces(command->getData()->XForces, command->getData()->YForces, command->getData()->ZForces);
	model->getForceController()->processAppliedForces();
	return model->getSafetyController()->checkSafety(States::Ignore);
}

States::Type ActiveEngineeringState::clearOffsetForces(ClearOffsetForcesCommand* command, IModel* model) {
	model->getForceController()->zeroOffsetForces();
	model->getForceController()->processAppliedForces();
	return model->getSafetyController()->checkSafety(States::Ignore);
}

States::Type ActiveEngineeringState::lowerM1M3(LowerM1M3Command* command, IModel* model) {
	States::Type newState = States::ParkedEngineeringState;
	model->getForceController()->zeroStaticForces();
	model->getForceController()->zeroOffsetForces();
	model->getForceController()->zeroElevationForces();
	model->getForceController()->zeroAberration();
	model->getForceController()->zeroAOSCorrection();
	model->getForceController()->processAppliedForces();
	return model->getSafetyController()->checkSafety(newState);
}

States::Type ActiveEngineeringState::applyAberrationByBendingModes(ApplyAberrationByBendingModesCommand* command, IModel* model) {
	model->getForceController()->applyAberrationByBendingModes(command->getData()->Coefficients);
	model->getForceController()->processAppliedForces();
	return model->getSafetyController()->checkSafety(States::Ignore);
}

States::Type ActiveEngineeringState::applyAberrationByForces(ApplyAberrationByForcesCommand* command, IModel* model) {
	model->getForceController()->applyAberrationByForces(command->getData()->ZForces);
	model->getForceController()->processAppliedForces();
	return model->getSafetyController()->checkSafety(States::Ignore);
}

States::Type ActiveEngineeringState::clearAberration(ClearAberrationCommand* command, IModel* model) {
	model->getForceController()->zeroAberration();
	model->getForceController()->processAppliedForces();
	return model->getSafetyController()->checkSafety(States::Ignore);
}

States::Type ActiveEngineeringState::applyAOSCorrectionByBendingModes(ApplyAOSCorrectionByBendingModesCommand* command, IModel* model) {
	model->getForceController()->applyAOSCorrectionByBendingModes(command->getData()->Coefficients);
	model->getForceController()->processAppliedForces();
	return model->getSafetyController()->checkSafety(States::Ignore);
}

States::Type ActiveEngineeringState::applyAOSCorrectionByForces(ApplyAOSCorrectionByForcesCommand* command, IModel* model) {
	model->getForceController()->applyAOSCorrectionByForces(command->getData()->ZForces);
	model->getForceController()->processAppliedForces();
	return model->getSafetyController()->checkSafety(States::Ignore);
}

States::Type ActiveEngineeringState::clearAOSCorrection(ClearAOSCorrectionCommand* command, IModel* model) {
	model->getForceController()->zeroAOSCorrection();
	model->getForceController()->processAppliedForces();
	return model->getSafetyController()->checkSafety(States::Ignore);
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
