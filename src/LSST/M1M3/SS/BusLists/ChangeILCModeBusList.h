/*
 * ChangeILCModeBusList.h
 *
 *  Created on: Oct 19, 2017
 *      Author: ccontaxis
 */

#ifndef CHANGEILCMODEBUSLIST_H_
#define CHANGEILCMODEBUSLIST_H_

#include <BusList.h>
#include <ILCDataTypes.h>
#include <ModbusBuffer.h>

namespace LSST {
namespace M1M3 {
namespace SS {

class ChangeILCModeBusList: public BusList {
public:
	ChangeILCModeBusList(ILCSubnetData* subnetData, ILCMessageFactory* ilcMessageFactory, ILCModes::Type mode, ILCModes::Type hmMode);
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* CHANGEILCMODEBUSLIST_H_ */
