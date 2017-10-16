/*
 * ForceActuatorApplicationSettings.h
 *
 *  Created on: Sep 29, 2017
 *      Author: ccontaxis
 */

#ifndef FORCEACTUATORAPPLICATIONSETTINGS_H_
#define FORCEACTUATORAPPLICATIONSETTINGS_H_

#include <DataTypes.h>
#include <string>

namespace LSST {
namespace M1M3 {
namespace SS {

struct ForceActuatorTypes {
	enum Type {
		SAA = 0,
		DAA = 1
	};
};

struct ForceActuatorOrientations {
	enum Type {
		NA = 0,
		PositiveY = 1,
		NegativeY = 2,
		PositiveX = 3,
		NegativeX = 4
	};
};

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

struct ForceActuatorApplicationSettings {
	ForceActuatorTableRow Table[156];

	void load(const std::string &filename);
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* FORCEACTUATORAPPLICATIONSETTINGS_H_ */
