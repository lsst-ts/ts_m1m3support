/*
 * EnableHardpointChaseCommand.h
 *
 *  Created on: Oct 30, 2017
 *      Author: ccontaxis
 */

#ifndef ENABLEHARDPOINTCHASECOMMAND_H_
#define ENABLEHARDPOINTCHASECOMMAND_H_

#include <SALCommand.h>
#include <SAL_MTM1M3C.h>
#include <DataTypes.h>

namespace LSST {
namespace M1M3 {
namespace SS {

class EnableHardpointChaseCommand : public SALCommand<MTM1M3_command_enableHardpointChaseC> {
   public:
    EnableHardpointChaseCommand(Context* context, M1M3SSPublisher* publisher, int32_t commandID, MTM1M3_command_enableHardpointChaseC* data);

    bool validate();
    void execute();
    void ack(int32_t ack, int32_t errorCode, std::string reason);
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* ENABLEHARDPOINTCHASECOMMAND_H_ */
