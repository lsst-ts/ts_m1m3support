/*
 * TestAirCommand.h
 *
 *  Created on: Oct 30, 2017
 *      Author: ccontaxis
 */

#ifndef TESTAIRCOMMAND_H_
#define TESTAIRCOMMAND_H_

#include <SALCommand.h>
#include <SAL_MTM1M3C.h>
#include <DataTypes.h>

namespace LSST {
namespace M1M3 {
namespace SS {

class TestAirCommand : public SALCommand<MTM1M3_command_testAirC> {
public:
    TestAirCommand(Context* context, M1M3SSPublisher* publisher, int32_t commandID, MTM1M3_command_testAirC* data);

    void execute();
    void ack(int32_t ack, int32_t errorCode, std::string reason);
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* TESTAIRCOMMAND_H_ */
