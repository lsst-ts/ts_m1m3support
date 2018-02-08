/*
 * CommandFactory.cpp
 *
 *  Created on: Sep 29, 2017
 *      Author: ccontaxis
 */

#include <CommandFactory.h>
#include <BootCommand.h>
#include <StartCommand.h>
#include <EnableCommand.h>
#include <DisableCommand.h>
#include <StandbyCommand.h>
#include <UpdateCommand.h>
#include <ShutdownCommand.h>
#include <TurnAirOnCommand.h>
#include <TurnAirOffCommand.h>
#include <ApplyOffsetForcesCommand.h>
#include <ClearOffsetForcesCommand.h>
#include <RaiseM1M3Command.h>
#include <LowerM1M3Command.h>
#include <ApplyAberrationByBendingModesCommand.h>
#include <ApplyAberrationByForcesCommand.h>
#include <ClearAberrationCommand.h>
#include <ApplyAOSCorrectionByBendingModesCommand.h>
#include <ApplyAOSCorrectionByForcesCommand.h>
#include <ClearAOSCorrectionCommand.h>
#include <EnterEngineeringCommand.h>
#include <ExitEngineeringCommand.h>
#include <TestAirCommand.h>
#include <TestHardpointCommand.h>
#include <TestForceActuatorCommand.h>
#include <MoveHardpointActuatorsCommand.h>
#include <EnableHardpointChaseCommand.h>
#include <DisableHardpointChaseCommand.h>
#include <AbortRaiseM1M3Command.h>
#include <TranslateM1M3Command.h>
#include <StopHardpointMotionCommand.h>
#include <TMAAzimuthSampleCommand.h>
#include <TMAElevationSampleCommand.h>
#include <PositionM1M3Command.h>
#include <TurnLightsOnCommand.h>
#include <TurnLightsOffCommand.h>
#include <TurnPowerOnCommand.h>
#include <TurnPowerOffCommand.h>
#include <EnableHardpointCorrectionsCommand.h>
#include <DisableHardpointCorrectionsCommand.h>
#include <pthread.h>

