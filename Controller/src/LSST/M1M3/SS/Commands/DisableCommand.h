/*
 * DisableCommand.h
 *
 *  Created on: Sep 26, 2017
 *      Author: ccontaxis
 */

#ifndef DISABLECOMMAND_H_
#define DISABLECOMMAND_H_

#include <Command.h>
#include <SAL_m1m3C.h>
#include <DataTypes.h>

namespace LSST {
namespace M1M3 {
namespace SS {

/*!
 * This command is responsible for transitioning the state
 * machine from the enabled state to the disabled state.
 * This is an external command and can be issued via SAL.
 */
class DisableCommand: public Command {
private:
	IContext* context;
	IPublisher* publisher;
	int32_t commandID;
	m1m3_command_DisableC data;

public:
	DisableCommand(IContext* context, IPublisher* publisher, int32_t commandID, m1m3_command_DisableC* data);

	int32_t getCommandID() { return this->commandID; }
	m1m3_command_DisableC* getData() { return &this->data; }

	bool validate();
	void execute();
	void ackInProgress();
	void ackComplete();
	void ackFailed(std::string reason);
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* DISABLECOMMAND_H_ */
