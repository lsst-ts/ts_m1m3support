/*
 * AbortRaiseM1M3Command.h
 *
 *  Created on: Oct 30, 2017
 *      Author: ccontaxis
 */

#ifndef ABORTRAISEM1M3COMMAND_H_
#define ABORTRAISEM1M3COMMAND_H_

#include <Command.h>
#include <SAL_MTM1M3C.h>
#include <DataTypes.h>

namespace LSST {
namespace M1M3 {
namespace SS {

class AbortRaiseM1M3Command: public Command {
private:
	Context* context;
	M1M3SSPublisher* publisher;
	MTM1M3_command_abortRaiseM1M3C data;

public:
	AbortRaiseM1M3Command(Context* context, M1M3SSPublisher* publisher, int32_t commandID, MTM1M3_command_abortRaiseM1M3C* data);

	MTM1M3_command_abortRaiseM1M3C* getData() { return &this->data; }

	bool validate();
	void execute();
	void ackInProgress();
	void ackComplete();
	void ackFailed(std::string reason);
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* ABORTRAISEM1M3COMMAND_H_ */
