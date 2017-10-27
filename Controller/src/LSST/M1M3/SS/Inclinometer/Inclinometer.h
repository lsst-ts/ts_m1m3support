/*
 * Inclinometer.h
 *
 *  Created on: Oct 2, 2017
 *      Author: ccontaxis
 */

#ifndef INCLINOMETER_H_
#define INCLINOMETER_H_

#include <IInclinometer.h>
#include <DataTypes.h>
#include <SAL_m1m3C.h>

struct m1m3_InclinometerDataC;
struct m1m3_logevent_InclinometerSensorWarningC;

namespace LSST {
namespace M1M3 {
namespace SS {

class IPublisher;
class ISafetyController;
class IFPGA;

class Inclinometer: public IInclinometer {
private:
	IPublisher* publisher;
	ISafetyController* safetyController;
	IFPGA* fpga;
	m1m3_InclinometerDataC* inclinometerData;
	m1m3_logevent_InclinometerSensorWarningC* inclinometerWarning;

	uint16_t txBuffer[32];
	uint8_t rxBuffer[32];

public:
	Inclinometer(IPublisher* publisher, ISafetyController* safetyController, IFPGA* fpga);

	void writeDataRequest();
	void readDataResponse();

private:
	void createTxBuffer();

	void clearWarning(double timestamp);
	void warnResponseTimeout(double timestamp);
	void warnInvalidCRC(double timestamp);
	void warnUnknownAddress(double timestamp);
	void warnUnknownFunction(double timestamp);
	void warnInvalidLength(double timestamp);
	void warnInvalidRegister(double timestamp);
	void warnInvalidFunction(double timestamp);
	void warnUnknownProblem(double timestamp);
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* INCLINOMETER_H_ */
