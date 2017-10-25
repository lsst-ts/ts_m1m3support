/*
 * ReportServerStatusBusList.h
 *
 *  Created on: Oct 19, 2017
 *      Author: ccontaxis
 */

#ifndef REPORTSERVERSTATUSBUSLIST_H_
#define REPORTSERVERSTATUSBUSLIST_H_

#include <BusList.h>

namespace LSST {
namespace M1M3 {
namespace SS {

class ReportServerStatusBusList: public BusList {
public:
	ReportServerStatusBusList(ILCSubnetData* subnetData, ILCMessageFactory* ilcMessageFactory);
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* REPORTSERVERSTATUSBUSLIST_H_ */
