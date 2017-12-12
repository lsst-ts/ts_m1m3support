/*
 * ParkedState.cpp
 *
 *  Created on: Nov 3, 2017
 *      Author: ccontaxis
 */

#include <ParkedState.h>
#include <IILC.h>
#include <IInterlockController.h>
#include <IModel.h>
#include <IPublisher.h>
#include <ISafetyController.h>
#include <IForceController.h>
#include <IPositionController.h>

namespace LSST {
namespace M1M3 {
namespace SS {

States::Type ParkedState::update(UpdateCommand* command, IModel* model) {
	EnabledState::update(command, model);
	return model->getSafetyController()->checkSafety(States::NoStateTransition);
}

States::Type ParkedState::raiseM1M3(RaiseM1M3Command* command, IModel* model) {
	States::Type newState = States::RaisingState;
	model->getSafetyController()->raiseOperationTimeout(false);
	model->getPositionController()->enableChaseAll();
	model->getForceController()->applyElevationForces();
	model->getForceController()->zeroStaticForces();
	model->getForceController()->zeroAzimuthForces();
	model->getForceController()->zeroTemperatureForces();
	model->getForceController()->zeroDynamicForces();
	model->getForceController()->zeroOffsetForces();
	model->getForceController()->zeroAberration();
	model->getForceController()->zeroAOSCorrection();
	model->getForceController()->zeroSupportPercentage();
	model->getInterlockController()->setMirrorParked(false);
	model->getInterlockController()->setMirrorLoweringRaising(true);
	model->setCachedTimestamp(model->getPublisher()->getTimestamp());
	return model->getSafetyController()->checkSafety(newState);
}

States::Type ParkedState::enterEngineering(EnterEngineeringCommand* command, IModel* model) {
	States::Type newState = States::ParkedEngineeringState;
	return model->getSafetyController()->checkSafety(newState);
}

States::Type ParkedState::disable(DisableCommand* command, IModel* model) {
	States::Type newState = States::DisabledState;
	model->getILC()->writeSetModeDisableBuffer();
	model->getILC()->triggerModbus();
	model->getILC()->waitForAllSubnets(5000);
	model->getILC()->readAll();
	model->getILC()->verifyResponses();
	return model->getSafetyController()->checkSafety(newState);
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
