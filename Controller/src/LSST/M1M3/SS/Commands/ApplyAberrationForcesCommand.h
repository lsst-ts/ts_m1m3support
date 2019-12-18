/*
 * ApplyAberrationByForcesCommand.h
 *
 *  Created on: Oct 26, 2017
 *      Author: ccontaxis
 */

#ifndef APPLYABERRATIONFORCESCOMMAND_H_
#define APPLYABERRATIONFORCESCOMMAND_H_

#include <SALCommand.h>
#include <SAL_MTM1M3C.h>
#include <DataTypes.h>

namespace LSST {
namespace M1M3 {
namespace SS {

class ApplyAberrationForcesCommand : public SALCommand<MTM1M3_command_applyAberrationForcesC> {
public:
    ApplyAberrationForcesCommand(Context* context, M1M3SSPublisher* publisher, int32_t commandID, MTM1M3_command_applyAberrationForcesC* data);

    void execute();
    void ack(int32_t ack, int32_t errorCode, std::string reason);
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* APPLYABERRATIONFORCESCOMMAND_H_ */
