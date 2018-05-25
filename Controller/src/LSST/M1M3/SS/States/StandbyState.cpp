/*
 * StandbyState.cpp
 *
 *  Created on: Sep 27, 2017
 *      Author: ccontaxis
 */

#include <StandbyState.h>
#include <ILC.h>
#include <DigitalInputOutput.h>
#include <Model.h>
#include <SafetyController.h>
#include <PowerController.h>
#include <StartCommand.h>
#include <Gyro.h>
#include <M1M3SSPublisher.h>
#include <unistd.h>

#include <Log.h>

namespace LSST {
namespace M1M3 {
namespace SS {

StandbyState::StandbyState(M1M3SSPublisher* publisher) : State(publisher, "StandbyState") { }

States::Type StandbyState::update(UpdateCommand* command, Model* model) {
	Log.Trace("StandbyState: update()");
	model->getDigitalInputOutput()->tryToggleHeartbeat();
	return States::NoStateTransition;
}

States::Type StandbyState::start(StartCommand* command, Model* model) {
	Log.Info("StandbyState: start()");
	States::Type newState = States::DisabledState;
	model->loadSettings(command->getData()->SettingsToApply);
	PowerController* powerController = model->getPowerController();
	ILC* ilc = model->getILC();
	DigitalInputOutput* digitalInputOutput = model->getDigitalInputOutput();
	Gyro* gyro = model->getGyro();

	powerController->setAllPowerNetworks(true);
	powerController->setAllAuxPowerNetworks(false);
	for(int i = 0; i < 10; ++i) {
		digitalInputOutput->tryToggleHeartbeat();
		usleep(500000);
	}

	// TODO: Wont need this because the power network is just turned on.
//	ilc->writeResetBuffer();
//	ilc->triggerModbus();
//	ilc->waitForAllSubnets(5000);
//	ilc->readAll();
//	digitalInputOutput->tryToggleHeartbeat();
	Log.Info("ClearBuffer");
	ilc->flushAll();
	Log.Info("ReportServerID");
	ilc->writeReportServerIDBuffer();
	ilc->triggerModbus();
	ilc->waitForAllSubnets(5000);
	ilc->readAll();
	digitalInputOutput->tryToggleHeartbeat();
	Log.Info("ReportServerStatus");
	ilc->writeReportServerStatusBuffer();
	ilc->triggerModbus();
	ilc->waitForAllSubnets(5000);
	ilc->readAll();
	digitalInputOutput->tryToggleHeartbeat();
	Log.Info("ReportADCScanRate");
	ilc->writeReportADCScanRateBuffer();
	ilc->triggerModbus();
	ilc->waitForAllSubnets(5000);
	ilc->readAll();
	digitalInputOutput->tryToggleHeartbeat();
	Log.Info("ReadCalibrationData");
	ilc->writeReadCalibrationDataBuffer();
	ilc->triggerModbus();
	ilc->waitForAllSubnets(5000);
	ilc->readAll();
	digitalInputOutput->tryToggleHeartbeat();
	Log.Info("ReadBoostValveDCAGain");
	ilc->writeReadBoostValveDCAGainBuffer();
	ilc->triggerModbus();
	ilc->waitForAllSubnets(5000);
	ilc->readAll();
	digitalInputOutput->tryToggleHeartbeat();
	Log.Info("ReportDCAID");
	ilc->writeReportDCAIDBuffer();
	ilc->triggerModbus();
	ilc->waitForAllSubnets(5000);
	ilc->readAll();
	digitalInputOutput->tryToggleHeartbeat();
//	Log.Info("ReportDCAStatus");
//	ilc->writeReportDCAStatusBuffer();
//	ilc->triggerModbus();
//	ilc->waitForAllSubnets(5000);
//	ilc->readAll();
//	digitalInputOutput->tryToggleHeartbeat();
//	Log.Info("SetModeStandby");
//	ilc->writeSetModeStandbyBuffer();
//	ilc->triggerModbus();
//	ilc->waitForAllSubnets(5000);
//	ilc->readAll();
//	digitalInputOutput->tryToggleHeartbeat();
//	model->getPublisher()->tryLogForceActuatorState();
	Log.Info("SetModeDisable");
	ilc->writeSetModeDisableBuffer();
	ilc->triggerModbus();
	ilc->waitForAllSubnets(5000);
	ilc->readAll();
	digitalInputOutput->tryToggleHeartbeat();
	model->getPublisher()->tryLogForceActuatorState();
	usleep(20000);
//	Log.Info("SetModeEnable");
//	ilc->writeSetModeEnableBuffer();
//	ilc->triggerModbus();
//	ilc->waitForAllSubnets(5000);
//	ilc->readAll();
//	digitalInputOutput->tryToggleHeartbeat();
//	model->getPublisher()->tryLogForceActuatorState();
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
	gyro->bit();
	digitalInputOutput->tryToggleHeartbeat();
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
