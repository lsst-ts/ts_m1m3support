/*
 * DisabledState.h
 *
 *  Created on: Sep 27, 2017
 *      Author: ccontaxis
 */

#ifndef DISABLEDSTATE_H_
#define DISABLEDSTATE_H_

#include <State.h>

namespace LSST {
namespace M1M3 {
namespace SS {

class DisabledState: public State {
public:
	DisabledState(M1M3SSPublisher* publisher);

	States::Type update(UpdateCommand* command, Model* model);
	States::Type enable(EnableCommand* command, Model* model);
	States::Type standby(StandbyCommand* command, Model* model);
	States::Type programILC(ProgramILCCommand* command, Model* model);
	States::Type modbusTransmit(ModbusTransmitCommand* command, Model* model);
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* DISABLEDSTATE_H_ */
