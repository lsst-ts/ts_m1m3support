#ifndef INCLINOMETERSETTINGS_H_
#define INCLINOMETERSETTINGS_H_

#include <string>

namespace LSST {
namespace M1M3 {
namespace SS {

class InclinometerSettings {
public:
    float Offset;
    float FaultLow;
    float WarningLow;
    float WarningHigh;
    float FaultHigh;

    void load(const std::string &filename);
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* INCLINOMETERSETTINGS_H_ */
