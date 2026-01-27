/*
 * This file is part of LSST M1M3 SS test suite. Tests Modbus buffer class.
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

#include <catch2/catch_all.hpp>

#include <cmath>
#include <memory>
#include <vector>

#include <SAL_MTM1M3.h>

#include <cRIO/DataTypes.h>

#include <FABumpTestData.h>
#include <ForceActuatorApplicationSettings.h>

using namespace LSST::M1M3::SS;
using namespace Catch::Matchers;

TEST_CASE("fromRaw", "[FABumpTestData]") {
    constexpr int size = 10;

    FABumpTestData data(size);

    std::vector<float> zeros(FA_COUNT, 0);
    std::vector<int> states(FA_COUNT, MTM1M3::MTM1M3_shared_BumpTest_TestingPositive);

    CHECK(data.empty() == true);

    for (size_t i = 0; i < size; i++) {
        CHECK(data.size() == i);
        CHECK(data.test_actuator(0, MTM1M3::MTM1M3_shared_BumpTestType_Primary, 0, 0.2, 0.1) ==
              BumpTestStatus::NO_DATA);

        CHECK_NOTHROW(data.add_data(zeros, zeros, zeros, zeros, zeros, states, states));
        CHECK(data.empty() == false);
    }

    CHECK(data.size() == size);

    CHECK(data.test_actuator(0, MTM1M3::MTM1M3_shared_BumpTestType_Primary, 0, 0.2, 0.1) ==
          BumpTestStatus::PASSED);

    for (size_t i = 0; i < size / 2; i++) {
        CHECK(data.size() == size);
        CHECK(data.test_actuator(0, MTM1M3::MTM1M3_shared_BumpTestType_Primary, 0, 0.2, 0.1) ==
              BumpTestStatus::PASSED);

        data.add_data(zeros, zeros, zeros, zeros, zeros, states, states);
        CHECK(data.empty() == false);
    }

    CHECK(data.test_actuator(1, MTM1M3::MTM1M3_shared_BumpTestType_Y, 0, 0.2, 0.1) == BumpTestStatus::PASSED);

    std::vector<float> warnings(FA_COUNT, 0.15);
    for (int i = 0; i < size; i++) {
        std::vector<float> x(FA_COUNT, sin(M_PI * static_cast<float>(i) / size / 2) * 0.21);
        std::vector<float> y(FA_COUNT, -0.15 + sin(M_PI * static_cast<float>(i) / size / 2) * 0.1);
        CHECK_NOTHROW(data.add_data(x, y, zeros, warnings, zeros, states, states));
    }

    CHECK(data.test_actuator(0, MTM1M3::MTM1M3_shared_BumpTestType_Primary, 0, 0.2, 0.1) ==
          BumpTestStatus::OVERSHOOT_WARNING);

    CHECK(data.test_actuator(0, MTM1M3::MTM1M3_shared_BumpTestType_X, 0, 0.2, 0.1) ==
          BumpTestStatus::INVALID_ACTUATOR);

    auto stat_x = data.statistics(147, 11, MTM1M3::MTM1M3_shared_BumpTestType_X, 0);
    CHECK_THAT(stat_x.max, WithinAbs(0.207414553, 1e-3));
    CHECK_THAT(stat_x.error_rms, WithinAbs(0.140872285, 1e-3));

    CHECK(data.test_actuator(147, MTM1M3::MTM1M3_shared_BumpTestType_X, 0, 0.2, 0.1) ==
          BumpTestStatus::OVERSHOOT_WARNING);

    auto stat = data.statistics(0, 0, MTM1M3::MTM1M3_shared_BumpTestType_Y, 0);
    CHECK_THAT(stat.min, WithinAbs(-0.15, 1e-3));
    CHECK_THAT(stat.max, WithinAbs(-0.051231164, 1e-3));
    CHECK_THAT(stat.average, WithinAbs(-0.091468975, 1e-3));

    CHECK_NOTHROW(
            data.add_data(std::vector<float>(FA_COUNT, 0.21), zeros, zeros, zeros, zeros, states, states));

    for (int x = 0; x < FA_X_COUNT; x++) {
        int fa_index = ForceActuatorApplicationSettings::instance().XIndexToZIndex[x];
        auto stat = data.statistics(fa_index, x, MTM1M3::MTM1M3_shared_BumpTestType_X, 0);
        CHECK_THAT(stat.min, WithinAbs(0.032851238, 1e-5));
        CHECK_THAT(stat.max, WithinAbs(0.21, 1e-6));
        CHECK_THAT(stat.average, WithinAbs(0.143915147, 1e-6));
    }

    CHECK_NOTHROW(data.add_data(zeros, zeros, zeros, zeros, zeros, states, states));

    for (int x = 0; x < FA_X_COUNT; x++) {
        int fa_index = ForceActuatorApplicationSettings::instance().XIndexToZIndex[x];
        auto stat = data.statistics(fa_index, x, MTM1M3::MTM1M3_shared_BumpTestType_X, 0);
        CHECK_THAT(stat.min, WithinAbs(0, 1e-5));
        CHECK_THAT(stat.max, WithinAbs(0.21, 1e-6));
        CHECK_THAT(stat.average, WithinAbs(0.140630037, 1e-6));
        CHECK_THAT(stat.error_rms, WithinAbs(0.155393302, 1e-3));
    }

    for (int y = 0; y < FA_Y_COUNT; y++) {
        int fa_index = ForceActuatorApplicationSettings::instance().YIndexToZIndex[y];
        auto stat = data.statistics(fa_index, y, MTM1M3::MTM1M3_shared_BumpTestType_Y, 0);
        CHECK_THAT(stat.min, WithinAbs(-0.119098298, 1e-5));
        CHECK_THAT(stat.max, WithinAbs(0, 1e-6));
        CHECK_THAT(stat.average, WithinAbs(-0.06303332, 1e-6));
        CHECK_THAT(stat.error_rms, WithinAbs(0.073386133, 1e-3));
    }
}
