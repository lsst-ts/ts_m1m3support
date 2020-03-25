/*
 * ExitEngineeringCommand.h
 *
 *  Created on: Oct 30, 2017
 *      Author: ccontaxis
 */

#ifndef EXITENGINEERINGCOMMAND_H_
#define EXITENGINEERINGCOMMAND_H_

#include <Command.h>
#include <SAL_MTM1M3C.h>
#include <DataTypes.h>

namespace LSST {
namespace M1M3 {
namespace SS {

class ExitEngineeringCommand: public Command {
private:
	Context* context;
	M1M3SSPublisher* publisher;
	MTM1M3_command_exitEngineeringC data;

public:
	ExitEngineeringCommand(Context* context, M1M3SSPublisher* publisher, int32_t commandID, MTM1M3_command_exitEngineeringC* data);

	MTM1M3_command_exitEngineeringC* getData() { return &this->data; }

	bool validate();
	void execute();
	void ackInProgress();
	void ackComplete();
	void ackFailed(std::string reason);
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* EXITENGINEERINGCOMMAND_H_ */
