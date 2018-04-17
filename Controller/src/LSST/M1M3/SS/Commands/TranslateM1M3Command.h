/*
 * TranslateM1M3Command.h
 *
 *  Created on: Oct 30, 2017
 *      Author: ccontaxis
 */

#ifndef TRANSLATEM1M3COMMAND_H_
#define TRANSLATEM1M3COMMAND_H_

#include <Command.h>
#include <SAL_m1m3C.h>
#include <DataTypes.h>

namespace LSST {
namespace M1M3 {
namespace SS {

class TranslateM1M3Command: public Command {
private:
	Context* context;
	M1M3SSPublisher* publisher;
	m1m3_command_TranslateM1M3C data;

public:
	TranslateM1M3Command(Context* context, M1M3SSPublisher* publisher, int32_t commandID, m1m3_command_TranslateM1M3C* data);

	m1m3_command_TranslateM1M3C* getData() { return &this->data; }

	bool validate();
	void execute();
	void ackInProgress();
	void ackComplete();
	void ackFailed(std::string reason);
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* TRANSLATEM1M3COMMAND_H_ */
