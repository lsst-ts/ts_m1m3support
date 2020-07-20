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

#ifndef INCLINOMETER_H_
#define INCLINOMETER_H_

#include <DataTypes.h>

struct MTM1M3_inclinometerDataC;
struct MTM1M3_logevent_inclinometerSensorWarningC;

namespace LSST {
namespace M1M3 {
namespace SS {

struct SupportFPGAData;
class M1M3SSPublisher;
class SafetyController;
class InclinometerSettings;

/*!
 * The class used to process inclinometer data.
 */
class Inclinometer {
public:
    /*!
     * Instantiates the inclinometer.
     * @param[in] fpgaData The fpga data.
     * @param[in] publisher The publisher.
     * @param[in] safetyController The safety controller.
     */
    Inclinometer(SupportFPGAData* fpgaData, M1M3SSPublisher* publisher, SafetyController* safetyController,
                 InclinometerSettings* inclinometerSettings);

    /*!
     * Processes currently available inclinometer data and publish it.
     */
    void processData();

private:
    SupportFPGAData* _fpgaData;
    M1M3SSPublisher* _publisher;
    SafetyController* _safetyController;
    InclinometerSettings* _inclinometerSettings;

    MTM1M3_inclinometerDataC* _inclinometerData;
    MTM1M3_logevent_inclinometerSensorWarningC* _inclinometerWarning;

    uint64_t _lastSampleTimestamp;
    uint64_t _lastErrorTimestamp;
    bool _errorCleared;
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* INCLINOMETER_H_ */
