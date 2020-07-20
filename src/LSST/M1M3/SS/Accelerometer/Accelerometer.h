/*
 * This file is part of LSST M1M3 support system package.
 *
 * Developed for the LSST Data Management System.
 * This product includes software developed by the LSST Project
 * (https://www.lsst.org).
 * See the COPYRIGHT file at the top-level directory of this distribution
 * for details of code ownership.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
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
public:
    /*!
     * Instantiates the accelerometer.
     * @param[in] accelerometerSettings The accelerometer settings.
     * @param[in] _publisher The publisher.
     */
    Accelerometer(AccelerometerSettings* accelerometerSettings, M1M3SSPublisher* publisher);

    /*!
     * Processes currently available accelerometer data and publish it.
     */
    void processData();

private:
    AccelerometerSettings* _accelerometerSettings;
    M1M3SSPublisher* _publisher;

    MTM1M3_accelerometerDataC* _accelerometerData;
    MTM1M3_logevent_accelerometerWarningC* _accelerometerWarning;
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* ACCELEROMETER_H_ */
