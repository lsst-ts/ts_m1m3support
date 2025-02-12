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

#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>

#include <ForceActuatorApplicationSettings.h>

using namespace LSST::M1M3::SS;

TEST_CASE("M1M3 ForceActuator data", "[ForceActuatorApplicationSettings]") {
    ForceActuatorApplicationSettings settings;

    REQUIRE(settings.ActuatorIdToZIndex(100) < 0);
    REQUIRE(settings.ActuatorIdToZIndex(101) == 0);
    REQUIRE(settings.ActuatorIdToZIndex(150) < 0);
    REQUIRE(settings.ActuatorIdToZIndex(443) == 155);
    REQUIRE(settings.ActuatorIdToZIndex(444) < 0);

    REQUIRE(settings.ZIndexToActuatorId(0) == 101);
    REQUIRE(settings.ZIndexToActuatorId(155) == 443);
    REQUIRE(settings.ZIndexToActuatorId(156) < 0);
    REQUIRE(settings.ZIndexToActuatorId(-2) < 0);

    REQUIRE_THAT(settings.ActuatorDistance(2, 3), Catch::Matchers::WithinRel(2.34, 0.001));
}
