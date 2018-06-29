/*
 * LoweringFaultState.cpp
 *
 *  Created on: Feb 7, 2018
 *      Author: ccontaxis
 */

#include <LoweringFaultState.h>
#include <Model.h>
#include <PowerController.h>
#include <DigitalInputOutput.h>
#include <Log.h>

namespace LSST {
namespace M1M3 {
namespace SS {

LoweringFaultState::LoweringFaultState(M1M3SSPublisher* publisher) : FaultState(publisher, "LoweringFaultState") { }

States::Type LoweringFaultState::update(UpdateCommand* command, Model* model) {
	Log.Trace("LoweringFaultState: update()");
	model->getPowerController()->setAllAuxPowerNetworks(false);
	model->getDigitalInputOutput()->turnAirOff();
	FaultState::update(command, model);
	return States::FaultState;
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
