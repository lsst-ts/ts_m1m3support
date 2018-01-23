/*
 * TurnAirOnCommand.h
 *
 *  Created on: Oct 16, 2017
 *      Author: ccontaxis
 */

#ifndef TURNAIRONCOMMAND_H_
#define TURNAIRONCOMMAND_H_

#include <Command.h>
#include <SAL_m1m3C.h>
#include <DataTypes.h>

namespace LSST {
namespace M1M3 {
namespace SS {

class TurnAirOnCommand: public Command {
private:
	Context* context;
	M1M3SSPublisher* publisher;
	int32_t commandID;
	m1m3_command_TurnAirOnC data;

public:
	TurnAirOnCommand(Context* context, M1M3SSPublisher* publisher, int32_t commandID, m1m3_command_TurnAirOnC* data);

	int32_t getCommandID() { return this->commandID; }
	m1m3_command_TurnAirOnC* getData() { return &this->data; }

	bool validate();
	void execute();
	void ackInProgress();
	void ackComplete();
	void ackFailed(std::string reason);
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* TURNAIRONCOMMAND_H_ */
