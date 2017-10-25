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

struct m1m3_logevent_ForceActuatorInfoC;
struct m1m3_logevent_HardpointActuatorInfoC;

namespace LSST {
namespace M1M3 {
namespace SS {

class SetADCChanneOffsetAndSensitivityBusList: public BusList {
private:
	m1m3_logevent_ForceActuatorInfoC* forceInfo;
	m1m3_logevent_HardpointActuatorInfoC* hardpointInfo;

public:
	SetADCChanneOffsetAndSensitivityBusList(ILCSubnetData* subnetData, ILCMessageFactory* ilcMessageFactory, m1m3_logevent_ForceActuatorInfoC* forceInfo, m1m3_logevent_HardpointActuatorInfoC* hardpointInfo);
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* SETADCCHANNEOFFSETANDSENSITIVITYBUSLIST_H_ */
