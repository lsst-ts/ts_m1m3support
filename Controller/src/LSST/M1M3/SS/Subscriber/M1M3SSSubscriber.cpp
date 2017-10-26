/*
 * Subscriber.cpp
 *
 *  Created on: Sep 27, 2017
 *      Author: ccontaxis
 */

#include <M1M3SSSubscriber.h>
#include <SAL_m1m3.h>
#include <ICommandFactory.h>

namespace LSST {
namespace M1M3 {
namespace SS {

M1M3SSSubscriber::M1M3SSSubscriber(SAL_m1m3* m1m3SAL, ICommandFactory* commandFactory) {
	this->m1m3SAL = m1m3SAL;
	this->commandFactory = commandFactory;
	this->m1m3SAL->salProcessor("m1m3_command_Start");
	this->m1m3SAL->salProcessor("m1m3_command_Enable");
	this->m1m3SAL->salProcessor("m1m3_command_Disable");
	this->m1m3SAL->salProcessor("m1m3_command_Standby");
	this->m1m3SAL->salProcessor("m1m3_command_Shutdown");
	this->m1m3SAL->salProcessor("m1m3_command_TurnAirOn");
	this->m1m3SAL->salProcessor("m1m3_command_TurnAirOff");
	this->m1m3SAL->salProcessor("m1m3_command_ApplyOffsetForces");
	this->m1m3SAL->salProcessor("m1m3_command_ClearOffsetForces");
	this->m1m3SAL->salProcessor("m1m3_command_RaiseM1M3");
	this->m1m3SAL->salProcessor("m1m3_command_LowerM1M3");
}

ICommand* M1M3SSSubscriber::tryAcceptCommandStart() {
	int32_t commandID = this->m1m3SAL->acceptCommand_Start(&this->startData);
	if (commandID > 0) {
		return this->commandFactory->create(Commands::StartCommand, &this->startData, commandID);
	}
	return 0;
}

ICommand* M1M3SSSubscriber::tryAcceptCommandEnable() {
	int32_t commandID = this->m1m3SAL->acceptCommand_Enable(&this->enableData);
	if (commandID > 0) {
		return this->commandFactory->create(Commands::EnableCommand, &this->enableData, commandID);
	}
	return 0;
}

ICommand* M1M3SSSubscriber::tryAcceptCommandDisable() {
	int32_t commandID = this->m1m3SAL->acceptCommand_Disable(&this->disableData);
	if (commandID > 0) {
		return this->commandFactory->create(Commands::DisableCommand, &this->disableData, commandID);
	}
	return 0;
}

ICommand* M1M3SSSubscriber::tryAcceptCommandStandby() {
	int32_t commandID = this->m1m3SAL->acceptCommand_Standby(&this->standbyData);
	if (commandID > 0) {
		return this->commandFactory->create(Commands::StandbyCommand, &this->standbyData, commandID);
	}
	return 0;
}

ICommand* M1M3SSSubscriber::tryAcceptCommandShutdown() {
	int32_t commandID = this->m1m3SAL->acceptCommand_Shutdown(&this->shutdownData);
	if (commandID > 0) {
		return this->commandFactory->create(Commands::ShutdownCommand, &this->shutdownData, commandID);
	}
	return 0;
}

ICommand* M1M3SSSubscriber::tryAcceptCommandTurnAirOn() {
	int32_t commandID = this->m1m3SAL->acceptCommand_TurnAirOn(&this->turnAirOnData);
	if (commandID > 0) {
		return this->commandFactory->create(Commands::TurnAirOnCommand, &this->turnAirOnData, commandID);
	}
	return 0;
}

ICommand* M1M3SSSubscriber::tryAcceptCommandTurnAirOff() {
	int32_t commandID = this->m1m3SAL->acceptCommand_TurnAirOff(&this->turnAirOffData);
	if (commandID > 0) {
		return this->commandFactory->create(Commands::TurnAirOffCommand, &this->turnAirOffData, commandID);
	}
	return 0;
}

ICommand* M1M3SSSubscriber::tryAcceptCommandApplyOffsetForces() {
	int32_t commandID = this->m1m3SAL->acceptCommand_ApplyOffsetForces(&this->applyOffsetForcesData);
	if (commandID > 0) {
		return this->commandFactory->create(Commands::ApplyOffsetForcesCommand, &this->applyOffsetForcesData, commandID);
	}
	return 0;
}

ICommand* M1M3SSSubscriber::tryAcceptCommandClearOffsetForces() {
	int32_t commandID = this->m1m3SAL->acceptCommand_ClearOffsetForces(&this->clearOffsetForcesData);
	if (commandID > 0) {
		return this->commandFactory->create(Commands::ClearOffsetForcesCommand, &this->clearOffsetForcesData, commandID);
	}
	return 0;
}

ICommand* M1M3SSSubscriber::tryAcceptCommandRaiseM1M3() {
	int32_t commandID = this->m1m3SAL->acceptCommand_RaiseM1M3(&this->raiseM1M3Data);
	if (commandID > 0) {
		return this->commandFactory->create(Commands::RaiseM1M3Command, &this->raiseM1M3Data, commandID);
	}
	return 0;
}

ICommand* M1M3SSSubscriber::tryAcceptCommandLowerM1M3() {
	int32_t commandID = this->m1m3SAL->acceptCommand_LowerM1M3(&this->lowerM1M3Data);
	if (commandID > 0) {
		return this->commandFactory->create(Commands::LowerM1M3Command, &this->lowerM1M3Data, commandID);
	}
	return 0;
}

ICommand* M1M3SSSubscriber::tryAcceptCommandApplyAberrationByBendingModes() {
	int32_t commandID = this->m1m3SAL->acceptCommand_ApplyAberrationByBendingModes(&this->applyAberrationByBendingModesData);
	if (commandID > 0) {
		return this->commandFactory->create(Commands::ApplyAberrationByBendingModesCommand, &this->applyAberrationByBendingModesData, commandID);
	}
	return 0;
}

ICommand* M1M3SSSubscriber::tryAcceptCommandApplyAberrationByForces() {
	int32_t commandID = this->m1m3SAL->acceptCommand_ApplyAberrationByForces(&this->applyAberrationByForcesData);
	if (commandID > 0) {
		return this->commandFactory->create(Commands::ApplyAberrationByForcesCommand, &this->applyAberrationByForcesData, commandID);
	}
	return 0;
}

ICommand* M1M3SSSubscriber::tryAcceptCommandClearAberration() {
	int32_t commandID = this->m1m3SAL->acceptCommand_ClearAberration(&this->clearAberrationData);
	if (commandID > 0) {
		return this->commandFactory->create(Commands::ClearAberrationCommand, &this->clearAberrationData, commandID);
	}
	return 0;
}

ICommand* M1M3SSSubscriber::tryAcceptCommandApplyAOSCorrectionByBendingModes() {
	int32_t commandID = this->m1m3SAL->acceptCommand_ApplyAOSCorrectionByBendingModes(&this->applyAOSCorrectionByBendingModesData);
	if (commandID > 0) {
		return this->commandFactory->create(Commands::ApplyAOSCorrectionByBendingModesCommand, &this->applyAOSCorrectionByBendingModesData, commandID);
	}
	return 0;
}

ICommand* M1M3SSSubscriber::tryAcceptCommandApplyAOSCorrectionByForces() {
	int32_t commandID = this->m1m3SAL->acceptCommand_ApplyAOSCorrectionByForces(&this->applyAOSCorrectionByForcesData);
	if (commandID > 0) {
		return this->commandFactory->create(Commands::ApplyAOSCorrectionByForcesCommand, &this->applyAOSCorrectionByForcesData, commandID);
	}
	return 0;
}

ICommand* M1M3SSSubscriber::tryAcceptCommandClearAOSCorrection() {
	int32_t commandID = this->m1m3SAL->acceptCommand_ClearAOSCorrection(&this->clearAOSCorrectionData);
	if (commandID > 0) {
		return this->commandFactory->create(Commands::ClearAOSCorrectionCommand, &this->clearAOSCorrectionData, commandID);
	}
	return 0;
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
