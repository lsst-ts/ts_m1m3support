/*
 * AirController.h
 *
 *  Created on: Oct 16, 2017
 *      Author: ccontaxis
 */

#ifndef AIRCONTROLLER_H_
#define AIRCONTROLLER_H_

#include <DataTypes.h>

struct m1m3_logevent_AirSupplyStatusC;
struct m1m3_logevent_AirSupplyWarningC;

namespace LSST {
namespace M1M3 {
namespace SS {

class M1M3SSPublisher;
class SafetyController;
class FPGA;

/*!
 * Class used for controlling air supply.
 */
class AirController {
private:
	M1M3SSPublisher* publisher;
	SafetyController* safetyController;
	FPGA* fpga;

	uint16_t turnAirOnBuffer[2];
	uint16_t turnAirOffBuffer[2];
	uint16_t statusBuffer[1];

	m1m3_logevent_AirSupplyStatusC* airSupplyStatus;
	m1m3_logevent_AirSupplyWarningC* airSupplyWarning;

	bool airCommandedOnChanged;
	bool forcePublishAirSupplyWarning;

public:
	AirController(M1M3SSPublisher* publisher, SafetyController* safetyController, FPGA* fpga);

	void turnAirOn();
	void turnAirOff();

	void checkStatus();

private:
	void createTurnAirOnBuffer();
	void createTurnAirOffBuffer();

	bool checkControlStatus();
	bool checkValveOpenStatus();
	bool checkValveClosedStatus();

	bool checkForCommandOutputMismatch();
	bool checkForCommandSensorMismatch();

	void publishAirSupplyStatus();
	void publishAirSupplyWarning();
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* AIRCONTROLLER_H_ */
