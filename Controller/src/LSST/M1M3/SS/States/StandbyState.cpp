/*
 * StandbyState.cpp
 *
 *  Created on: Sep 27, 2017
 *      Author: ccontaxis
 */

#include <StandbyState.h>
#include <ILC.h>
#include <InterlockController.h>
#include <Model.h>
#include <SafetyController.h>
#include <PowerController.h>
#include <StartCommand.h>
#include <Gyro.h>

namespace LSST {
namespace M1M3 {
namespace SS {

StandbyState::StandbyState(M1M3SSPublisher* publisher) : State(publisher, "StandbyState") { }

States::Type StandbyState::update(UpdateCommand* command, Model* model) {
	model->getInterlockController()->tryToggleHeartbeat();
	return States::NoStateTransition;
}

States::Type StandbyState::start(StartCommand* command, Model* model) {
	States::Type newState = States::DisabledState;
	model->loadSettings(command->getData()->SettingsToApply);
	PowerController* powerController = model->getPowerController();
	ILC* ilc = model->getILC();
	InterlockController* interlockController = model->getInterlockController();
	Gyro* gyro = model->getGyro();
	powerController->setBothPowerNetworks(true);
	ilc->writeResetBuffer();
	ilc->triggerModbus();
	ilc->waitForAllSubnets(5000);
	ilc->readAll();
	interlockController->tryToggleHeartbeat();
	ilc->writeReportServerIDBuffer();
	ilc->triggerModbus();
	ilc->waitForAllSubnets(5000);
	ilc->readAll();
	interlockController->tryToggleHeartbeat();
	ilc->writeReportServerStatusBuffer();
	ilc->triggerModbus();
	ilc->waitForAllSubnets(5000);
	ilc->readAll();
	interlockController->tryToggleHeartbeat();
	ilc->writeReportADCScanRateBuffer();
	ilc->triggerModbus();
	ilc->waitForAllSubnets(5000);
	ilc->readAll();
	interlockController->tryToggleHeartbeat();
	ilc->writeReadCalibrationDataBuffer();
	ilc->triggerModbus();
	ilc->waitForAllSubnets(5000);
	ilc->readAll();
	interlockController->tryToggleHeartbeat();
	ilc->writeReadBoostValveDCAGainBuffer();
	ilc->triggerModbus();
	ilc->waitForAllSubnets(5000);
	ilc->readAll();
	interlockController->tryToggleHeartbeat();
	ilc->writeReportDCAIDBuffer();
	ilc->triggerModbus();
	ilc->waitForAllSubnets(5000);
	ilc->readAll();
	interlockController->tryToggleHeartbeat();
	ilc->writeReportDCAStatusBuffer();
	ilc->triggerModbus();
	ilc->waitForAllSubnets(5000);
	ilc->readAll();
	interlockController->tryToggleHeartbeat();
	ilc->writeSetModeDisableBuffer();
	ilc->triggerModbus();
	ilc->waitForAllSubnets(5000);
	ilc->readAll();
	interlockController->tryToggleHeartbeat();
	ilc->verifyResponses();
	ilc->publishForceActuatorInfo();
	ilc->publishHardpointActuatorInfo();
	ilc->publishHardpointMonitorInfo();
	gyro->enterConfigurationMode();
	gyro->resetConfiguration();
	gyro->setAxis();
	gyro->setDataRate();
	gyro->setRotationFormatRate();
	gyro->setRotationUnitsRadians();
	gyro->exitConfigurationMode();
	gyro->disableIgnore();
	gyro->bit();
	gyro->read();
	gyro->publishGyroWarningIfRequired();
	interlockController->tryToggleHeartbeat();
	interlockController->setCriticalFault(false);
	interlockController->setMirrorLoweringRaising(false);
	interlockController->setMirrorParked(true);
	return model->getSafetyController()->checkSafety(newState);
}

States::Type StandbyState::shutdown(ShutdownCommand* command, Model* model) {
	States::Type newState = States::OfflineState;
	model->shutdown();
	return newState;
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
