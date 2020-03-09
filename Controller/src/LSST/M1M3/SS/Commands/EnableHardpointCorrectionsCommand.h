/*
 * EnableHardpointCorrectionsCommand.h
 *
 *  Created on: Feb 8, 2018
 *      Author: ccontaxis
 */

#ifndef ENABLEHARDPOINTCORRECTIONSCOMMAND_H_
#define ENABLEHARDPOINTCORRECTIONSCOMMAND_H_

#include <Command.h>
#include <SAL_MTM1M3C.h>
#include <DataTypes.h>

namespace LSST {
namespace M1M3 {
namespace SS {

class EnableHardpointCorrectionsCommand: public Command {
private:
	Context* context;
	M1M3SSPublisher* publisher;
	MTM1M3_command_enableHardpointCorrectionsC data;

public:
	EnableHardpointCorrectionsCommand(Context* context, M1M3SSPublisher* publisher, int32_t commandID, MTM1M3_command_enableHardpointCorrectionsC* data);

	MTM1M3_command_enableHardpointCorrectionsC* getData() { return &this->data; }

	bool validate();
	void execute();
	void ackInProgress();
	void ackComplete();
	void ackFailed(std::string reason);
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* ENABLEHARDPOINTCORRECTIONSCOMMAND_H_ */
