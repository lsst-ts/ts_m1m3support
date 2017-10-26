/*
 * ForceActuatorTableRow.h
 *
 *  Created on: Oct 25, 2017
 *      Author: ccontaxis
 */

#ifndef FORCEACTUATORTABLEROW_H_
#define FORCEACTUATORTABLEROW_H_

#include <DataTypes.h>
#include <ForceActuatorOrientations.h>
#include <ForceActuatorTypes.h>

struct ForceActuatorTableRow {
	int32_t Index;
	int32_t ActuatorID;
	double XPosition;
	double YPosition;
	double ZPosition;
	ForceActuatorTypes::Type Type;
	uint8_t Subnet;
	uint8_t Address;
	ForceActuatorOrientations::Type Orientation;
	float PrimaryAxisSensorOffset;
	float PrimaryAxisSensorSensitivity;
	float SecondaryAxisSensorOffset;
	float SecondaryAxisSensorSensitivity;
};

#endif /* FORCEACTUATORTABLEROW_H_ */
