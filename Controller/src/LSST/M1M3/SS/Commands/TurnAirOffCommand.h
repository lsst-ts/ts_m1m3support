/*
 * TurnAirOffCommand.h
 *
 *  Created on: Oct 16, 2017
 *      Author: ccontaxis
 */

#ifndef TURNAIROFFCOMMAND_H_
#define TURNAIROFFCOMMAND_H_

#include <Command.h>
#include <SAL_MTM1M3C.h>
#include <DataTypes.h>

namespace LSST {
namespace M1M3 {
namespace SS {

class TurnAirOffCommand: public Command {
private:
	Context* context;
	M1M3SSPublisher* publisher;
	MTM1M3_command_turnAirOffC data;

public:
	TurnAirOffCommand(Context* context, M1M3SSPublisher* publisher, int32_t commandID, MTM1M3_command_turnAirOffC* data);

	MTM1M3_command_turnAirOffC* getData() { return &this->data; }

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
