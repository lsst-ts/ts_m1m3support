/*
 * EnabledState.cpp
 *
 *  Created on: Sep 27, 2017
 *      Author: ccontaxis
 */

#include <EnabledState.h>
#include <IModel.h>
#include <ISafetyController.h>
#include <IForceController.h>
#include <TMAAzimuthSampleCommand.h>
#include <TMAElevationSampleCommand.h>

namespace LSST {
namespace M1M3 {
namespace SS {

States::Type EnabledState::storeTMAAzimuthSample(TMAAzimuthSampleCommand* command, IModel* model) {
	model->getForceController()->updateTMAAzimuthData(command->getData());
	return model->getSafetyController()->checkSafety(States::NoStateTransition);
}

States::Type EnabledState::storeTMAElevationSample(TMAElevationSampleCommand* command, IModel* model) {
	model->getForceController()->updateTMAElevationData(command->getData());
	return model->getSafetyController()->checkSafety(States::NoStateTransition);
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
