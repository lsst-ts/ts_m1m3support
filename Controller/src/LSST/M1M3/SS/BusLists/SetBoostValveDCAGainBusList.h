/*
 * SetBoostValveDCAGainBusList.h
 *
 *  Created on: Oct 19, 2017
 *      Author: ccontaxis
 */

#ifndef SETBOOSTVALVEDCAGAINBUSLIST_H_
#define SETBOOSTVALVEDCAGAINBUSLIST_H_

#include <BusList.h>

struct m1m3_logevent_ForceActuatorInfoC;

namespace LSST {
namespace M1M3 {
namespace SS {

class SetBoostValveDCAGainBusList: public BusList {
private:
	m1m3_logevent_ForceActuatorInfoC* forceInfo;

public:
	SetBoostValveDCAGainBusList();
	SetBoostValveDCAGainBusList(ILCSubnetData* subnetData, ILCMessageFactory* ilcMessageFactory, m1m3_logevent_ForceActuatorInfoC* forceInfo);
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* SETBOOSTVALVEDCAGAINBUSLIST_H_ */
