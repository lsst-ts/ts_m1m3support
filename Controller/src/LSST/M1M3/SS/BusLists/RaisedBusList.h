/*
 * RaisedBusList.h
 *
 *  Created on: Oct 20, 2017
 *      Author: ccontaxis
 */

#ifndef RAISEDBUSLIST_H_
#define RAISEDBUSLIST_H_

#include <BusList.h>
#include <SAL_MTM1M3C.h>

namespace LSST {
namespace M1M3 {
namespace SS {

class RaisedBusList: public BusList {
private:
	MTM1M3_outerLoopDataC* outerLoopData;
	MTM1M3_forceActuatorDataC* forceActuatorData;
	MTM1M3_logevent_appliedCylinderForcesC* appliedCylinderForces;
	MTM1M3_logevent_appliedHardpointStepsC* appliedHardpointSteps;
	MTM1M3_logevent_forceActuatorStateC* forceActuatorState;

	int32_t setForceCommandIndex[5];
	int32_t moveStepCommandIndex[5];
	int32_t faStatusCommandIndex[5];
	int32_t roundRobinFAReportServerStatusIndex[5];
	int32_t hmLVDTCommandIndex[5];
	int32_t lvdtSampleClock;

public:
	RaisedBusList(ILCSubnetData* subnetData, ILCMessageFactory* ilcMessageFactory, MTM1M3_outerLoopDataC* outerLoopData, MTM1M3_forceActuatorDataC* forceActuatorData, MTM1M3_logevent_appliedHardpointStepsC* appliedHardpointSteps, MTM1M3_logevent_appliedCylinderForcesC* appliedCylinderForces, MTM1M3_logevent_forceActuatorStateC* forceActuatorState);

	void update();
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* RAISEDBUSLIST_H_ */
