/*
 * EnabledState.cpp
 *
 *  Created on: Sep 27, 2017
 *      Author: ccontaxis
 */

#include <EnabledState.h>
#include <SAL_m1m3C.h>
#include <IModel.h>
#include <IPublisher.h>

namespace LSST {
namespace M1M3 {
namespace SS {

States::Type EnabledState::disable(DisableCommand* command, IModel* model) {
	States::Type newState = States::DisabledState;
	model->publishStateChange(newState);
	return newState;
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
