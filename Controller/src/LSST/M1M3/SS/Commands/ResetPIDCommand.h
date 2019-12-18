/*
 * ResetPIDCommand.h
 *
 *  Created on: Feb 13, 2018
 *      Author: ccontaxis
 */

#ifndef RESETPIDCOMMAND_H_
#define RESETPIDCOMMAND_H_

#include <SALCommand.h>
#include <SAL_MTM1M3C.h>
#include <DataTypes.h>

namespace LSST {
namespace M1M3 {
namespace SS {

class ResetPIDCommand : public SALCommand<MTM1M3_command_resetPIDC> {
public:
    ResetPIDCommand(Context* context, M1M3SSPublisher* publisher, int32_t commandID, MTM1M3_command_resetPIDC* data);

    bool validate();
    void execute();
    void ack(int32_t ack, int32_t errorCode, std::string reason);
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* RESETPIDCOMMAND_H_ */
