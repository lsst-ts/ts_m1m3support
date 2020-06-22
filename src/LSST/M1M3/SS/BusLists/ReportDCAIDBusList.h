#ifndef REPORTDCAIDBUSLIST_H_
#define REPORTDCAIDBUSLIST_H_

#include <BusList.h>

namespace LSST {
namespace M1M3 {
namespace SS {

class ReportDCAIDBusList : public BusList {
public:
    ReportDCAIDBusList(ILCSubnetData* subnetData, ILCMessageFactory* ilcMessageFactory);
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* REPORTDCAIDBUSLIST_H_ */
