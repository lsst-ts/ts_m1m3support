/*
 * EnabledState.cpp
 *
 *  Created on: Sep 27, 2017
 *      Author: ccontaxis
 */

#include <EnabledState.h>
#include <IAccelerometer.h>
#include <IAirController.h>
#include <IDisplacement.h>
#include <IForceController.h>
#include <IILC.h>
#include <IInclinometer.h>
#include <IInterlockController.h>
#include <IModel.h>
#include <IPositionController.h>
#include <ISafetyController.h>
#include <IPowerController.h>
#include <TMAAzimuthSampleCommand.h>
#include <TMAElevationSampleCommand.h>

namespace LSST {
namespace M1M3 {
namespace SS {

States::Type EnabledState::update(UpdateCommand* command, IModel* model) {
	model->getPositionController()->updateSteps();
	model->getILC()->writeRaisedListBuffer();
	model->getILC()->triggerModbus();
	model->getDisplacement()->writeDataRequest();
	model->getInclinometer()->writeDataRequest();
	model->getILC()->waitForAllSubnets(5000);
	model->getILC()->readAll();
	model->getILC()->verifyResponses();
	model->getAccelerometer()->sampleData();
	model->getAirController()->checkStatus();
	model->getDisplacement()->readDataResponse();
	model->getInclinometer()->readDataResponse();
	model->getForceController()->updateAppliedForces();
	model->getForceController()->processAppliedForces();
	model->getILC()->publishForceActuatorStatus();
	model->getILC()->publishForceActuatorData();
	model->getILC()->publishHardpointStatus();
	model->getILC()->publishHardpointData();
	model->getPowerController()->checkPowerStatus();
	model->getInterlockController()->checkInterlockStatus();
	model->getInterlockController()->tryToggleHeartbeat();
	return States::NoStateTransition;
}

States::Type EnabledState::storeTMAAzimuthSample(TMAAzimuthSampleCommand* command, IModel* model) {
	model->getForceController()->updateTMAAzimuthData(command->getData());
	return model->getSafetyController()->checkSafety(States::NoStateTransition);
}

States::Type EnabledState::storeTMAElevationSample(TMAElevationSampleCommand* command, IModel* model) {
	model->getForceController()->updateTMAElevationData(command->getData());
	return model->getSafetyController()->checkSafety(States::NoStateTransition);
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
