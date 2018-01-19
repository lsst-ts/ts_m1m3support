/*
 * TurnAirOffCommand.h
 *
 *  Created on: Oct 16, 2017
 *      Author: ccontaxis
 */

#ifndef TURNAIROFFCOMMAND_H_
#define TURNAIROFFCOMMAND_H_

#include <Command.h>
#include <SAL_m1m3C.h>
#include <DataTypes.h>

namespace LSST {
namespace M1M3 {
namespace SS {

class TurnAirOffCommand: public Command {
private:
	Context* context;
	M1M3SSPublisher* publisher;
	int32_t commandID;
	m1m3_command_TurnAirOffC data;

public:
	TurnAirOffCommand(Context* context, M1M3SSPublisher* publisher, int32_t commandID, m1m3_command_TurnAirOffC* data);

	int32_t getCommandID() { return this->commandID; }
	m1m3_command_TurnAirOffC* getData() { return &this->data; }

	bool validate();
	void execute();
	void ackInProgress();
	void ackComplete();
	void ackFailed(std::string reason);
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* TURNAIROFFCOMMAND_H_ */
