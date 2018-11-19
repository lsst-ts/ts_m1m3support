/*
 * ProgramILCCommand.h
 *
 *  Created on: May 9, 2018
 *      Author: ccontaxis
 */

#ifndef PROGRAMILCCOMMAND_H_
#define PROGRAMILCCOMMAND_H_

#include <SALCommand.h>
#include <SAL_MTM1M3C.h>
#include <DataTypes.h>


namespace LSST {
namespace M1M3 {
namespace SS {

class ProgramILCCommand: public SALCommand<MTM1M3_command_programILCC> {
public:
	ProgramILCCommand(Context* context, M1M3SSPublisher* publisher, int32_t commandID, MTM1M3_command_programILCC* data);

	void execute();
	void ack(int32_t ack, int32_t errorCode, std::string reason);
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* PROGRAMILCCOMMAND_H_ */
