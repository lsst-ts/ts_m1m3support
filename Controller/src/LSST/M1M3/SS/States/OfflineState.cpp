/*
 * OfflineState.cpp
 *
 *  Created on: Sep 26, 2017
 *      Author: ccontaxis
 */

#include <OfflineState.h>
#include <Model.h>
#include <InterlockController.h>

namespace LSST {
namespace M1M3 {
namespace SS {

States::Type OfflineState::boot(BootCommand* command, Model* model) {
	States::Type newState = States::StandbyState;
	model->publishRecommendedSettings();
	model->getInterlockController()->setCriticalFault(false);
	model->getInterlockController()->setMirrorLoweringRaising(false);
	model->getInterlockController()->setMirrorParked(true);
	return newState;
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