namespace LSST {
namespace M1M3 {
namespace SS {

CommandFactory::CommandFactory(M1M3SSPublisher* publisher, Context* context) {
	this->publisher = publisher;
	this->context = context;
}

Command* CommandFactory::create(Commands::Type commandType, void* data, int32_t commandID) {
	switch(commandType) {
	case Commands::BootCommand: return new BootCommand(this->context);
	case Commands::StartCommand: return new StartCommand(this->context, this->publisher, commandID, (m1m3_command_StartC*)data);
	case Commands::EnableCommand: return new EnableCommand(this->context, this->publisher, commandID, (m1m3_command_EnableC*)data);
	case Commands::DisableCommand: return new DisableCommand(this->context, this->publisher, commandID, (m1m3_command_DisableC*)data);
	case Commands::StandbyCommand: return new StandbyCommand(this->context, this->publisher, commandID, (m1m3_command_StandbyC*)data);
	case Commands::UpdateCommand: return new UpdateCommand(this->context, (pthread_mutex_t*)data);
	case Commands::ShutdownCommand: return new ShutdownCommand(this->context, this->publisher, commandID, (m1m3_command_ShutdownC*)data);
	case Commands::TurnAirOnCommand: return new TurnAirOnCommand(this->context, this->publisher, commandID, (m1m3_command_TurnAirOnC*)data);
	case Commands::TurnAirOffCommand: return new TurnAirOffCommand(this->context, this->publisher, commandID, (m1m3_command_TurnAirOffC*)data);
	case Commands::ApplyOffsetForcesCommand: return new ApplyOffsetForcesCommand(this->context, this->publisher, commandID, (m1m3_command_ApplyOffsetForcesC*)data);
	case Commands::ClearOffsetForcesCommand: return new ClearOffsetForcesCommand(this->context, this->publisher, commandID, (m1m3_command_ClearOffsetForcesC*)data);
	case Commands::RaiseM1M3Command: return new RaiseM1M3Command(this->context, this->publisher, commandID, (m1m3_command_RaiseM1M3C*)data);
	case Commands::LowerM1M3Command: return new LowerM1M3Command(this->context, this->publisher, commandID, (m1m3_command_LowerM1M3C*)data);
	case Commands::ApplyAberrationByBendingModesCommand: return new ApplyAberrationByBendingModesCommand(this->context, this->publisher, commandID, (m1m3_command_ApplyAberrationByBendingModesC*)data);
	case Commands::ApplyAberrationByForcesCommand: return new ApplyAberrationByForcesCommand(this->context, this->publisher, commandID, (m1m3_command_ApplyAberrationByForcesC*)data);
	case Commands::ClearAberrationCommand: return new ClearAberrationCommand(this->context, this->publisher, commandID, (m1m3_command_ClearAberrationC*)data);
	case Commands::ApplyAOSCorrectionByBendingModesCommand: return  new ApplyAOSCorrectionByBendingModesCommand(this->context, this->publisher, commandID, (m1m3_command_ApplyAOSCorrectionByBendingModesC*)data);
	case Commands::ApplyAOSCorrectionByForcesCommand: return new ApplyAOSCorrectionByForcesCommand(this->context, this->publisher, commandID, (m1m3_command_ApplyAOSCorrectionByForcesC*)data);
	case Commands::ClearAOSCorrectionCommand: return new ClearAOSCorrectionCommand(this->context, this->publisher, commandID, (m1m3_command_ClearAOSCorrectionC*)data);
	case Commands::EnterEngineeringCommand: return new EnterEngineeringCommand(this->context, this->publisher, commandID, (m1m3_command_EnterEngineeringC*)data);
	case Commands::ExitEngineeringCommand: return new ExitEngineeringCommand(this->context, this->publisher, commandID, (m1m3_command_ExitEngineeringC*)data);
	case Commands::TestAirCommand: return new TestAirCommand(this->context, this->publisher, commandID, (m1m3_command_TestAirC*)data);
	case Commands::TestHardpointCommand: return new TestHardpointCommand(this->context, this->publisher, commandID, (m1m3_command_TestHardpointC*)data);
	case Commands::TestForceActuatorCommand: return new TestForceActuatorCommand(this->context, this->publisher, commandID, (m1m3_command_TestForceActuatorC*)data);
	case Commands::MoveHardpointActuatorsCommand: return new MoveHardpointActuatorsCommand(this->context, this->publisher, commandID, (m1m3_command_MoveHardpointActuatorsC*)data);
	case Commands::EnableHardpointChaseCommand: return new EnableHardpointChaseCommand(this->context, this->publisher, commandID, (m1m3_command_EnableHardpointChaseC*)data);
	case Commands::DisableHardpointChaseCommand: return new DisableHardpointChaseCommand(this->context, this->publisher, commandID, (m1m3_command_DisableHardpointChaseC*)data);
	case Commands::AbortRaiseM1M3Command: return new AbortRaiseM1M3Command(this->context, this->publisher, commandID, (m1m3_command_AbortRaiseM1M3C*)data);
	case Commands::TranslateM1M3Command: return new TranslateM1M3Command(this->context, this->publisher, commandID, (m1m3_command_TranslateM1M3C*)data);
	case Commands::StopHardpointMotionCommand: return new StopHardpointMotionCommand(this->context, this->publisher, commandID, (m1m3_command_StopHardpointMotionC*)data);
	case Commands::TMAAzimuthSampleCommand: return new TMAAzimuthSampleCommand(this->context, (MTMount_AzC*)data);
	case Commands::TMAElevationSampleCommand: return new TMAElevationSampleCommand(this->context, (MTMount_AltC*)data);
	case Commands::PositionM1M3Command: return new PositionM1M3Command(this->context, this->publisher, commandID, (m1m3_command_PositionM1M3C*)data);
	case Commands::TurnLightsOnCommand: return new TurnLightsOnCommand(this->context, this->publisher, commandID, (m1m3_command_TurnLightsOnC*)data);
	case Commands::TurnLightsOffCommand: return new TurnLightsOffCommand(this->context, this->publisher, commandID, (m1m3_command_TurnLightsOffC*)data);
	case Commands::TurnPowerOnCommand: return new TurnPowerOnCommand(this->context, this->publisher, commandID, (m1m3_command_TurnPowerOnC*)data);
	case Commands::TurnPowerOffCommand: return new TurnPowerOffCommand(this->context, this->publisher, commandID, (m1m3_command_TurnPowerOffC*)data);
	case Commands::EnableHardpointCorrectionsCommand: return new EnableHardpointCorrectionsCommand(this->context, this->publisher, commandID, (m1m3_command_EnableHardpointCorrectionsC*)data);
	case Commands::DisableHardpointCorrectionsCommand: return new DisableHardpointCorrectionsCommand(this->context, this->publisher, commandID, (m1m3_command_DisableHardpointCorrectionsC*)data);
	}
	return 0;
}

void CommandFactory::destroy(Command* command) {
	if (command) {
		delete command;
	}
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
