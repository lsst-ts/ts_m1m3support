/*
 * TurnLightsOffCommand.h
 *
 *  Created on: Nov 30, 2017
 *      Author: ccontaxis
 */

#ifndef TURNLIGHTSOFFCOMMAND_H_
#define TURNLIGHTSOFFCOMMAND_H_

#include <Command.h>
#include <SAL_m1m3C.h>
#include <DataTypes.h>

namespace LSST {
namespace M1M3 {
namespace SS {

class TurnLightsOffCommand: public Command {
private:
	Context* context;
	M1M3SSPublisher* publisher;
	m1m3_command_TurnLightsOffC data;

public:
	TurnLightsOffCommand(Context* context, M1M3SSPublisher* publisher, int32_t commandID, m1m3_command_TurnLightsOffC* data);

	m1m3_command_TurnLightsOffC* getData() { return &this->data; }

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
