/*
 * Accelerometer.h
 *
 *  Created on: Nov 1, 2017
 *      Author: ccontaxis
 */

#ifndef ACCELEROMETER_H_
#define ACCELEROMETER_H_

struct m1m3_AccelerometerDataC;
struct m1m3_logevent_AccelerometerWarningC;

namespace LSST {
namespace M1M3 {
namespace SS {

class AccelerometerSettings;
struct SupportFPGAData;
class M1M3SSPublisher;

/*!
 * The class used to process accelerometer data.
 */
class Accelerometer {
private:
	AccelerometerSettings* accelerometerSettings;
	SupportFPGAData* fpgaData;
	M1M3SSPublisher* publisher;

	m1m3_AccelerometerDataC* accelerometerData;
	m1m3_logevent_AccelerometerWarningC* accelerometerWarning;

public:
	/*!
	 * Instantiates the accelerometer.
	 * @param[in] accelerometerSettings The accelerometer settings.
	 * @param[in] fpgaData The fpga data.
	 * @param[in] publisher The publisher.
	 */
	Accelerometer(AccelerometerSettings* accelerometerSettings, SupportFPGAData* fpgaData, M1M3SSPublisher* publisher);

	/*!
	 * Processes currently available accelerometer data and publish it.
	 */
	void processData();
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* ACCELEROMETER_H_ */
