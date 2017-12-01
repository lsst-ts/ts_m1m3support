/*
 * TurnLightsOnCommand.h
 *
 *  Created on: Nov 30, 2017
 *      Author: ccontaxis
 */

#ifndef TURNLIGHTSONCOMMAND_H_
#define TURNLIGHTSONCOMMAND_H_

#include <Command.h>
#include <SAL_m1m3C.h>
#include <DataTypes.h>

namespace LSST {
namespace M1M3 {
namespace SS {

class TurnLightsOnCommand: public Command {
private:
	IContext* context;
	IPublisher* publisher;
	int32_t commandID;
	m1m3_command_TurnLightsOnC data;

public:
	TurnLightsOnCommand(IContext* context, IPublisher* publisher, int32_t commandID, m1m3_command_TurnLightsOnC* data);

	int32_t getCommandID() { return this->commandID; }
	m1m3_command_TurnLightsOnC* getData() { return &this->data; }

	bool validate();
	void execute();
	void ackInProgress();
	void ackComplete();
	void ackFailed(std::string reason);
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* TURNLIGHTSONCOMMAND_H_ */
