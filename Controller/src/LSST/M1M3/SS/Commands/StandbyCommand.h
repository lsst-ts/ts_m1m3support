/*
 * StandbyCommand.h
 *
 *  Created on: Sep 26, 2017
 *      Author: ccontaxis
 */

#ifndef STANDBYCOMMAND_H_
#define STANDBYCOMMAND_H_

#include <SALCommand.h>
#include <SAL_MTM1M3C.h>
#include <DataTypes.h>

namespace LSST {
namespace M1M3 {
namespace SS {

/*!
 * This command is responsible for transitioning the state
 * machine from the disabled state to the standby state.
 * This is an external command and can be issued via SAL.
 */
class StandbyCommand: public SALCommand<MTM1M3_command_standbyC> {
public:
	StandbyCommand(Context* context, M1M3SSPublisher* publisher, int32_t commandID, MTM1M3_command_standbyC* data);

	void execute();
	void ack(int32_t ack, int32_t errorCode, std::string reason);
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* STANDBYCOMMAND_H_ */
