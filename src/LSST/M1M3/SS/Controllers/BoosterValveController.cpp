/*
 * This file is part of LSST M1M3 support system package.
 *
 * Developed for the LSST Telescope & Site Software Systems.
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

#include "BoosterValveController.h"
#include "BoosterValveSettings.h"
#include "BoosterValveStatus.h"
#include "ForceActuatorApplicationSettings.h"
#include "ForceActuatorData.h"
#include "SettingReader.h"

using namespace LSST::M1M3::SS;

BoosterValveController::BoosterValveController(token) {}

void BoosterValveController::checkTriggers() {
    checkFollowingErrors();
    checkAccelerometer();
}

void BoosterValveController::checkFollowingErrors() {
    if (BoosterValveSettings::instance().followingErrorTriggerEnabled == false) {
        return;
    }
    if (BoosterValveStatus::instance().followingErrorTriggered == false) {
        _checkFollowingErrorOpen();
    } else {
        _checkFollowingErrorClose();
    }
}

void BoosterValveController::checkAccelerometer() {
    if (BoosterValveSettings::instance().accelerometerTriggerEnabled == false) {
        return;
    }
    if (BoosterValveStatus::instance().accelerometerTriggered == false) {
        _checkAccelerometerOpen();
    } else {
        _checkAccelerometerClose();
    }
}

void BoosterValveController::_checkFollowingErrorOpen() {
    auto &forceActuatorData = ForceActuatorData::instance();
    auto &faa_settings = ForceActuatorApplicationSettings::instance();
    auto trigger = BoosterValveSettings::instance().followingErrorTriggerOpen;
    for (int i = 0; i < FA_COUNT; i++) {
        auto primaryFE = forceActuatorData.primaryCylinderFollowingError[i];
        if (fabs(primaryFE) > trigger) {
            BoosterValveStatus::instance().setFollowingErrorTriggered(true);
            SPDLOG_INFO(
                    "Booster valve triggered to open by force actuator ID {} "
                    "({}) primary cylinder "
                    "following error (as {} > {})",
                    faa_settings.ZIndexToActuatorId(i), i, primaryFE, trigger);
            break;
        }

        if (i < FA_S_COUNT) {
            auto secondaryFE = forceActuatorData.secondaryCylinderFollowingError[i];
            if (fabs(secondaryFE) > trigger) {
                BoosterValveStatus::instance().setFollowingErrorTriggered(true);
                SPDLOG_INFO(
                        "Booster valve triggered to open by force actuator ID {} ({}) "
                        "secondary cylinder "
                        "following error (as {} > {})",
                        faa_settings.SecondaryCylinderIndexToActuatorId(i), i, primaryFE, trigger);
                break;
            }
        }
    }
}

void BoosterValveController::_checkFollowingErrorClose() {
    auto &forceActuatorData = ForceActuatorData::instance();
    auto trigger = BoosterValveSettings::instance().followingErrorTriggerClose;
    bool shouldClose = true;
    for (int i = 0; i < FA_COUNT; i++) {
        auto primaryFE = forceActuatorData.primaryCylinderFollowingError[i];
        if (fabs(primaryFE) > trigger) {
            shouldClose = false;
            break;
        }

        if (i < FA_S_COUNT) {
            auto secondaryFE = forceActuatorData.secondaryCylinderFollowingError[i];
            if (fabs(secondaryFE) > trigger) {
                shouldClose = false;
                break;
            }
        }
    }

    if (shouldClose == true) {
        BoosterValveStatus::instance().setFollowingErrorTriggered(false);
        SPDLOG_INFO(
                "Booster valve following error close conditions are met, "
                "booster valve can close");
    }
}

void BoosterValveController::_checkAccelerometerOpen() {
    auto accelerometerData = M1M3SSPublisher::instance().getAccelerometerData();
    auto triggerX = BoosterValveSettings::instance().accelerometerXTriggerOpen;
    auto triggerY = BoosterValveSettings::instance().accelerometerYTriggerOpen;
    auto triggerZ = BoosterValveSettings::instance().accelerometerZTriggerOpen;

    if (fabs(accelerometerData->angularAccelerationX) > triggerX) {
        BoosterValveStatus::instance().setAccelerometerTriggered(true);
        SPDLOG_INFO(
                "Booster valves angular acceleration in X trigger conditions "
                "are met ({} > {}), booster "
                "valves shall open",
                accelerometerData->angularAccelerationX, triggerX);
    } else if (fabs(accelerometerData->angularAccelerationY) > triggerY) {
        BoosterValveStatus::instance().setAccelerometerTriggered(true);
        SPDLOG_INFO(
                "Booster valves angular acceleration in Y trigger conditions "
                "are met ({} > {}), booster "
                "valves shall open",
                accelerometerData->angularAccelerationY, triggerY);
    } else if (fabs(accelerometerData->angularAccelerationZ) > triggerZ) {
        BoosterValveStatus::instance().setAccelerometerTriggered(true);
        SPDLOG_INFO(
                "Booster valves angular acceleration in Z trigger conditions "
                "are met ({} > {}), booster "
                "valves shall open",
                accelerometerData->angularAccelerationZ, triggerZ);
    }
}

void BoosterValveController::_checkAccelerometerClose() {
    auto accelerometerData = M1M3SSPublisher::instance().getAccelerometerData();
    auto triggerX = BoosterValveSettings::instance().accelerometerXTriggerClose;
    auto triggerY = BoosterValveSettings::instance().accelerometerYTriggerClose;
    auto triggerZ = BoosterValveSettings::instance().accelerometerZTriggerClose;

    if (fabs(accelerometerData->angularAccelerationX) < triggerX &&
        fabs(accelerometerData->angularAccelerationY) < triggerY &&
        fabs(accelerometerData->angularAccelerationZ) < triggerZ) {
        BoosterValveStatus::instance().setAccelerometerTriggered(false);
        SPDLOG_INFO(
                "Booster valves accelerometer trigger conditions are met, "
                "booster valves can close");
    }
}
