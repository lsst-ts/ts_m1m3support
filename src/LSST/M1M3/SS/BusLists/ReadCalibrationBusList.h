#ifndef READCALIBRATIONBUSLIST_H_
#define READCALIBRATIONBUSLIST_H_

#include <BusList.h>

namespace LSST {
namespace M1M3 {
namespace SS {

class ReadCalibrationBusList : public BusList {
public:
    ReadCalibrationBusList(ILCSubnetData* subnetData, ILCMessageFactory* ilcMessageFactory);
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* READCALIBRATIONBUSLIST_H_ */
