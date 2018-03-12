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

#include <Log.h>

namespace LSST {
namespace M1M3 {
namespace SS {

StandbyState::StandbyState(M1M3SSPublisher* publisher) : State(publisher, "StandbyState") { }

States::Type StandbyState::update(UpdateCommand* command, Model* model) {
	Log.Trace("StandbyState: update()");
	model->getInterlockController()->tryToggleHeartbeat();
	return States::NoStateTransition;
}

States::Type StandbyState::start(StartCommand* command, Model* model) {
	Log.Info("StandbyState: start()");
	States::Type newState = States::DisabledState;
	model->loadSettings(command->getData()->SettingsToApply);
	PowerController* powerController = model->getPowerController();
	ILC* ilc = model->getILC();
	InterlockController* interlockController = model->getInterlockController();
	Gyro* gyro = model->getGyro();
	powerController->setBothPowerNetworks(true);
	Log.Debug("StandbyState: Reset ILC");
	ilc->writeResetBuffer();
	ilc->triggerModbus();
	ilc->waitForAllSubnets(5000);
	ilc->readAll();
	interlockController->tryToggleHeartbeat();
	Log.Debug("StandbyState: Report server ID");
	ilc->writeReportServerIDBuffer();
	ilc->triggerModbus();
	ilc->waitForAllSubnets(5000);
	ilc->readAll();
	interlockController->tryToggleHeartbeat();
	Log.Debug("StandbyState: Report server status");
	ilc->writeReportServerStatusBuffer();
	ilc->triggerModbus();
	ilc->waitForAllSubnets(5000);
	ilc->readAll();
	interlockController->tryToggleHeartbeat();
	Log.Debug("StandbyState: Report ADC scan rate");
	ilc->writeReportADCScanRateBuffer();
	ilc->triggerModbus();
	ilc->waitForAllSubnets(5000);
	ilc->readAll();
	interlockController->tryToggleHeartbeat();
	Log.Debug("StandbyState: Read calibration data");
	ilc->writeReadCalibrationDataBuffer();
	ilc->triggerModbus();
	ilc->waitForAllSubnets(5000);
	ilc->readAll();
	interlockController->tryToggleHeartbeat();
	Log.Debug("StandbyState: Read boost valve DCA gain");
	ilc->writeReadBoostValveDCAGainBuffer();
	ilc->triggerModbus();
	ilc->waitForAllSubnets(5000);
	ilc->readAll();
	interlockController->tryToggleHeartbeat();
	Log.Debug("StandbyState: Report DCA ID");
	ilc->writeReportDCAIDBuffer();
	ilc->triggerModbus();
	ilc->waitForAllSubnets(5000);
	ilc->readAll();
	interlockController->tryToggleHeartbeat();
	Log.Debug("StandbyState: Report DCA status");
	ilc->writeReportDCAStatusBuffer();
	ilc->triggerModbus();
	ilc->waitForAllSubnets(5000);
	ilc->readAll();
	interlockController->tryToggleHeartbeat();
	Log.Debug("StandbyState: Set mode disable");
	ilc->writeSetModeDisableBuffer();
	ilc->triggerModbus();
	ilc->waitForAllSubnets(5000);
	ilc->readAll();
	interlockController->tryToggleHeartbeat();
	ilc->verifyResponses();
	ilc->publishForceActuatorInfo();
	ilc->publishHardpointActuatorInfo();
	ilc->publishHardpointMonitorInfo();
	Log.Debug("StandbyState: Configuring gyro");
	gyro->enterConfigurationMode();
	gyro->resetConfiguration();
	gyro->setAxis();
	gyro->setDataRate();
	gyro->setRotationFormatRate();
	gyro->setRotationUnitsRadians();
	gyro->exitConfigurationMode();
	gyro->disableIgnore();
	Log.Debug("StandbyState: Running gyro BIT");
	gyro->bit();
	gyro->read();
	gyro->publishGyroWarningIfRequired();
	interlockController->tryToggleHeartbeat();
	Log.Debug("StandbyState: Configuring interlock");
	interlockController->setCriticalFault(false);
	interlockController->setMirrorLoweringRaising(false);
	interlockController->setMirrorParked(true);
	Log.Debug("StandbyState: Start completed");
	return model->getSafetyController()->checkSafety(newState);
}

States::Type StandbyState::shutdown(ShutdownCommand* command, Model* model) {
	Log.Info("StandbyState: shutdown()");
	States::Type newState = States::OfflineState;
	model->shutdown();
	return newState;
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
