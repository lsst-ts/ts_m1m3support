/*
 * ForceActuatorNeighbors.h
 *
 *  Created on: Nov 6, 2017
 *      Author: ccontaxis
 */

#ifndef FORCEACTUATORNEIGHBORS_H_
#define FORCEACTUATORNEIGHBORS_H_

#include <DataTypes.h>
#include <vector>

struct ForceActuatorNeighbors {
	std::vector<int32_t> NearZIDs;
	std::vector<int32_t> FarIDs;
};

#endif /* FORCEACTUATORNEIGHBORS_H_ */
