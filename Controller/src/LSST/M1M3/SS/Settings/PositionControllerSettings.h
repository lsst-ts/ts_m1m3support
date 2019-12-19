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
    double EncoderToStepsCoefficient;
    int32_t MaxStepsPerLoop;
    double RaiseLowerForceLimitLow;
    double RaiseLowerForceLimitHigh;
    double RaiseLowerTimeoutInSeconds;
    int32_t ReferencePositionEncoder1;
    int32_t ReferencePositionEncoder2;
    int32_t ReferencePositionEncoder3;
    int32_t ReferencePositionEncoder4;
    int32_t ReferencePositionEncoder5;
    int32_t ReferencePositionEncoder6;

    void load(const std::string& filename);
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* POSITIONCONTROLLERSETTINGS_H_ */
