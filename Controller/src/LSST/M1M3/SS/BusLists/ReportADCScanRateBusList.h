/*
 * ReportADCScanRateBusList.h
 *
 *  Created on: Oct 19, 2017
 *      Author: ccontaxis
 */

#ifndef REPORTADCSCANRATEBUSLIST_H_
#define REPORTADCSCANRATEBUSLIST_H_

#include <BusList.h>

namespace LSST {
namespace M1M3 {
namespace SS {

class ReportADCScanRateBusList : public BusList {
public:
    ReportADCScanRateBusList(ILCSubnetData* subnetData, ILCMessageFactory* ilcMessageFactory);
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* REPORTADCSCANRATEBUSLIST_H_ */
