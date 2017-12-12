/*
 * StandbyState.cpp
 *
 *  Created on: Sep 27, 2017
 *      Author: ccontaxis
 */

#include <StandbyState.h>
#include <IILC.h>
#include <IInterlockController.h>
#include <IModel.h>
#include <ISafetyController.h>
#include <IPowerController.h>
#include <StartCommand.h>

namespace LSST {
namespace M1M3 {
namespace SS {

States::Type StandbyState::update(UpdateCommand* command, IModel* model) {
	model->getInterlockController()->tryToggleHeartbeat();
	return States::NoStateTransition;
}

States::Type StandbyState::start(StartCommand* command, IModel* model) {
	States::Type newState = States::DisabledState;
	model->loadSettings(command->getData()->SettingsToApply);
	model->getPowerController()->setAllPowerNetworks(true);
//	model->getILC()->writeCalibrationDataBuffer();
//	model->getILC()->triggerModbus();
//	model->getILC()->waitForAllSubnets(5000);
//	model->getILC()->readAll();
//	model->getILC()->writeSetADCScanRateBuffer();
//	model->getILC()->triggerModbus();
//	model->getILC()->waitForAllSubnets(5000);
//	model->getILC()->readAll();
//	model->getILC()->writeSetBoostValveDCAGainBuffer();
//	model->getILC()->triggerModbus();
//	model->getILC()->waitForAllSubnets(5000);
//	model->getILC()->readAll();
	model->getILC()->writeResetBuffer();
	model->getILC()->triggerModbus();
	model->getILC()->waitForAllSubnets(5000);
	model->getILC()->readAll();
	model->getILC()->writeReportServerIDBuffer();
	model->getILC()->triggerModbus();
	model->getILC()->waitForAllSubnets(5000);
	model->getILC()->readAll();
	model->getILC()->writeReportServerStatusBuffer();
	model->getILC()->triggerModbus();
	model->getILC()->waitForAllSubnets(5000);
	model->getILC()->readAll();
	model->getILC()->writeReportADCScanRateBuffer();
	model->getILC()->triggerModbus();
	model->getILC()->waitForAllSubnets(5000);
	model->getILC()->readAll();
	model->getILC()->writeReadCalibrationDataBuffer();
	model->getILC()->triggerModbus();
	model->getILC()->waitForAllSubnets(5000);
	model->getILC()->readAll();
	model->getILC()->writeReadBoostValveDCAGainBuffer();
	model->getILC()->triggerModbus();
	model->getILC()->waitForAllSubnets(5000);
	model->getILC()->readAll();
	model->getILC()->writeReportDCAIDBuffer();
	model->getILC()->triggerModbus();
	model->getILC()->waitForAllSubnets(5000);
	model->getILC()->readAll();
	model->getILC()->writeReportDCAStatusBuffer();
	model->getILC()->triggerModbus();
	model->getILC()->waitForAllSubnets(5000);
	model->getILC()->readAll();
	model->getILC()->writeSetModeDisableBuffer();
	model->getILC()->triggerModbus();
	model->getILC()->waitForAllSubnets(5000);
	model->getILC()->readAll();
	model->getILC()->verifyResponses();
	model->getInterlockController()->setCriticalFault(false);
	model->getInterlockController()->setMirrorLoweringRaising(false);
	model->getInterlockController()->setMirrorParked(true);
	return model->getSafetyController()->checkSafety(newState);
}

States::Type StandbyState::shutdown(ShutdownCommand* command, IModel* model) {
	States::Type newState = States::OfflineState;
	model->shutdown();
	return newState;
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
