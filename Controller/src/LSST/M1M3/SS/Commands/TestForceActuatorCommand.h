/*
 * TestForceActuatorCommand.h
 *
 *  Created on: Oct 30, 2017
 *      Author: ccontaxis
 */

#ifndef TESTFORCEACTUATORCOMMAND_H_
#define TESTFORCEACTUATORCOMMAND_H_

#include <SALCommand.h>
#include <SAL_MTM1M3C.h>
#include <DataTypes.h>

namespace LSST {
namespace M1M3 {
namespace SS {

class TestForceActuatorCommand : public SALCommand<MTM1M3_command_testForceActuatorC> {
   public:
    TestForceActuatorCommand(Context* context, M1M3SSPublisher* publisher, int32_t commandID, MTM1M3_command_testForceActuatorC* data);

    bool validate();
    void execute();
    void ack(int32_t ack, int32_t errorCode, std::string reason);
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* TESTFORCEACTUATORCOMMAND_H_ */
