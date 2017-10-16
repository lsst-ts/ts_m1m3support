/*
 * DisabledState.cpp
 *
 *  Created on: Sep 27, 2017
 *      Author: ccontaxis
 */

#include <DisabledState.h>
#include <SAL_m1m3C.h>
#include <IModel.h>
#include <IRS232.h>
#include <unistd.h>
#include <IPublisher.h>
#include <IILC.h>
#include <IFPGA.h>
#include <iostream>

using namespace std;

namespace LSST {
namespace M1M3 {
namespace SS {

States::Type DisabledState::enable(EnableCommand* command, IModel* model) {
	States::Type newState = States::EnabledState;
	model->publishStateChange(newState);
	return newState;
}

States::Type DisabledState::standby(StandbyCommand* command, IModel* model) {
	States::Type newState = States::StandbyState;
	model->getILC()->writeSetModeStandbyBuffer();
	model->getILC()->triggerModbus();
	model->getILC()->waitForAllSubnets(5000);
	model->getILC()->readAll();
	model->publishStateChange(newState);
	return newState;
}

States::Type DisabledState::update(UpdateCommand* command, IModel* model) {
	//model->getILC()->writeFreezeSensorListBuffer();
	//model->getFPGA()->triggerModbus();
	//model->queryFPGAData();
	model->getRS232()->writeDisplacementRequest();
	//model->getRS232()->writeInclinometerRequest();
	//model->getILC()->waitForAllSubnets(5000);
	usleep(50000);
	//model->getILC()->readAll();
	model->getRS232()->readDisplacementResponse();
	//model->getRS232()->readInclinometerResponse();
	//model->getRS232()->read(2);
	//model->publishFPGAData();
	return States::Ignore;
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
