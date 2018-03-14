/*
 * UpdatePIDCommand.h
 *
 *  Created on: Feb 13, 2018
 *      Author: ccontaxis
 */

#ifndef UPDATEPIDCOMMAND_H_
#define UPDATEPIDCOMMAND_H_

#include <Command.h>
#include <SAL_m1m3C.h>
#include <DataTypes.h>

namespace LSST {
namespace M1M3 {
namespace SS {

class UpdatePIDCommand: public Command {
private:
	Context* context;
	M1M3SSPublisher* publisher;
	m1m3_command_UpdatePIDC data;

public:
	UpdatePIDCommand(Context* context, M1M3SSPublisher* publisher, int32_t commandID, m1m3_command_UpdatePIDC* data);

	m1m3_command_UpdatePIDC* getData() { return &this->data; }

	bool validate();
	void execute();
	void ackInProgress();
	void ackComplete();
	void ackFailed(std::string reason);
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* UPDATEPIDCOMMAND_H_ */
