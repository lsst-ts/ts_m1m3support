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
#include <cmath>

#include <ForceActuatorApplicationSettings.h>
#include <ForceActuatorSettings.h>
#include <ForceController.h>
#include <PIDSettings.h>
#include <SafetyController.h>
#include <SafetyControllerSettings.h>
#include <SettingReader.h>
#include <StateTypes.h>

#include <SAL_MTM1M3.h>

using namespace LSST::M1M3::SS;

ForceActuatorApplicationSettings forceActuatorApplicationSettings;

void checkAppliedActuatorForcesZ(int zIndex, float zForce) {
    REQUIRE(forceActuatorApplicationSettings.ZIndexToXIndex[zIndex] == -1);
    REQUIRE(forceActuatorApplicationSettings.ZIndexToYIndex[zIndex] == -1);
    CHECK(M1M3SSPublisher::get().getEventAppliedForces()->zForces[zIndex] == zForce);
}

void checkAppliedActuatorForcesXZ(int zIndex, float xForce, float zForce) {
    REQUIRE(forceActuatorApplicationSettings.ZIndexToYIndex[zIndex] == -1);

    int xIndex = forceActuatorApplicationSettings.ZIndexToXIndex[zIndex];
    REQUIRE_FALSE(xIndex < 0);
    CHECK(M1M3SSPublisher::get().getEventAppliedForces()->xForces[xIndex] == xForce);
    CHECK(M1M3SSPublisher::get().getEventAppliedForces()->zForces[zIndex] == zForce);
}

void checkAppliedActuatorForcesYZ(int zIndex, float yForce, float zForce) {
    REQUIRE(forceActuatorApplicationSettings.ZIndexToXIndex[zIndex] == -1);

    int yIndex = forceActuatorApplicationSettings.ZIndexToYIndex[zIndex];
    REQUIRE_FALSE(yIndex < 0);
    CHECK(M1M3SSPublisher::get().getEventAppliedForces()->yForces[yIndex] == yForce);
    CHECK(M1M3SSPublisher::get().getEventAppliedForces()->zForces[zIndex] == zForce);
}

void checkAppliedForces(float fx, float fy, float fz, float mx, float my, float mz) {
    CHECK(M1M3SSPublisher::get().getEventAppliedForces()->fx == Approx(fx));
    CHECK(M1M3SSPublisher::get().getEventAppliedForces()->fy == Approx(fy));
    CHECK(M1M3SSPublisher::get().getEventAppliedForces()->fz == Approx(fz));
    CHECK(M1M3SSPublisher::get().getEventAppliedForces()->mx == Approx(mx));
    CHECK(M1M3SSPublisher::get().getEventAppliedForces()->my == Approx(my));
    CHECK(M1M3SSPublisher::get().getEventAppliedForces()->mz == Approx(mz));
}

void checkRejectedActuatorForcesZ(int zIndex, float zForce) {
    REQUIRE(forceActuatorApplicationSettings.ZIndexToXIndex[zIndex] == -1);
    REQUIRE(forceActuatorApplicationSettings.ZIndexToYIndex[zIndex] == -1);
    CHECK(M1M3SSPublisher::get().getEventRejectedForces()->zForces[zIndex] == zForce);
}

void checkRejectedActuatorForcesXZ(int zIndex, float xForce, float zForce) {
    REQUIRE(forceActuatorApplicationSettings.ZIndexToYIndex[zIndex] == -1);

    int xIndex = forceActuatorApplicationSettings.ZIndexToXIndex[zIndex];
    REQUIRE_FALSE(xIndex < 0);
    CHECK(M1M3SSPublisher::get().getEventRejectedForces()->xForces[xIndex] == xForce);
    CHECK(M1M3SSPublisher::get().getEventRejectedForces()->zForces[zIndex] == zForce);
}

void checkRejectedActuatorForcesYZ(int zIndex, float yForce, float zForce) {
    REQUIRE(forceActuatorApplicationSettings.ZIndexToXIndex[zIndex] == -1);

    int yIndex = forceActuatorApplicationSettings.ZIndexToYIndex[zIndex];
    REQUIRE_FALSE(yIndex < 0);
    CHECK(M1M3SSPublisher::get().getEventRejectedForces()->yForces[yIndex] == yForce);
    CHECK(M1M3SSPublisher::get().getEventRejectedForces()->zForces[zIndex] == zForce);
}

void checkRejectedForces(float fx, float fy, float fz, float mx, float my, float mz) {
    CHECK(M1M3SSPublisher::get().getEventRejectedForces()->fx == Approx(fx));
    CHECK(M1M3SSPublisher::get().getEventRejectedForces()->fy == Approx(fy));
    CHECK(M1M3SSPublisher::get().getEventRejectedForces()->fz == Approx(fz));
    CHECK(M1M3SSPublisher::get().getEventRejectedForces()->mx == Approx(mx));
    CHECK(M1M3SSPublisher::get().getEventRejectedForces()->my == Approx(my));
    CHECK(M1M3SSPublisher::get().getEventRejectedForces()->mz == Approx(mz));
}

