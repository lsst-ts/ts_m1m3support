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

#include <catch2/catch.hpp>

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
    std::vector<int> states(FA_COUNT, MTM1M3::MTM1M3_shared_BumpTest_Passed);

    for (int i = 0; i < size; i++) {
        data.add_data(zeros, zeros, zeros, zeros, zeros, states, states);
    }
}
