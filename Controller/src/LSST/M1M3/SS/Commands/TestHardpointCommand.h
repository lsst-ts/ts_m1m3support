/*
 * TestHardpointCommand.h
 *
 *  Created on: Oct 30, 2017
 *      Author: ccontaxis
 */

#ifndef TESTHARDPOINTCOMMAND_H_
#define TESTHARDPOINTCOMMAND_H_

#include <SALCommand.h>
#include <SAL_MTM1M3C.h>
#include <DataTypes.h>

namespace LSST {
namespace M1M3 {
namespace SS {

class TestHardpointCommand : public SALCommand<MTM1M3_command_testHardpointC> {
   public:
    TestHardpointCommand(Context* context, M1M3SSPublisher* publisher, int32_t commandID, MTM1M3_command_testHardpointC* data);

    bool validate();
    void execute();
    void ack(int32_t ack, int32_t errorCode, std::string reason);
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* TESTHARDPOINTCOMMAND_H_ */
