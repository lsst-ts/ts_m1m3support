/*
 * ProgramILCCommand.h
 *
 *  Created on: May 9, 2018
 *      Author: ccontaxis
 */

#ifndef PROGRAMILCCOMMAND_H_
#define PROGRAMILCCOMMAND_H_

#include <Command.h>
#include <SAL_m1m3C.h>
#include <DataTypes.h>


namespace LSST {
namespace M1M3 {
namespace SS {

class ProgramILCCommand: public Command {
private:
	Context* context;
	M1M3SSPublisher* publisher;
	m1m3_command_ProgramILCC data;

public:
	ProgramILCCommand(Context* context, M1M3SSPublisher* publisher, int32_t commandID, m1m3_command_ProgramILCC* data);

	m1m3_command_ProgramILCC* getData() { return &this->data; }

	bool validate();
	void execute();
	void ackInProgress();
	void ackComplete();
	void ackFailed(std::string reason);
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* PROGRAMILCCOMMAND_H_ */
