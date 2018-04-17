/*
 * ApplyAberrationForcesByBendingModesCommand.h
 *
 *  Created on: Oct 26, 2017
 *      Author: ccontaxis
 */

#ifndef APPLYABERRATIONFORCESBYBENDINGMODESCOMMAND_H_
#define APPLYABERRATIONFORCESBYBENDINGMODESCOMMAND_H_

#include <Command.h>
#include <SAL_m1m3C.h>
#include <DataTypes.h>

namespace LSST {
namespace M1M3 {
namespace SS {

class ApplyAberrationForcesByBendingModesCommand: public Command {
private:
	Context* context;
	M1M3SSPublisher* publisher;
	m1m3_command_ApplyAberrationForcesByBendingModesC data;

public:
	ApplyAberrationForcesByBendingModesCommand(Context* context, M1M3SSPublisher* publisher, int32_t commandID, m1m3_command_ApplyAberrationForcesByBendingModesC* data);

	m1m3_command_ApplyAberrationForcesByBendingModesC* getData() { return &this->data; }

	bool validate();
	void execute();
	void ackInProgress();
	void ackComplete();
	void ackFailed(std::string reason);
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* APPLYABERRATIONFORCESBYBENDINGMODESCOMMAND_H_ */
