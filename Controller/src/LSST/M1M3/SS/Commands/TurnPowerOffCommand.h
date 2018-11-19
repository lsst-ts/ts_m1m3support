/*
 * TurnPowerOffCommand.h
 *
 *  Created on: Dec 7, 2017
 *      Author: ccontaxis
 */

#ifndef TURNPOWEROFFCOMMAND_H_
#define TURNPOWEROFFCOMMAND_H_

#include <SALCommand.h>
#include <SAL_MTM1M3C.h>
#include <DataTypes.h>

namespace LSST {
namespace M1M3 {
namespace SS {

class TurnPowerOffCommand: public SALCommand<MTM1M3_command_turnPowerOffC> {
public:
	TurnPowerOffCommand(Context* context, M1M3SSPublisher* publisher, int32_t commandID, MTM1M3_command_turnPowerOffC* data);

	bool validate();
	void execute();
	void ack(int32_t ack, int32_t errorCode, std::string reason);
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* TURNPOWEROFFCOMMAND_H_ */
