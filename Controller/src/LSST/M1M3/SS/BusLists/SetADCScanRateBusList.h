/*
 * SetADCScanRateBusList.h
 *
 *  Created on: Oct 19, 2017
 *      Author: ccontaxis
 */

#ifndef SETADCSCANRATEBUSLIST_H_
#define SETADCSCANRATEBUSLIST_H_

#include <BusList.h>

struct m1m3_logevent_ForceActuatorInfoC;
struct m1m3_logevent_HardpointActuatorInfoC;

namespace LSST {
namespace M1M3 {
namespace SS {

class SetADCScanRateBusList: public BusList {
private:
	m1m3_logevent_ForceActuatorInfoC* forceInfo;
	m1m3_logevent_HardpointActuatorInfoC* hardpointInfo;

public:
	SetADCScanRateBusList(ILCSubnetData* subnetData, ILCMessageFactory* ilcMessageFactory, m1m3_logevent_ForceActuatorInfoC* forceInfo, m1m3_logevent_HardpointActuatorInfoC* hardpointInfo);
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* SETADCSCANRATEBUSLIST_H_ */
