/*
 * AccelerometerSettings.h
 *
 *  Created on: Nov 2, 2017
 *      Author: ccontaxis
 */

#ifndef ACCELEROMETERSETTINGS_H_
#define ACCELEROMETERSETTINGS_H_

#include <string>

namespace LSST {
namespace M1M3 {
namespace SS {

class AccelerometerSettings {
public:
    AccelerometerSettings() = default;
    void load(const std::string& filename);
    float GsToMetersPerSecondSqrd;
    float AngularAccelerationXDistance;
    float AngularAccelerationYDistance;
    float AngularAccelerationZDistance;
    float AccelerometerBias[8];
    float AccelerometerSensitivity[8];
    float AccelerometerOffsets[8];
    float AccelerometerScalars[8];
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* ACCELEROMETERSETTINGS_H_ */
