/*
 * Accelerometer.h
 *
 *  Created on: Nov 1, 2017
 *      Author: ccontaxis
 */

#ifndef ACCELEROMETER_H_
#define ACCELEROMETER_H_

struct MTM1M3_accelerometerDataC;
struct MTM1M3_logevent_accelerometerWarningC;

namespace LSST {
namespace M1M3 {
namespace SS {

class AccelerometerSettings;
class M1M3SSPublisher;

/*!
 * The class used to process accelerometer data.
 */
class Accelerometer {
private:
    AccelerometerSettings* accelerometerSettings;
    M1M3SSPublisher* publisher;

    MTM1M3_accelerometerDataC* accelerometerData;
    MTM1M3_logevent_accelerometerWarningC* accelerometerWarning;

public:
    /*!
     * Instantiates the accelerometer.
     * @param[in] accelerometerSettings The accelerometer settings.
     * @param[in] publisher The publisher.
     */
    Accelerometer(AccelerometerSettings* accelerometerSettings, M1M3SSPublisher* publisher);

    /*!
     * Processes currently available accelerometer data and publish it.
     */
    void processData();
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* ACCELEROMETER_H_ */
