/*
 * ClearAberrationCommand.h
 *
 *  Created on: Oct 26, 2017
 *      Author: ccontaxis
 */

#ifndef CLEARABERRATIONCOMMAND_H_
#define CLEARABERRATIONCOMMAND_H_

#include <Command.h>
#include <SAL_m1m3C.h>
#include <DataTypes.h>

namespace LSST {
namespace M1M3 {
namespace SS {

class ClearAberrationCommand: public Command {
private:
	Context* context;
	M1M3SSPublisher* publisher;
	int32_t commandID;
	m1m3_command_ClearAberrationC data;

public:
	ClearAberrationCommand(Context* context, M1M3SSPublisher* publisher, int32_t commandID, m1m3_command_ClearAberrationC* data);

	int32_t getCommandID() { return this->commandID; }
	m1m3_command_ClearAberrationC* getData() { return &this->data; }

	bool validate();
	void execute();
	void ackInProgress();
	void ackComplete();
	void ackFailed(std::string reason);
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* CLEARABERRATIONCOMMAND_H_ */
