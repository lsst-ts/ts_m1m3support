/*
 * OfflineState.cpp
 *
 *  Created on: Sep 26, 2017
 *      Author: ccontaxis
 */

#include <OfflineState.h>
#include <SAL_m1m3C.h>
#include <IModel.h>
#include <IPublisher.h>

namespace LSST {
namespace M1M3 {
namespace SS {

States::Type OfflineState::boot(BootCommand* command, IModel* model) {
	States::Type newState = States::StandbyState;
	model->publishRecommendedSettings();
	model->publishStateChange(newState);
	return newState;
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
