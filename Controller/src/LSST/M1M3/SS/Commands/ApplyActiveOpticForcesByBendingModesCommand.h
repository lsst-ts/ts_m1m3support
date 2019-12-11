/*
 * ApplyActiveOpticForcesByBendingModesCommand.h
 *
 *  Created on: Oct 26, 2017
 *      Author: ccontaxis
 */

#ifndef APPLYACTIVEOPTICFORCESBYBENDINGMODESCOMMAND_H_
#define APPLYACTIVEOPTICFORCESBYBENDINGMODESCOMMAND_H_

#include <SALCommand.h>
#include <SAL_MTM1M3C.h>
#include <DataTypes.h>

namespace LSST {
namespace M1M3 {
namespace SS {

class ApplyActiveOpticForcesByBendingModesCommand : public SALCommand<MTM1M3_command_applyActiveOpticForcesByBendingModesC> {
   public:
    ApplyActiveOpticForcesByBendingModesCommand(Context* context, M1M3SSPublisher* publisher, int32_t commandID, MTM1M3_command_applyActiveOpticForcesByBendingModesC* data);

    void execute();
    void ack(int32_t ack, int32_t errorCode, std::string reason);
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* APPLYACTIVEOPTICFORCESBYBENDINGMODESCOMMAND_H_ */
