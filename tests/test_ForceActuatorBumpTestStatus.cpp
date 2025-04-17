/*
 * This file is part of LSST M1M3 SS test suite. Tests ForceController.
 *
 * Developed for the LSST Telescope and Site Systems.
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

#include <catch2/catch_all.hpp>

#include <SAL_MTM1M3.h>

#include <ForceActuatorBumpTestStatus.h>
#include <M1M3SSPublisher.h>

using namespace LSST::M1M3::SS;
using namespace Catch::Matchers;

TEST_CASE("M1M3 ForceActuatorBumpTestStatus data", "[ForceActuatorBumpTestStatus]") {
    std::shared_ptr<SAL_MTM1M3> m1m3SAL = std::make_shared<SAL_MTM1M3>();
    M1M3SSPublisher::instance().setSAL(m1m3SAL);

    auto &fa_bump_status = ForceActuatorBumpTestStatus::instance();

    int actuator_id;
    CHECK(fa_bump_status.minimal_tested_distance(10, actuator_id) == std::numeric_limits<float>::infinity());
    CHECK(fa_bump_status.test_in_progress() == false);

    CHECK_NOTHROW(fa_bump_status.trigger_bump_test(101, true, false));
    CHECK(fa_bump_status.test_in_progress() == true);

    CHECK_THAT(fa_bump_status.minimal_tested_distance(10, actuator_id), WithinRel(2.4005, 1e-3));
    CHECK(actuator_id == 101);

    CHECK_THAT(fa_bump_status.minimal_tested_distance(155, actuator_id), WithinRel(3.784299, 1e-3));
    CHECK(actuator_id == 101);

    CHECK_NOTHROW(fa_bump_status.trigger_bump_test(437, true, true));

    CHECK_THAT(fa_bump_status.minimal_tested_distance(10, actuator_id), WithinRel(2.4005, 1e-3));
    CHECK(actuator_id == 101);

    CHECK_THAT(fa_bump_status.minimal_tested_distance(155, actuator_id), WithinRel(1.401, 1e-3));
    CHECK(actuator_id == 437);

    CHECK_NOTHROW(fa_bump_status.stop_all());

    CHECK(fa_bump_status.minimal_tested_distance(1, actuator_id) == std::numeric_limits<float>::infinity());
    CHECK(fa_bump_status.test_in_progress() == false);
}
