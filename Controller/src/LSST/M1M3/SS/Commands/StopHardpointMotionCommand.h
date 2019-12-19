/*
 * StopHardpointMotionCommand.h
 *
 *  Created on: Oct 31, 2017
 *      Author: ccontaxis
 */

#ifndef STOPHARDPOINTMOTIONCOMMAND_H_
#define STOPHARDPOINTMOTIONCOMMAND_H_

#include <SALCommand.h>
#include <SAL_MTM1M3C.h>
#include <DataTypes.h>

namespace LSST {
namespace M1M3 {
namespace SS {

class StopHardpointMotionCommand : public SALCommand<MTM1M3_command_stopHardpointMotionC> {
public:
    StopHardpointMotionCommand(Context* context, M1M3SSPublisher* publisher, int32_t commandID, MTM1M3_command_stopHardpointMotionC* data);

    void execute();
    void ack(int32_t ack, int32_t errorCode, std::string reason);
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* STOPHARDPOINTMOTIONCOMMAND_H_ */
