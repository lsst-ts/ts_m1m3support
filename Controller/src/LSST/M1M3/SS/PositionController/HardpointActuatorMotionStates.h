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
		Standby = m1m3::m1m3_shared_HardpointMotionStates_Standby,
		Chasing = m1m3::m1m3_shared_HardpointMotionStates_Chasing,
		Moving = m1m3::m1m3_shared_HardpointMotionStates_Moving
	};
};

#endif /* HARDPOINTACTUATORMOTIONSTATES_H_ */
