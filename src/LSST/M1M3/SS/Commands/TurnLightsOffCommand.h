/*
 * TurnLightsOffCommand.h
 *
 *  Created on: Nov 30, 2017
 *      Author: ccontaxis
 */

#ifndef TURNLIGHTSOFFCOMMAND_H_
#define TURNLIGHTSOFFCOMMAND_H_

#include <Command.h>
#include <SAL_MTM1M3C.h>
#include <DataTypes.h>

namespace LSST {
namespace M1M3 {
namespace SS {

class TurnLightsOffCommand: public Command {
private:
	Context* context;
	M1M3SSPublisher* publisher;
	MTM1M3_command_turnLightsOffC data;

public:
	TurnLightsOffCommand(Context* context, M1M3SSPublisher* publisher, int32_t commandID, MTM1M3_command_turnLightsOffC* data);

	MTM1M3_command_turnLightsOffC* getData() { return &this->data; }

	bool validate();
	void execute();
	void ackInProgress();
	void ackComplete();
	void ackFailed(std::string reason);
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* TURNLIGHTSOFFCOMMAND_H_ */
