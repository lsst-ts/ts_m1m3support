/*
 * ActiveEngineeringState.cpp
 *
 *  Created on: Oct 25, 2017
 *      Author: ccontaxis
 */

#include <ActiveEngineeringState.h>
#include <Model.h>
#include <ILC.h>
#include <Displacement.h>
#include <Inclinometer.h>
#include <AirController.h>
#include <ForceController.h>
#include <ApplyOffsetForcesCommand.h>
#include <ApplyAberrationByBendingModesCommand.h>
#include <ApplyAberrationByForcesCommand.h>
#include <ClearAberrationCommand.h>
#include <ApplyAOSCorrectionByBendingModesCommand.h>
#include <ApplyAOSCorrectionByForcesCommand.h>
#include <ClearAOSCorrectionCommand.h>
#include <SafetyController.h>
#include <InterlockController.h>
#include <TMAAzimuthSampleCommand.h>
#include <TMAElevationSampleCommand.h>
#include <MoveHardpointActuatorsCommand.h>
#include <TranslateM1M3Command.h>
#include <PositionController.h>
#include <PositionM1M3Command.h>
#include <M1M3SSPublisher.h>
#include <PowerController.h>
#include <TurnPowerOnCommand.h>
#include <TurnPowerOffCommand.h>
#include <AutomaticOperationsController.h>
#include <unistd.h>

#include <iostream>
using namespace std;

