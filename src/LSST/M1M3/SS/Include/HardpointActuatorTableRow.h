/*
 * HardpointActuatorTableRow.h
 *
 *  Created on: Oct 25, 2017
 *      Author: ccontaxis
 */

#ifndef HARDPOINTACTUATORTABLEROW_H_
#define HARDPOINTACTUATORTABLEROW_H_

#include <DataTypes.h>

struct HardpointActuatorTableRow {
	int32_t Index;
	int32_t ActuatorID;
	double XPosition;
	double YPosition;
	double ZPosition;
	uint8_t Subnet;
	uint8_t Address;
	float SensorOffset;
	float SensorSensitivity;
};

#endif /* HARDPOINTACTUATORTABLEROW_H_ */
