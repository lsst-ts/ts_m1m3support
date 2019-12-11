/*
 * DisableHardpointCorrectionsCommand.h
 *
 *  Created on: Feb 8, 2018
 *      Author: ccontaxis
 */

#ifndef DISABLEHARDPOINTCORRECTIONSCOMMAND_H_
#define DISABLEHARDPOINTCORRECTIONSCOMMAND_H_

#include <SALCommand.h>
#include <SAL_MTM1M3C.h>
#include <DataTypes.h>

namespace LSST {
namespace M1M3 {
namespace SS {

class DisableHardpointCorrectionsCommand : public SALCommand<MTM1M3_command_disableHardpointCorrectionsC> {
   public:
    DisableHardpointCorrectionsCommand(Context* context, M1M3SSPublisher* publisher, int32_t commandID, MTM1M3_command_disableHardpointCorrectionsC* data);

    void execute();
    void ack(int32_t ack, int32_t errorCode, std::string reason);
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* DISABLEHARDPOINTCORRECTIONSCOMMAND_H_ */
