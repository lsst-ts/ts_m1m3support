/*
 * StandbyState.cpp
 *
 *  Created on: Sep 27, 2017
 *      Author: ccontaxis
 */

#include <StandbyState.h>
#include <ILC.h>
#include <IFPGA.h>
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
	model->getFPGA()->pullTelemetry();
	model->getDigitalInputOutput()->tryToggleHeartbeat();
	model->getDigitalInputOutput()->processData();
	return States::NoStateTransition;
}

States::Type StandbyState::start(StartCommand* command, Model* model) {
	Log.Info("StandbyState: start()");
	States::Type newState = States::DisabledState;
	model->loadSettings(command->getData()->settingsToApply);
	PowerController* powerController = model->getPowerController();
	ILC* ilc = model->getILC();
	DigitalInputOutput* digitalInputOutput = model->getDigitalInputOutput();
	Gyro* gyro = model->getGyro();

	powerController->setAllAuxPowerNetworks(false);
	powerController->setAllPowerNetworks(true);
	for(int i = 0; i < 2; ++i) {
		digitalInputOutput->tryToggleHeartbeat();
		usleep(500000);
	}

	ilc->flushAll();
	ilc->writeSetModeClearFaultsBuffer();
	ilc->triggerModbus();
	ilc->waitForAllSubnets(5000);
	ilc->readAll();
	digitalInputOutput->tryToggleHeartbeat();
	ilc->writeReportServerIDBuffer();
	ilc->triggerModbus();
	ilc->waitForAllSubnets(5000);
	ilc->readAll();
	digitalInputOutput->tryToggleHeartbeat();
	ilc->writeReportServerStatusBuffer();
	ilc->triggerModbus();
	ilc->waitForAllSubnets(5000);
	ilc->readAll();
	digitalInputOutput->tryToggleHeartbeat();
	ilc->writeReportADCScanRateBuffer();
	ilc->triggerModbus();
	ilc->waitForAllSubnets(5000);
	ilc->readAll();
	digitalInputOutput->tryToggleHeartbeat();
	ilc->writeReadCalibrationDataBuffer();
	ilc->triggerModbus();
	ilc->waitForAllSubnets(5000);
	ilc->readAll();
	digitalInputOutput->tryToggleHeartbeat();
	ilc->writeReadBoostValveDCAGainBuffer();
	ilc->triggerModbus();
	ilc->waitForAllSubnets(5000);
	ilc->readAll();
	digitalInputOutput->tryToggleHeartbeat();
	ilc->writeReportDCAIDBuffer();
	ilc->triggerModbus();
	ilc->waitForAllSubnets(5000);
	ilc->readAll();
	digitalInputOutput->tryToggleHeartbeat();
	ilc->writeReportDCAStatusBuffer();
	ilc->triggerModbus();
	ilc->waitForAllSubnets(5000);
	ilc->readAll();
	digitalInputOutput->tryToggleHeartbeat();
	ilc->writeSetModeDisableBuffer();
	ilc->triggerModbus();
	ilc->waitForAllSubnets(5000);
	ilc->readAll();
	digitalInputOutput->tryToggleHeartbeat();
	model->getPublisher()->tryLogForceActuatorState();
	usleep(20000);
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
