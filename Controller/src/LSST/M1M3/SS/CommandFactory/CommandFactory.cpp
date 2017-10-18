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
#include <pthread.h>

namespace LSST {
namespace M1M3 {
namespace SS {

CommandFactory::CommandFactory(IPublisher* publisher, IContext* context) {
	this->publisher = publisher;
	this->context = context;
}

ICommand* CommandFactory::create(Commands::Type commandType, void* data, int32_t commandID) {
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
	}
	return 0;
}

void CommandFactory::destroy(ICommand* command) {
	if (command) {
		delete command;
	}
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
