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

void checkAppliedForces(ForceController &forceController, float fx, float fy, float fz, float mx, float my,
                        float mz) {
    forceController.updateAppliedForces();
    forceController.processAppliedForces();

    CHECK(M1M3SSPublisher::get().getEventAppliedForces()->fx == Approx(fx));
    CHECK(M1M3SSPublisher::get().getEventAppliedForces()->fy == Approx(fy));
    CHECK(M1M3SSPublisher::get().getEventAppliedForces()->fz == Approx(fz));
    CHECK(M1M3SSPublisher::get().getEventAppliedForces()->mx == Approx(mx));
    CHECK(M1M3SSPublisher::get().getEventAppliedForces()->my == Approx(my));
    CHECK(M1M3SSPublisher::get().getEventAppliedForces()->mz == Approx(mz));
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
    REQUIRE(forceActuatorSettings.UseInclinometer == true);

    PIDSettings pidSettings;
    REQUIRE_NOTHROW(pidSettings.load("../SettingFiles/Sets/Default/1/PIDSettings.xml"));

    SafetyControllerSettings safetyControllerSettings;
    REQUIRE_NOTHROW(
            safetyControllerSettings.load("../SettingFiles/Sets/Default/1/SafetyControllerSettings.xml"));

    SafetyController safetyController(&safetyControllerSettings);

    ForceController forceController(&forceActuatorApplicationSettings, &forceActuatorSettings, &pidSettings,
                                    &safetyController);
    checkAppliedForces(forceController, 0, 0, 0, 0, 0, 0);

    SECTION("Elevation 0 deg with 0% support") {
        forceController.applyElevationForces();
        checkAppliedForces(forceController, 0, 0, 0, 0, 0, 0);
    }

    SECTION("Elevation 0 deg with 100% support") {
        forceController.applyElevationForces();
        forceController.fillSupportPercentage();

        checkAppliedForces(forceController, 0, 10500.0, -0.79729, 89.23988, 0.8879, 11.76017);
    }

    SECTION("Elevation 45 deg with 100% support, with force rejection") {
        forceController.applyElevationForces();
        forceController.fillSupportPercentage();

        M1M3SSPublisher::get().getInclinometerData()->inclinometerAngle = 45.0;

        checkAppliedForces(forceController, 0, 8148.78857, 8148.49805, 62.31575, -0.04463, 9.12726);
        checkAppliedForces(forceController, 0, 16297.57715, 16296.99609, 124.63051, -0.08975, 18.25452);
    }

    SECTION("Elevation 90 deg with 100% support") {
        forceController.applyElevationForces();
        forceController.fillSupportPercentage();

        M1M3SSPublisher::get().getInclinometerData()->inclinometerAngle = 90.0;

        checkAppliedForces(forceController, 0, 0.06511, 11065.59961, -3.54472, -0.10448, 0.00007);
    }

    SECTION("Offset rejection test, elevation 45 deg with 100% support") {
        forceController.applyElevationForces();
        forceController.fillSupportPercentage();

        M1M3SSPublisher::get().getInclinometerData()->inclinometerAngle = 45.0;

        checkAppliedForces(forceController, 0, 8148.78857, 8148.49805, 62.31575, -0.04463, 9.12726);

        forceController.applyOffsetForcesByMirrorForces(1000, -1000, 200000, 20000, 20000, -300000);
        checkAppliedForces(forceController, 7.04272, 16290.53516, 17706.45117, 265.5639, 140.84802,
                           -2095.92236);
        checkAppliedForces(forceController, 14.08545, 24432.2793, 27264.40625, 468.81464, 281.73798,
                           -4200.97021);
    }
}
