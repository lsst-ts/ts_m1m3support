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
#include <ApplyAberrationForcesByBendingModesCommand.h>
#include <ApplyAberrationForcesCommand.h>
#include <ClearAberrationForcesCommand.h>
#include <ApplyActiveOpticForcesByBendingModesCommand.h>
#include <ApplyActiveOpticForcesCommand.h>
#include <ClearActiveOpticForcesCommand.h>
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
#include <ApplyOffsetForcesByMirrorForceCommand.h>
#include <UpdatePIDCommand.h>
#include <ResetPIDCommand.h>
#include <ProgramILCCommand.h>
#include <ModbusTransmitCommand.h>
#include <pthread.h>
#include <Log.h>

namespace LSST {
namespace M1M3 {
namespace SS {

CommandFactory::CommandFactory(M1M3SSPublisher* publisher, Context* context) {
    Log.Debug("CommandFactory: CommandFactory()");
    this->publisher = publisher;
    this->context = context;
}

Command* CommandFactory::create(Commands::Type commandType, void* data, int32_t commandID) {
    Log.Trace("CommandFactory: create(%d, data, %d)", commandType, commandID);
    switch (commandType) {
        case Commands::BootCommand:
            return new BootCommand(this->context);
        case Commands::StartCommand:
            return new StartCommand(this->context, this->publisher, commandID, (MTM1M3_command_startC*)data);
        case Commands::EnableCommand:
            return new EnableCommand(this->context, this->publisher, commandID, (MTM1M3_command_enableC*)data);
        case Commands::DisableCommand:
            return new DisableCommand(this->context, this->publisher, commandID, (MTM1M3_command_disableC*)data);
        case Commands::StandbyCommand:
            return new StandbyCommand(this->context, this->publisher, commandID, (MTM1M3_command_standbyC*)data);
        case Commands::UpdateCommand:
            return new UpdateCommand(this->context, (pthread_mutex_t*)data);
        case Commands::ShutdownCommand:
            return new ShutdownCommand(this->context, this->publisher, commandID, (MTM1M3_command_shutdownC*)data);
        case Commands::TurnAirOnCommand:
            return new TurnAirOnCommand(this->context, this->publisher, commandID, (MTM1M3_command_turnAirOnC*)data);
        case Commands::TurnAirOffCommand:
            return new TurnAirOffCommand(this->context, this->publisher, commandID, (MTM1M3_command_turnAirOffC*)data);
        case Commands::ApplyOffsetForcesCommand:
            return new ApplyOffsetForcesCommand(this->context, this->publisher, commandID, (MTM1M3_command_applyOffsetForcesC*)data);
        case Commands::ClearOffsetForcesCommand:
            return new ClearOffsetForcesCommand(this->context, this->publisher, commandID, (MTM1M3_command_clearOffsetForcesC*)data);
        case Commands::RaiseM1M3Command:
            return new RaiseM1M3Command(this->context, this->publisher, commandID, (MTM1M3_command_raiseM1M3C*)data);
        case Commands::LowerM1M3Command:
            return new LowerM1M3Command(this->context, this->publisher, commandID, (MTM1M3_command_lowerM1M3C*)data);
        case Commands::ApplyAberrationForcesByBendingModesCommand:
            return new ApplyAberrationForcesByBendingModesCommand(this->context, this->publisher, commandID, (MTM1M3_command_applyAberrationForcesByBendingModesC*)data);
        case Commands::ApplyAberrationForcesCommand:
            return new ApplyAberrationForcesCommand(this->context, this->publisher, commandID, (MTM1M3_command_applyAberrationForcesC*)data);
        case Commands::ClearAberrationForcesCommand:
            return new ClearAberrationForcesCommand(this->context, this->publisher, commandID, (MTM1M3_command_clearAberrationForcesC*)data);
        case Commands::ApplyActiveOpticForcesByBendingModesCommand:
            return new ApplyActiveOpticForcesByBendingModesCommand(this->context, this->publisher, commandID, (MTM1M3_command_applyActiveOpticForcesByBendingModesC*)data);
        case Commands::ApplyActiveOpticForcesCommand:
            return new ApplyActiveOpticForcesCommand(this->context, this->publisher, commandID, (MTM1M3_command_applyActiveOpticForcesC*)data);
        case Commands::ClearActiveOpticForcesCommand:
            return new ClearActiveOpticForcesCommand(this->context, this->publisher, commandID, (MTM1M3_command_clearActiveOpticForcesC*)data);
        case Commands::EnterEngineeringCommand:
            return new EnterEngineeringCommand(this->context, this->publisher, commandID, (MTM1M3_command_enterEngineeringC*)data);
        case Commands::ExitEngineeringCommand:
            return new ExitEngineeringCommand(this->context, this->publisher, commandID, (MTM1M3_command_exitEngineeringC*)data);
        case Commands::TestAirCommand:
            return new TestAirCommand(this->context, this->publisher, commandID, (MTM1M3_command_testAirC*)data);
        case Commands::TestHardpointCommand:
            return new TestHardpointCommand(this->context, this->publisher, commandID, (MTM1M3_command_testHardpointC*)data);
        case Commands::TestForceActuatorCommand:
            return new TestForceActuatorCommand(this->context, this->publisher, commandID, (MTM1M3_command_testForceActuatorC*)data);
        case Commands::MoveHardpointActuatorsCommand:
            return new MoveHardpointActuatorsCommand(this->context, this->publisher, commandID, (MTM1M3_command_moveHardpointActuatorsC*)data);
        case Commands::EnableHardpointChaseCommand:
            return new EnableHardpointChaseCommand(this->context, this->publisher, commandID, (MTM1M3_command_enableHardpointChaseC*)data);
        case Commands::DisableHardpointChaseCommand:
            return new DisableHardpointChaseCommand(this->context, this->publisher, commandID, (MTM1M3_command_disableHardpointChaseC*)data);
        case Commands::AbortRaiseM1M3Command:
            return new AbortRaiseM1M3Command(this->context, this->publisher, commandID, (MTM1M3_command_abortRaiseM1M3C*)data);
        case Commands::TranslateM1M3Command:
            return new TranslateM1M3Command(this->context, this->publisher, commandID, (MTM1M3_command_translateM1M3C*)data);
        case Commands::StopHardpointMotionCommand:
            return new StopHardpointMotionCommand(this->context, this->publisher, commandID, (MTM1M3_command_stopHardpointMotionC*)data);
        case Commands::TMAAzimuthSampleCommand:
            return new TMAAzimuthSampleCommand(this->context, (MTMount_AzimuthC*)data);
        case Commands::TMAElevationSampleCommand:
            return new TMAElevationSampleCommand(this->context, (MTMount_ElevationC*)data);
        case Commands::PositionM1M3Command:
            return new PositionM1M3Command(this->context, this->publisher, commandID, (MTM1M3_command_positionM1M3C*)data);
        case Commands::TurnLightsOnCommand:
            return new TurnLightsOnCommand(this->context, this->publisher, commandID, (MTM1M3_command_turnLightsOnC*)data);
        case Commands::TurnLightsOffCommand:
            return new TurnLightsOffCommand(this->context, this->publisher, commandID, (MTM1M3_command_turnLightsOffC*)data);
        case Commands::TurnPowerOnCommand:
            return new TurnPowerOnCommand(this->context, this->publisher, commandID, (MTM1M3_command_turnPowerOnC*)data);
        case Commands::TurnPowerOffCommand:
            return new TurnPowerOffCommand(this->context, this->publisher, commandID, (MTM1M3_command_turnPowerOffC*)data);
        case Commands::EnableHardpointCorrectionsCommand:
            return new EnableHardpointCorrectionsCommand(this->context, this->publisher, commandID, (MTM1M3_command_enableHardpointCorrectionsC*)data);
        case Commands::DisableHardpointCorrectionsCommand:
            return new DisableHardpointCorrectionsCommand(this->context, this->publisher, commandID, (MTM1M3_command_disableHardpointCorrectionsC*)data);
        case Commands::ApplyOffsetForcesByMirrorForceCommand:
            return new ApplyOffsetForcesByMirrorForceCommand(this->context, this->publisher, commandID, (MTM1M3_command_applyOffsetForcesByMirrorForceC*)data);
        case Commands::UpdatePIDCommand:
            return new UpdatePIDCommand(this->context, this->publisher, commandID, (MTM1M3_command_updatePIDC*)data);
        case Commands::ResetPIDCommand:
            return new ResetPIDCommand(this->context, this->publisher, commandID, (MTM1M3_command_resetPIDC*)data);
        case Commands::ProgramILCCommand:
            return new ProgramILCCommand(this->context, this->publisher, commandID, (MTM1M3_command_programILCC*)data);
        case Commands::ModbusTransmitCommand:
            return new ModbusTransmitCommand(this->context, this->publisher, commandID, (MTM1M3_command_modbusTransmitC*)data);
    }
    return 0;
}

void CommandFactory::destroy(Command* command) {
    Log.Trace("CommandFactory: destroy()");
    if (command) {
        delete command;
    }
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
