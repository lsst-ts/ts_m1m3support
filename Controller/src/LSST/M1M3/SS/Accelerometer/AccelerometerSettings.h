/*
 * AccelerometerSettings.h
 *
 *  Created on: Nov 2, 2017
 *      Author: ccontaxis
 */

#ifndef ACCELEROMETERSETTINGS_H_
#define ACCELEROMETERSETTINGS_H_

#include <string>
#include <OffsetScalar.h>

namespace LSST {
namespace M1M3 {
namespace SS {

class AccelerometerSettings {
public:
	double VoltsToMetersPerSecondSqrd;
	double AngularAccelerationXDistance;
	double AngularAccelerationYDistance;
	double AngularAccelerationZDistance;
	float AccelerometerOffsets[8];
	float AccelerometerScalars[8];

	void load(const std::string &filename);
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* ACCELEROMETERSETTINGS_H_ */
