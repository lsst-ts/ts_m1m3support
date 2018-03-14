/*
 * Accelerometer.h
 *
 *  Created on: Nov 1, 2017
 *      Author: ccontaxis
 */

#ifndef ACCELEROMETER_H_
#define ACCELEROMETER_H_

#include <DataTypes.h>
#include <TypeAlias.h>

namespace LSST {
namespace M1M3 {
namespace SS {

class Accelerometer {
private:
	M1M3SSPublisher* publisher;
	FPGA* fpga;
	AccelerometerSettings* accelerometerSettings;

	m1m3_AccelerometerDataC* accelerometerData;
	m1m3_logevent_AccelerometerWarningC* accelerometerWarning;

	uint16_t timestampBuffer[4];
	float dataBuffer[8];

public:
	Accelerometer(M1M3SSPublisher* publisher, FPGA* fpga, AccelerometerSettings* accelerometerSettings);

	void sampleData();
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* ACCELEROMETER_H_ */
