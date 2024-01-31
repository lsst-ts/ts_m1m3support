/*
 * This file is part of LSST M1M3 support system package.
 *
 * Developed for the Vera C. Rubin Telescope and Site System.
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

#ifndef GYRO_H_
#define GYRO_H_

#include <GyroSettings.h>
#include <SupportFPGAData.h>
#include <cRIO/DataTypes.h>
#include <string>

struct MTM1M3_gyroDataC;
struct MTM1M3_logevent_gyroWarningC;

namespace LSST {
namespace M1M3 {
namespace SS {

/*!
 * The class used to process gyro data.
 */
class Gyro {
public:
    /*!
     * Instantiates the gyro sensor.
     */
    Gyro();

    /*!
     * Executes a built in test.
     */
    void bit();

    /*!
     * Commands the gyro to enter configuration mode.
     */
    void enterConfigurationMode();
    /*!
     * Commands the gyro to exit configuration mode.
     */
    void exitConfigurationMode();

    /*!
     * Commands the gyro to reset its configuration.
     */
    void resetConfiguration();

    /*!
     * Commands the gyro to set its rotation format to rate.
     */
    void setRotationFormatRate();
    /*!
     * Commands the gyro to set its rotation units to radians.
     */
    void setRotationUnitsDegrees();
    /*!
     * Commands the gyro to set its axis configuration.
     */
    void setAxis();
    /*!
     * Commands the gyro to apply a data rate.
     */
    void setDataRate();

    /*!
     * Processes currently available gyro data and publish it.
     */
    void processData();

private:
    /*!
     * Writes a command to the gyro.
     * @param[in] command The command.
     */
    void writeCommand(std::string command);

    GyroSettings *_gyroSettings;

    MTM1M3_gyroDataC *_gyroData;
    MTM1M3_logevent_gyroWarningC *_gyroWarning;

    uint64_t _lastSampleTimestamp;
    uint64_t _lastBITTimestamp;
    uint64_t _lastErrorTimestamp;

    bool _errorCleared;
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* GYRO_H_ */
