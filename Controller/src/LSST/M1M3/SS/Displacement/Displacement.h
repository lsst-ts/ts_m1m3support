/*
 * Displacement.h
 *
 *  Created on: Oct 6, 2017
 *      Author: ccontaxis
 */

#ifndef DISPLACEMENT_H_
#define DISPLACEMENT_H_

#include <IDisplacement.h>
#include <DataTypes.h>

struct m1m3_IMSDataC;
struct m1m3_logevent_DisplacementSensorWarningC;

namespace LSST {
namespace M1M3 {
namespace SS {

class IPublisher;
class IFPGA;

class Displacement: public IDisplacement {
private:
	IPublisher* publisher;
	IFPGA* fpga;
	m1m3_IMSDataC* imsData;
	m1m3_logevent_DisplacementSensorWarningC* displacementWarning;

	uint16_t txBuffer[32];
	uint8_t rxBuffer[128];

public:
	Displacement(IPublisher* publisher, IFPGA* fpga);

	void writeDataRequest();
	void readDataResponse();

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
