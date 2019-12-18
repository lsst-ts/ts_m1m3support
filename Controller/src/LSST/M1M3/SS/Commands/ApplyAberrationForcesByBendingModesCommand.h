/*
 * ApplyAberrationForcesByBendingModesCommand.h
 *
 *  Created on: Oct 26, 2017
 *      Author: ccontaxis
 */

#ifndef APPLYABERRATIONFORCESBYBENDINGMODESCOMMAND_H_
#define APPLYABERRATIONFORCESBYBENDINGMODESCOMMAND_H_

#include <SALCommand.h>
#include <SAL_MTM1M3C.h>
#include <DataTypes.h>

namespace LSST {
namespace M1M3 {
namespace SS {

class ApplyAberrationForcesByBendingModesCommand : public SALCommand<MTM1M3_command_applyAberrationForcesByBendingModesC> {
public:
    ApplyAberrationForcesByBendingModesCommand(Context* context, M1M3SSPublisher* publisher, int32_t commandID, MTM1M3_command_applyAberrationForcesByBendingModesC* data);

    void execute();
    void ack(int32_t ack, int32_t errorCode, std::string reason);
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* APPLYABERRATIONFORCESBYBENDINGMODESCOMMAND_H_ */
