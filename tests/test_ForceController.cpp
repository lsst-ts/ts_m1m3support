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
#include <catch2/catch.hpp>
#include <cmath>

#include <ForceController.h>
#include <Model.h>
#include <SettingReader.h>
#include <StateTypes.h>

#include <SAL_MTM1M3.h>

using namespace LSST::M1M3::SS;

void checkAppliedActuatorForcesZ(int zIndex, float zForce) {
    REQUIRE(SettingReader::get().getForceActuatorApplicationSettings()->ZIndexToXIndex[zIndex] == -1);
    REQUIRE(SettingReader::get().getForceActuatorApplicationSettings()->ZIndexToYIndex[zIndex] == -1);
    CHECK(M1M3SSPublisher::get().getEventAppliedForces()->zForces[zIndex] == zForce);
}

void checkAppliedActuatorForcesXZ(int zIndex, float xForce, float zForce) {
    REQUIRE(SettingReader::get().getForceActuatorApplicationSettings()->ZIndexToYIndex[zIndex] == -1);

    int xIndex = SettingReader::get().getForceActuatorApplicationSettings()->ZIndexToXIndex[zIndex];
    REQUIRE_FALSE(xIndex < 0);
    CHECK(M1M3SSPublisher::get().getEventAppliedForces()->xForces[xIndex] == xForce);
    CHECK(M1M3SSPublisher::get().getEventAppliedForces()->zForces[zIndex] == zForce);
}

void checkAppliedActuatorForcesYZ(int zIndex, float yForce, float zForce) {
    REQUIRE(SettingReader::get().getForceActuatorApplicationSettings()->ZIndexToXIndex[zIndex] == -1);

    int yIndex = SettingReader::get().getForceActuatorApplicationSettings()->ZIndexToYIndex[zIndex];
    REQUIRE_FALSE(yIndex < 0);
    CHECK(M1M3SSPublisher::get().getEventAppliedForces()->yForces[yIndex] == yForce);
    CHECK(M1M3SSPublisher::get().getEventAppliedForces()->zForces[zIndex] == zForce);
}

void checkAppliedForces(float fx, float fy, float fz, float mx, float my, float mz) {
    constexpr float absl = 0.000005;
    CHECK(M1M3SSPublisher::get().getEventAppliedForces()->fx == Approx(fx).margin(absl));
    CHECK(M1M3SSPublisher::get().getEventAppliedForces()->fy == Approx(fy).margin(absl));
    CHECK(M1M3SSPublisher::get().getEventAppliedForces()->fz == Approx(fz).margin(absl));
    CHECK(M1M3SSPublisher::get().getEventAppliedForces()->mx == Approx(mx).margin(absl));
    CHECK(M1M3SSPublisher::get().getEventAppliedForces()->my == Approx(my).margin(absl));
    CHECK(M1M3SSPublisher::get().getEventAppliedForces()->mz == Approx(mz).margin(absl));
}

void checkRejectedActuatorForcesZ(int zIndex, float zForce) {
    REQUIRE(SettingReader::get().getForceActuatorApplicationSettings()->ZIndexToXIndex[zIndex] == -1);
    REQUIRE(SettingReader::get().getForceActuatorApplicationSettings()->ZIndexToYIndex[zIndex] == -1);
    CHECK(M1M3SSPublisher::get().getEventPreclippedForces()->zForces[zIndex] == zForce);
}

void checkRejectedActuatorForcesXZ(int zIndex, float xForce, float zForce) {
    REQUIRE(SettingReader::get().getForceActuatorApplicationSettings()->ZIndexToYIndex[zIndex] == -1);

    int xIndex = SettingReader::get().getForceActuatorApplicationSettings()->ZIndexToXIndex[zIndex];
    REQUIRE_FALSE(xIndex < 0);
    CHECK(M1M3SSPublisher::get().getEventPreclippedForces()->xForces[xIndex] == xForce);
    CHECK(M1M3SSPublisher::get().getEventPreclippedForces()->zForces[zIndex] == zForce);
}

void checkRejectedActuatorForcesYZ(int zIndex, float yForce, float zForce) {
    REQUIRE(SettingReader::get().getForceActuatorApplicationSettings()->ZIndexToXIndex[zIndex] == -1);

    int yIndex = SettingReader::get().getForceActuatorApplicationSettings()->ZIndexToYIndex[zIndex];
    REQUIRE_FALSE(yIndex < 0);
    CHECK(M1M3SSPublisher::get().getEventPreclippedForces()->yForces[yIndex] == yForce);
    CHECK(M1M3SSPublisher::get().getEventPreclippedForces()->zForces[zIndex] == zForce);
}

void checkRejectedForces(float fx, float fy, float fz, float mx, float my, float mz) {
    CHECK(M1M3SSPublisher::get().getEventPreclippedForces()->fx == Approx(fx));
    CHECK(M1M3SSPublisher::get().getEventPreclippedForces()->fy == Approx(fy));
    CHECK(M1M3SSPublisher::get().getEventPreclippedForces()->fz == Approx(fz));
    CHECK(M1M3SSPublisher::get().getEventPreclippedForces()->mx == Approx(mx));
    CHECK(M1M3SSPublisher::get().getEventPreclippedForces()->my == Approx(my));
    CHECK(M1M3SSPublisher::get().getEventPreclippedForces()->mz == Approx(mz));
}

void runAndCheck(ForceController *forceController, float fx, float fy, float fz, float mx, float my, float mz,
                 int n = 1) {
    for (int i = 0; i < n; i++) {
        forceController->updateAppliedForces();
        forceController->processAppliedForces();
    }

    checkAppliedForces(fx, fy, fz, mx, my, mz);
}

