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
	Context* context;
	M1M3SSPublisher* publisher;
	m1m3_command_TurnPowerOffC data;

public:
	TurnPowerOffCommand(Context* context, M1M3SSPublisher* publisher, int32_t commandID, m1m3_command_TurnPowerOffC* data);

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
