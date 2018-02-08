/*
 * EnableHardpointCorrectionsCommand.h
 *
 *  Created on: Feb 8, 2018
 *      Author: ccontaxis
 */

#ifndef ENABLEHARDPOINTCORRECTIONSCOMMAND_H_
#define ENABLEHARDPOINTCORRECTIONSCOMMAND_H_

#include <Command.h>
#include <SAL_m1m3C.h>
#include <DataTypes.h>

namespace LSST {
namespace M1M3 {
namespace SS {

class EnableHardpointCorrectionsCommand: public Command {
private:
	Context* context;
	M1M3SSPublisher* publisher;
	int32_t commandID;
	m1m3_command_EnableHardpointCorrectionsC data;

public:
	EnableHardpointCorrectionsCommand(Context* context, M1M3SSPublisher* publisher, int32_t commandID, m1m3_command_EnableHardpointCorrectionsC* data);

	int32_t getCommandID() { return this->commandID; }
	m1m3_command_EnableHardpointCorrectionsC* getData() { return &this->data; }

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
