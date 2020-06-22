/*
 * DisplacementSensorSettings.h
 *
 *  Created on: Nov 16, 2017
 *      Author: ccontaxis
 */

#ifndef DISPLACEMENTSENSORSETTINGS_H_
#define DISPLACEMENTSENSORSETTINGS_H_

#include <DataTypes.h>
#include <string>
#include <vector>

namespace LSST {
namespace M1M3 {
namespace SS {

class DisplacementSensorSettings {
public:
    std::vector<double> ConverterMatrix;
    int32_t N1Port;
    int32_t N2Port;
    int32_t N3Port;
    int32_t N4Port;
    int32_t N5Port;
    int32_t N6Port;
    int32_t N7Port;
    int32_t N8Port;
    double N1Offset;
    double N2Offset;
    double N3Offset;
    double N4Offset;
    double N5Offset;
    double N6Offset;
    double N7Offset;
    double N8Offset;
    double XPositionOffset;
    double YPositionOffset;
    double ZPositionOffset;
    double XRotationOffset;
    double YRotationOffset;
    double ZRotationOffset;

    void load(const std::string &filename);
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* DISPLACEMENTSENSORSETTINGS_H_ */
