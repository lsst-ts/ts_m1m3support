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

#ifndef GYRO_H_
#define GYRO_H_

#include <DataTypes.h>
#include <string>

struct MTM1M3_gyroDataC;
struct MTM1M3_logevent_gyroWarningC;

namespace LSST {
namespace M1M3 {
namespace SS {

class GyroSettings;
struct SupportFPGAData;
class M1M3SSPublisher;

/*!
 * The class used to process gyro data.
 */
class Gyro {
private:
    GyroSettings* gyroSettings;
    M1M3SSPublisher* publisher;

    MTM1M3_gyroDataC* gyroData;
    MTM1M3_logevent_gyroWarningC* gyroWarning;

    uint64_t lastSampleTimestamp;
    uint64_t lastBITTimestamp;
    uint64_t lastErrorTimestamp;

    bool errorCleared;

public:
    /*!
     * Instantiates the gyro sensor.
     * @param[in] gyroSettings The gyro settings.
     * @param[in] publisher The publisher.
     */
    Gyro(GyroSettings* gyroSettings, M1M3SSPublisher* publisher);

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
    void setRotationUnitsRadians();
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
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* GYRO_H_ */
