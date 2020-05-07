/*
 * ForceActuatorOrientations.h
 *
 *  Created on: Oct 25, 2017
 *      Author: ccontaxis
 */

#ifndef FORCEACTUATORORIENTATIONS_H_
#define FORCEACTUATORORIENTATIONS_H_

struct ForceActuatorOrientations {
	enum Type {
		NA = 0,
		PositiveY = 1,
		NegativeY = 2,
		PositiveX = 3,
		NegativeX = 4
	};
};

#endif /* FORCEACTUATORORIENTATIONS_H_ */
