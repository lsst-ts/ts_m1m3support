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

#ifndef LSST_AIRSUPPLYSTATUS_H
#define LSST_AIRSUPPLYSTATUS_H

#include <chrono>

#include <SAL_MTM1M3.h>

#include <cRIO/Singleton.h>

#include "DataTypes.h"

namespace LSST {
namespace M1M3 {
namespace SS {

/**
 * Handles air supply status.
 */
class AirSupplyStatus : public MTM1M3_logevent_airSupplyStatusC, public cRIO::Singleton<AirSupplyStatus> {
public:
    AirSupplyStatus(token);

    void send();

    void setAirCommanded(bool _airCommandedOn);

    /**
     * Set outputs, returns if mismatch is detected.
     *
     * @param _timestamp timestamp when new commended output was readout
     * @param _airCommandOutputOn new air commanded output
     *
     * @return true if airCommandedOn != airCommandOutputOn
     */
    bool setOutputs(double _timestamp, bool _airCommandOutputOn);

    bool setInputs(double _timestamp, bool _airValveClosed, bool _airValveOpened);

private:
    bool _updated;
    std::chrono::steady_clock::time_point _airToggledTime;
};

}  // namespace SS
}  // namespace M1M3
}  // namespace LSST

#endif  // ! LSST_AIRSUPPLYSTATUS_H
