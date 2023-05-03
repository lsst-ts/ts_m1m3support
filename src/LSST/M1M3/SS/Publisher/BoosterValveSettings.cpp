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

#include <BoosterValveSettings.h>

using namespace LSST::M1M3::SS;

BoosterValveSettings::BoosterValveSettings(token) {
    followingErrorTriggerEnabled = false;
    followingErrorTriggerOpen = 50;
    followingErrorTriggerClose = 45;

    accelerometerTriggerEnabled = false;

    accelerometerXTriggerOpen = 1;
    accelerometerXTriggerClose = 0.9;

    accelerometerYTriggerOpen = 1;
    accelerometerYTriggerClose = 0.9;

    accelerometerZTriggerOpen = 1;
    accelerometerZTriggerClose = 0.9;
}

void BoosterValveSettings::load(YAML::Node node) {
    auto followingError = node["FollowingError"];
    followingErrorTriggerEnabled = followingError["Enabled"].as<bool>();
    followingErrorTriggerOpen = followingError["Open"].as<float>();
    followingErrorTriggerClose = followingError["Close"].as<float>();

    auto accelerometer = node["Accelerometer"];
    accelerometerTriggerEnabled = accelerometer["Enabled"].as<bool>();

    accelerometerXTriggerOpen = accelerometer["OpenX"].as<float>();
    accelerometerXTriggerClose = accelerometer["CloseX"].as<float>();

    accelerometerYTriggerOpen = accelerometer["OpenY"].as<float>();
    accelerometerYTriggerClose = accelerometer["CloseY"].as<float>();

    accelerometerZTriggerOpen = accelerometer["OpenZ"].as<float>();
    accelerometerZTriggerClose = accelerometer["CloseZ"].as<float>();

    log();
}