/*
 * SetBoostValveDCAGainBusList.h
 *
 *  Created on: Oct 19, 2017
 *      Author: ccontaxis
 */

#ifndef SETBOOSTVALVEDCAGAINBUSLIST_H_
#define SETBOOSTVALVEDCAGAINBUSLIST_H_

#include <BusList.h>
#include <SAL_MTM1M3C.h>

namespace LSST {
namespace M1M3 {
namespace SS {

class SetBoostValveDCAGainBusList: public BusList {
private:
	MTM1M3_logevent_forceActuatorInfoC* forceInfo;

public:
	SetBoostValveDCAGainBusList(ILCSubnetData* subnetData, ILCMessageFactory* ilcMessageFactory, MTM1M3_logevent_forceActuatorInfoC* forceInfo);
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* SETBOOSTVALVEDCAGAINBUSLIST_H_ */
