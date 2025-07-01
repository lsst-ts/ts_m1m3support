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
#include <thread>
#include <vector>

#include <catch2/catch_all.hpp>

#include <SAL_MTM1M3.h>

#include "PreclippedForces.h"

using namespace Catch::Matchers;
using namespace LSST::M1M3::SS;
using namespace std::chrono_literals;

class TestPreclipped : public PreclippedForces<MTM1M3_logevent_preclippedBalanceForcesC> {
public:
    TestPreclipped()
            : PreclippedForces<MTM1M3_logevent_preclippedBalanceForcesC>(
                      [this](MTM1M3_logevent_preclippedBalanceForcesC*) { send_count++; }, 20.0f, 20ms),
              send_count(0) {}

    int send_count;
};

TEST_CASE("M1M3 PreclippedForces template - time", "[PreclippedForces]") {
    TestPreclipped test;

    test.calculate_forces_and_moments();

    CHECK(test.check_changes() == true);
    CHECK(test.send_count == 1);

    test.xForces = std::vector<float>(FA_X_COUNT, 0);
    test.yForces = std::vector<float>(FA_Y_COUNT, 0);
    test.zForces = std::vector<float>(FA_Z_COUNT, 0);

    test.calculate_forces_and_moments();

    CHECK_THAT(test.fx, WithinAbs(0, 1e-1));
    CHECK_THAT(test.fy, WithinAbs(0, 1e-1));
    CHECK_THAT(test.fz, WithinAbs(0, 1e-1));
    CHECK_THAT(test.mx, WithinAbs(0, 1e-1));
    CHECK_THAT(test.my, WithinAbs(0, 1e-1));
    CHECK_THAT(test.mz, WithinAbs(0, 1e-1));
    CHECK_THAT(test.forceMagnitude, WithinAbs(0, 1e-1));

    CHECK(test.check_changes() == true);
    CHECK(test.send_count == 1);

    std::this_thread::sleep_for(21ms);

    CHECK(test.check_changes() == false);
    CHECK(test.send_count == 2);

    test.xForces[0] = 1;

    CHECK(test.check_changes() == true);
    CHECK(test.send_count == 2);

    // some moment is created with 1N force on x actuators

    test.xForces = std::vector<float>(FA_X_COUNT, 1);

    test.calculate_forces_and_moments();

    CHECK_THAT(test.fx, WithinAbs(12.0, 1e-1));
    CHECK_THAT(test.fy, WithinAbs(0, 1e-1));
    CHECK_THAT(test.fz, WithinAbs(0, 1e-1));
    CHECK_THAT(test.mx, WithinAbs(0, 1e-1));
    CHECK_THAT(test.my, WithinAbs(-25.9, 1e-1));
    CHECK_THAT(test.mz, WithinAbs(0, 1e-1));
    CHECK_THAT(test.forceMagnitude, WithinAbs(12.0, 1e-1));

    std::this_thread::sleep_for(21ms);

    CHECK(test.check_changes() == true);
    CHECK(test.send_count == 3);
    CHECK(test.check_changes() == false);
    CHECK(test.send_count == 3);
    CHECK(test.check_changes() == false);
    CHECK(test.send_count == 3);
    CHECK(test.check_changes() == false);
    CHECK(test.send_count == 3);
}

TEST_CASE("M1M3 PreclippedForces template - forces difference", "[PreclippedForces]") {
    TestPreclipped test;

    test.xForces = std::vector<float>(FA_X_COUNT, 0);
    test.yForces = std::vector<float>(FA_Y_COUNT, 0);
    test.zForces = std::vector<float>(FA_Z_COUNT, 0);

    test.calculate_forces_and_moments();

    CHECK_THAT(test.fx, WithinAbs(0, 1e-1));
    CHECK_THAT(test.fy, WithinAbs(0, 1e-1));
    CHECK_THAT(test.fz, WithinAbs(0, 1e-1));
    CHECK_THAT(test.mx, WithinAbs(0, 1e-1));
    CHECK_THAT(test.my, WithinAbs(0, 1e-1));
    CHECK_THAT(test.mz, WithinAbs(0, 1e-1));
    CHECK_THAT(test.forceMagnitude, WithinAbs(0, 1e-1));

    CHECK(test.check_changes() == true);
    CHECK(test.send_count == 1);

    for (int i = 0; i < FA_Z_COUNT; i++) {
        test.zForces[i] = 0.2;
    }

    test.calculate_forces_and_moments();

    CHECK_THAT(test.forceMagnitude, WithinAbs(31.2, 1e-1));

    CHECK(test.check_changes() == true);
    CHECK(test.send_count == 2);

    for (int i = 0; i < FA_Z_COUNT; i++) {
        test.zForces[i] = 0.3;
    }

    test.calculate_forces_and_moments();

    CHECK_THAT(test.forceMagnitude, WithinAbs(46.8, 1e-1));

    CHECK(test.check_changes() == true);
    CHECK(test.send_count == 2);

    std::this_thread::sleep_for(21ms);

    CHECK(test.check_changes() == false);
    CHECK(test.send_count == 3);
    CHECK(test.check_changes() == false);
    CHECK(test.send_count == 3);

    std::this_thread::sleep_for(21ms);

    CHECK(test.check_changes() == false);
    CHECK(test.send_count == 3);
    CHECK(test.check_changes() == false);
    CHECK(test.send_count == 3);
}
