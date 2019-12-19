/*
 * ApplyOffsetForcesCommand.h
 *
 *  Created on: Oct 24, 2017
 *      Author: ccontaxis
 */

#ifndef APPLYOFFSETFORCESCOMMAND_H_
#define APPLYOFFSETFORCESCOMMAND_H_

#include <SALCommand.h>
#include <SAL_MTM1M3C.h>
#include <DataTypes.h>

namespace LSST {
namespace M1M3 {
namespace SS {

class ApplyOffsetForcesCommand : public SALCommand<MTM1M3_command_applyOffsetForcesC> {
public:
    ApplyOffsetForcesCommand(Context* context, M1M3SSPublisher* publisher, int32_t commandID, MTM1M3_command_applyOffsetForcesC* data);

    void execute();
    void ack(int32_t ack, int32_t errorCode, std::string reason);
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* APPLYOFFSETFORCESCOMMAND_H_ */
