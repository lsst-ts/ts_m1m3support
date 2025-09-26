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
#include <yaml-cpp/yaml.h>

#include <SAL_MTM1M3C.h>

#include <SlewControllerSettings.h>

using namespace LSST::M1M3::SS;

SlewControllerSettings::SlewControllerSettings(token) {}

void SlewControllerSettings::load(YAML::Node doc) {
    SPDLOG_INFO("Loading SlewControllerSettings");

    triggerBoosterValves = doc["TriggerBoosterValves"].as<bool>();
    useAccelerationForces = doc["UseAccelerationForces"].as<bool>();
    useBalanceForces = doc["UseBalanceForces"].as<bool>();
    useVelocityForces = doc["UseVelocityForces"].as<bool>();

    log();
}

void SlewControllerSettings::set(int slewSettings, bool enableSlewManagement) {
    bool changed = false;
    switch (slewSettings) {
        case MTM1M3::setSlewControllerSettings_AccelerationForces:
            if (useAccelerationForces != enableSlewManagement) {
                useAccelerationForces = enableSlewManagement;
                changed = true;
            }
            break;
        case MTM1M3::setSlewControllerSettings_BalanceForces:
            if (useBalanceForces != enableSlewManagement) {
                useBalanceForces = enableSlewManagement;
                changed = true;
            }
            break;
        case MTM1M3::setSlewControllerSettings_BoosterValves:
            if (triggerBoosterValves != enableSlewManagement) {
                triggerBoosterValves = enableSlewManagement;
                changed = true;
            }
            break;
        case MTM1M3::setSlewControllerSettings_VelocityForces:
            if (useVelocityForces != enableSlewManagement) {
                useVelocityForces = enableSlewManagement;
                changed = true;
            }
            break;
    }
    if (changed) {
        log();
    }
}
