/*
 * Displacement.h
 *
 *  Created on: Oct 6, 2017
 *      Author: ccontaxis
 */

#ifndef DISPLACEMENT_H_
#define DISPLACEMENT_H_

#include <DataTypes.h>
#include <SAL_m1m3C.h>

namespace LSST {
namespace M1M3 {
namespace SS {

class IPublisher;
class IFPGA;

class Displacement {
private:
	IPublisher* publisher;
	IFPGA* fpga;

	uint8_t txBuffer[7];
	uint8_t rxBuffer[128];

	m1m3_logevent_DisplacementSensorWarningC displacementWarning;
	m1m3_IMSDataC imsData;

public:
	Displacement(IPublisher* publisher, IFPGA* fpga);

	void writeDataRequest();
	void readDataResponse(uint8_t* buffer, int32_t* index);

private:
	void createTxBuffer();

	void clearWarning(double timestamp);
	void warnSensorReportsInvalidCommand(double timestamp);
	void warnSensorReportsCommunicationTimeoutError(double timestamp);
	void warnSensorReportsDataLengthError(double timestamp);
	void warnSensorReportsNumberOfParametersError(double timestamp);
	void warnSensorReportsParameterError(double timestamp);
	void warnSensorReportsCommunicationError(double timestamp);
	void warnSensorReportsIDNumberError(double timestamp);
	void warnSensorReportsExpansionLineError(double timestamp);
	void warnSensorReportsWriteControlError(double timestamp);
	void warnResponseTimeoutError(double timestamp);
	void warnInvalidLength(double timestamp);
	void warnUnknownCommand(double timestamp);
	void warnUnknownProblem(double timestamp);
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* DISPLACEMENT_H_ */
