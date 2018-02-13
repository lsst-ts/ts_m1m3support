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

#include <iostream>
using namespace std;

namespace LSST {
namespace M1M3 {
namespace SS {

DisabledState::DisabledState(M1M3SSPublisher* publisher) : State(publisher, "DisabledState") { }

States::Type DisabledState::update(UpdateCommand* command, Model* model) {
//	cout << "Update,";
	this->startTimer();
	ILC* ilc = model->getILC();
	PowerController* powerController = model->getPowerController();
	Displacement* displacement = model->getDisplacement();
	Inclinometer* inclinometer = model->getInclinometer();
	Accelerometer* accelerometer = model->getAccelerometer();
	Gyro* gyro = model->getGyro();
	ForceController* forceController = model->getForceController();
	InterlockController* interlockController = model->getInterlockController();
//	cout << "GetModels," << this->getCurrentTimer();
	ilc->writeFreezeSensorListBuffer();
//	cout << ",WriteList," << this->getCurrentTimer();
	ilc->triggerModbus();
//	cout << ",TriggerModbus," << this->getCurrentTimer();
	powerController->samplePowerSupplyDataAndStatus();
//	cout << ",SampleStatusPS," << this->getCurrentTimer();
	displacement->writeDataRequest();
//	cout << ",DisplacementRequest," << this->getCurrentTimer();
	inclinometer->writeDataRequest();
//	cout << ",InclinometerRequest," << this->getCurrentTimer();
	accelerometer->sampleData();
//	cout << ",AccelerometerSample," << this->getCurrentTimer();
	gyro->read();
//	cout << ",GyroSample," << this->getCurrentTimer();
	gyro->publishGyroData();
//	cout << ",GyroPubData," << this->getCurrentTimer();
	gyro->publishGyroWarningIfRequired();
//	cout << ",GyroPubWarning," << this->getCurrentTimer();
	powerController->publishPowerSupplyData();
//	cout << ",PWRPubData," << this->getCurrentTimer();
	powerController->publishPowerSupplyStatusIfRequired();
//	cout << ",PWRPubStatus," << this->getCurrentTimer();
	powerController->checkPowerStatus();
//	cout << ",PWRCheckStatus," << this->getCurrentTimer();
	interlockController->tryToggleHeartbeat();
//	cout << ",InterHeartbeat," << this->getCurrentTimer();
	interlockController->checkInterlockStatus();
//	cout << ",InterStatus," << this->getCurrentTimer();
	ilc->waitForAllSubnets(5000);
//	cout << ",WaitAllSubnets," << this->getCurrentTimer();
	ilc->readAll();
//	cout << ",ILCReadAll," << this->getCurrentTimer();
	ilc->verifyResponses();
	forceController->calculateMirrorForces();
//	cout << ",ILCVerify," << this->getCurrentTimer();
	ilc->publishForceActuatorStatus();
//	cout << ",ILCPubForceStatus," << this->getCurrentTimer();
	ilc->publishForceActuatorData();
//	cout << ",ILCPubForceData," << this->getCurrentTimer();
	ilc->publishHardpointStatus();
//	cout << ",ILCPubHPStatus," << this->getCurrentTimer();
	ilc->publishHardpointData();
//	cout << ",ILCPubHPData," << this->getCurrentTimer();
	ilc->publishHardpointMonitorStatus();
//	cout << ",ILCPubHMStatus," << this->getCurrentTimer();
	inclinometer->readDataResponse();
//	cout << ",InclinometerResponse," << this->getCurrentTimer();
	displacement->readDataResponse();
//	cout << ",DisplacementResponse," << this->getCurrentTimer();
	this->stopTimer();
//	cout << ",TotalTime," << this->getTimer() << endl;
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
