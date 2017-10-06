/*
 * ILCDataTypes.h
 *
 *  Created on: Sep 29, 2017
 *      Author: ccontaxis
 */

#ifndef ILCDATATYPES_H_
#define ILCDATATYPES_H_

#include <DataTypes.h>
#include <string>

struct ILCTypes {
	enum Type {
		Unknown = 0,
		HP = 1,
		FA = 2
	};
};

struct ILCMap {
	ILCTypes::Type Type;
	int32_t DataIndex;
	int32_t ActuatorId;
};

struct ILCSubnetData {
	uint8_t HPAddress[6];
	uint8_t FAAddress[156];
	ILCMap ILCDataFromAddress[256];
};

struct HPSettings {
	uint8_t subnet[6];
	uint8_t address[6];
	int16_t id[6];
	float xPosition[6];
	float yPosition[6];
	float zPosition[6];
	float loadCellOffset[6];
	float loadCellSensitivity[6];
};

struct ILCScanRates {
	enum Type {
		Hz50 = 0,
		Hz60 = 1,
		Hz100 = 2,
		Hz120 = 3,
		Hz200 = 4,
		Hz240 = 5,
		Hz300 = 6,
		Hz400 = 7,
		Hz480 = 8,
		Hz600 = 9,
		Hz1200 = 10,
		Hz2400 = 11,
		Hz4800 = 12,
		NoChange = 255
	};
};

struct ILCModes {
	enum Type {
		Standby = 0,
		Disabled = 1,
		Enabled = 2,
		FirmwareUpdate = 3,
		Fault = 4,
		ClearFaults = 5,
		NoChange = 65535
	};
};

#endif /* ILCDATATYPES_H_ */
