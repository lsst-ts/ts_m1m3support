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

#include <catch2/catch_all.hpp>

#include <ForceActuatorApplicationSettings.h>

using namespace LSST::M1M3::SS;
using namespace Catch::Matchers;

TEST_CASE("M1M3 ForceActuator data", "[ForceActuatorApplicationSettings]") {
    ForceActuatorApplicationSettings settings;

    CHECK(settings.ActuatorIdToZIndex(100) < 0);
    CHECK(settings.ActuatorIdToZIndex(101) == 0);
    CHECK(settings.ActuatorIdToZIndex(150) < 0);
    CHECK(settings.ActuatorIdToZIndex(443) == 155);
    CHECK(settings.ActuatorIdToZIndex(444) < 0);

    CHECK(settings.ZIndexToActuatorId(0) == 101);
    CHECK(settings.ZIndexToActuatorId(155) == 443);
    CHECK(settings.ZIndexToActuatorId(156) < 0);
    CHECK(settings.ZIndexToActuatorId(-2) < 0);

    CHECK_THAT(settings.ActuatorDistance(2, 3), WithinRel(0.66581, 0.001));
    CHECK_THAT(settings.ActuatorDistance(10, 155), WithinRel(4.52624, 0.001));
}
