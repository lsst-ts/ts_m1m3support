/*
 * RunHardpointCorrectionProfileCommand.h
 *
 *  Created on: Feb 9, 2018
 *      Author: ccontaxis
 */

#ifndef RUNHARDPOINTCORRECTIONPROFILECOMMAND_H_
#define RUNHARDPOINTCORRECTIONPROFILECOMMAND_H_

#include <Command.h>
#include <SAL_m1m3C.h>
#include <DataTypes.h>

namespace LSST {
namespace M1M3 {
namespace SS {

class RunHardpointCorrectionProfileCommand: public Command {
private:
	Context* context;
	M1M3SSPublisher* publisher;
	int32_t commandID;
	m1m3_command_RunHardpointCorrectionProfileC data;

public:
	RunHardpointCorrectionProfileCommand(Context* context, M1M3SSPublisher* publisher, int32_t commandID, m1m3_command_RunHardpointCorrectionProfileC* data);

	int32_t getCommandID() { return this->commandID; }
	m1m3_command_RunHardpointCorrectionProfileC* getData() { return &this->data; }

	bool validate();
	void execute();
	void ackInProgress();
	void ackComplete();
	void ackFailed(std::string reason);
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* RUNHARDPOINTCORRECTIONPROFILECOMMAND_H_ */
