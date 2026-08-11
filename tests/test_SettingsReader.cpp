/*
 * This file is part of LSST M1M3 SS test suite. Tests SafetyController.
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

#include <SAL_MTM1M3.h>

#include <ForceActuatorSettings.h>
#include <M1M3SSPublisher.h>
#include <Model.h>
#include <SettingReader.h>

using namespace Catch::Matchers;
using namespace LSST::M1M3::SS;

TEST_CASE("List available settings", "[SettingReader]") {
    std::shared_ptr<SAL_MTM1M3> m1m3SAL = std::make_shared<SAL_MTM1M3>();
    M1M3SSPublisher::instance().setSAL(m1m3SAL);

    SettingReader::instance().setRootPath("../SettingFiles/");

    auto configs = SettingReader::instance().getAvailableConfigurations();
    CHECK(configs.size() == 0);
}

TEST_CASE("ForceActuatorSettings", "[SettingReader]") {
    std::shared_ptr<SAL_MTM1M3> m1m3SAL = std::make_shared<SAL_MTM1M3>();
    M1M3SSPublisher::instance().setSAL(m1m3SAL);

    SettingReader::instance().setRootPath("../SettingFiles/");

    REQUIRE_NOTHROW(Model::instance().loadSettings("Default"));

    auto& fa_settings = ForceActuatorSettings::instance();

    CHECK_THAT(fa_settings.bumpTestTestedError, WithinAbs(5, 1e-3));
    CHECK_THAT(fa_settings.bumpTestTestedWarning, WithinAbs(2.5, 1e-3));

    CHECK_THAT(fa_settings.bumpTestNonTestedError, WithinAbs(6, 1e-3));
    CHECK_THAT(fa_settings.bumpTestNonTestedWarning, WithinAbs(5.2, 1e-3));
}
