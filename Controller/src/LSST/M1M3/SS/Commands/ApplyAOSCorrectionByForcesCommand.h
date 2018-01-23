/*
 * ApplyAOSCorrectionByForcesCommand.h
 *
 *  Created on: Oct 26, 2017
 *      Author: ccontaxis
 */

#ifndef APPLYAOSCORRECTIONBYFORCESCOMMAND_H_
#define APPLYAOSCORRECTIONBYFORCESCOMMAND_H_

#include <Command.h>
#include <SAL_m1m3C.h>
#include <DataTypes.h>

namespace LSST {
namespace M1M3 {
namespace SS {

class ApplyAOSCorrectionByForcesCommand: public Command {
private:
	Context* context;
	M1M3SSPublisher* publisher;
	int32_t commandID;
	m1m3_command_ApplyAOSCorrectionByForcesC data;

public:
	ApplyAOSCorrectionByForcesCommand(Context* context, M1M3SSPublisher* publisher, int32_t commandID, m1m3_command_ApplyAOSCorrectionByForcesC* data);

	int32_t getCommandID() { return this->commandID; }
	m1m3_command_ApplyAOSCorrectionByForcesC* getData() { return &this->data; }

	bool validate();
	void execute();
	void ackInProgress();
	void ackComplete();
	void ackFailed(std::string reason);
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* APPLYAOSCORRECTIONBYFORCESCOMMAND_H_ */
