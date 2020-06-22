/*
 * HardpointActuatorMotionStates.h
 *
 *  Created on: Oct 31, 2017
 *      Author: ccontaxis
 */

#ifndef HARDPOINTACTUATORMOTIONSTATES_H_
#define HARDPOINTACTUATORMOTIONSTATES_H_

#include <ccpp_sal_MTM1M3.h>  // Provides access to enumerations

struct HardpointActuatorMotionStates {
    enum Type {
        Standby = 0,
        Chasing = 1,
        Stepping = 2,
        QuickPositioning = 3,
        FinePositioning = 4,
    };
};

#endif /* HARDPOINTACTUATORMOTIONSTATES_H_ */
