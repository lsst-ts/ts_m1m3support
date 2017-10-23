/*
 * ReportServerIDBusList.h
 *
 *  Created on: Oct 19, 2017
 *      Author: ccontaxis
 */

#ifndef REPORTSERVERIDBUSLIST_H_
#define REPORTSERVERIDBUSLIST_H_

#include <BusList.h>

namespace LSST {
namespace M1M3 {
namespace SS {

class ReportServerIDBusList : public BusList {
public:
	ReportServerIDBusList();
	ReportServerIDBusList(ILCSubnetData* subnetData, ILCMessageFactory* ilcMessageFactory);
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* REPORTSERVERIDBUSLIST_H_ */
