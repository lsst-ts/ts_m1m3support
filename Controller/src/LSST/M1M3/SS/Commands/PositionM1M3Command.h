/*
 * PositionM1M3Command.h
 *
 *  Created on: Nov 27, 2017
 *      Author: ccontaxis
 */

#ifndef POSITIONM1M3COMMAND_H_
#define POSITIONM1M3COMMAND_H_

#include <Command.h>
#include <SAL_m1m3C.h>
#include <DataTypes.h>

namespace LSST {
namespace M1M3 {
namespace SS {

class PositionM1M3Command: public Command {
private:
	IContext* context;
	IPublisher* publisher;
	int32_t commandID;
	m1m3_command_PositionM1M3C data;

public:
	PositionM1M3Command(IContext* context, IPublisher* publisher, int32_t commandID, m1m3_command_PositionM1M3C* data);

	int32_t getCommandID() { return this->commandID; }
	m1m3_command_PositionM1M3C* getData() { return &this->data; }

	bool validate();
	void execute();
	void ackInProgress();
	void ackComplete();
	void ackFailed(std::string reason);
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* POSITIONM1M3COMMAND_H_ */