void runAndCheck(ForceController &forceController, float fx, float fy, float fz, float mx, float my, float mz,
                 int n = 1) {
    for (int i = 0; i < n; i++) {
        forceController.updateAppliedForces();
        forceController.processAppliedForces();
    }

    checkAppliedForces(fx, fy, fz, mx, my, mz);
}

TEST_CASE("M1M3 ForceController tests", "[M1M3]") {
    std::shared_ptr<SAL_MTM1M3> m1m3SAL = std::make_shared<SAL_MTM1M3>();
    M1M3SSPublisher::get().setSAL(m1m3SAL);

    SettingReader::get().setRootPath("../SettingFiles");

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
    runAndCheck(forceController, 0, 0, 0, 0, 0, 0);

    SECTION("Elevation 0 deg with 0% support") {
        forceController.applyElevationForces();
        runAndCheck(forceController, 0, 0, 0, 0, 0, 0);
    }

    SECTION("Elevation 0 deg with 100% support") {
        forceController.applyElevationForces();
        forceController.fillSupportPercentage();

        runAndCheck(forceController, 0, 10500.0, -0.79729, 89.23988, 0.8879, 11.76017);
    }

    SECTION("Elevation 45 deg with 100% support") {
        forceController.applyElevationForces();
        forceController.fillSupportPercentage();

        M1M3SSPublisher::get().getInclinometerData()->inclinometerAngle = 45.0;

        runAndCheck(forceController, 0, 8148.78857, 8148.49805, 62.31575, -0.04463, 9.12726);
        runAndCheck(forceController, 0, 16297.57715, 16296.99609, 124.63051, -0.08975, 18.25452);

        runAndCheck(forceController, 0, 119990.42188, 119985.95312, 917.60748, -0.64774, 134.39301, 1000);
        checkRejectedForces(0, 119990.42188, 119985.95312, 917.60748, -0.64774, 134.39301);

        runAndCheck(forceController, 0, 119990.42188, 119985.95312, 917.60748, -0.64774, 134.39301);

        checkAppliedActuatorForcesYZ(1, 1199.90308, 595.77222);
        checkRejectedActuatorForcesYZ(1, 1199.90308, 595.77222);

        checkAppliedActuatorForcesYZ(1, 1199.90308, 595.77222);
        checkRejectedActuatorForcesYZ(1, 1199.90308, 595.77222);
    }

    SECTION("Elevation 90 deg with 100% support") {
        forceController.applyElevationForces();
        forceController.fillSupportPercentage();

        M1M3SSPublisher::get().getInclinometerData()->inclinometerAngle = 90.0;

        runAndCheck(forceController, 0, 0.06511, 11065.59961, -3.54472, -0.10448, 0.00007);
    }

    SECTION("Elevation 45 deg with 100% support, progressing load") {
        forceController.applyElevationForces();
        forceController.fillSupportPercentage();

        M1M3SSPublisher::get().getInclinometerData()->inclinometerAngle = 45.0;

        runAndCheck(forceController, 0, 8148.78857, 8148.49805, 62.31575, -0.04463, 9.12726);
        checkRejectedForces(0, 8148.78857, 8148.49805, 62.31575, -0.04463, 9.12726);

        forceController.applyOffsetForcesByMirrorForces(1000, -1000, 200000, 20000, 20000, -300000);
        runAndCheck(forceController, 7.04272, 16290.53516, 17706.45117, 265.5639, 140.84802, -2095.92236);
        checkRejectedForces(7.04272, 16290.53516, 17706.45117, 265.5639, 140.84802, -2095.92236);
        runAndCheck(forceController, 14.08545, 24432.2793, 27264.40625, 468.81464, 281.73798, -4200.97021);
    }

    SECTION("Elevation 45 deg with 100% support, force sum doesn't support mirror") {
        forceController.applyElevationForces();
        forceController.fillSupportPercentage();

        M1M3SSPublisher::get().getInclinometerData()->inclinometerAngle = 45.0;

        runAndCheck(forceController, 0, 8148.78857, 8148.49805, 62.31575, -0.04463, 9.12726);

        MTM1M3_logevent_appliedElevationForcesC *appliedElevationForces =
                M1M3SSPublisher::get().getEventAppliedElevationForces();
        MTM1M3_logevent_appliedForcesC *appliedForces = M1M3SSPublisher::get().getEventAppliedForces();
        MTM1M3_logevent_errorCodeC *errorCodeData = M1M3SSPublisher::get().getEventErrorCode();

        // only elevation force is being applied
        REQUIRE(appliedElevationForces->zForces[1] == appliedForces->zForces[1]);
        for (int i = 0; i < FA_Z_COUNT; i++) {
            appliedElevationForces->zForces[i] = -50000;
        }

        for (int i = 0; i < 7; i++) {
            forceController.processAppliedForces();
            CHECK(safetyController.checkSafety(States::ActiveState) == States::ActiveState);
        }

        forceController.processAppliedForces();
        CHECK(safetyController.checkSafety(States::ActiveState) == States::LoweringFaultState);
    }
}
