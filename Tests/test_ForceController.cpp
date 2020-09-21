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

#define CATCH_CONFIG_MAIN
#include <catch/catch.hpp>

#include <ForceActuatorApplicationSettings.h>
#include <ForceActuatorSettings.h>
#include <ForceController.h>
#include <PIDSettings.h>
#include <SafetyController.h>
#include <SafetyControllerSettings.h>
#include <SettingReader.h>

#include <SAL_MTM1M3.h>

using namespace LSST::M1M3::SS;

void checkAppliedForces(float fx, float fy, float fz, float mx, float my, float mz) {
    REQUIRE(M1M3SSPublisher::get().getEventAppliedForces()->fx == fx);
    REQUIRE(M1M3SSPublisher::get().getEventAppliedForces()->fy == fy);
    REQUIRE(M1M3SSPublisher::get().getEventAppliedForces()->fz == fz);
    REQUIRE(M1M3SSPublisher::get().getEventAppliedForces()->mx == mx);
    REQUIRE(M1M3SSPublisher::get().getEventAppliedForces()->my == my);
    REQUIRE(M1M3SSPublisher::get().getEventAppliedForces()->mz == mz);
}

TEST_CASE("M1M3 ForceController tests", "[M1M3]") {
    std::shared_ptr<SAL_MTM1M3> m1m3SAL = std::make_shared<SAL_MTM1M3>();
    M1M3SSPublisher::get().setSAL(m1m3SAL);

    SettingReader::get().setRootPath("../SettingFiles");

    ForceActuatorApplicationSettings forceActuatorApplicationSettings;
    REQUIRE_NOTHROW(forceActuatorApplicationSettings.load(
            "../SettingFiles/Base/ForceActuatorApplicationSettings.xml"));

    ForceActuatorSettings forceActuatorSettings;
    REQUIRE_NOTHROW(forceActuatorSettings.load("../SettingFiles/Sets/Default/1/ForceActuatorSettings.xml"));

    PIDSettings pidSettings;
    REQUIRE_NOTHROW(pidSettings.load("../SettingFiles/Sets/Default/1/PIDSettings.xml"));

    SafetyControllerSettings safetyControllerSettings;
    REQUIRE_NOTHROW(
            safetyControllerSettings.load("../SettingFiles/Sets/Default/1/SafetyControllerSettings.xml"));

    SafetyController safetyController(&safetyControllerSettings);

    ForceController forceController(&forceActuatorApplicationSettings, &forceActuatorSettings, &pidSettings,
                                    &safetyController);
    forceController.updateAppliedForces();
    forceController.processAppliedForces();
    checkAppliedForces(0, 0, 0, 0, 0, 0);
}
