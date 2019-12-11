/*
 * TurnAirOffCommand.h
 *
 *  Created on: Oct 16, 2017
 *      Author: ccontaxis
 */

#ifndef TURNAIROFFCOMMAND_H_
#define TURNAIROFFCOMMAND_H_

#include <SALCommand.h>
#include <SAL_MTM1M3C.h>
#include <DataTypes.h>

namespace LSST {
namespace M1M3 {
namespace SS {

class TurnAirOffCommand : public SALCommand<MTM1M3_command_turnAirOffC> {
   public:
    TurnAirOffCommand(Context* context, M1M3SSPublisher* publisher, int32_t commandID, MTM1M3_command_turnAirOffC* data);

    void execute();
    void ack(int32_t ack, int32_t errorCode, std::string reason);
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* TURNAIROFFCOMMAND_H_ */
