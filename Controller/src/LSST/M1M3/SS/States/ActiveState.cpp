/*
 * ActiveState.cpp
 *
 *  Created on: Nov 3, 2017
 *      Author: ccontaxis
 */

#include <ActiveState.h>
#include <Model.h>
#include <Publisher.h>
#include <SafetyController.h>
#include <AutomaticOperationsController.h>
#include <Log.h>

namespace LSST {
namespace M1M3 {
namespace SS {

ActiveState::ActiveState(M1M3SSPublisher* publisher) : EnabledState(publisher, "ActiveState") { }

States::Type ActiveState::update(UpdateCommand* command, Model* model) {
	Log.Trace("ActiveState: update()");
	this->startTimer();
	EnabledState::update(command, model);
	this->stopTimer();
	model->publishOuterLoop(this->getTimer());
	return model->getSafetyController()->checkSafety(States::NoStateTransition);
}

States::Type ActiveState::enterEngineering(EnterEngineeringCommand* command, Model* model) {
	Log.Info("ActiveState: enterEngineering()");
	States::Type newState = States::ActiveEngineeringState;
	return model->getSafetyController()->checkSafety(newState);
}

States::Type ActiveState::lowerM1M3(LowerM1M3Command* command, Model* model) {
	Log.Info("ActiveState: lowerM1M3()");
	States::Type newState = States::LoweringState;
	model->getAutomaticOperationsController()->startLowerOperation();
	return model->getSafetyController()->checkSafety(newState);
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
