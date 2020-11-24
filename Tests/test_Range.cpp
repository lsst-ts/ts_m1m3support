/*
 * This file is part of LSST M1M3 tests. Tests Range functions.
 *
 * Developed for the Telescope & Site Software Systems.  This product includes
 * software developed by the LSST Project (https://www.lsst.org). See the
 * COPYRIGHT file at the top-level directory of this distribution for details
 * of code ownership.
 *
 * This program is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation, either version 3 of the License, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program. If not, see <https://www.gnu.org/licenses/>.
 */

#define CATCH_CONFIG_MAIN
#include <catch/catch.hpp>

#include <Range.h>

using namespace LSST::M1M3::SS;

TEST_CASE("InRangeWithWarning", "[Range]") {
    int counter = 0;
    REQUIRE(Range::InRangeWithWarning(1.23, 1.25, 1.24, counter, 1, "Test case {} {}", "test", 10) == true);
    REQUIRE(counter == 0);
    REQUIRE(Range::InRangeWithWarning(1.23, 1.25, 1.26, counter, 1, "Test case {} {}", "test", 10) == false);
    REQUIRE(counter == 1);
    REQUIRE(Range::InRangeWithWarning(1.23, 1.25, 1.22, counter, 1, "Test case {} {}", "test", 10) == false);
    REQUIRE(counter == 2);
}
