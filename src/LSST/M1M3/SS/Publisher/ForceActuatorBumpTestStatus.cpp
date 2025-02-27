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

#include <ForceActuatorBumpTestStatus.h>
#include <M1M3SSPublisher.h>
#include <SettingReader.h>

using namespace LSST::M1M3::SS;
using namespace MTM1M3;

ForceActuatorBumpTestStatus::ForceActuatorBumpTestStatus(token) { reset(); }

void ForceActuatorBumpTestStatus::reset() {
    for (int i = 0; i < FA_COUNT; i++) {
        primaryTest[i] = MTM1M3_shared_BumpTest_NotTested;
        if (i < FA_S_COUNT) {
            secondaryTest[i] = MTM1M3_shared_BumpTest_NotTested;
        }
    }
    _should_send = true;
    send();
}

void ForceActuatorBumpTestStatus::trigger_bump_test(int actuator_id, bool test_primary, bool test_secondary) {
    auto fa_app_settings = SettingReader::instance().getForceActuatorApplicationSettings();

    auto z_index = fa_app_settings->ActuatorIdToZIndex(actuator_id);
    auto secondary_index = fa_app_settings->ZIndexToSecondaryCylinderIndex[z_index];

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