namespace LSST {
namespace M1M3 {
namespace SS {

ActiveEngineeringState::ActiveEngineeringState(M1M3SSPublisher* publisher) : EngineeringState(publisher, "ActiveEngineeringState") { }

States::Type ActiveEngineeringState::update(UpdateCommand* command, Model* model) {
	EnabledState::update(command, model);
	return model->getSafetyController()->checkSafety(States::NoStateTransition);
}

States::Type ActiveEngineeringState::lowerM1M3(LowerM1M3Command* command, Model* model) {
	States::Type newState = States::LoweringEngineeringState;
	model->getAutomaticOperationsController()->startLowerOperation();
	return model->getSafetyController()->checkSafety(newState);
}

States::Type ActiveEngineeringState::exitEngineering(ExitEngineeringCommand* command, Model* model) {
	States::Type newState = States::ActiveState;
	return model->getSafetyController()->checkSafety(newState);
}

States::Type ActiveEngineeringState::applyOffsetForces(ApplyOffsetForcesCommand* command, Model* model) {
	model->getForceController()->applyOffsetForces(command->getData()->XForces, command->getData()->YForces, command->getData()->ZForces);
	model->getForceController()->processAppliedForces();
	return model->getSafetyController()->checkSafety(States::NoStateTransition);
}

States::Type ActiveEngineeringState::clearOffsetForces(ClearOffsetForcesCommand* command, Model* model) {
	model->getForceController()->zeroOffsetForces();
	model->getForceController()->processAppliedForces();
	return model->getSafetyController()->checkSafety(States::NoStateTransition);
}

States::Type ActiveEngineeringState::applyAberrationByBendingModes(ApplyAberrationByBendingModesCommand* command, Model* model) {
	model->getForceController()->applyAberrationByBendingModes(command->getData()->Coefficients);
	model->getForceController()->processAppliedForces();
	return model->getSafetyController()->checkSafety(States::NoStateTransition);
}

States::Type ActiveEngineeringState::applyAberrationByForces(ApplyAberrationByForcesCommand* command, Model* model) {
	model->getForceController()->applyAberrationByForces(command->getData()->ZForces);
	model->getForceController()->processAppliedForces();
	return model->getSafetyController()->checkSafety(States::NoStateTransition);
}

States::Type ActiveEngineeringState::clearAberration(ClearAberrationCommand* command, Model* model) {
	model->getForceController()->zeroAberration();
	model->getForceController()->processAppliedForces();
	return model->getSafetyController()->checkSafety(States::NoStateTransition);
}

States::Type ActiveEngineeringState::applyAOSCorrectionByBendingModes(ApplyAOSCorrectionByBendingModesCommand* command, Model* model) {
	model->getForceController()->applyAOSCorrectionByBendingModes(command->getData()->Coefficients);
	model->getForceController()->processAppliedForces();
	return model->getSafetyController()->checkSafety(States::NoStateTransition);
}

States::Type ActiveEngineeringState::applyAOSCorrectionByForces(ApplyAOSCorrectionByForcesCommand* command, Model* model) {
	model->getForceController()->applyAOSCorrectionByForces(command->getData()->ZForces);
	model->getForceController()->processAppliedForces();
	return model->getSafetyController()->checkSafety(States::NoStateTransition);
}

States::Type ActiveEngineeringState::clearAOSCorrection(ClearAOSCorrectionCommand* command, Model* model) {
	model->getForceController()->zeroAOSCorrection();
	model->getForceController()->processAppliedForces();
	return model->getSafetyController()->checkSafety(States::NoStateTransition);
}

States::Type ActiveEngineeringState::stopHardpointMotion(StopHardpointMotionCommand* command, Model* model) {
	model->getPositionController()->stopMotion();
	return model->getSafetyController()->checkSafety(States::NoStateTransition);
}

States::Type ActiveEngineeringState::moveHardpointActuators(MoveHardpointActuatorsCommand* command, Model* model) {
	if (!model->getPositionController()->move(command->getData()->Steps)) {
		model->getPublisher()->logCommandRejectionWarning("MoveHardpointActuators", "At least one hardpoint actuator commanded to move is already MOVING or CHASING.");
	}
	return model->getSafetyController()->checkSafety(States::NoStateTransition);
}

States::Type ActiveEngineeringState::translateM1M3(TranslateM1M3Command* command, Model* model) {
	if (!model->getPositionController()->translate(command->getData()->XTranslation, command->getData()->YTranslation, command->getData()->ZTranslation,
			command->getData()->XRotation, command->getData()->YRotation, command->getData()->ZRotation)) {
		model->getPublisher()->logCommandRejectionWarning("TranslateM1M3", "At least one hardpoint actuator commanded to move is already MOVING or CHASING.");
	}
	return model->getSafetyController()->checkSafety(States::NoStateTransition);
}

States::Type ActiveEngineeringState::positionM1M3(PositionM1M3Command* command, Model* model) {
	if (!model->getPositionController()->moveToAbsolute(command->getData()->XPosition, command->getData()->YPosition, command->getData()->ZPosition,
			command->getData()->XRotation, command->getData()->YRotation, command->getData()->ZRotation)) {
		model->getPublisher()->logCommandRejectionWarning("PositionM1M3", "At least one hardpoint actuator commanded to move is already MOVING or CHASING.");
	}
	return model->getSafetyController()->checkSafety(States::NoStateTransition);
}

States::Type ActiveEngineeringState::turnLightsOn(TurnLightsOnCommand* command, Model* model) {
	model->getInterlockController()->setCellLightsOn(true);
	return model->getSafetyController()->checkSafety(States::NoStateTransition);
}

States::Type ActiveEngineeringState::turnLightsOff(TurnLightsOffCommand* command, Model* model) {
	model->getInterlockController()->setCellLightsOn(false);
	return model->getSafetyController()->checkSafety(States::NoStateTransition);
}

States::Type ActiveEngineeringState::turnPowerOn(TurnPowerOnCommand* command, Model* model) {
	if (command->getData()->TurnPowerNetworkAOn) {
		model->getPowerController()->setPowerNetworkA(true);
	}
	if (command->getData()->TurnPowerNetworkBOn) {
		model->getPowerController()->setPowerNetworkB(true);
	}
	if (command->getData()->TurnPowerNetworkCOn) {
		model->getPowerController()->setPowerNetworkC(true);
	}
	if (command->getData()->TurnPowerNetworkDOn) {
		model->getPowerController()->setPowerNetworkD(true);
	}
	if (command->getData()->TurnAuxPowerNetworkAOn) {
		model->getPowerController()->setAuxPowerNetworkA(true);
	}
	if (command->getData()->TurnAuxPowerNetworkBOn) {
		model->getPowerController()->setAuxPowerNetworkB(true);
	}
	if (command->getData()->TurnAuxPowerNetworkCOn) {
		model->getPowerController()->setAuxPowerNetworkC(true);
	}
	if (command->getData()->TurnAuxPowerNetworkDOn) {
		model->getPowerController()->setAuxPowerNetworkD(true);
	}
	return model->getSafetyController()->checkSafety(States::NoStateTransition);
}

States::Type ActiveEngineeringState::turnPowerOff(TurnPowerOffCommand* command, Model* model) {
	if (command->getData()->TurnPowerNetworkAOff) {
		model->getPowerController()->setPowerNetworkA(false);
	}
	if (command->getData()->TurnPowerNetworkBOff) {
		model->getPowerController()->setPowerNetworkB(false);
	}
	if (command->getData()->TurnPowerNetworkCOff) {
		model->getPowerController()->setPowerNetworkC(false);
	}
	if (command->getData()->TurnPowerNetworkDOff) {
		model->getPowerController()->setPowerNetworkD(false);
	}
	if (command->getData()->TurnAuxPowerNetworkAOff) {
		model->getPowerController()->setAuxPowerNetworkA(false);
	}
	if (command->getData()->TurnAuxPowerNetworkBOff) {
		model->getPowerController()->setAuxPowerNetworkB(false);
	}
	if (command->getData()->TurnAuxPowerNetworkCOff) {
		model->getPowerController()->setAuxPowerNetworkC(false);
	}
	if (command->getData()->TurnAuxPowerNetworkDOff) {
		model->getPowerController()->setAuxPowerNetworkD(false);
	}
	return model->getSafetyController()->checkSafety(States::NoStateTransition);
}

States::Type ActiveEngineeringState::enableHardpointCorrections(EnableHardpointCorrectionsCommand* command, Model* model) {
	model->getForceController()->applyHardpointCorrections();
	return model->getSafetyController()->checkSafety(States::NoStateTransition);
}

States::Type ActiveEngineeringState::disableHardpointCorrections(DisableHardpointCorrectionsCommand* command, Model* model) {
	model->getForceController()->zeroHardpointCorrections();
	return model->getSafetyController()->checkSafety(States::NoStateTransition);
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
