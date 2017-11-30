/*
 * ActiveEngineeringState.cpp
 *
 *  Created on: Oct 25, 2017
 *      Author: ccontaxis
 */

#include <ActiveEngineeringState.h>
#include <IModel.h>
#include <IILC.h>
#include <IDisplacement.h>
#include <IInclinometer.h>
#include <IAirController.h>
#include <IForceController.h>
#include <ApplyOffsetForcesCommand.h>
#include <ApplyAberrationByBendingModesCommand.h>
#include <ApplyAberrationByForcesCommand.h>
#include <ClearAberrationCommand.h>
#include <ApplyAOSCorrectionByBendingModesCommand.h>
#include <ApplyAOSCorrectionByForcesCommand.h>
#include <ClearAOSCorrectionCommand.h>
#include <ISafetyController.h>
#include <IInterlockController.h>
#include <TMAAzimuthSampleCommand.h>
#include <TMAElevationSampleCommand.h>
#include <MoveHardpointActuatorsCommand.h>
#include <TranslateM1M3Command.h>
#include <IPositionController.h>
#include <PositionM1M3Command.h>
#include <IPublisher.h>
#include <unistd.h>

#include <iostream>
using namespace std;

namespace LSST {
namespace M1M3 {
namespace SS {

States::Type ActiveEngineeringState::update(UpdateCommand* command, IModel* model) {
	EnabledState::update(command, model);
	return model->getSafetyController()->checkSafety(States::NoStateTransition);
}

States::Type ActiveEngineeringState::lowerM1M3(LowerM1M3Command* command, IModel* model) {
	States::Type newState = States::LoweringEngineeringState;
	model->getForceController()->zeroStaticForces();
	model->getForceController()->zeroOffsetForces();
	model->getForceController()->zeroElevationForces();
	model->getForceController()->zeroAzimuthForces();
	model->getForceController()->zeroTemperatureForces();
	model->getForceController()->zeroAberration();
	model->getForceController()->zeroAOSCorrection();
	model->getForceController()->processAppliedForces();
	model->getInterlockController()->setMirrorLoweringRaising(true);
	model->setCachedTimestamp(model->getPublisher()->getTimestamp());
	return model->getSafetyController()->checkSafety(newState);
}

States::Type ActiveEngineeringState::exitEngineering(ExitEngineeringCommand* command, IModel* model) {
	States::Type newState = States::ActiveState;
	return model->getSafetyController()->checkSafety(newState);
}

States::Type ActiveEngineeringState::applyOffsetForces(ApplyOffsetForcesCommand* command, IModel* model) {
	model->getForceController()->applyOffsetForces(command->getData()->XForces, command->getData()->YForces, command->getData()->ZForces);
	model->getForceController()->processAppliedForces();
	return model->getSafetyController()->checkSafety(States::NoStateTransition);
}

States::Type ActiveEngineeringState::clearOffsetForces(ClearOffsetForcesCommand* command, IModel* model) {
	model->getForceController()->zeroOffsetForces();
	model->getForceController()->processAppliedForces();
	return model->getSafetyController()->checkSafety(States::NoStateTransition);
}

States::Type ActiveEngineeringState::applyAberrationByBendingModes(ApplyAberrationByBendingModesCommand* command, IModel* model) {
	model->getForceController()->applyAberrationByBendingModes(command->getData()->Coefficients);
	model->getForceController()->processAppliedForces();
	return model->getSafetyController()->checkSafety(States::NoStateTransition);
}

States::Type ActiveEngineeringState::applyAberrationByForces(ApplyAberrationByForcesCommand* command, IModel* model) {
	model->getForceController()->applyAberrationByForces(command->getData()->ZForces);
	model->getForceController()->processAppliedForces();
	return model->getSafetyController()->checkSafety(States::NoStateTransition);
}

States::Type ActiveEngineeringState::clearAberration(ClearAberrationCommand* command, IModel* model) {
	model->getForceController()->zeroAberration();
	model->getForceController()->processAppliedForces();
	return model->getSafetyController()->checkSafety(States::NoStateTransition);
}

States::Type ActiveEngineeringState::applyAOSCorrectionByBendingModes(ApplyAOSCorrectionByBendingModesCommand* command, IModel* model) {
	model->getForceController()->applyAOSCorrectionByBendingModes(command->getData()->Coefficients);
	model->getForceController()->processAppliedForces();
	return model->getSafetyController()->checkSafety(States::NoStateTransition);
}

States::Type ActiveEngineeringState::applyAOSCorrectionByForces(ApplyAOSCorrectionByForcesCommand* command, IModel* model) {
	model->getForceController()->applyAOSCorrectionByForces(command->getData()->ZForces);
	model->getForceController()->processAppliedForces();
	return model->getSafetyController()->checkSafety(States::NoStateTransition);
}

States::Type ActiveEngineeringState::clearAOSCorrection(ClearAOSCorrectionCommand* command, IModel* model) {
	model->getForceController()->zeroAOSCorrection();
	model->getForceController()->processAppliedForces();
	return model->getSafetyController()->checkSafety(States::NoStateTransition);
}

States::Type ActiveEngineeringState::stopHardpointMotion(StopHardpointMotionCommand* command, IModel* model) {
	model->getPositionController()->stopMotion();
	return model->getSafetyController()->checkSafety(States::NoStateTransition);
}

States::Type ActiveEngineeringState::moveHardpointActuators(MoveHardpointActuatorsCommand* command, IModel* model) {
	model->getPositionController()->move(command->getData()->Steps);
	return model->getSafetyController()->checkSafety(States::NoStateTransition);
}

States::Type ActiveEngineeringState::translateM1M3(TranslateM1M3Command* command, IModel* model) {
	model->getPositionController()->translate(command->getData()->XTranslation, command->getData()->YTranslation, command->getData()->ZTranslation,
			command->getData()->XRotation, command->getData()->YRotation, command->getData()->ZRotation);
	return model->getSafetyController()->checkSafety(States::NoStateTransition);
}

States::Type ActiveEngineeringState::positionM1M3(PositionM1M3Command* command, IModel* model) {
	model->getPositionController()->moveToAbsolute(command->getData()->XPosition, command->getData()->YPosition, command->getData()->ZPosition,
			command->getData()->XRotation, command->getData()->YRotation, command->getData()->ZRotation);
	return model->getSafetyController()->checkSafety(States::NoStateTransition);
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
