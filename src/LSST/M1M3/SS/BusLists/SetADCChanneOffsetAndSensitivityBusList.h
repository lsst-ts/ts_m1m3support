/*
 * SetADCChanneOffsetAndSensitivityBusList.h
 *
 *  Created on: Oct 19, 2017
 *      Author: ccontaxis
 */

#ifndef SETADCCHANNEOFFSETANDSENSITIVITYBUSLIST_H_
#define SETADCCHANNEOFFSETANDSENSITIVITYBUSLIST_H_

#include <BusList.h>
#include <ILCDataTypes.h>
#include <ModbusBuffer.h>
#include <SAL_MTM1M3C.h>

namespace LSST {
namespace M1M3 {
namespace SS {

class SetADCChanneOffsetAndSensitivityBusList: public BusList {
private:
	MTM1M3_logevent_forceActuatorInfoC* forceInfo;
	MTM1M3_logevent_hardpointActuatorInfoC* hardpointInfo;

public:
	SetADCChanneOffsetAndSensitivityBusList(ILCSubnetData* subnetData, ILCMessageFactory* ilcMessageFactory, MTM1M3_logevent_forceActuatorInfoC* forceInfo, MTM1M3_logevent_hardpointActuatorInfoC* hardpointInfo);
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* SETADCCHANNEOFFSETANDSENSITIVITYBUSLIST_H_ */
