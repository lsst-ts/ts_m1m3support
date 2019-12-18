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

struct SupportFPGAData;
class AccelerometerSettings;
class M1M3SSPublisher;

/*!
 * The class used to process accelerometer data.
 */
class Accelerometer {
public:
    Accelerometer(AccelerometerSettings* accelerometerSettings, SupportFPGAData* fpgaData, M1M3SSPublisher* publisher);
    ~Accelerometer() = default;
    void processData();

private:
    AccelerometerSettings* accelerometerSettings = nullptr;
    SupportFPGAData* fpgaData = nullptr;
    M1M3SSPublisher* publisher = nullptr;
    MTM1M3_accelerometerDataC* accelerometerData = nullptr;
    MTM1M3_logevent_accelerometerWarningC* accelerometerWarning = nullptr;
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* ACCELEROMETER_H_ */
