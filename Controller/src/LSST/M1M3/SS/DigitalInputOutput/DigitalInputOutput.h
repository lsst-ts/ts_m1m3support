/*
 * DigitalInputOutput.h
 *
 *  Created on: Mar 29, 2018
 *      Author: ccontaxis
 */

#ifndef DIGITALINPUTOUTPUT_H_
#define DIGITALINPUTOUTPUT_H_

#include <DataTypes.h>

struct m1m3_logevent_AirSupplyStatusC;
struct m1m3_logevent_AirSupplyWarningC;
struct m1m3_logevent_CellLightStatusC;
struct m1m3_logevent_CellLightWarningC;
struct m1m3_logevent_InterlockStatusC;
struct m1m3_logevent_InterlockWarningC;

namespace LSST {
namespace M1M3 {
namespace SS {

class InterlockApplicationSettings;
class FPGA;
struct SupportFPGAData;
class M1M3SSPublisher;
class SafetyController;

/*!
 * The class used to process digital inputs and outputs.
 */
class DigitalInputOutput {
private:
	InterlockApplicationSettings* interlockApplicationSettings;
	FPGA* fpga;
	SupportFPGAData* fpgaData;
	M1M3SSPublisher* publisher;
	SafetyController* safetyController;

	m1m3_logevent_AirSupplyStatusC* airSupplyStatus;
	m1m3_logevent_AirSupplyWarningC* airSupplyWarning;
	m1m3_logevent_CellLightStatusC* cellLightStatus;
	m1m3_logevent_CellLightWarningC* cellLightWarning;
	m1m3_logevent_InterlockStatusC* interlockStatus;
	m1m3_logevent_InterlockWarningC* interlockWarning;

	uint64_t lastDOTimestamp;
	uint64_t lastDITimestamp;

	double lastToggleTimestamp;

public:
	/*!
	 * Instantiates the accelerometer.
	 * @param[in] accelerometerSettings The accelerometer settings.
	 * @param[in] fpga The fpga.
	 * @param[in] publisher The publisher.
	 */
	DigitalInputOutput(InterlockApplicationSettings* interlockApplicationSettings, FPGA* fpga, M1M3SSPublisher* publisher);

	/*!
	 * Sets the safety controller.
	 * @param[in] safetyController The safety controller.
	 */
	void setSafetyController(SafetyController* safetyController);

	/*!
	 * Processes currently available digital input and output data and publish it.
	 */
	void processData();

	/*!
	 * Toggles the heartbeat signal if a configured amount of time has passed.
	 */
	void tryToggleHeartbeat();
	/*!
	 * Turns the air on.
	 */
	void turnAirOn();
	/*!
	 * Turns the air off.
	 */
	void turnAirOff();
	/*!
	 * Turns the cell lights on.
	 */
	void turnCellLightsOn();
	/*!
	 * Turns the cell lights off.
	 */
	void turnCellLightsOff();
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* DIGITALINPUTOUTPUT_H_ */
