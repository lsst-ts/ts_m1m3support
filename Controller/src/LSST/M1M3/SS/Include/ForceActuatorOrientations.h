/*
 * ForceActuatorOrientations.h
 *
 *  Created on: Oct 25, 2017
 *      Author: ccontaxis
 */

#ifndef FORCEACTUATORORIENTATIONS_H_
#define FORCEACTUATORORIENTATIONS_H_

#include <ccpp_sal_m1m3.h> // Provides access to enumerations

struct ForceActuatorOrientations {
	enum Type {
		NA = m1m3::m1m3_shared_ForceActuatorOrientations_NA,
		PositiveY = m1m3::m1m3_shared_ForceActuatorOrientations_PositiveY,
		NegativeY = m1m3::m1m3_shared_ForceActuatorOrientations_NegativeY,
		PositiveX = m1m3::m1m3_shared_ForceActuatorOrientations_PositiveX,
		NegativeX = m1m3::m1m3_shared_ForceActuatorOrientations_NegativeX
	};
};

#endif /* FORCEACTUATORORIENTATIONS_H_ */
