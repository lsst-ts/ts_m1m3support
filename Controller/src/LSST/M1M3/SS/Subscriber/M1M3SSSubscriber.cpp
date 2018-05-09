/*
 * Subscriber.cpp
 *
 *  Created on: Sep 27, 2017
 *      Author: ccontaxis
 */

#include <M1M3SSSubscriber.h>
#include <SAL_m1m3.h>
#include <SAL_MTMount.h>
#include <CommandFactory.h>
#include <Log.h>

namespace LSST {
namespace M1M3 {
namespace SS {

M1M3SSSubscriber::M1M3SSSubscriber(SAL_m1m3* m1m3SAL, SAL_MTMount* mtMountSAL, CommandFactory* commandFactory) {
	Log.Debug("M1M3SSSubscriber: M1M3SSSubscriber()");
	this->m1m3SAL = m1m3SAL;
	this->mtMountSAL = mtMountSAL;
	this->commandFactory = commandFactory;
	this->m1m3SAL->salProcessor((char*)"m1m3_command_Start");
	this->m1m3SAL->salProcessor((char*)"m1m3_command_Enable");
	this->m1m3SAL->salProcessor((char*)"m1m3_command_Disable");
	this->m1m3SAL->salProcessor((char*)"m1m3_command_Standby");
	this->m1m3SAL->salProcessor((char*)"m1m3_command_Shutdown");
	this->m1m3SAL->salProcessor((char*)"m1m3_command_TurnAirOn");
	this->m1m3SAL->salProcessor((char*)"m1m3_command_TurnAirOff");
	this->m1m3SAL->salProcessor((char*)"m1m3_command_ApplyOffsetForces");
	this->m1m3SAL->salProcessor((char*)"m1m3_command_ClearOffsetForces");
	this->m1m3SAL->salProcessor((char*)"m1m3_command_RaiseM1M3");
	this->m1m3SAL->salProcessor((char*)"m1m3_command_LowerM1M3");
	this->m1m3SAL->salProcessor((char*)"m1m3_command_ApplyAberrationForcesByBendingModes");
	this->m1m3SAL->salProcessor((char*)"m1m3_command_ApplyAberrationForces");
	this->m1m3SAL->salProcessor((char*)"m1m3_command_ClearAberrationForces");
	this->m1m3SAL->salProcessor((char*)"m1m3_command_ApplyActiveOpticForcesByBendingModes");
	this->m1m3SAL->salProcessor((char*)"m1m3_command_ApplyActiveOpticForces");
	this->m1m3SAL->salProcessor((char*)"m1m3_command_ClearActievOpticForces");
	this->m1m3SAL->salProcessor((char*)"m1m3_command_EnterEngineering");
	this->m1m3SAL->salProcessor((char*)"m1m3_command_ExitEngineering");
	this->m1m3SAL->salProcessor((char*)"m1m3_command_TestAir");
	this->m1m3SAL->salProcessor((char*)"m1m3_command_TestHardpoint");
	this->m1m3SAL->salProcessor((char*)"m1m3_command_TestForceActuator");
	this->m1m3SAL->salProcessor((char*)"m1m3_command_MoveHardpointActuators");
	this->m1m3SAL->salProcessor((char*)"m1m3_command_EnableHardpointChase");
	this->m1m3SAL->salProcessor((char*)"m1m3_command_DisableHardpointChase");
	this->m1m3SAL->salProcessor((char*)"m1m3_command_AbortRaiseM1M3");
	this->m1m3SAL->salProcessor((char*)"m1m3_command_TranslateM1M3");
	this->m1m3SAL->salProcessor((char*)"m1m3_command_StopHardpointMotion");
	this->m1m3SAL->salProcessor((char*)"m1m3_command_PositionM1M3");
	this->m1m3SAL->salProcessor((char*)"m1m3_command_TurnLightsOn");
	this->m1m3SAL->salProcessor((char*)"m1m3_command_TurnLightsOff");
	this->m1m3SAL->salProcessor((char*)"m1m3_command_TurnPowerOn");
	this->m1m3SAL->salProcessor((char*)"m1m3_command_TurnPowerOff");
	this->m1m3SAL->salProcessor((char*)"m1m3_command_EnableHardpointCorrections");
	this->m1m3SAL->salProcessor((char*)"m1m3_command_DisableHardpointCorrections");
	this->m1m3SAL->salProcessor((char*)"m1m3_command_RunMirrorForceProfile");
	this->m1m3SAL->salProcessor((char*)"m1m3_command_AbortProfile");
	this->m1m3SAL->salProcessor((char*)"m1m3_command_ApplyOffsetForcesByMirrorForce");
	this->m1m3SAL->salProcessor((char*)"m1m3_command_UpdatePID");
	this->m1m3SAL->salProcessor((char*)"m1m3_command_ResetPID");
	this->m1m3SAL->salProcessor((char*)"m1m3_command_ProgramILC");
	this->mtMountSAL->salTelemetrySub((char*)"MTMount_Az");
	this->mtMountSAL->salTelemetrySub((char*)"MTMount_Alt");
}

Command* M1M3SSSubscriber::tryAcceptCommandStart() {
	int32_t commandID = this->m1m3SAL->acceptCommand_Start(&this->startData);
	if (commandID > 0) {
		return this->commandFactory->create(Commands::StartCommand, &this->startData, commandID);
	}
	return 0;
}

Command* M1M3SSSubscriber::tryAcceptCommandEnable() {
	int32_t commandID = this->m1m3SAL->acceptCommand_Enable(&this->enableData);
	if (commandID > 0) {
		return this->commandFactory->create(Commands::EnableCommand, &this->enableData, commandID);
	}
	return 0;
}

Command* M1M3SSSubscriber::tryAcceptCommandDisable() {
	int32_t commandID = this->m1m3SAL->acceptCommand_Disable(&this->disableData);
	if (commandID > 0) {
		return this->commandFactory->create(Commands::DisableCommand, &this->disableData, commandID);
	}
	return 0;
}

Command* M1M3SSSubscriber::tryAcceptCommandStandby() {
	int32_t commandID = this->m1m3SAL->acceptCommand_Standby(&this->standbyData);
	if (commandID > 0) {
		return this->commandFactory->create(Commands::StandbyCommand, &this->standbyData, commandID);
	}
	return 0;
}

Command* M1M3SSSubscriber::tryAcceptCommandShutdown() {
	int32_t commandID = this->m1m3SAL->acceptCommand_Shutdown(&this->shutdownData);
	if (commandID > 0) {
		return this->commandFactory->create(Commands::ShutdownCommand, &this->shutdownData, commandID);
	}
	return 0;
}

Command* M1M3SSSubscriber::tryAcceptCommandTurnAirOn() {
	int32_t commandID = this->m1m3SAL->acceptCommand_TurnAirOn(&this->turnAirOnData);
	if (commandID > 0) {
		return this->commandFactory->create(Commands::TurnAirOnCommand, &this->turnAirOnData, commandID);
	}
	return 0;
}

Command* M1M3SSSubscriber::tryAcceptCommandTurnAirOff() {
	int32_t commandID = this->m1m3SAL->acceptCommand_TurnAirOff(&this->turnAirOffData);
	if (commandID > 0) {
		return this->commandFactory->create(Commands::TurnAirOffCommand, &this->turnAirOffData, commandID);
	}
	return 0;
}

Command* M1M3SSSubscriber::tryAcceptCommandApplyOffsetForces() {
	int32_t commandID = this->m1m3SAL->acceptCommand_ApplyOffsetForces(&this->applyOffsetForcesData);
	if (commandID > 0) {
		return this->commandFactory->create(Commands::ApplyOffsetForcesCommand, &this->applyOffsetForcesData, commandID);
	}
	return 0;
}

Command* M1M3SSSubscriber::tryAcceptCommandClearOffsetForces() {
	int32_t commandID = this->m1m3SAL->acceptCommand_ClearOffsetForces(&this->clearOffsetForcesData);
	if (commandID > 0) {
		return this->commandFactory->create(Commands::ClearOffsetForcesCommand, &this->clearOffsetForcesData, commandID);
	}
	return 0;
}

Command* M1M3SSSubscriber::tryAcceptCommandRaiseM1M3() {
	int32_t commandID = this->m1m3SAL->acceptCommand_RaiseM1M3(&this->raiseM1M3Data);
	if (commandID > 0) {
		return this->commandFactory->create(Commands::RaiseM1M3Command, &this->raiseM1M3Data, commandID);
	}
	return 0;
}

Command* M1M3SSSubscriber::tryAcceptCommandLowerM1M3() {
	int32_t commandID = this->m1m3SAL->acceptCommand_LowerM1M3(&this->lowerM1M3Data);
	if (commandID > 0) {
		return this->commandFactory->create(Commands::LowerM1M3Command, &this->lowerM1M3Data, commandID);
	}
	return 0;
}

Command* M1M3SSSubscriber::tryAcceptCommandApplyAberrationForcesByBendingModes() {
	int32_t commandID = this->m1m3SAL->acceptCommand_ApplyAberrationForcesByBendingModes(&this->applyAberrationForcesByBendingModesData);
	if (commandID > 0) {
		return this->commandFactory->create(Commands::ApplyAberrationForcesByBendingModesCommand, &this->applyAberrationForcesByBendingModesData, commandID);
	}
	return 0;
}

Command* M1M3SSSubscriber::tryAcceptCommandApplyAberrationForces() {
	int32_t commandID = this->m1m3SAL->acceptCommand_ApplyAberrationForces(&this->applyAberrationForcesData);
	if (commandID > 0) {
		return this->commandFactory->create(Commands::ApplyAberrationForcesCommand, &this->applyAberrationForcesData, commandID);
	}
	return 0;
}

Command* M1M3SSSubscriber::tryAcceptCommandClearAberrationForces() {
	int32_t commandID = this->m1m3SAL->acceptCommand_ClearAberrationForces(&this->clearAberrationForcesData);
	if (commandID > 0) {
		return this->commandFactory->create(Commands::ClearAberrationForcesCommand, &this->clearAberrationForcesData, commandID);
	}
	return 0;
}

Command* M1M3SSSubscriber::tryAcceptCommandApplyActiveOpticForcesByBendingModes() {
	int32_t commandID = this->m1m3SAL->acceptCommand_ApplyActiveOpticForcesByBendingModes(&this->applyActiveOpticForcesByBendingModesData);
	if (commandID > 0) {
		return this->commandFactory->create(Commands::ApplyActiveOpticForcesByBendingModesCommand, &this->applyActiveOpticForcesByBendingModesData, commandID);
	}
	return 0;
}

Command* M1M3SSSubscriber::tryAcceptCommandApplyActiveOpticForces() {
	int32_t commandID = this->m1m3SAL->acceptCommand_ApplyActiveOpticForces(&this->applyActiveOpticForcesData);
	if (commandID > 0) {
		return this->commandFactory->create(Commands::ApplyActiveOpticForcesCommand, &this->applyActiveOpticForcesData, commandID);
	}
	return 0;
}

Command* M1M3SSSubscriber::tryAcceptCommandClearActiveOpticForces() {
	int32_t commandID = this->m1m3SAL->acceptCommand_ClearActiveOpticForces(&this->clearActiveOpticForcesData);
	if (commandID > 0) {
		return this->commandFactory->create(Commands::ClearActiveOpticForcesCommand, &this->clearActiveOpticForcesData, commandID);
	}
	return 0;
}

Command* M1M3SSSubscriber::tryAcceptCommandEnterEngineering() {
	int32_t commandID = this->m1m3SAL->acceptCommand_EnterEngineering(&this->enterEngineeringData);
	if (commandID > 0) {
		return this->commandFactory->create(Commands::EnterEngineeringCommand, &this->enterEngineeringData, commandID);
	}
	return 0;
}

Command* M1M3SSSubscriber::tryAcceptCommandExitEngineering() {
	int32_t commandID = this->m1m3SAL->acceptCommand_ExitEngineering(&this->exitEngineeringData);
	if (commandID > 0) {
		return this->commandFactory->create(Commands::ExitEngineeringCommand, &this->exitEngineeringData, commandID);
	}
	return 0;
}

Command* M1M3SSSubscriber::tryAcceptCommandTestAir() {
	int32_t commandID = this->m1m3SAL->acceptCommand_TestAir(&this->testAirData);
	if (commandID > 0) {
		return this->commandFactory->create(Commands::TestAirCommand, &this->testAirData, commandID);
	}
	return 0;
}

Command* M1M3SSSubscriber::tryAcceptCommandTestHardpoint() {
	int32_t commandID = this->m1m3SAL->acceptCommand_TestHardpoint(&this->testHardpointData);
	if (commandID > 0) {
		return this->commandFactory->create(Commands::TestHardpointCommand, &this->testHardpointData, commandID);
	}
	return 0;
}

Command* M1M3SSSubscriber::tryAcceptCommandTestForceActuator() {
	int32_t commandID = this->m1m3SAL->acceptCommand_TestForceActuator(&this->testForceActuatorData);
	if (commandID > 0) {
		return this->commandFactory->create(Commands::TestForceActuatorCommand, &this->testForceActuatorData, commandID);
	}
	return 0;
}

Command* M1M3SSSubscriber::tryAcceptCommandMoveHardpointActuators() {
	int32_t commandID = this->m1m3SAL->acceptCommand_MoveHardpointActuators(&this->moveHardpointActuatorsData);
	if (commandID > 0) {
		return this->commandFactory->create(Commands::MoveHardpointActuatorsCommand, &this->moveHardpointActuatorsData, commandID);
	}
	return 0;
}

Command* M1M3SSSubscriber::tryAcceptCommandEnableHardpointChase() {
	int32_t commandID = this->m1m3SAL->acceptCommand_EnableHardpointChase(&this->enableHardpointChaseData);
	if (commandID > 0) {
		return this->commandFactory->create(Commands::EnableHardpointChaseCommand, &this->enableHardpointChaseData, commandID);
	}
	return 0;
}

Command* M1M3SSSubscriber::tryAcceptCommandDisableHardpointChase() {
	int32_t commandID = this->m1m3SAL->acceptCommand_DisableHardpointChase(&this->disableHardpointChaseData);
	if (commandID > 0) {
		return this->commandFactory->create(Commands::DisableHardpointChaseCommand, &this->disableHardpointChaseData, commandID);
	}
	return 0;
}

Command* M1M3SSSubscriber::tryAcceptCommandAbortRaiseM1M3() {
	int32_t commandID = this->m1m3SAL->acceptCommand_AbortRaiseM1M3(&this->abortRaiseM1M3Data);
	if (commandID > 0) {
		return this->commandFactory->create(Commands::AbortRaiseM1M3Command, &this->abortRaiseM1M3Data, commandID);
	}
	return 0;
}

Command* M1M3SSSubscriber::tryAcceptCommandTranslateM1M3() {
	int32_t commandID = this->m1m3SAL->acceptCommand_TranslateM1M3(&this->translateM1M3Data);
	if (commandID > 0) {
		return this->commandFactory->create(Commands::TranslateM1M3Command, &this->translateM1M3Data, commandID);
	}
	return 0;
}

Command* M1M3SSSubscriber::tryAcceptCommandStopHardpointMotion() {
	int32_t commandID = this->m1m3SAL->acceptCommand_StopHardpointMotion(&this->stopHardpointMotionData);
	if (commandID > 0) {
		return this->commandFactory->create(Commands::StopHardpointMotionCommand, &this->stopHardpointMotionData, commandID);
	}
	return 0;
}

Command* M1M3SSSubscriber::tryAcceptCommandPositionM1M3() {
	int32_t commandID = this->m1m3SAL->acceptCommand_PositionM1M3(&this->positionM1M3Data);
	if (commandID > 0) {
		return this->commandFactory->create(Commands::PositionM1M3Command, &this->positionM1M3Data, commandID);
	}
	return 0;
}

Command* M1M3SSSubscriber::tryAcceptCommandTurnLightsOn() {
	int32_t commandID = this->m1m3SAL->acceptCommand_TurnLightsOn(&this->turnLightsOnData);
	if (commandID > 0) {
		return this->commandFactory->create(Commands::TurnLightsOnCommand, &this->turnLightsOnData, commandID);
	}
	return 0;
}

Command* M1M3SSSubscriber::tryAcceptCommandTurnLightsOff() {
	int32_t commandID = this->m1m3SAL->acceptCommand_TurnLightsOff(&this->turnLightsOffData);
	if (commandID > 0) {
		return this->commandFactory->create(Commands::TurnLightsOffCommand, &this->turnLightsOffData, commandID);
	}
	return 0;
}

Command* M1M3SSSubscriber::tryAcceptCommandTurnPowerOn() {
	int32_t commandID = this->m1m3SAL->acceptCommand_TurnPowerOn(&this->turnPowerOnData);
	if (commandID > 0) {
		return this->commandFactory->create(Commands::TurnPowerOnCommand, &this->turnPowerOnData, commandID);
	}
	return 0;
}

Command* M1M3SSSubscriber::tryAcceptCommandTurnPowerOff() {
	int32_t commandID = this->m1m3SAL->acceptCommand_TurnPowerOff(&this->turnPowerOffData);
	if (commandID > 0) {
		return this->commandFactory->create(Commands::TurnPowerOffCommand, &this->turnPowerOffData, commandID);
	}
	return 0;
}

Command* M1M3SSSubscriber::tryAcceptCommandEnableHardpointCorrections() {
	int32_t commandID = this->m1m3SAL->acceptCommand_EnableHardpointCorrections(&this->enableHardpointCorrectionsData);
	if (commandID > 0) {
		return this->commandFactory->create(Commands::EnableHardpointCorrectionsCommand, &this->enableHardpointCorrectionsData, commandID);
	}
	return 0;
}

Command* M1M3SSSubscriber::tryAcceptCommandDisableHardpointCorrections() {
	int32_t commandID = this->m1m3SAL->acceptCommand_DisableHardpointCorrections(&this->disableHardpointCorrectionsData);
	if (commandID > 0) {
		return this->commandFactory->create(Commands::DisableHardpointCorrectionsCommand, &this->disableHardpointCorrectionsData, commandID);
	}
	return 0;
}

Command* M1M3SSSubscriber::tryAcceptCommandRunMirrorForceProfile() {
	int32_t commandID = this->m1m3SAL->acceptCommand_RunMirrorForceProfile(&this->runMirrorForceProfileData);
	if (commandID > 0) {
		return this->commandFactory->create(Commands::RunMirrorForceProfileCommand, &this->runMirrorForceProfileData, commandID);
	}
	return 0;
}

Command* M1M3SSSubscriber::tryAcceptCommandAbortProfile() {
	int32_t commandID = this->m1m3SAL->acceptCommand_AbortProfile(&this->abortProfileData);
	if (commandID > 0) {
		return this->commandFactory->create(Commands::AbortProfileCommand, &this->abortProfileData, commandID);
	}
	return 0;
}

Command* M1M3SSSubscriber::tryAcceptCommandApplyOffsetForcesByMirrorForce() {
	int32_t commandID = this->m1m3SAL->acceptCommand_ApplyOffsetForcesByMirrorForce(&this->applyOffsetForcesByMirrorForceData);
	if (commandID > 0) {
		return this->commandFactory->create(Commands::ApplyOffsetForcesByMirrorForceCommand, &this->applyOffsetForcesByMirrorForceData, commandID);
	}
	return 0;
}

Command* M1M3SSSubscriber::tryAcceptCommandUpdatePID() {
	int32_t commandID = this->m1m3SAL->acceptCommand_UpdatePID(&this->updatePIDData);
	if (commandID > 0) {
		return this->commandFactory->create(Commands::UpdatePIDCommand, &this->updatePIDData, commandID);
	}
	return 0;
}

Command* M1M3SSSubscriber::tryAcceptCommandResetPID() {
	int32_t commandID = this->m1m3SAL->acceptCommand_ResetPID(&this->resetPIDData);
	if (commandID > 0) {
		return this->commandFactory->create(Commands::ResetPIDCommand, &this->resetPIDData, commandID);
	}
	return 0;
}

Command* M1M3SSSubscriber::tryAcceptCommandProgramILC() {
	int32_t commandID = this->m1m3SAL->acceptCommand_ProgramILC(&this->programILCData);
	if (commandID > 0) {
		return this->commandFactory->create(Commands::ProgramILCCommand, &this->programILCData, commandID);
	}
	return 0;
}

Command* M1M3SSSubscriber::tryGetSampleTMAAzimuth() {
	int32_t result = this->mtMountSAL->getSample_Az(&this->tmaAzimuth);
	if (result == 0) {
		return this->commandFactory->create(Commands::TMAAzimuthSampleCommand, &this->tmaAzimuth, 0);
	}
	return 0;
}

Command* M1M3SSSubscriber::tryGetSampleTMAElevation() {
	int32_t result = this->mtMountSAL->getSample_Alt(&this->tmaElevation);
	if (result == 0) {
		return this->commandFactory->create(Commands::TMAElevationSampleCommand, &this->tmaElevation, 0);
	}
	return 0;
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
