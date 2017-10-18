/*
 * ParkedEngineeringState.h
 *
 *  Created on: Oct 17, 2017
 *      Author: ccontaxis
 */

#ifndef PARKEDENGINEERINGSTATE_H_
#define PARKEDENGINEERINGSTATE_H_

#include <State.h>

namespace LSST {
namespace M1M3 {
namespace SS {

class ParkedEngineeringState: public State {
public:
	States::Type disable(DisableCommand* command, IModel* model);
	States::Type update(UpdateCommand* command, IModel* model);
	States::Type turnAirOn(TurnAirOnCommand* command, IModel* model);
	States::Type turnAirOff(TurnAirOffCommand* command, IModel* model);
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* PARKEDENGINEERINGSTATE_H_ */
