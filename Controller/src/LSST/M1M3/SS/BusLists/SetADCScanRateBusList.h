/*
 * SetADCScanRateBusList.h
 *
 *  Created on: Oct 19, 2017
 *      Author: ccontaxis
 */

#ifndef SETADCSCANRATEBUSLIST_H_
#define SETADCSCANRATEBUSLIST_H_

#include <BusList.h>
#include <SAL_MTM1M3C.h>

namespace LSST {
namespace M1M3 {
namespace SS {

class SetADCScanRateBusList: public BusList {
private:
	MTM1M3_logevent_forceActuatorInfoC* forceInfo;
	MTM1M3_logevent_hardpointActuatorInfoC* hardpointInfo;

public:
	SetADCScanRateBusList(ILCSubnetData* subnetData, ILCMessageFactory* ilcMessageFactory, MTM1M3_logevent_forceActuatorInfoC* forceInfo, MTM1M3_logevent_hardpointActuatorInfoC* hardpointInfo);
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* SETADCSCANRATEBUSLIST_H_ */
