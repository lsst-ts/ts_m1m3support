/*
 * ReadBoostValveDCAGainBusList.h
 *
 *  Created on: Oct 19, 2017
 *      Author: ccontaxis
 */

#ifndef READBOOSTVALVEDCAGAINBUSLIST_H_
#define READBOOSTVALVEDCAGAINBUSLIST_H_

#include <BusList.h>

namespace LSST {
namespace M1M3 {
namespace SS {

class ReadBoostValveDCAGainBusList: public BusList {
public:
	ReadBoostValveDCAGainBusList();
	ReadBoostValveDCAGainBusList(ILCSubnetData* subnetData, ILCMessageFactory* ilcMessageFactory);
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* READBOOSTVALVEDCAGAINBUSLIST_H_ */
