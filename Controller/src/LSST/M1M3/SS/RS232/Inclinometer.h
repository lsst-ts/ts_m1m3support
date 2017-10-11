/*
 * Inclinometer.h
 *
 *  Created on: Oct 2, 2017
 *      Author: ccontaxis
 */

#ifndef INCLINOMETER_H_
#define INCLINOMETER_H_

#include <DataTypes.h>
#include <SAL_m1m3C.h>

namespace LSST {
namespace M1M3 {
namespace SS {

class IFPGA;
class IPublisher;

class Inclinometer {
private:
	IPublisher* publisher;
	IFPGA* fpga;

	uint8_t txBuffer[11];

	m1m3_logevent_InclinometerSensorWarningC inclinometerWarning;

	m1m3_InclinometerDataC inclinometerData;

	void createTxBuffer();

	void clearWarning(double timestamp);
	void warnInvalidCRC(double timestamp);
	void warnUnknownAddress(double timestamp);
	void warnUnknownFunction(double timestamp);
	void warnInvalidLength(double timestamp);
	void warnInvalidRegister(double timestamp);
	void warnInvalidFunction(double timestamp);
	void warnUnknownProblem(double timestamp);

public:
	Inclinometer();
	Inclinometer(IFPGA* fpga, IPublisher* publisher);

	void writeDataRequest();
	void readDataResponse(uint8_t* buffer, int32_t* index);
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* INCLINOMETER_H_ */
