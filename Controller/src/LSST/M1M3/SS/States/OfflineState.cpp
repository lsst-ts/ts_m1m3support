/*
 * OfflineState.cpp
 *
 *  Created on: Sep 26, 2017
 *      Author: ccontaxis
 */

#include <OfflineState.h>
#include <Model.h>
#include <DigitalInputOutput.h>
#include <PowerController.h>
#include <Log.h>

namespace LSST {
namespace M1M3 {
namespace SS {

OfflineState::OfflineState(M1M3SSPublisher* publisher) : State(publisher, "OfflineState") { }

States::Type OfflineState::boot(BootCommand* command, Model* model) {
	Log.Info("OfflineState: boot()");
	States::Type newState = States::StandbyState;
	model->publishRecommendedSettings();
	model->getDigitalInputOutput()->turnAirOff();
	model->getDigitalInputOutput()->turnCellLightsOff();
	// TODO: May need to change power controller to act like digital input output
//	model->getPowerController()->setBothPowerNetworks(false);
	return newState;
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
