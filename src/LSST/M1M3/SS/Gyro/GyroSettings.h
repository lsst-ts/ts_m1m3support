#ifndef GYROSETTINGS_H_
#define GYROSETTINGS_H_

#include <string>
#include <vector>

namespace LSST {
namespace M1M3 {
namespace SS {

struct GyroSettings {
    int DataRate;
    float AngularVelocityXOffset;
    float AngularVelocityYOffset;
    float AngularVelocityZOffset;
    std::vector<double> AxesMatrix;

    void load(const std::string &filename);
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* GYROSETTINGS_H_ */
