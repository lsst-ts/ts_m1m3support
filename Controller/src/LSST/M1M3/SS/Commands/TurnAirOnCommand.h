/*
 * TurnAirOnCommand.h
 *
 *  Created on: Oct 16, 2017
 *      Author: ccontaxis
 */

#ifndef TURNAIRONCOMMAND_H_
#define TURNAIRONCOMMAND_H_

#include <SALCommand.h>
#include <SAL_MTM1M3C.h>
#include <DataTypes.h>

namespace LSST {
namespace M1M3 {
namespace SS {

class TurnAirOnCommand : public SALCommand<MTM1M3_command_turnAirOnC> {
   public:
    TurnAirOnCommand(Context* context, M1M3SSPublisher* publisher, int32_t commandID, MTM1M3_command_turnAirOnC* data);

    void execute();
    void ack(int32_t ack, int32_t errorCode, std::string reason);
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* TURNAIRONCOMMAND_H_ */
