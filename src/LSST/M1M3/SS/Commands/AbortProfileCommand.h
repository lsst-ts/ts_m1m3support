/*
 * AbortProfileCommand.h
 *
 *  Created on: Feb 9, 2018
 *      Author: ccontaxis
 */

#ifndef ABORTPROFILECOMMAND_H_
#define ABORTPROFILECOMMAND_H_

#include <Command.h>
#include <SAL_MTM1M3C.h>
#include <DataTypes.h>

namespace LSST {
namespace M1M3 {
namespace SS {

class AbortProfileCommand: public Command {
private:
	Context* context;
	M1M3SSPublisher* publisher;
	MTM1M3_command_abortProfileC data;

public:
	AbortProfileCommand(Context* context, M1M3SSPublisher* publisher, int32_t commandID, MTM1M3_command_abortProfileC* data);

	MTM1M3_command_abortProfileC* getData() { return &this->data; }

	bool validate();
	void execute();
	void ackInProgress();
	void ackComplete();
	void ackFailed(std::string reason);
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* ABORTPROFILECOMMAND_H_ */
