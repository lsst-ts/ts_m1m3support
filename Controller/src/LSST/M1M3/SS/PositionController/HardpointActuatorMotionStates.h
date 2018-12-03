/*
 * HardpointActuatorMotionStates.h
 *
 *  Created on: Oct 31, 2017
 *      Author: ccontaxis
 */

#ifndef HARDPOINTACTUATORMOTIONSTATES_H_
#define HARDPOINTACTUATORMOTIONSTATES_H_

#include <ccpp_sal_MTM1M3.h> // Provides access to enumerations

struct HardpointActuatorMotionStates {
	enum Type {
		Standby = MTM1M3::MTM1M3_shared_HardpointActuatorMotionStates_Standby,
		Chasing = MTM1M3::MTM1M3_shared_HardpointActuatorMotionStates_Chasing,
		Stepping = MTM1M3::MTM1M3_shared_HardpointActuatorMotionStates_Stepping,
		QuickPositioning = MTM1M3::MTM1M3_shared_HardpointActuatorMotionStates_QuickPositioning,
		FinePositioning = MTM1M3::MTM1M3_shared_HardpointActuatorMotionStates_FinePositioning,
	};
};

#endif /* HARDPOINTACTUATORMOTIONSTATES_H_ */
