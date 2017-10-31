/*
 * TestForceActuatorCommand.h
 *
 *  Created on: Oct 30, 2017
 *      Author: ccontaxis
 */

#ifndef TESTFORCEACTUATORCOMMAND_H_
#define TESTFORCEACTUATORCOMMAND_H_

#include <Command.h>
#include <SAL_m1m3C.h>
#include <DataTypes.h>

namespace LSST {
namespace M1M3 {
namespace SS {

class TestForceActuatorCommand: public Command {
private:
	IContext* context;
	IPublisher* publisher;
	int32_t commandID;
	m1m3_command_TestForceActuatorC data;

public:
	TestForceActuatorCommand(IContext* context, IPublisher* publisher, int32_t commandID, m1m3_command_TestForceActuatorC* data);

	int32_t getCommandID() { return this->commandID; }
	m1m3_command_TestForceActuatorC* getData() { return &this->data; }

	bool validate();
	void execute();
	void ackInProgress();
	void ackComplete();
	void ackFailed(std::string reason);
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* TESTFORCEACTUATORCOMMAND_H_ */
