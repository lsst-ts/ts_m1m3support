/*
 * RaisedBusList.h
 *
 *  Created on: Oct 20, 2017
 *      Author: ccontaxis
 */

#ifndef RAISEDBUSLIST_H_
#define RAISEDBUSLIST_H_

#include <BusList.h>

struct m1m3_OuterLoopDataC;
struct m1m3_ForceActuatorDataC;
struct m1m3_HardpointDataC;
struct m1m3_logevent_ForceActuatorInfoC;

namespace LSST {
namespace M1M3 {
namespace SS {

class RaisedBusList: public BusList {
private:
	m1m3_OuterLoopDataC* outerLoopData;
	m1m3_ForceActuatorDataC* forceData;
	m1m3_HardpointDataC* hardpointData;
	m1m3_logevent_ForceActuatorInfoC* forceInfo;

	int32_t setForceCommandIndex[5];
	int32_t moveStepCommandIndex[5];
	int32_t faStatusCommandIndex[5];
	int32_t roundRobinFAReportServerStatusIndex[5];

public:
	RaisedBusList(ILCSubnetData* subnetData, ILCMessageFactory* ilcMessageFactory, m1m3_OuterLoopDataC* outerLoopData, m1m3_ForceActuatorDataC* forceData, m1m3_HardpointDataC* hardpointData, m1m3_logevent_ForceActuatorInfoC* forceInfo);

	void update();
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* RAISEDBUSLIST_H_ */
