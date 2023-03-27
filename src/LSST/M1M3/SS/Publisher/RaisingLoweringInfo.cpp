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

#include <spdlog/spdlog.h>

#include <ForceActuatorSettings.h>
#include <M1M3SSPublisher.h>
#include <RaisingLoweringInfo.h>

using namespace LSST::M1M3::SS;

RaisingLoweringInfo::RaisingLoweringInfo(token) {
    supportPercentage = 0;
    waitAirPressure = false;

    memset(waitHardpoint, 0, sizeof(waitHardpoint));
    memset(waitForceActuator, 0, sizeof(waitForceActuator));

    _updated = true;
}

void RaisingLoweringInfo::incSupportPercentage() {
    SPDLOG_TRACE("Incrementing support percentage");
    supportPercentage += ForceActuatorSettings::instance().raiseIncrementPercentage;
    if (supportPercentageFilled()) {
        supportPercentage = 100.0;
    }
    M1M3SSPublisher::instance().logRaisingLoweringInfo(this);
}

void RaisingLoweringInfo::decSupportPercentage() {
    SPDLOG_TRACE("Decrementing support percentage");
    supportPercentage -= ForceActuatorSettings::instance().lowerDecrementPercentage;
    if (supportPercentageZeroed()) {
        supportPercentage = 0.0;
    }
    M1M3SSPublisher::instance().logRaisingLoweringInfo(this);
}

void RaisingLoweringInfo::zeroSupportPercentage() {
    SPDLOG_DEBUG("Zero support percentage");
    supportPercentage = 0;
    M1M3SSPublisher::instance().logRaisingLoweringInfo(this);
}

void RaisingLoweringInfo::fillSupportPercentage() {
    SPDLOG_DEBUG("Filled support percentage");
    supportPercentage = 100;
    M1M3SSPublisher::instance().logRaisingLoweringInfo(this);
}

bool RaisingLoweringInfo::supportPercentageFilled() { return supportPercentage >= 100.0; }

bool RaisingLoweringInfo::supportPercentageZeroed() { return supportPercentage <= 0; }
