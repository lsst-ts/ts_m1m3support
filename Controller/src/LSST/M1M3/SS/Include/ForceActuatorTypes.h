/*
 * ForceActuatorTypes.h
 *
 *  Created on: Oct 25, 2017
 *      Author: ccontaxis
 */

#ifndef FORCEACTUATORTYPES_H_
#define FORCEACTUATORTYPES_H_

#include <ccpp_sal_m1m3.h> // Provides access to enumerations

struct ForceActuatorTypes {
	enum Type {
		SAA = m1m3::m1m3_shared_ForceActuatorTypes_SAA,
		DAA = m1m3::m1m3_shared_ForceActuatorTypes_DAA
	};
};

#endif /* FORCEACTUATORTYPES_H_ */
