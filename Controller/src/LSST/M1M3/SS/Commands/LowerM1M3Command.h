/*
 * LowerM1M3Command.h
 *
 *  Created on: Oct 25, 2017
 *      Author: ccontaxis
 */

#ifndef LOWERM1M3COMMAND_H_
#define LOWERM1M3COMMAND_H_

#include <SALCommand.h>
#include <SAL_MTM1M3C.h>
#include <DataTypes.h>

namespace LSST {
namespace M1M3 {
namespace SS {

class LowerM1M3Command : public SALCommand<MTM1M3_command_lowerM1M3C> {
public:
    LowerM1M3Command(Context* context, M1M3SSPublisher* publisher, int32_t commandID, MTM1M3_command_lowerM1M3C* data);

    void execute();
    void ack(int32_t ack, int32_t errorCode, std::string reason);
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* LOWERM1M3COMMAND_H_ */
