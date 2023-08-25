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

#include <BoosterValveStatus.h>
#include <M1M3SSPublisher.h>

using namespace LSST::M1M3::SS;

BoosterValveStatus::BoosterValveStatus(token) {
    opened = false;
    slewTriggered = false;
    userTriggered = false;
    followingErrorTriggered = false;
    accelerometerTriggered = false;
}

void BoosterValveStatus::enterSlew() {
    if (slewTriggered != true) {
        SPDLOG_DEBUG("Triggered slew for booster valves");
        slewTriggered = true;
        log();
    }
}

void BoosterValveStatus::exitSlew() {
    if (slewTriggered != false) {
        SPDLOG_DEBUG("Exit slew for booster valves");
        slewTriggered = false;
        log();
    }
}

void BoosterValveStatus::setUserTriggered(bool newUserTriggered) {
    if (userTriggered != newUserTriggered) {
        SPDLOG_INFO("Observer would like to have booster valves {}", newUserTriggered ? "opened" : "closed");
        userTriggered = newUserTriggered;
        log();
    }
}

void BoosterValveStatus::setFollowingErrorTriggered(bool newFollowingErrorTriggered) {
    if (followingErrorTriggered != newFollowingErrorTriggered) {
        followingErrorTriggered = newFollowingErrorTriggered;
        log();
    }
}

void BoosterValveStatus::setAccelerometerTriggered(bool newAccelerometerTriggered) {
    if (accelerometerTriggered != newAccelerometerTriggered) {
        accelerometerTriggered = newAccelerometerTriggered;
        log();
    }
}

void BoosterValveStatus::reset() {
    opened = false;
    slewTriggered = false;
    userTriggered = false;
    followingErrorTriggered = false;
    accelerometerTriggered = false;
    log();
}

void BoosterValveStatus::log() {
    bool open = slewTriggered || userTriggered || followingErrorTriggered || accelerometerTriggered;
    if (open != opened) {
        SPDLOG_INFO("Booster valve {}", open ? "opened" : "closed");
        opened = open;
    }
    M1M3SSPublisher::instance().logBoosterValveStatus(this);
}
