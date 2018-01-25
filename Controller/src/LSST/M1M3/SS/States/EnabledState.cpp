/*
 * EnabledState.cpp
 *
 *  Created on: Sep 27, 2017
 *      Author: ccontaxis
 */

#include <EnabledState.h>
#include <Accelerometer.h>
#include <AirController.h>
#include <Displacement.h>
#include <ForceController.h>
#include <ILC.h>
#include <Inclinometer.h>
#include <InterlockController.h>
#include <Model.h>
#include <PositionController.h>
#include <SafetyController.h>
#include <PowerController.h>
#include <TMAAzimuthSampleCommand.h>
#include <TMAElevationSampleCommand.h>
#include <M1M3SSPublisher.h>
#include <Gyro.h>

namespace LSST {
namespace M1M3 {
namespace SS {

EnabledState::EnabledState(M1M3SSPublisher* publisher) : State(publisher, "EnabledState") { }
EnabledState::EnabledState(M1M3SSPublisher* publisher, std::string name) : State(publisher, name) { }

States::Type EnabledState::update(UpdateCommand* command, Model* model) {
	model->getPositionController()->updateSteps();
	model->getILC()->writeRaisedListBuffer();
	model->getILC()->triggerModbus();
	model->getPowerController()->samplePowerSupplyDataAndStatus();
	model->getDisplacement()->writeDataRequest();
	model->getInclinometer()->writeDataRequest();
	model->getILC()->waitForAllSubnets(5000);
	model->getILC()->readAll();
	model->getILC()->verifyResponses();
	model->getAccelerometer()->sampleData();
	model->getAirController()->checkStatus();
	model->getDisplacement()->readDataResponse();
	model->getInclinometer()->readDataResponse();
	model->getGyro()->read();
	model->getForceController()->updateAppliedForces();
	model->getForceController()->processAppliedForces();
	model->getILC()->publishForceActuatorStatus();
	model->getILC()->publishForceActuatorData();
	model->getILC()->publishHardpointStatus();
	model->getILC()->publishHardpointData();
	model->getILC()->publishHardpointMonitorStatus();
	model->getGyro()->publishGyroData();
	model->getGyro()->publishGyroWarningIfRequired();
	model->getPowerController()->publishPowerSupplyData();
	model->getPowerController()->publishPowerSupplyStatusIfRequired();
	model->getPowerController()->checkPowerStatus();
	model->getInterlockController()->checkInterlockStatus();
	model->getInterlockController()->tryToggleHeartbeat();
	return States::NoStateTransition;
}

States::Type EnabledState::storeTMAAzimuthSample(TMAAzimuthSampleCommand* command, Model* model) {
	model->getForceController()->updateTMAAzimuthData(command->getData());
	return model->getSafetyController()->checkSafety(States::NoStateTransition);
}

States::Type EnabledState::storeTMAElevationSample(TMAElevationSampleCommand* command, Model* model) {
	model->getForceController()->updateTMAElevationData(command->getData());
	return model->getSafetyController()->checkSafety(States::NoStateTransition);
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
