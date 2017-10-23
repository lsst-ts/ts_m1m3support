/*
 * ReportDCAStatusBusList.h
 *
 *  Created on: Oct 19, 2017
 *      Author: ccontaxis
 */

#ifndef REPORTDCASTATUSBUSLIST_H_
#define REPORTDCASTATUSBUSLIST_H_

#include <BusList.h>

namespace LSST {
namespace M1M3 {
namespace SS {

class ReportDCAStatusBusList: public BusList {
public:
	ReportDCAStatusBusList();
	ReportDCAStatusBusList(ILCSubnetData* subnetData, ILCMessageFactory* ilcMessageFactory);
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* REPORTDCASTATUSBUSLIST_H_ */
