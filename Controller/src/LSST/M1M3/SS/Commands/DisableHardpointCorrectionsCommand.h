/*
 * DisableHardpointCorrectionsCommand.h
 *
 *  Created on: Feb 8, 2018
 *      Author: ccontaxis
 */

#ifndef DISABLEHARDPOINTCORRECTIONSCOMMAND_H_
#define DISABLEHARDPOINTCORRECTIONSCOMMAND_H_

#include <Command.h>
#include <SAL_m1m3C.h>
#include <DataTypes.h>

namespace LSST {
namespace M1M3 {
namespace SS {

class DisableHardpointCorrectionsCommand: public Command {
private:
	Context* context;
	M1M3SSPublisher* publisher;
	m1m3_command_DisableHardpointCorrectionsC data;

public:
	DisableHardpointCorrectionsCommand(Context* context, M1M3SSPublisher* publisher, int32_t commandID, m1m3_command_DisableHardpointCorrectionsC* data);

	m1m3_command_DisableHardpointCorrectionsC* getData() { return &this->data; }

	bool validate();
	void execute();
	void ackInProgress();
	void ackComplete();
	void ackFailed(std::string reason);
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* DISABLEHARDPOINTCORRECTIONSCOMMAND_H_ */
