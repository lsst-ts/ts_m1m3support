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

struct m1m3_logevent_CellLightStatusC;
struct m1m3_logevent_CellLightWarningC;
struct m1m3_logevent_InterlockStatusC;
struct m1m3_logevent_InterlockWarningC;

namespace LSST {
namespace M1M3 {
namespace SS {

class IPublisher;
class ISafetyController;
class IFPGA;
class InterlockSettings;

class InterlockController: public IInterlockController {
private:
	IPublisher* publisher;
	ISafetyController* safetyController;
	IFPGA* fpga;
	InterlockSettings* interlockSettings;

	m1m3_logevent_CellLightStatusC* cellLightStatus;
	m1m3_logevent_CellLightWarningC* cellLightWarning;
	m1m3_logevent_InterlockStatusC* interlockStatus;
	m1m3_logevent_InterlockWarningC* interlockWarning;

	uint16_t txBuffer[2];
	uint16_t statusBuffer[1];

	double lastToggleTimestamp;

public:
	InterlockController(IPublisher* publisher, ISafetyController* safetyController, IFPGA* fpga, InterlockSettings* interlockSettings);

	void tryToggleHeartbeat();

	void setHeartbeat(bool state);
	void setCriticalFault(bool state);
	void setMirrorLoweringRaising(bool state);
	void setMirrorParked(bool state);

	void checkInterlockStatus();

	void setCellLightsOn(bool state);

private:
	bool checkHeartbeatOutputState();
	bool checkCriticalFaultOutputState();
	bool checkMirrorLoweringRaisingOutputState();
	bool checkMirrorParkedOutputState();
	bool checkCellLightOutputState();
	bool checkCellLightState();

	bool checkForHeartbeatOutputStateMismatch();
	bool checkForCriticalFaultOutputStateMismatch();
	bool checkForMirrorLoweringRaisingOutputStateMismatch();
	bool checkForMirrorParkedOutputStateMismatch();
	bool checkForCellLightOutputMismatch();
	bool checkForCellLightSensorMismatch();

	void publishCellLightStatus();
	void publishCellLightWarning();
	void publishInterlockStatus();
	void publishInterlockWarning();
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* INTERLOCKCONTROLLER_H_ */
