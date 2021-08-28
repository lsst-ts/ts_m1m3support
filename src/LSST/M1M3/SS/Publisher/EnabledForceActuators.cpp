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

#include <EnabledForceActuators.h>
#include <M1M3SSPublisher.h>
#include <SettingReader.h>

using namespace LSST::M1M3::SS;

EnabledForceActuators::EnabledForceActuators() {
    for (int i = 0; i < FA_COUNT; i++) {
        forceActuatorEnabled[i] = true;
    }
    _shouldSend = true;
}

void EnabledForceActuators::setEnabled(int32_t actuatorId, bool enabled) {
    int32_t actuatorIndex =
            SettingReader::get().getForceActuatorApplicationSettings()->ActuatorIdToZIndex(actuatorId);
    if (forceActuatorEnabled[actuatorIndex] == enabled) {
        return;
    }
    forceActuatorEnabled[actuatorIndex] = enabled;
    if (enabled == false) {
        // if disabling an FA, makes sure its reported force is 0
        MTM1M3_forceActuatorDataC* faData = M1M3SSPublisher::get().getForceActuatorData();
        faData->primaryCylinderForce[actuatorIndex] = 0;
        faData->zForce[actuatorIndex] = 0;

        int secondaryIndex = SettingReader::get()
                                     .getForceActuatorApplicationSettings()
                                     ->ZIndexToSecondaryCylinderIndex[actuatorIndex];
        if (secondaryIndex >= 0) {
            faData->secondaryCylinderForce[secondaryIndex] = 0;
            int xIndex =
                    SettingReader::get().getForceActuatorApplicationSettings()->ZIndexToXIndex[actuatorIndex];
            if (xIndex >= 0) {
                faData->xForce[xIndex] = 0;
            }

            int yIndex =
                    SettingReader::get().getForceActuatorApplicationSettings()->ZIndexToYIndex[actuatorIndex];
            if (yIndex >= 0) {
                faData->yForce[yIndex] = 0;
            }
        }
    }
    _shouldSend = true;
}

void EnabledForceActuators::setEnabledAll() {
    for (int i = 0; i < FA_COUNT; i++) {
        if (forceActuatorEnabled[i] == false) {
            forceActuatorEnabled[i] = true;
            _shouldSend = true;
        }
    }
}

void EnabledForceActuators::log() {
    if (_shouldSend == false) {
        return;
    }
    M1M3SSPublisher::get().logEnabledForceActuators(this);
    _shouldSend = false;
}
