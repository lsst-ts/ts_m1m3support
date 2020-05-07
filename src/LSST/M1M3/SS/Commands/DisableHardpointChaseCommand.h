/*
 * DisableHardpointChaseCommand.h
 *
 *  Created on: Oct 30, 2017
 *      Author: ccontaxis
 */

#ifndef DISABLEHARDPOINTCHASECOMMAND_H_
#define DISABLEHARDPOINTCHASECOMMAND_H_

#include <Command.h>
#include <SAL_MTM1M3C.h>
#include <DataTypes.h>

namespace LSST {
namespace M1M3 {
namespace SS {

class DisableHardpointChaseCommand: public Command {
private:
	Context* context;
	M1M3SSPublisher* publisher;
	MTM1M3_command_disableHardpointChaseC data;

public:
	DisableHardpointChaseCommand(Context* context, M1M3SSPublisher* publisher, int32_t commandID, MTM1M3_command_disableHardpointChaseC* data);

	MTM1M3_command_disableHardpointChaseC* getData() { return &this->data; }

	bool validate();
	void execute();
	void ackInProgress();
	void ackComplete();
	void ackFailed(std::string reason);
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* DISABLEHARDPOINTCHASECOMMAND_H_ */
