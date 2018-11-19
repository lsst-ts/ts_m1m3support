/*
 * SALCommand.h
 *
 *  Created on: Nov 15, 2018
 *      Author: ccontaxis
 */

#ifndef LSST_M1M3_SS_COMMANDS_SALCOMMAND_H_
#define LSST_M1M3_SS_COMMANDS_SALCOMMAND_H_

#include <Command.h>

namespace LSST {
namespace M1M3 {
namespace SS {

template<class topic> class SALCommand: public Command {
protected:
	int32_t commandID;
	M1M3SSPublisher* publisher;
	topic data;

public:
	SALCommand(Context* context, M1M3SSPublisher* publisher, int32_t commandID, topic* data) {
		this->context = context;
		this->publisher = publisher;
		this->commandID = commandID;
		this->data = *data;
	}

	topic* getData() { return &this->data; }
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* LSST_M1M3_SS_COMMANDS_SALCOMMAND_H_ */
