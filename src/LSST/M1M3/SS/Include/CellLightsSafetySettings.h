#ifndef CELLLIGHTSSAFETYSETTINGS_H_
#define CELLLIGHTSSAFETYSETTINGS_H_

namespace LSST {
namespace M1M3 {
namespace SS {

struct CellLightsSafetySettings {
    bool FaultOnOutputMismatch;
    bool FaultOnSensorMismatch;
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* CELLLIGHTSSAFETYSETTINGS_H_ */
