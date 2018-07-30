/*
 * ActiveBusList.h
 *
 *  Created on: Apr 9, 2018
 *      Author: ccontaxis
 */

#ifndef ACTIVEBUSLIST_H_
#define ACTIVEBUSLIST_H_

#include <BusList.h>
#include <SAL_m1m3C.h>

namespace LSST {
namespace M1M3 {
namespace SS {

class ActiveBusList: public BusList {
private:
	m1m3_OuterLoopDataC* outerLoopData;
	m1m3_ForceActuatorDataC* forceActuatorData;
	m1m3_logevent_AppliedCylinderForcesC* appliedCylinderForces;
	m1m3_HardpointActuatorDataC* hardpointActuatorData;
	m1m3_logevent_ForceActuatorInfoC* forceInfo;

	int32_t setForceCommandIndex[5];
	int32_t hpFreezeCommandIndex[5];
	int32_t faStatusCommandIndex[5];
	int32_t roundRobinFAReportServerStatusIndex[5];
	int32_t hmLVDTCommandIndex[5];
	int32_t lvdtSampleClock;

public:
	ActiveBusList(ILCSubnetData* subnetData, ILCMessageFactory* ilcMessageFactory, m1m3_OuterLoopDataC* outerLoopData, m1m3_ForceActuatorDataC* forceActuatorData, m1m3_HardpointActuatorDataC* hardpointActuatorData, m1m3_logevent_ForceActuatorInfoC* forceInfo, m1m3_logevent_AppliedCylinderForcesC* appliedCylinderForces);

	void update();
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* ACTIVEBUSLIST_H_ */
