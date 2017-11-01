/*
 * InterlockController.h
 *
 *  Created on: Nov 1, 2017
 *      Author: ccontaxis
 */

#ifndef INTERLOCKCONTROLLER_H_
#define INTERLOCKCONTROLLER_H_

#include <IInterlockController.h>
#include <DataTypes.h>

struct m1m3_logevent_InterlockStatusC;
struct m1m3_logevent_InterlockWarningC;

namespace LSST {
namespace M1M3 {
namespace SS {

class IPublisher;
class ISafetyController;
class IFPGA;

class InterlockController: public IInterlockController {
private:
	IPublisher* publisher;
	ISafetyController* safetyController;
	IFPGA* fpga;

	m1m3_logevent_InterlockStatusC* interlockStatus;
	m1m3_logevent_InterlockWarningC* interlockWarning;

	uint16_t txBuffer[2];
	uint16_t statusBuffer[1];

public:
	InterlockController(IPublisher* publisher, ISafetyController* safetyController, IFPGA* fpga);

	void setHeartbeat(bool state);
	void setCriticalFault(bool state);
	void setMirrorLoweringRaising(bool state);
	void setMirrorParked(bool state);

private:
	bool checkHeartbeatOutputState();
	bool checkCriticalFaultOutputState();
	bool checkMirrorLoweringRaisingOutputState();
	bool checkMirrorParkedOutputState();

	bool checkForHeartbeatOutputStateMismatch();
	bool checkForCriticalFaultOutputStateMismatch();
	bool checkForMirrorLoweringRaisingOutputStateMismatch();
	bool checkForMirrorParkedOutputStateMismatch();

	void publishInterlockStatus();
	void publishInterlockWarning();
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* INTERLOCKCONTROLLER_H_ */
