/*
 * ApplyAberrationByForcesCommand.h
 *
 *  Created on: Oct 26, 2017
 *      Author: ccontaxis
 */

#ifndef APPLYABERRATIONFORCESCOMMAND_H_
#define APPLYABERRATIONFORCESCOMMAND_H_

#include <Command.h>
#include <SAL_m1m3C.h>
#include <DataTypes.h>

namespace LSST {
namespace M1M3 {
namespace SS {

class ApplyAberrationForcesCommand: public Command {
private:
	Context* context;
	M1M3SSPublisher* publisher;
	m1m3_command_ApplyAberrationForcesC data;

public:
	ApplyAberrationForcesCommand(Context* context, M1M3SSPublisher* publisher, int32_t commandID, m1m3_command_ApplyAberrationForcesC* data);

	m1m3_command_ApplyAberrationForcesC* getData() { return &this->data; }

	bool validate();
	void execute();
	void ackInProgress();
	void ackComplete();
	void ackFailed(std::string reason);
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* APPLYABERRATIONFORCESCOMMAND_H_ */
