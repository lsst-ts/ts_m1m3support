/*
 * DisabledState.cpp
 *
 *  Created on: Sep 27, 2017
 *      Author: ccontaxis
 */

#include <DisabledState.h>
#include <Accelerometer.h>
#include <AirController.h>
#include <Displacement.h>
#include <ILC.h>
#include <Inclinometer.h>
#include <InterlockController.h>
#include <Model.h>
#include <SafetyController.h>
#include <PowerController.h>
#include <Gyro.h>
#include <ForceController.h>
#include <M1M3SSPublisher.h>
#include <Log.h>

namespace LSST {
namespace M1M3 {
namespace SS {

DisabledState::DisabledState(M1M3SSPublisher* publisher) : State(publisher, "DisabledState") { }

States::Type DisabledState::update(UpdateCommand* command, Model* model) {
	Log.Trace("DisabledState::update()");
	this->startTimer();
	ILC* ilc = model->getILC();
	PowerController* powerController = model->getPowerController();
	Displacement* displacement = model->getDisplacement();
	Inclinometer* inclinometer = model->getInclinometer();
	Accelerometer* accelerometer = model->getAccelerometer();
	Gyro* gyro = model->getGyro();
	ForceController* forceController = model->getForceController();
	InterlockController* interlockController = model->getInterlockController();
	M1M3SSPublisher* publisher = model->getPublisher();
	ilc->writeFreezeSensorListBuffer();
	ilc->triggerModbus();
	powerController->samplePowerSupplyDataAndStatus();
	displacement->writeDataRequest();
	inclinometer->writeDataRequest();
	accelerometer->sampleData();
	gyro->read();
	gyro->publishGyroData();
	gyro->publishGyroWarningIfRequired();
	powerController->publishPowerSupplyData();
	powerController->publishPowerSupplyStatusIfRequired();
	powerController->checkPowerStatus();
	interlockController->tryToggleHeartbeat();
	interlockController->checkInterlockStatus();
	ilc->waitForAllSubnets(5000);
	ilc->readAll();
	ilc->calculateHPPostion();
	ilc->calculateHPMirrorForces();
	ilc->calculateFAMirrorForces();
	ilc->verifyResponses();
	ilc->publishForceActuatorStatus();
	ilc->publishForceActuatorData();
	ilc->publishHardpointStatus();
	ilc->publishHardpointData();
	ilc->publishHardpointMonitorStatus();
	inclinometer->readDataResponse();
	displacement->readDataResponse();
	this->stopTimer();
	return model->getSafetyController()->checkSafety(States::NoStateTransition);
}

States::Type DisabledState::enable(EnableCommand* command, Model* model) {
	Log.Info("DisabledState: enable()");
	States::Type newState = States::ParkedState;
	model->getILC()->writeSetModeEnableBuffer();
	model->getILC()->triggerModbus();
	model->getILC()->waitForAllSubnets(5000);
	model->getILC()->readAll();
	model->getILC()->verifyResponses();
	return model->getSafetyController()->checkSafety(newState);
}

States::Type DisabledState::standby(StandbyCommand* command, Model* model) {
	Log.Info("DisabledState: standby()");
	States::Type newState = States::StandbyState;
	model->getGyro()->enableIgnore();
	model->getGyro()->read();
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
