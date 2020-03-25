/*
 * ClearActiveOpticForcesCommand.h
 *
 *  Created on: Oct 26, 2017
 *      Author: ccontaxis
 */

#ifndef CLEARACTIVEOPTICFORCESCOMMAND_H_
#define CLEARACTIVEOPTICFORCESCOMMAND_H_

#include <Command.h>
#include <SAL_MTM1M3C.h>
#include <DataTypes.h>

namespace LSST {
namespace M1M3 {
namespace SS {

class ClearActiveOpticForcesCommand: public Command {
private:
	Context* context;
	M1M3SSPublisher* publisher;
	MTM1M3_command_clearActiveOpticForcesC data;

public:
	ClearActiveOpticForcesCommand(Context* context, M1M3SSPublisher* publisher, int32_t commandID, MTM1M3_command_clearActiveOpticForcesC* data);

	MTM1M3_command_clearActiveOpticForcesC* getData() { return &this->data; }

	bool validate();
	void execute();
	void ackInProgress();
	void ackComplete();
	void ackFailed(std::string reason);
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* CLEARACTIVEOPTICFORCESCOMMAND_H_ */
