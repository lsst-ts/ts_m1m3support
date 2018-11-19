/*
 * ApplyOffsetForcesByMirrorForceCommand.h
 *
 *  Created on: Feb 9, 2018
 *      Author: ccontaxis
 */

#ifndef APPLYOFFSETFORCESBYMIRRORFORCECOMMAND_H_
#define APPLYOFFSETFORCESBYMIRRORFORCECOMMAND_H_

#include <SALCommand.h>
#include <SAL_MTM1M3C.h>
#include <DataTypes.h>

namespace LSST {
namespace M1M3 {
namespace SS {

class ApplyOffsetForcesByMirrorForceCommand: public SALCommand<MTM1M3_command_applyOffsetForcesByMirrorForceC> {
public:
	ApplyOffsetForcesByMirrorForceCommand(Context* context, M1M3SSPublisher* publisher, int32_t commandID, MTM1M3_command_applyOffsetForcesByMirrorForceC* data);

	void execute();
	void ack(int32_t ack, int32_t errorCode, std::string reason);
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* APPLYOFFSETFORCESBYMIRRORFORCECOMMAND_H_ */
