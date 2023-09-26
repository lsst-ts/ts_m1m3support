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
    weightSupportedPercent = 0;
    waitAirPressure = false;

    memset(waitHardpoint, 0, sizeof(waitHardpoint));
    memset(waitXForceActuator, 0, sizeof(waitXForceActuator));
    memset(waitYForceActuator, 0, sizeof(waitYForceActuator));
    memset(waitZForceActuator, 0, sizeof(waitZForceActuator));

    _updated = true;
}

void RaisingLoweringInfo::sendUpdates(bool force) {
    if (_updated || force) {
        M1M3SSPublisher::instance().logRaisingLoweringInfo(this);
        _updated = false;
    }
}

void RaisingLoweringInfo::incSupportPercentage() {
    SPDLOG_TRACE("Incrementing support percentage");
    weightSupportedPercent += ForceActuatorSettings::instance().raiseIncrementPercentage;
    if (supportPercentageFilled()) {
        weightSupportedPercent = 100.0;
    }
    _updated = true;
}

void RaisingLoweringInfo::decSupportPercentage() {
    SPDLOG_TRACE("Decrementing support percentage");
    weightSupportedPercent -= ForceActuatorSettings::instance().lowerDecrementPercentage;
    if (supportPercentageZeroed()) {
        weightSupportedPercent = 0.0;
    }
    _updated = true;
}

void RaisingLoweringInfo::zeroSupportPercentage() {
    SPDLOG_DEBUG("Zero support percentage");
    if (weightSupportedPercent != 0) {
        weightSupportedPercent = 0;
        sendUpdates(true);
    }
}

void RaisingLoweringInfo::fillSupportPercentage() {
    SPDLOG_DEBUG("Filled support percentage");
    if (weightSupportedPercent != 100) {
        weightSupportedPercent = 100;
        sendUpdates(true);
    }
}

bool RaisingLoweringInfo::supportPercentageFilled() { return weightSupportedPercent >= 100.0; }

bool RaisingLoweringInfo::supportPercentageZeroed() { return weightSupportedPercent <= 0; }

void RaisingLoweringInfo::setTimeTimeout(double new_timeTimeout) {
    if (timeTimeout != new_timeTimeout) {
        _updated = true;
        timeTimeout = new_timeTimeout;
    }
}

void RaisingLoweringInfo::setHPWait(size_t hpIndex, bool newWait) {
    if (waitHardpoint[hpIndex] != newWait) {
        _updated = true;
        waitHardpoint[hpIndex] = newWait;
    }
}

void RaisingLoweringInfo::setFAXWait(size_t faXIndex, bool newWait) {
    if (waitXForceActuator[faXIndex] != newWait) {
        _updated = true;
        waitXForceActuator[faXIndex] = newWait;
    }
}

void RaisingLoweringInfo::setFAYWait(size_t faYIndex, bool newWait) {
    if (waitYForceActuator[faYIndex] != newWait) {
        _updated = true;
        waitYForceActuator[faYIndex] = newWait;
    }
}

void RaisingLoweringInfo::setFAZWait(size_t faZIndex, bool newWait) {
    if (waitZForceActuator[faZIndex] != newWait) {
        _updated = true;
        waitZForceActuator[faZIndex] = newWait;
    }
}

void RaisingLoweringInfo::setWaitAirPressure(bool newWait) {
    if (waitAirPressure != newWait) {
        _updated = true;
        waitAirPressure = newWait;
        if (newWait) {
            SPDLOG_INFO("Waiting for air pressure in the hardpoints");
        } else {
            SPDLOG_INFO("Hardpoint air pressure stabilized, raising the mirror");
        }
    }
}
