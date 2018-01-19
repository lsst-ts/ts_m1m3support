/*
 * ApplyAOSCorrectionByBendingModesCommand.h
 *
 *  Created on: Oct 26, 2017
 *      Author: ccontaxis
 */

#ifndef APPLYAOSCORRECTIONBYBENDINGMODESCOMMAND_H_
#define APPLYAOSCORRECTIONBYBENDINGMODESCOMMAND_H_

#include <Command.h>
#include <SAL_m1m3C.h>
#include <DataTypes.h>

namespace LSST {
namespace M1M3 {
namespace SS {

class ApplyAOSCorrectionByBendingModesCommand: public Command {
private:
	Context* context;
	M1M3SSPublisher* publisher;
	int32_t commandID;
	m1m3_command_ApplyAOSCorrectionByBendingModesC data;

public:
	ApplyAOSCorrectionByBendingModesCommand(Context* context, M1M3SSPublisher* publisher, int32_t commandID, m1m3_command_ApplyAOSCorrectionByBendingModesC* data);

	int32_t getCommandID() { return this->commandID; }
	m1m3_command_ApplyAOSCorrectionByBendingModesC* getData() { return &this->data; }

	bool validate();
	void execute();
	void ackInProgress();
	void ackComplete();
	void ackFailed(std::string reason);
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* APPLYAOSCORRECTIONBYBENDINGMODESCOMMAND_H_ */
