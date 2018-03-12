/*
 * AbortProfileCommand.h
 *
 *  Created on: Feb 9, 2018
 *      Author: ccontaxis
 */

#ifndef ABORTPROFILECOMMAND_H_
#define ABORTPROFILECOMMAND_H_

#include <Command.h>
#include <SAL_m1m3C.h>
#include <DataTypes.h>

namespace LSST {
namespace M1M3 {
namespace SS {

class AbortProfileCommand: public Command {
private:
	Context* context;
	M1M3SSPublisher* publisher;
	m1m3_command_AbortProfileC data;

public:
	AbortProfileCommand(Context* context, M1M3SSPublisher* publisher, int32_t commandID, m1m3_command_AbortProfileC* data);

	m1m3_command_AbortProfileC* getData() { return &this->data; }

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
