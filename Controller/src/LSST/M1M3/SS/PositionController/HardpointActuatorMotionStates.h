/*
 * HardpointActuatorMotionStates.h
 *
 *  Created on: Oct 31, 2017
 *      Author: ccontaxis
 */

#ifndef HARDPOINTACTUATORMOTIONSTATES_H_
#define HARDPOINTACTUATORMOTIONSTATES_H_

#include <ccpp_sal_m1m3.h> // Provides access to enumerations

struct HardpointActuatorMotionStates {
	enum Type {
		Standby = 0,
		Chasing = 1,
		Moving = 2
	};
};

#endif /* HARDPOINTACTUATORMOTIONSTATES_H_ */
