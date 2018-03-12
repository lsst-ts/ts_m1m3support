/*
 * ClearActiveOpticForcesCommand.h
 *
 *  Created on: Oct 26, 2017
 *      Author: ccontaxis
 */

#ifndef CLEARACTIVEOPTICFORCESCOMMAND_H_
#define CLEARACTIVEOPTICFORCESCOMMAND_H_

#include <Command.h>
#include <SAL_m1m3C.h>
#include <DataTypes.h>

namespace LSST {
namespace M1M3 {
namespace SS {

class ClearActiveOpticForcesCommand: public Command {
private:
	Context* context;
	M1M3SSPublisher* publisher;
	m1m3_command_ClearActiveOpticForcesC data;

public:
	ClearActiveOpticForcesCommand(Context* context, M1M3SSPublisher* publisher, int32_t commandID, m1m3_command_ClearActiveOpticForcesC* data);

	m1m3_command_ClearActiveOpticForcesC* getData() { return &this->data; }

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
