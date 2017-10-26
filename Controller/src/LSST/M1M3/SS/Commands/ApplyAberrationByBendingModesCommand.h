/*
 * ApplyAberrationByBendingModesCommand.h
 *
 *  Created on: Oct 26, 2017
 *      Author: ccontaxis
 */

#ifndef APPLYABERRATIONBYBENDINGMODESCOMMAND_H_
#define APPLYABERRATIONBYBENDINGMODESCOMMAND_H_

#include <Command.h>
#include <SAL_m1m3C.h>
#include <DataTypes.h>

namespace LSST {
namespace M1M3 {
namespace SS {

class ApplyAberrationByBendingModesCommand: public Command {
private:
	IContext* context;
	IPublisher* publisher;
	int32_t commandID;
	m1m3_command_ApplyAberrationByBendingModesC data;

public:
	ApplyAberrationByBendingModesCommand(IContext* context, IPublisher* publisher, int32_t commandID, m1m3_command_ApplyAberrationByBendingModesC* data);

	int32_t getCommandID() { return this->commandID; }
	m1m3_command_ApplyAberrationByBendingModesC* getData() { return &this->data; }

	bool validate();
	void execute();
	void ackInProgress();
	void ackComplete();
	void ackFailed(std::string reason);
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* APPLYABERRATIONBYBENDINGMODESCOMMAND_H_ */
