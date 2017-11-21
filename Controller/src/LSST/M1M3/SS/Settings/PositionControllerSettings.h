/*
 * PositionControllerSettings.h
 *
 *  Created on: Oct 30, 2017
 *      Author: ccontaxis
 */

#ifndef POSITIONCONTROLLERSETTINGS_H_
#define POSITIONCONTROLLERSETTINGS_H_

#include <DataTypes.h>
#include <string>

namespace LSST {
namespace M1M3 {
namespace SS {

class PositionControllerSettings {
public:
	double ForceToStepsCoefficient;
	int32_t MaxStepsPerLoop;
	double RaiseLowerForceLimitLow;
	double RaiseLowerForceLimitHigh;

	void load(const std::string &filename);
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* POSITIONCONTROLLERSETTINGS_H_ */
