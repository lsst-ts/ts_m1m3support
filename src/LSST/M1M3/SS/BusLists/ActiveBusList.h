/*
 * ActiveBusList.h
 *
 *  Created on: Apr 9, 2018
 *      Author: ccontaxis
 */

#ifndef ACTIVEBUSLIST_H_
#define ACTIVEBUSLIST_H_

#include <BusList.h>
#include <SAL_MTM1M3C.h>

namespace LSST {
namespace M1M3 {
namespace SS {

class ActiveBusList : public BusList {
private:
    MTM1M3_outerLoopDataC* outerLoopData;
    MTM1M3_forceActuatorDataC* forceActuatorData;
    MTM1M3_logevent_appliedCylinderForcesC* appliedCylinderForces;
    MTM1M3_hardpointActuatorDataC* hardpointActuatorData;
    MTM1M3_logevent_forceActuatorInfoC* forceInfo;

    int32_t setForceCommandIndex[5];
    int32_t hpFreezeCommandIndex[5];
    int32_t faStatusCommandIndex[5];
    int32_t roundRobinFAReportServerStatusIndex[5];
    int32_t hmLVDTCommandIndex[5];
    int32_t lvdtSampleClock;

public:
    ActiveBusList(ILCSubnetData* subnetData, ILCMessageFactory* ilcMessageFactory,
                  MTM1M3_outerLoopDataC* outerLoopData, MTM1M3_forceActuatorDataC* forceActuatorData,
                  MTM1M3_hardpointActuatorDataC* hardpointActuatorData,
                  MTM1M3_logevent_forceActuatorInfoC* forceInfo,
                  MTM1M3_logevent_appliedCylinderForcesC* appliedCylinderForces);

    void update();
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* ACTIVEBUSLIST_H_ */
