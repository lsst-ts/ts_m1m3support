/*
 * HardpointMonitorTableRow.h
 *
 *  Created on: Nov 17, 2017
 *      Author: ccontaxis
 */

#ifndef HARDPOINTMONITORTABLEROW_H_
#define HARDPOINTMONITORTABLEROW_H_

#include <DataTypes.h>

struct HardpointMonitorTableRow {
	int32_t Index;
	int32_t ActuatorID;
	uint8_t Subnet;
	uint8_t Address;
};

#endif /* HARDPOINTMONITORTABLEROW_H_ */