TEST_CASE("M1M3 ForceController tests", "[M1M3]") {
    std::shared_ptr<SAL_MTM1M3> m1m3SAL = std::make_shared<SAL_MTM1M3>();
    M1M3SSPublisher::get().setSAL(m1m3SAL);
    SettingReader::get().setRootPath("../SettingFiles");

    REQUIRE_NOTHROW(Model::get().loadSettings("Default"));

    REQUIRE(SettingReader::get().getForceActuatorSettings()->useInclinometer == true);

    SafetyControllerSettings safetyControllerSettings;
    REQUIRE_NOTHROW(
            safetyControllerSettings.load("../SettingFiles/Sets/Default/1/SafetyControllerSettings.yaml"));

    runAndCheck(Model::get().getForceController(), 0, 0, 0, 0, 0, 0);

    SECTION("Elevation 0 deg with 0% support") {
        Model::get().getForceController()->applyElevationForces();
        runAndCheck(Model::get().getForceController(), 0, 0, 0, 0, 0, 0);
    }

    SECTION("Elevation 0 deg with 100% support") {
        Model::get().getForceController()->applyElevationForces();
        Model::get().getForceController()->fillSupportPercentage();

        runAndCheck(Model::get().getForceController(), 0, 10500.0, -0.79729, 89.23988, 0.8879, 11.76017);
    }

    SECTION("Elevation 45 deg with 100% support") {
        Model::get().getForceController()->applyElevationForces();
        Model::get().getForceController()->fillSupportPercentage();

        M1M3SSPublisher::get().getInclinometerData()->inclinometerAngle = 45.0;

        runAndCheck(Model::get().getForceController(), 0, 8148.78857, 8148.49805, 62.31575, -0.04463,
                    9.12726);
        runAndCheck(Model::get().getForceController(), 0, 16297.57715, 16296.99609, 124.63051, -0.089749,
                    18.25452);

        runAndCheck(Model::get().getForceController(), 0, 119990.42188, 119985.95312, 917.60748, -0.64774,
                    134.39301, 1000);
        checkRejectedForces(0, 119990.42188, 119985.95312, 917.60748, -0.64774, 134.39301);

        runAndCheck(Model::get().getForceController(), 0, 119990.42188, 119985.95312, 917.60748, -0.64774,
                    134.39301);

        checkAppliedActuatorForcesYZ(1, 1199.90308, 595.77222);
        checkRejectedActuatorForcesYZ(1, 1199.90308, 595.77222);

        checkAppliedActuatorForcesYZ(1, 1199.90308, 595.77222);
        checkRejectedActuatorForcesYZ(1, 1199.90308, 595.77222);
    }

    SECTION("Elevation 90 deg with 100% support") {
        Model::get().getForceController()->applyElevationForces();
        Model::get().getForceController()->fillSupportPercentage();

        M1M3SSPublisher::get().getInclinometerData()->inclinometerAngle = 90.0;

        runAndCheck(Model::get().getForceController(), 0, 0.06511, 11065.59961, -3.54472, -0.10448, 0.00007);
    }

    SECTION("Elevation 45 deg with 100% support, progressing load") {
        Model::get().getForceController()->applyElevationForces();
        Model::get().getForceController()->fillSupportPercentage();

        M1M3SSPublisher::get().getInclinometerData()->inclinometerAngle = 45.0;

        runAndCheck(Model::get().getForceController(), 0, 8148.78857, 8148.49805, 62.31575, -0.04463,
                    9.12726);
        checkRejectedForces(0, 8148.78857, 8148.49805, 62.31575, -0.04463, 9.12726);

        Model::get().getForceController()->applyOffsetForcesByMirrorForces(1000, -1000, 200000, 20000, 20000,
                                                                           -300000);
        runAndCheck(Model::get().getForceController(), 6.9610, 16290.5351, 17690.03906, 258.0899, 135.1448,
                    -2071.3198);
        checkRejectedForces(6.96104, 16290.5351, 17690.0390, 258.0899, 135.1448, -2071.3198);
        runAndCheck(Model::get().getForceController(), 13.92207, 24432.2793, 27231.5839, 453.8573, 270.3363,
                    -4151.7695);
    }

    SECTION("Elevation 45 deg with 100% support, force sum doesn't support mirror") {
        Model::get().getForceController()->applyElevationForces();
        Model::get().getForceController()->fillSupportPercentage();

        M1M3SSPublisher::get().getInclinometerData()->inclinometerAngle = 45.0;

        runAndCheck(Model::get().getForceController(), 0, 8148.78857, 8148.49805, 62.31575, -0.04463,
                    9.12726);

        MTM1M3_logevent_appliedElevationForcesC *appliedElevationForces =
                M1M3SSPublisher::get().getEventAppliedElevationForces();
        MTM1M3_logevent_appliedForcesC *appliedForces = M1M3SSPublisher::get().getEventAppliedForces();
        MTM1M3_logevent_errorCodeC *errorCodeData = M1M3SSPublisher::get().getEventErrorCode();

        REQUIRE(errorCodeData->errorCode == 0);

        // only elevation force is being applied
        REQUIRE(appliedElevationForces->zForces[1] == appliedForces->zForces[1]);
        for (int i = 0; i < FA_Z_COUNT; i++) {
            appliedElevationForces->zForces[i] = -50000;
        }

        for (int i = 0; i < 7; i++) {
            Model::get().getForceController()->processAppliedForces();
            CHECK(Model::get().getSafetyController()->checkSafety(States::ActiveState) ==
                  States::ActiveState);
        }

        Model::get().getForceController()->processAppliedForces();
        CHECK(Model::get().getSafetyController()->checkSafety(States::ActiveState) ==
              States::LoweringFaultState);
    }
}
