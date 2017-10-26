/*
 * StandbyState.cpp
 *
 *  Created on: Sep 27, 2017
 *      Author: ccontaxis
 */

#include <StandbyState.h>
#include <SAL_m1m3C.h>
#include <IModel.h>
#include <IPublisher.h>
#include <IFPGA.h>
#include <StartCommand.h>
#include <IILC.h>
#include <iostream>
#include <unistd.h>

using namespace std;

namespace LSST {
namespace M1M3 {
namespace SS {

States::Type StandbyState::start(StartCommand* command, IModel* model) {
	States::Type newState = States::DisabledState;
	model->loadSettings(command->getData()->SettingsToApply);
	model->getILC()->writeCalibrationDataBuffer();
	model->getILC()->triggerModbus();
	model->getILC()->waitForAllSubnets(5000);
	model->getILC()->readAll();
	model->getILC()->writeSetADCScanRateBuffer();
	model->getILC()->triggerModbus();
	model->getILC()->waitForAllSubnets(5000);
	model->getILC()->readAll();
	model->getILC()->writeSetBoostValveDCAGainBuffer();
	model->getILC()->triggerModbus();
	model->getILC()->waitForAllSubnets(5000);
	model->getILC()->readAll();
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
	usleep(50000);
	model->queryFPGAData();
	usleep(10000);
	model->publishFPGAData();
	model->publishStateChange(newState);
	return newState;
}

States::Type StandbyState::shutdown(ShutdownCommand* command, IModel* model) {
	States::Type newState = States::OfflineState;
	model->publishStateChange(newState);
	model->shutdown();
	return newState;
}

States::Type StandbyState::update(UpdateCommand* command, IModel* model) {
	//model->queryFPGAData();
	//usleep(10000);
	//model->publishFPGAData();
	return States::Ignore;
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
