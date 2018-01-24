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

#include <iostream>
using namespace std;

namespace LSST {
namespace M1M3 {
namespace SS {

States::Type DisabledState::update(UpdateCommand* command, Model* model) {
	this->startTimer();
	ILC* ilc = model->getILC();
	PowerController* powerController = model->getPowerController();
	Displacement* displacement = model->getDisplacement();
	Inclinometer* inclinometer = model->getInclinometer();
	Accelerometer* accelerometer = model->getAccelerometer();
	Gyro* gyro = model->getGyro();
	InterlockController* interlockController = model->getInterlockController();
	ilc->writeFreezeSensorListBuffer();
	ilc->triggerModbus();
	powerController->samplePowerSupplyDataAndStatus();
	displacement->writeDataRequest();
	inclinometer->writeDataRequest();
	accelerometer->sampleData();
	ilc->waitForAllSubnets(5000);
	displacement->readDataResponse();
	inclinometer->readDataResponse();
	gyro->read();
	ilc->readAll();
	ilc->verifyResponses();
	ilc->publishForceActuatorStatus();
	ilc->publishForceActuatorData();
	ilc->publishHardpointStatus();
	ilc->publishHardpointData();
	ilc->publishHardpointMonitorStatus();
	gyro->publishGyroData();
	gyro->publishGyroWarningIfRequired();
	powerController->publishPowerSupplyData();
	powerController->publishPowerSupplyStatusIfRequired();
	powerController->checkPowerStatus();
	interlockController->tryToggleHeartbeat();
	interlockController->checkInterlockStatus();
	this->stopTimer();
	//cout << "Time: " << this->getTimer() << endl;
	return model->getSafetyController()->checkSafety(States::NoStateTransition);
}

States::Type DisabledState::enable(EnableCommand* command, Model* model) {
	States::Type newState = States::ParkedState;
	model->getILC()->writeSetModeEnableBuffer();
	model->getILC()->triggerModbus();
	model->getILC()->waitForAllSubnets(5000);
	model->getILC()->readAll();
	model->getILC()->verifyResponses();
	return model->getSafetyController()->checkSafety(newState);
}

States::Type DisabledState::standby(StandbyCommand* command, Model* model) {
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
