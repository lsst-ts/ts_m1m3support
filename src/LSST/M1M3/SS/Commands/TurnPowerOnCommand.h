/*
 * TurnPowerOnCommand.h
 *
 *  Created on: Dec 7, 2017
 *      Author: ccontaxis
 */

#ifndef TURNPOWERONCOMMAND_H_
#define TURNPOWERONCOMMAND_H_

#include <Command.h>
#include <SAL_MTM1M3C.h>
#include <DataTypes.h>

namespace LSST {
namespace M1M3 {
namespace SS {

class TurnPowerOnCommand: public Command {
private:
	Context* context;
	M1M3SSPublisher* publisher;
	MTM1M3_command_turnPowerOnC data;

public:
	TurnPowerOnCommand(Context* context, M1M3SSPublisher* publisher, int32_t commandID, MTM1M3_command_turnPowerOnC* data);

	MTM1M3_command_turnPowerOnC* getData() { return &this->data; }

	bool validate();
	void execute();
	void ackInProgress();
	void ackComplete();
	void ackFailed(std::string reason);
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* TURNPOWERONCOMMAND_H_ */
