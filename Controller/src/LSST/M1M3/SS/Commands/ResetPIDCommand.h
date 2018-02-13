/*
 * ResetPIDCommand.h
 *
 *  Created on: Feb 13, 2018
 *      Author: ccontaxis
 */

#ifndef RESETPIDCOMMAND_H_
#define RESETPIDCOMMAND_H_

#include <Command.h>
#include <SAL_m1m3C.h>
#include <DataTypes.h>

namespace LSST {
namespace M1M3 {
namespace SS {

class ResetPIDCommand: public Command {
private:
	Context* context;
	M1M3SSPublisher* publisher;
	int32_t commandID;
	m1m3_command_ResetPIDC data;

public:
	ResetPIDCommand(Context* context, M1M3SSPublisher* publisher, int32_t commandID, m1m3_command_ResetPIDC* data);

	int32_t getCommandID() { return this->commandID; }
	m1m3_command_ResetPIDC* getData() { return &this->data; }

	bool validate();
	void execute();
	void ackInProgress();
	void ackComplete();
	void ackFailed(std::string reason);
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* RESETPIDCOMMAND_H_ */
