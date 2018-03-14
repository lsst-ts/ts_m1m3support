/*
 * ClearAberrationForcesCommand.h
 *
 *  Created on: Oct 26, 2017
 *      Author: ccontaxis
 */

#ifndef CLEARABERRATIONFORCESCOMMAND_H_
#define CLEARABERRATIONFORCESCOMMAND_H_

#include <Command.h>
#include <SAL_m1m3C.h>
#include <DataTypes.h>

namespace LSST {
namespace M1M3 {
namespace SS {

class ClearAberrationForcesCommand: public Command {
private:
	Context* context;
	M1M3SSPublisher* publisher;
	m1m3_command_ClearAberrationForcesC data;

public:
	ClearAberrationForcesCommand(Context* context, M1M3SSPublisher* publisher, int32_t commandID, m1m3_command_ClearAberrationForcesC* data);

	m1m3_command_ClearAberrationForcesC* getData() { return &this->data; }

	bool validate();
	void execute();
	void ackInProgress();
	void ackComplete();
	void ackFailed(std::string reason);
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* CLEARABERRATIONFORCESCOMMAND_H_ */
