/*
 * ClearAOSCorrectionCommand.h
 *
 *  Created on: Oct 26, 2017
 *      Author: ccontaxis
 */

#ifndef CLEARAOSCORRECTIONCOMMAND_H_
#define CLEARAOSCORRECTIONCOMMAND_H_

#include <Command.h>
#include <SAL_m1m3C.h>
#include <DataTypes.h>

namespace LSST {
namespace M1M3 {
namespace SS {

class ClearAOSCorrectionCommand: public Command {
private:
	Context* context;
	M1M3SSPublisher* publisher;
	int32_t commandID;
	m1m3_command_ClearAOSCorrectionC data;

public:
	ClearAOSCorrectionCommand(Context* context, M1M3SSPublisher* publisher, int32_t commandID, m1m3_command_ClearAOSCorrectionC* data);

	int32_t getCommandID() { return this->commandID; }
	m1m3_command_ClearAOSCorrectionC* getData() { return &this->data; }

	bool validate();
	void execute();
	void ackInProgress();
	void ackComplete();
	void ackFailed(std::string reason);
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* CLEARAOSCORRECTIONCOMMAND_H_ */
