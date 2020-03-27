/*
 * ClearOffsetForcesCommand.h
 *
 *  Created on: Oct 24, 2017
 *      Author: ccontaxis
 */

#ifndef CLEAROFFSETFORCESCOMMAND_H_
#define CLEAROFFSETFORCESCOMMAND_H_

#include <Command.h>
#include <SAL_MTM1M3C.h>
#include <DataTypes.h>

namespace LSST {
namespace M1M3 {
namespace SS {

class ClearOffsetForcesCommand: public Command {
private:
	Context* context;
	M1M3SSPublisher* publisher;
	MTM1M3_command_clearOffsetForcesC data;

public:
	ClearOffsetForcesCommand(Context* context, M1M3SSPublisher* publisher, int32_t commandID, MTM1M3_command_clearOffsetForcesC* data);

	MTM1M3_command_clearOffsetForcesC* getData() { return &this->data; }

	bool validate();
	void execute();
	void ackInProgress();
	void ackComplete();
	void ackFailed(std::string reason);
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* CLEAROFFSETFORCESCOMMAND_H_ */
