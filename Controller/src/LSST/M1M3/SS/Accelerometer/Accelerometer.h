/*
 * Accelerometer.h
 *
 *  Created on: Nov 1, 2017
 *      Author: ccontaxis
 */

#ifndef ACCELEROMETER_H_
#define ACCELEROMETER_H_

#include <IAccelerometer.h>
#include <DataTypes.h>

struct m1m3_AccelerometerDataC;
struct m1m3_logevent_AccelerometerWarningC;

namespace LSST {
namespace M1M3 {
namespace SS {

class IPublisher;
class IFPGA;
class AccelerometerSettings;

class Accelerometer: public IAccelerometer {
private:
	IPublisher* publisher;
	IFPGA* fpga;
	AccelerometerSettings* accelerometerSettings;

	m1m3_AccelerometerDataC* accelerometerData;
	m1m3_logevent_AccelerometerWarningC* accelerometerWarning;

	uint16_t timestampBuffer[4];
	float dataBuffer[8];

public:
	Accelerometer(IPublisher* publisher, IFPGA* fpga, AccelerometerSettings* accelerometerSettings);

	void sampleData();

private:

};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* ACCELEROMETER_H_ */
