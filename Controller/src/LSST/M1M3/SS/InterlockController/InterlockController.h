/*
 * InterlockController.h
 *
 *  Created on: Nov 1, 2017
 *      Author: ccontaxis
 */

#ifndef INTERLOCKCONTROLLER_H_
#define INTERLOCKCONTROLLER_H_

#include <DataTypes.h>
#include <SAL_m1m3C.h>

namespace LSST {
namespace M1M3 {
namespace SS {

class M1M3SSPublisher;
class SafetyController;
class FPGA;
class InterlockApplicationSettings;

class InterlockController {
private:
	M1M3SSPublisher* publisher;
	SafetyController* safetyController;
	FPGA* fpga;
	InterlockApplicationSettings* interlockApplicationSettings;

	m1m3_logevent_CellLightStatusC* cellLightStatus;
	m1m3_logevent_CellLightWarningC* cellLightWarning;
	m1m3_logevent_InterlockStatusC* interlockStatus;
	m1m3_logevent_InterlockWarningC* interlockWarning;

	uint16_t txBuffer[2];
	uint16_t statusBuffer[1];

	double lastToggleTimestamp;

public:
	InterlockController(M1M3SSPublisher* publisher, FPGA* fpga, InterlockApplicationSettings* interlockApplicationSettings);

	void setSafetyController(SafetyController* safetyController);

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
