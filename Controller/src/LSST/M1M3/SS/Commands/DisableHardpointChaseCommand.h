/*
 * DisableHardpointChaseCommand.h
 *
 *  Created on: Oct 30, 2017
 *      Author: ccontaxis
 */

#ifndef DISABLEHARDPOINTCHASECOMMAND_H_
#define DISABLEHARDPOINTCHASECOMMAND_H_

#include <SALCommand.h>
#include <SAL_MTM1M3C.h>
#include <DataTypes.h>

namespace LSST {
namespace M1M3 {
namespace SS {

class DisableHardpointChaseCommand: public SALCommand<MTM1M3_command_disableHardpointChaseC> {
public:
	DisableHardpointChaseCommand(Context* context, M1M3SSPublisher* publisher, int32_t commandID, MTM1M3_command_disableHardpointChaseC* data);

	bool validate();
	void execute();
	void ack(int32_t ack, int32_t errorCode, std::string reason);
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* DISABLEHARDPOINTCHASECOMMAND_H_ */
