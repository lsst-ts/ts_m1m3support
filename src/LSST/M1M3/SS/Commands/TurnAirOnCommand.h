/*
 * TurnAirOnCommand.h
 *
 *  Created on: Oct 16, 2017
 *      Author: ccontaxis
 */

#ifndef TURNAIRONCOMMAND_H_
#define TURNAIRONCOMMAND_H_

#include <Command.h>
#include <SAL_MTM1M3C.h>
#include <DataTypes.h>

namespace LSST {
namespace M1M3 {
namespace SS {

class TurnAirOnCommand: public Command {
private:
	Context* context;
	M1M3SSPublisher* publisher;
	MTM1M3_command_turnAirOnC data;

public:
	TurnAirOnCommand(Context* context, M1M3SSPublisher* publisher, int32_t commandID, MTM1M3_command_turnAirOnC* data);

	MTM1M3_command_turnAirOnC* getData() { return &this->data; }

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
