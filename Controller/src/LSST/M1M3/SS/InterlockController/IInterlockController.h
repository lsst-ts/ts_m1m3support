/*
 * IInterlockController.h
 *
 *  Created on: Nov 1, 2017
 *      Author: ccontaxis
 */

#ifndef IINTERLOCKCONTROLLER_H_
#define IINTERLOCKCONTROLLER_H_

namespace LSST {
namespace M1M3 {
namespace SS {

class IInterlockController {
public:
	virtual ~IInterlockController();

	virtual void tryToggleHeartbeat();

	virtual void setHeartbeat(bool state);
	virtual void setCriticalFault(bool state);
	virtual void setMirrorLoweringRaising(bool state);
	virtual void setMirrorParked(bool state);
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* IINTERLOCKCONTROLLER_H_ */
