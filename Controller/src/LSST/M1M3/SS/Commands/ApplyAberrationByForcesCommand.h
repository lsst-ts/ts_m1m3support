/*
 * ApplyAberrationByForcesCommand.h
 *
 *  Created on: Oct 26, 2017
 *      Author: ccontaxis
 */

#ifndef APPLYABERRATIONBYFORCESCOMMAND_H_
#define APPLYABERRATIONBYFORCESCOMMAND_H_

#include <Command.h>
#include <SAL_m1m3C.h>
#include <DataTypes.h>

namespace LSST {
namespace M1M3 {
namespace SS {

class ApplyAberrationByForcesCommand: public Command {
private:
	IContext* context;
	IPublisher* publisher;
	int32_t commandID;
	m1m3_command_ApplyAberrationByForcesC data;

public:
	ApplyAberrationByForcesCommand(IContext* context, IPublisher* publisher, int32_t commandID, m1m3_command_ApplyAberrationByForcesC* data);

	int32_t getCommandID() { return this->commandID; }
	m1m3_command_ApplyAberrationByForcesC* getData() { return &this->data; }

	bool validate();
	void execute();
	void ackInProgress();
	void ackComplete();
	void ackFailed(std::string reason);
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* APPLYABERRATIONBYFORCESCOMMAND_H_ */
