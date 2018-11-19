/*
 * Subscriber.cpp
 *
 *  Created on: Sep 27, 2017
 *      Author: ccontaxis
 */

#include <M1M3SSSubscriber.h>
#include <SAL_MTM1M3.h>
#include <SAL_MTMount.h>
#include <CommandFactory.h>
#include <Log.h>

namespace LSST {
namespace M1M3 {
namespace SS {

M1M3SSSubscriber::M1M3SSSubscriber(SAL_MTM1M3* m1m3SAL, SAL_MTMount* mtMountSAL, CommandFactory* commandFactory) {
	Log.Debug("M1M3SSSubscriber: M1M3SSSubscriber()");
	this->m1m3SAL = m1m3SAL;
	this->mtMountSAL = mtMountSAL;
	this->commandFactory = commandFactory;
	this->initCommand("MTM1M3_command_start");
	this->initCommand("MTM1M3_command_enable");
	this->initCommand("MTM1M3_command_disable");
	this->initCommand("MTM1M3_command_standby");
	this->initCommand("MTM1M3_command_shutdown");
	this->initCommand("MTM1M3_command_turnAirOn");
	this->initCommand("MTM1M3_command_turnAirOff");
	this->initCommand("MTM1M3_command_applyOffsetForces");
	this->initCommand("MTM1M3_command_clearOffsetForces");
	this->initCommand("MTM1M3_command_raiseM1M3");
	this->initCommand("MTM1M3_command_lowerM1M3");
	this->initCommand("MTM1M3_command_applyAberrationForcesByBendingModes");
	this->initCommand("MTM1M3_command_applyAberrationForces");
	this->initCommand("MTM1M3_command_clearAberrationForces");
	this->initCommand("MTM1M3_command_applyActiveOpticForcesByBendingModes");
	this->initCommand("MTM1M3_command_applyActiveOpticForces");
	this->initCommand("MTM1M3_command_clearActiveOpticForces");
	this->initCommand("MTM1M3_command_enterEngineering");
	this->initCommand("MTM1M3_command_exitEngineering");
	this->initCommand("MTM1M3_command_testAir");
	this->initCommand("MTM1M3_command_testHardpoint");
	this->initCommand("MTM1M3_command_testForceActuator");
	this->initCommand("MTM1M3_command_moveHardpointActuators");
	this->initCommand("MTM1M3_command_enableHardpointChase");
	this->initCommand("MTM1M3_command_disableHardpointChase");
	this->initCommand("MTM1M3_command_abortRaiseM1M3");
	this->initCommand("MTM1M3_command_translateM1M3");
	this->initCommand("MTM1M3_command_stopHardpointMotion");
	this->initCommand("MTM1M3_command_positionM1M3");
	this->initCommand("MTM1M3_command_turnLightsOn");
	this->initCommand("MTM1M3_command_turnLightsOff");
	this->initCommand("MTM1M3_command_turnPowerOn");
	this->initCommand("MTM1M3_command_turnPowerOff");
	this->initCommand("MTM1M3_command_enableHardpointCorrections");
	this->initCommand("MTM1M3_command_disableHardpointCorrections");
	this->initCommand("MTM1M3_command_applyOffsetForcesByMirrorForce");
	this->initCommand("MTM1M3_command_updatePID");
	this->initCommand("MTM1M3_command_resetPID");
	this->initCommand("MTM1M3_command_programILC");
	this->initCommand("MTM1M3_command_modbusTransmit");
	this->initTelemetry("MTMount_Azimuth");
	this->initTelemetry("MTMount_Elevation");
}

void M1M3SSSubscriber::initCommand(std::string topic) {
	Log.Debug("M1M3SSSubscriber: initCommand(%s)", topic.c_str());
	this->m1m3SAL->salProcessor((char*)topic.c_str());
}

void M1M3SSSubscriber::initTelemetry(std::string topic) {
	Log.Debug("M1M3SSSubscriber: initTelemetry(%s)", topic.c_str());
	this->mtMountSAL->salTelemetrySub((char*)topic.c_str());
}

Command* M1M3SSSubscriber::tryAcceptCommandStart() {
	int32_t commandID = this->m1m3SAL->acceptCommand_start(&this->startData);
	if (commandID > 0) {
		return this->commandFactory->create(Commands::StartCommand, &this->startData, commandID);
	}
	return 0;
}

Command* M1M3SSSubscriber::tryAcceptCommandEnable() {
	int32_t commandID = this->m1m3SAL->acceptCommand_enable(&this->enableData);
	if (commandID > 0) {
		return this->commandFactory->create(Commands::EnableCommand, &this->enableData, commandID);
	}
	return 0;
}

Command* M1M3SSSubscriber::tryAcceptCommandDisable() {
	int32_t commandID = this->m1m3SAL->acceptCommand_disable(&this->disableData);
	if (commandID > 0) {
		return this->commandFactory->create(Commands::DisableCommand, &this->disableData, commandID);
	}
	return 0;
}

Command* M1M3SSSubscriber::tryAcceptCommandStandby() {
	int32_t commandID = this->m1m3SAL->acceptCommand_standby(&this->standbyData);
	if (commandID > 0) {
		return this->commandFactory->create(Commands::StandbyCommand, &this->standbyData, commandID);
	}
	return 0;
}

Command* M1M3SSSubscriber::tryAcceptCommandShutdown() {
	int32_t commandID = this->m1m3SAL->acceptCommand_shutdown(&this->shutdownData);
	if (commandID > 0) {
		return this->commandFactory->create(Commands::ShutdownCommand, &this->shutdownData, commandID);
	}
	return 0;
}

Command* M1M3SSSubscriber::tryAcceptCommandTurnAirOn() {
	int32_t commandID = this->m1m3SAL->acceptCommand_turnAirOn(&this->turnAirOnData);
	if (commandID > 0) {
		return this->commandFactory->create(Commands::TurnAirOnCommand, &this->turnAirOnData, commandID);
	}
	return 0;
}

Command* M1M3SSSubscriber::tryAcceptCommandTurnAirOff() {
	int32_t commandID = this->m1m3SAL->acceptCommand_turnAirOff(&this->turnAirOffData);
	if (commandID > 0) {
		return this->commandFactory->create(Commands::TurnAirOffCommand, &this->turnAirOffData, commandID);
	}
	return 0;
}

Command* M1M3SSSubscriber::tryAcceptCommandApplyOffsetForces() {
	int32_t commandID = this->m1m3SAL->acceptCommand_applyOffsetForces(&this->applyOffsetForcesData);
	if (commandID > 0) {
		return this->commandFactory->create(Commands::ApplyOffsetForcesCommand, &this->applyOffsetForcesData, commandID);
	}
	return 0;
}

Command* M1M3SSSubscriber::tryAcceptCommandClearOffsetForces() {
	int32_t commandID = this->m1m3SAL->acceptCommand_clearOffsetForces(&this->clearOffsetForcesData);
	if (commandID > 0) {
		return this->commandFactory->create(Commands::ClearOffsetForcesCommand, &this->clearOffsetForcesData, commandID);
	}
	return 0;
}

Command* M1M3SSSubscriber::tryAcceptCommandRaiseM1M3() {
	int32_t commandID = this->m1m3SAL->acceptCommand_raiseM1M3(&this->raiseM1M3Data);
	if (commandID > 0) {
		return this->commandFactory->create(Commands::RaiseM1M3Command, &this->raiseM1M3Data, commandID);
	}
	return 0;
}

Command* M1M3SSSubscriber::tryAcceptCommandLowerM1M3() {
	int32_t commandID = this->m1m3SAL->acceptCommand_lowerM1M3(&this->lowerM1M3Data);
	if (commandID > 0) {
		return this->commandFactory->create(Commands::LowerM1M3Command, &this->lowerM1M3Data, commandID);
	}
	return 0;
}

Command* M1M3SSSubscriber::tryAcceptCommandApplyAberrationForcesByBendingModes() {
	int32_t commandID = this->m1m3SAL->acceptCommand_applyAberrationForcesByBendingModes(&this->applyAberrationForcesByBendingModesData);
	if (commandID > 0) {
		return this->commandFactory->create(Commands::ApplyAberrationForcesByBendingModesCommand, &this->applyAberrationForcesByBendingModesData, commandID);
	}
	return 0;
}

Command* M1M3SSSubscriber::tryAcceptCommandApplyAberrationForces() {
	int32_t commandID = this->m1m3SAL->acceptCommand_applyAberrationForces(&this->applyAberrationForcesData);
	if (commandID > 0) {
		return this->commandFactory->create(Commands::ApplyAberrationForcesCommand, &this->applyAberrationForcesData, commandID);
	}
	return 0;
}

Command* M1M3SSSubscriber::tryAcceptCommandClearAberrationForces() {
	int32_t commandID = this->m1m3SAL->acceptCommand_clearAberrationForces(&this->clearAberrationForcesData);
	if (commandID > 0) {
		return this->commandFactory->create(Commands::ClearAberrationForcesCommand, &this->clearAberrationForcesData, commandID);
	}
	return 0;
}

Command* M1M3SSSubscriber::tryAcceptCommandApplyActiveOpticForcesByBendingModes() {
	int32_t commandID = this->m1m3SAL->acceptCommand_applyActiveOpticForcesByBendingModes(&this->applyActiveOpticForcesByBendingModesData);
	if (commandID > 0) {
		return this->commandFactory->create(Commands::ApplyActiveOpticForcesByBendingModesCommand, &this->applyActiveOpticForcesByBendingModesData, commandID);
	}
	return 0;
}

Command* M1M3SSSubscriber::tryAcceptCommandApplyActiveOpticForces() {
	int32_t commandID = this->m1m3SAL->acceptCommand_applyActiveOpticForces(&this->applyActiveOpticForcesData);
	if (commandID > 0) {
		return this->commandFactory->create(Commands::ApplyActiveOpticForcesCommand, &this->applyActiveOpticForcesData, commandID);
	}
	return 0;
}

Command* M1M3SSSubscriber::tryAcceptCommandClearActiveOpticForces() {
	int32_t commandID = this->m1m3SAL->acceptCommand_clearActiveOpticForces(&this->clearActiveOpticForcesData);
	if (commandID > 0) {
		return this->commandFactory->create(Commands::ClearActiveOpticForcesCommand, &this->clearActiveOpticForcesData, commandID);
	}
	return 0;
}

Command* M1M3SSSubscriber::tryAcceptCommandEnterEngineering() {
	int32_t commandID = this->m1m3SAL->acceptCommand_enterEngineering(&this->enterEngineeringData);
	if (commandID > 0) {
		return this->commandFactory->create(Commands::EnterEngineeringCommand, &this->enterEngineeringData, commandID);
	}
	return 0;
}

Command* M1M3SSSubscriber::tryAcceptCommandExitEngineering() {
	int32_t commandID = this->m1m3SAL->acceptCommand_exitEngineering(&this->exitEngineeringData);
	if (commandID > 0) {
		return this->commandFactory->create(Commands::ExitEngineeringCommand, &this->exitEngineeringData, commandID);
	}
	return 0;
}

Command* M1M3SSSubscriber::tryAcceptCommandTestAir() {
	int32_t commandID = this->m1m3SAL->acceptCommand_testAir(&this->testAirData);
	if (commandID > 0) {
		return this->commandFactory->create(Commands::TestAirCommand, &this->testAirData, commandID);
	}
	return 0;
}

Command* M1M3SSSubscriber::tryAcceptCommandTestHardpoint() {
	int32_t commandID = this->m1m3SAL->acceptCommand_testHardpoint(&this->testHardpointData);
	if (commandID > 0) {
		return this->commandFactory->create(Commands::TestHardpointCommand, &this->testHardpointData, commandID);
	}
	return 0;
}

Command* M1M3SSSubscriber::tryAcceptCommandTestForceActuator() {
	int32_t commandID = this->m1m3SAL->acceptCommand_testForceActuator(&this->testForceActuatorData);
	if (commandID > 0) {
		return this->commandFactory->create(Commands::TestForceActuatorCommand, &this->testForceActuatorData, commandID);
	}
	return 0;
}

Command* M1M3SSSubscriber::tryAcceptCommandMoveHardpointActuators() {
	int32_t commandID = this->m1m3SAL->acceptCommand_moveHardpointActuators(&this->moveHardpointActuatorsData);
	if (commandID > 0) {
		return this->commandFactory->create(Commands::MoveHardpointActuatorsCommand, &this->moveHardpointActuatorsData, commandID);
	}
	return 0;
}

Command* M1M3SSSubscriber::tryAcceptCommandEnableHardpointChase() {
	int32_t commandID = this->m1m3SAL->acceptCommand_enableHardpointChase(&this->enableHardpointChaseData);
	if (commandID > 0) {
		return this->commandFactory->create(Commands::EnableHardpointChaseCommand, &this->enableHardpointChaseData, commandID);
	}
	return 0;
}

Command* M1M3SSSubscriber::tryAcceptCommandDisableHardpointChase() {
	int32_t commandID = this->m1m3SAL->acceptCommand_disableHardpointChase(&this->disableHardpointChaseData);
	if (commandID > 0) {
		return this->commandFactory->create(Commands::DisableHardpointChaseCommand, &this->disableHardpointChaseData, commandID);
	}
	return 0;
}

Command* M1M3SSSubscriber::tryAcceptCommandAbortRaiseM1M3() {
	int32_t commandID = this->m1m3SAL->acceptCommand_abortRaiseM1M3(&this->abortRaiseM1M3Data);
	if (commandID > 0) {
		return this->commandFactory->create(Commands::AbortRaiseM1M3Command, &this->abortRaiseM1M3Data, commandID);
	}
	return 0;
}

Command* M1M3SSSubscriber::tryAcceptCommandTranslateM1M3() {
	int32_t commandID = this->m1m3SAL->acceptCommand_translateM1M3(&this->translateM1M3Data);
	if (commandID > 0) {
		return this->commandFactory->create(Commands::TranslateM1M3Command, &this->translateM1M3Data, commandID);
	}
	return 0;
}

Command* M1M3SSSubscriber::tryAcceptCommandStopHardpointMotion() {
	int32_t commandID = this->m1m3SAL->acceptCommand_stopHardpointMotion(&this->stopHardpointMotionData);
	if (commandID > 0) {
		return this->commandFactory->create(Commands::StopHardpointMotionCommand, &this->stopHardpointMotionData, commandID);
	}
	return 0;
}

Command* M1M3SSSubscriber::tryAcceptCommandPositionM1M3() {
	int32_t commandID = this->m1m3SAL->acceptCommand_positionM1M3(&this->positionM1M3Data);
	if (commandID > 0) {
		return this->commandFactory->create(Commands::PositionM1M3Command, &this->positionM1M3Data, commandID);
	}
	return 0;
}

Command* M1M3SSSubscriber::tryAcceptCommandTurnLightsOn() {
	int32_t commandID = this->m1m3SAL->acceptCommand_turnLightsOn(&this->turnLightsOnData);
	if (commandID > 0) {
		return this->commandFactory->create(Commands::TurnLightsOnCommand, &this->turnLightsOnData, commandID);
	}
	return 0;
}

Command* M1M3SSSubscriber::tryAcceptCommandTurnLightsOff() {
	int32_t commandID = this->m1m3SAL->acceptCommand_turnLightsOff(&this->turnLightsOffData);
	if (commandID > 0) {
		return this->commandFactory->create(Commands::TurnLightsOffCommand, &this->turnLightsOffData, commandID);
	}
	return 0;
}

Command* M1M3SSSubscriber::tryAcceptCommandTurnPowerOn() {
	int32_t commandID = this->m1m3SAL->acceptCommand_turnPowerOn(&this->turnPowerOnData);
	if (commandID > 0) {
		return this->commandFactory->create(Commands::TurnPowerOnCommand, &this->turnPowerOnData, commandID);
	}
	return 0;
}

Command* M1M3SSSubscriber::tryAcceptCommandTurnPowerOff() {
	int32_t commandID = this->m1m3SAL->acceptCommand_turnPowerOff(&this->turnPowerOffData);
	if (commandID > 0) {
		return this->commandFactory->create(Commands::TurnPowerOffCommand, &this->turnPowerOffData, commandID);
	}
	return 0;
}

Command* M1M3SSSubscriber::tryAcceptCommandEnableHardpointCorrections() {
	int32_t commandID = this->m1m3SAL->acceptCommand_enableHardpointCorrections(&this->enableHardpointCorrectionsData);
	if (commandID > 0) {
		return this->commandFactory->create(Commands::EnableHardpointCorrectionsCommand, &this->enableHardpointCorrectionsData, commandID);
	}
	return 0;
}

Command* M1M3SSSubscriber::tryAcceptCommandDisableHardpointCorrections() {
	int32_t commandID = this->m1m3SAL->acceptCommand_disableHardpointCorrections(&this->disableHardpointCorrectionsData);
	if (commandID > 0) {
		return this->commandFactory->create(Commands::DisableHardpointCorrectionsCommand, &this->disableHardpointCorrectionsData, commandID);
	}
	return 0;
}

Command* M1M3SSSubscriber::tryAcceptCommandApplyOffsetForcesByMirrorForce() {
	int32_t commandID = this->m1m3SAL->acceptCommand_applyOffsetForcesByMirrorForce(&this->applyOffsetForcesByMirrorForceData);
	if (commandID > 0) {
		return this->commandFactory->create(Commands::ApplyOffsetForcesByMirrorForceCommand, &this->applyOffsetForcesByMirrorForceData, commandID);
	}
	return 0;
}

Command* M1M3SSSubscriber::tryAcceptCommandUpdatePID() {
	int32_t commandID = this->m1m3SAL->acceptCommand_updatePID(&this->updatePIDData);
	if (commandID > 0) {
		return this->commandFactory->create(Commands::UpdatePIDCommand, &this->updatePIDData, commandID);
	}
	return 0;
}

Command* M1M3SSSubscriber::tryAcceptCommandResetPID() {
	int32_t commandID = this->m1m3SAL->acceptCommand_resetPID(&this->resetPIDData);
	if (commandID > 0) {
		return this->commandFactory->create(Commands::ResetPIDCommand, &this->resetPIDData, commandID);
	}
	return 0;
}

Command* M1M3SSSubscriber::tryAcceptCommandProgramILC() {
	int32_t commandID = this->m1m3SAL->acceptCommand_programILC(&this->programILCData);
	if (commandID > 0) {
		return this->commandFactory->create(Commands::ProgramILCCommand, &this->programILCData, commandID);
	}
	return 0;
}

Command* M1M3SSSubscriber::tryAcceptCommandModbusTransmit() {
	int32_t commandID = this->m1m3SAL->acceptCommand_modbusTransmit(&this->modbusTransmitData);
	if (commandID > 0) {
		return this->commandFactory->create(Commands::ModbusTransmitCommand, &this->modbusTransmitData, commandID);
	}
	return 0;
}

Command* M1M3SSSubscriber::tryGetSampleTMAAzimuth() {
	int32_t result = this->mtMountSAL->getSample_Azimuth(&this->tmaAzimuth);
	if (result == 0) {
		return this->commandFactory->create(Commands::TMAAzimuthSampleCommand, &this->tmaAzimuth, 0);
	}
	return 0;
}

Command* M1M3SSSubscriber::tryGetSampleTMAElevation() {
	int32_t result = this->mtMountSAL->getSample_Elevation(&this->tmaElevation);
	if (result == 0) {
		return this->commandFactory->create(Commands::TMAElevationSampleCommand, &this->tmaElevation, 0);
	}
	return 0;
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
