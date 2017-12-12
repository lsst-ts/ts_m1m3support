/*
 * RaisingState.cpp
 *
 *  Created on: Nov 3, 2017
 *      Author: ccontaxis
 */

#include <RaisingState.h>
#include <IModel.h>
#include <ISafetyController.h>

namespace LSST {
namespace M1M3 {
namespace SS {

States::Type RaisingState::update(UpdateCommand* command, IModel* model) {
	States::Type newState = States::NoStateTransition;
	newState = EnabledState::performRaiseM1M3Actions(command, model);
	return model->getSafetyController()->checkSafety(newState);
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
