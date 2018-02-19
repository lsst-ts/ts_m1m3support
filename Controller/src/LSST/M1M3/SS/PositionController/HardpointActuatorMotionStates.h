/*
 * HardpointActuatorMotionStates.h
 *
 *  Created on: Oct 31, 2017
 *      Author: ccontaxis
 */

#ifndef HARDPOINTACTUATORMOTIONSTATES_H_
#define HARDPOINTACTUATORMOTIONSTATES_H_

struct HardpointActuatorMotionStates {
	enum Type {
		Standby = 1,
		Chasing = 2,
		Moving = 3
	};
};

#endif /* HARDPOINTACTUATORMOTIONSTATES_H_ */
