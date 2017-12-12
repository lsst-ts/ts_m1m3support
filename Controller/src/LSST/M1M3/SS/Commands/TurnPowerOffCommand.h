/*
 * TurnPowerOffCommand.h
 *
 *  Created on: Dec 7, 2017
 *      Author: ccontaxis
 */

#ifndef TURNPOWEROFFCOMMAND_H_
#define TURNPOWEROFFCOMMAND_H_

#include <Command.h>
#include <SAL_m1m3C.h>
#include <DataTypes.h>

namespace LSST {
namespace M1M3 {
namespace SS {

class TurnPowerOffCommand: public Command {
private:
	IContext* context;
	IPublisher* publisher;
	int32_t commandID;
	m1m3_command_TurnPowerOffC data;

public:
	TurnPowerOffCommand(IContext* context, IPublisher* publisher, int32_t commandID, m1m3_command_TurnPowerOffC* data);

	int32_t getCommandID() { return this->commandID; }
	m1m3_command_TurnPowerOffC* getData() { return &this->data; }

	bool validate();
	void execute();
	void ackInProgress();
	void ackComplete();
	void ackFailed(std::string reason);
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* TURNPOWEROFFCOMMAND_H_ */
