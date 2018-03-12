/*
 * StopHardpointMotionCommand.h
 *
 *  Created on: Oct 31, 2017
 *      Author: ccontaxis
 */

#ifndef STOPHARDPOINTMOTIONCOMMAND_H_
#define STOPHARDPOINTMOTIONCOMMAND_H_

#include <Command.h>
#include <SAL_m1m3C.h>
#include <DataTypes.h>

namespace LSST {
namespace M1M3 {
namespace SS {

class StopHardpointMotionCommand: public Command {
private:
	Context* context;
	M1M3SSPublisher* publisher;
	m1m3_command_StopHardpointMotionC data;

public:
	StopHardpointMotionCommand(Context* context, M1M3SSPublisher* publisher, int32_t commandID, m1m3_command_StopHardpointMotionC* data);

	m1m3_command_StopHardpointMotionC* getData() { return &this->data; }

	bool validate();
	void execute();
	void ackInProgress();
	void ackComplete();
	void ackFailed(std::string reason);
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* STOPHARDPOINTMOTIONCOMMAND_H_ */
