/*
 * HardpointActuatorApplicationSettings.h
 *
 *  Created on: Oct 3, 2017
 *      Author: ccontaxis
 */

#ifndef HARDPOINTACTUATORAPPLICATIONSETTINGS_H_
#define HARDPOINTACTUATORAPPLICATIONSETTINGS_H_

#include <DataTypes.h>
#include <string>

namespace LSST {
namespace M1M3 {
namespace SS {

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

struct HardpointActuatorApplicationSettings {
	HardpointActuatorTableRow Table[156];

	void load(const std::string &filename);
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* HARDPOINTACTUATORAPPLICATIONSETTINGS_H_ */
