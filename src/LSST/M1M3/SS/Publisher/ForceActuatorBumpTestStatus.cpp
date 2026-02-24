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

#include <limits>

#include "ForceActuatorApplicationSettings.h"
#include "ForceActuatorBumpTestStatus.h"
#include "M1M3SSPublisher.h"

using namespace LSST::M1M3::SS;
using namespace MTM1M3;

ForceActuatorBumpTestStatus::ForceActuatorBumpTestStatus(token) { reset(false); }

void ForceActuatorBumpTestStatus::reset(bool send_data) {
    for (int i = 0; i < FA_COUNT; i++) {
        primaryTest[i] = MTM1M3_shared_BumpTest_NotTested;
        if (i < FA_S_COUNT) {
            secondaryTest[i] = MTM1M3_shared_BumpTest_NotTested;
        }
    }
    _should_send = true;
    if (send_data) {
        send();
    }
}

float ForceActuatorBumpTestStatus::minimal_tested_distance(int index, int& min_actuator_id) {
    float min_distance = std::numeric_limits<float>::infinity();
    min_actuator_id = -1;

    auto& faa_settings = ForceActuatorApplicationSettings::instance();

    for (int i = 0; i < FA_COUNT; i++) {
        if (i == index) {
            continue;
        }

        if (primary_tested(i)) {
            float distance = faa_settings.actuator_distance(index, i);
            if (distance < min_distance) {
                min_actuator_id = faa_settings.ZIndexToActuatorId(i);
                min_distance = distance;
            }
        }

        if (i < FA_S_COUNT && secondary_tested(i)) {
            int z_index = faa_settings.SecondaryCylinderIndexToZIndex[i];
            float distance = faa_settings.actuator_distance(index, z_index);
            if (distance < min_distance) {
                min_actuator_id = faa_settings.ZIndexToActuatorId(z_index);
                min_distance = distance;
            }
        }
    }

    return min_distance;
}

void ForceActuatorBumpTestStatus::trigger_bump_test(int z_index, bool test_primary, bool test_secondary) {
    auto& faa_settings = ForceActuatorApplicationSettings::instance();

    auto secondary_index = faa_settings.ZIndexToSecondaryCylinderIndex[z_index];

    if (test_primary) {
        primaryTest[z_index] = MTM1M3_shared_BumpTest_Triggered;
    }

    if (test_secondary) {
        secondaryTest[secondary_index] = MTM1M3_shared_BumpTest_Triggered;
    }

    _should_send = true;
    send();
}

bool ForceActuatorBumpTestStatus::is_tested(int status) {
    switch (status) {
        case MTM1M3_shared_BumpTest_NotTested:
        case MTM1M3_shared_BumpTest_Passed:
        case MTM1M3_shared_BumpTest_Failed_Timeout:
        case MTM1M3_shared_BumpTest_Failed_TestedPositive_Overshoot:
        case MTM1M3_shared_BumpTest_Failed_TestedPositive_Undershoot:
        case MTM1M3_shared_BumpTest_Failed_TestedNegative_Overshoot:
        case MTM1M3_shared_BumpTest_Failed_TestedNegative_Undershoot:
        case MTM1M3_shared_BumpTest_Failed_NonTestedProblem:
            return false;
            break;
        default:
            return true;
    }
}

bool ForceActuatorBumpTestStatus::test_in_progress() {
    for (int i = 0; i < FA_COUNT; i++) {
        if (is_tested(primaryTest[i])) {
            return true;
        }
        if (i < FA_S_COUNT && is_tested(secondaryTest[i])) {
            return true;
        }
    }

    return false;
}

void ForceActuatorBumpTestStatus::stop_all() {
    for (int i = 0; i < FA_Z_COUNT; i++) {
        if (primaryTest[i] != MTM1M3_shared_BumpTest_NotTested) {
            _should_send = true;
            primaryTest[i] = MTM1M3_shared_BumpTest_NotTested;
        }
        if (i < FA_S_COUNT && secondaryTest[i] != MTM1M3_shared_BumpTest_NotTested) {
            _should_send = true;
            secondaryTest[i] = MTM1M3_shared_BumpTest_NotTested;
        }
    }

    send();
}

void ForceActuatorBumpTestStatus::send(bool force) {
    if (_should_send || force) {
        M1M3SSPublisher::instance().logForceActuatorBumpTestStatus(this);
        _should_send = false;
    }
}
