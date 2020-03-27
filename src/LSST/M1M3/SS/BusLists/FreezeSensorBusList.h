/*
 * FreezeSensorBusList.h
 *
 *  Created on: Oct 20, 2017
 *      Author: ccontaxis
 */

#ifndef FREEZESENSORBUSLIST_H_
#define FREEZESENSORBUSLIST_H_

#include <BusList.h>
#include <SAL_MTM1M3C.h>

namespace LSST {
namespace M1M3 {
namespace SS {

class FreezeSensorBusList: public BusList {
private:
	MTM1M3_outerLoopDataC* outerLoopData;

	int32_t freezeSensorCommandIndex[5];
	int32_t faStatusCommandIndex[5];
	int32_t roundRobinFAReportServerStatusIndex[5];
	int32_t hmLVDTCommandIndex[5];
	int32_t lvdtSampleClock;

public:
	FreezeSensorBusList(ILCSubnetData* subnetData, ILCMessageFactory* ilcMessageFactory, MTM1M3_outerLoopDataC* outerLoopData);

	void update();
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* FREEZESENSORBUSLIST_H_ */
