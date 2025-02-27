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

using namespace LSST::M1M3::SS;

TEST_CASE("fromRaw", "[FABumpTestData]") {
    constexpr int size = 10;

    FABumpTestData data(size);

    std::vector<float> zeros(FA_COUNT, 0);
    std::vector<int> states(FA_COUNT, MTM1M3::MTM1M3_shared_BumpTest_TestingPositive);

    CHECK(data.empty() == true);

    for (size_t i = 0; i < size; i++) {
        CHECK(data.size() == i);
        CHECK(data.test_actuator(0, 'P', 0, 0.2, 0.1) == BumpTestStatus::NO_DATA);

        CHECK_NOTHROW(data.add_data(zeros, zeros, zeros, zeros, zeros, states, states));
        CHECK(data.empty() == false);
    }

    CHECK(data.size() == size);

    CHECK(data.test_actuator(0, 'P', 0, 0.2, 0.1) == BumpTestStatus::PASSED);

    for (size_t i = 0; i < size / 2; i++) {
        CHECK(data.size() == size);
        CHECK(data.test_actuator(0, 'P', 0, 0.2, 0.1) == BumpTestStatus::PASSED);

        data.add_data(zeros, zeros, zeros, zeros, zeros, states, states);
        CHECK(data.empty() == false);
    }

    std::vector<float> warnings(FA_COUNT, 0.15);
    CHECK_NOTHROW(data.add_data(zeros, zeros, zeros, warnings, zeros, states, states));

    CHECK(data.test_actuator(0, 'P', 0, 0.2, 0.1) == BumpTestStatus::OVERSHOOT_WARNING);

    std::vector<float> errors(FA_COUNT, -0.21);
    CHECK_NOTHROW(data.add_data(errors, zeros, zeros, zeros, zeros, states, states));

    CHECK(data.test_actuator(0, 'X', 0, 0.2, 0.1) == BumpTestStatus::INVALID_ACTUATOR);
    CHECK(data.test_actuator(147, 'X', 0, 0.2, 0.1) == BumpTestStatus::UNDERSHOOT_ERROR);
}
