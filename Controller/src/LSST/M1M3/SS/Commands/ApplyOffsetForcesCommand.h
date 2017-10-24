/*
 * ApplyOffsetForcesCommand.h
 *
 *  Created on: Oct 24, 2017
 *      Author: ccontaxis
 */

#ifndef APPLYOFFSETFORCESCOMMAND_H_
#define APPLYOFFSETFORCESCOMMAND_H_

#include <Command.h>
#include <SAL_m1m3C.h>
#include <DataTypes.h>

namespace LSST {
namespace M1M3 {
namespace SS {

class ApplyOffsetForcesCommand: public Command {
private:
	IContext* context;
	IPublisher* publisher;
	int32_t commandID;
	m1m3_command_ApplyOffsetForcesC data;

public:
	ApplyOffsetForcesCommand(IContext* context, IPublisher* publisher, int32_t commandID, m1m3_command_ApplyOffsetForcesC* data);

	int32_t getCommandID() { return this->commandID; }
	m1m3_command_ApplyOffsetForcesC* getData() { return &this->data; }

	bool validate();
	void execute();
	void ackInProgress();
	void ackComplete();
	void ackFailed(std::string reason);
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* APPLYOFFSETFORCESCOMMAND_H_ */
