/*
 * PositionM1M3Command.h
 *
 *  Created on: Nov 27, 2017
 *      Author: ccontaxis
 */

#ifndef POSITIONM1M3COMMAND_H_
#define POSITIONM1M3COMMAND_H_

#include <SALCommand.h>
#include <SAL_MTM1M3C.h>
#include <DataTypes.h>

namespace LSST {
namespace M1M3 {
namespace SS {

class PositionM1M3Command : public SALCommand<MTM1M3_command_positionM1M3C> {
   public:
    PositionM1M3Command(Context* context, M1M3SSPublisher* publisher, int32_t commandID, MTM1M3_command_positionM1M3C* data);

    void execute();
    void ack(int32_t ack, int32_t errorCode, std::string reason);
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* POSITIONM1M3COMMAND_H_ */
