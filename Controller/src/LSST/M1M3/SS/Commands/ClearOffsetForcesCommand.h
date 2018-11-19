/*
 * ClearOffsetForcesCommand.h
 *
 *  Created on: Oct 24, 2017
 *      Author: ccontaxis
 */

#ifndef CLEAROFFSETFORCESCOMMAND_H_
#define CLEAROFFSETFORCESCOMMAND_H_

#include <SALCommand.h>
#include <SAL_MTM1M3C.h>
#include <DataTypes.h>

namespace LSST {
namespace M1M3 {
namespace SS {

class ClearOffsetForcesCommand: public SALCommand<MTM1M3_command_clearOffsetForcesC> {
public:
	ClearOffsetForcesCommand(Context* context, M1M3SSPublisher* publisher, int32_t commandID, MTM1M3_command_clearOffsetForcesC* data);

	void execute();
	void ack(int32_t ack, int32_t errorCode, std::string reason);
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* CLEAROFFSETFORCESCOMMAND_H_ */
