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
#include <catch2/catch_approx.hpp>
#include <cmath>

#include <ForceActuatorSettings.h>
#include <ForceController.h>
#include <Model.h>
#include <SettingReader.h>
#include <StateTypes.h>
#include <RaisingLoweringInfo.h>

#include <SAL_MTM1M3.h>

using namespace LSST::M1M3::SS;
using Catch::Approx;

void checkAppliedActuatorForcesZ(int zIndex, float zForce) {
    REQUIRE(SettingReader::instance().getForceActuatorApplicationSettings()->ZIndexToXIndex[zIndex] == -1);
    REQUIRE(SettingReader::instance().getForceActuatorApplicationSettings()->ZIndexToYIndex[zIndex] == -1);
    CHECK(M1M3SSPublisher::instance().getAppliedForces()->zForces[zIndex] == zForce);
}

void checkAppliedActuatorForcesXZ(int zIndex, float xForce, float zForce) {
    REQUIRE(SettingReader::instance().getForceActuatorApplicationSettings()->ZIndexToYIndex[zIndex] == -1);

    int xIndex = SettingReader::instance().getForceActuatorApplicationSettings()->ZIndexToXIndex[zIndex];
    REQUIRE_FALSE(xIndex < 0);
    CHECK(M1M3SSPublisher::instance().getAppliedForces()->xForces[xIndex] == xForce);
    CHECK(M1M3SSPublisher::instance().getAppliedForces()->zForces[zIndex] == zForce);
}

void checkAppliedActuatorForcesYZ(int zIndex, float yForce, float zForce) {
    REQUIRE(SettingReader::instance().getForceActuatorApplicationSettings()->ZIndexToXIndex[zIndex] == -1);

    int yIndex = SettingReader::instance().getForceActuatorApplicationSettings()->ZIndexToYIndex[zIndex];
    REQUIRE_FALSE(yIndex < 0);
    CHECK(M1M3SSPublisher::instance().getAppliedForces()->yForces[yIndex] == yForce);
    CHECK(M1M3SSPublisher::instance().getAppliedForces()->zForces[zIndex] == zForce);
}

void checkAppliedForces(float fx, float fy, float fz, float mx, float my, float mz) {
    constexpr float absl = 0.000005;
    CHECK(M1M3SSPublisher::instance().getAppliedForces()->fx == Approx(fx).margin(absl));
    CHECK(M1M3SSPublisher::instance().getAppliedForces()->fy == Approx(fy).margin(absl));
    CHECK(M1M3SSPublisher::instance().getAppliedForces()->fz == Approx(fz).margin(absl));
    CHECK(M1M3SSPublisher::instance().getAppliedForces()->mx == Approx(mx).margin(absl));
    CHECK(M1M3SSPublisher::instance().getAppliedForces()->my == Approx(my).margin(absl));
    CHECK(M1M3SSPublisher::instance().getAppliedForces()->mz == Approx(mz).margin(absl));
}

void checkRejectedActuatorForcesZ(int zIndex, float zForce) {
    REQUIRE(SettingReader::instance().getForceActuatorApplicationSettings()->ZIndexToXIndex[zIndex] == -1);
    REQUIRE(SettingReader::instance().getForceActuatorApplicationSettings()->ZIndexToYIndex[zIndex] == -1);
    CHECK(M1M3SSPublisher::instance().getEventPreclippedForces()->zForces[zIndex] == zForce);
}

void checkRejectedActuatorForcesXZ(int zIndex, float xForce, float zForce) {
    REQUIRE(SettingReader::instance().getForceActuatorApplicationSettings()->ZIndexToYIndex[zIndex] == -1);

    int xIndex = SettingReader::instance().getForceActuatorApplicationSettings()->ZIndexToXIndex[zIndex];
    REQUIRE_FALSE(xIndex < 0);
    CHECK(M1M3SSPublisher::instance().getEventPreclippedForces()->xForces[xIndex] == xForce);
    CHECK(M1M3SSPublisher::instance().getEventPreclippedForces()->zForces[zIndex] == zForce);
}

void checkRejectedActuatorForcesYZ(int zIndex, float yForce, float zForce) {
    REQUIRE(SettingReader::instance().getForceActuatorApplicationSettings()->ZIndexToXIndex[zIndex] == -1);

    int yIndex = SettingReader::instance().getForceActuatorApplicationSettings()->ZIndexToYIndex[zIndex];
    REQUIRE_FALSE(yIndex < 0);
    CHECK(M1M3SSPublisher::instance().getEventPreclippedForces()->yForces[yIndex] == yForce);
    CHECK(M1M3SSPublisher::instance().getEventPreclippedForces()->zForces[zIndex] == zForce);
}

void checkRejectedForces(float fx, float fy, float fz, float mx, float my, float mz) {
    CHECK(M1M3SSPublisher::instance().getEventPreclippedForces()->fx == Approx(fx));
    CHECK(M1M3SSPublisher::instance().getEventPreclippedForces()->fy == Approx(fy));
    CHECK(M1M3SSPublisher::instance().getEventPreclippedForces()->fz == Approx(fz));
    CHECK(M1M3SSPublisher::instance().getEventPreclippedForces()->mx == Approx(mx));
    CHECK(M1M3SSPublisher::instance().getEventPreclippedForces()->my == Approx(my));
    CHECK(M1M3SSPublisher::instance().getEventPreclippedForces()->mz == Approx(mz));
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
    M1M3SSPublisher::instance().setSAL(m1m3SAL);
    SettingReader::instance().setRootPath("../SettingFiles");

    REQUIRE_NOTHROW(Model::instance().loadSettings("Default"));

    REQUIRE(ForceActuatorSettings::instance().useInclinometer == true);

    runAndCheck(Model::instance().getForceController(), 0, 0, 0, 0, 0, 0);

    SECTION("Elevation 0 deg with 0% support") {
        Model::instance().getForceController()->applyElevationForces();
        runAndCheck(Model::instance().getForceController(), 0, 0, 0, 0, 0, 0);
    }

    SECTION("Elevation 0 deg with 100% support") {
        Model::instance().getForceController()->applyElevationForces();
        RaisingLoweringInfo::instance().fillSupportPercentage();

        runAndCheck(Model::instance().getForceController(), 0, 9965.86328, 30.3328, 169.59573, -32.7769,
                    -12.35224);
    }

    SECTION("Elevation 45 deg with 100% support") {
        Model::instance().getForceController()->applyElevationForces();
        RaisingLoweringInfo::instance().fillSupportPercentage();

        M1M3SSPublisher::instance().getInclinometerData()->inclinometerAngle = 45.0;

        runAndCheck(Model::instance().getForceController(), 0, 8157.62793, 8134.57812, 84.48508, -21.52274,
                    -25.1534);
        runAndCheck(Model::instance().getForceController(), 0, 16315.25586, 16269.15625, 168.97012, -43.04597,
                    -50.3063);

        runAndCheck(Model::instance().getForceController(), 0, 119141.60156, 118804.97656, 1233.89075,
                    -314.35049, -367.35291, 1000);
        checkRejectedForces(0, 119141.60156, 118804.97656, 1233.89075, -314.35049, -367.35291);

        runAndCheck(Model::instance().getForceController(), 0, 119141.60156, 118804.97656f, 1233.89075,
                    -314.35049, -367.35291);

        checkAppliedActuatorForcesYZ(1, 1186.86218, 590.02869);
        checkRejectedActuatorForcesYZ(1, 1186.86218, 590.02869);

        checkAppliedActuatorForcesYZ(1, 1186.86218, 590.02869);
        checkRejectedActuatorForcesYZ(1, 1186.86218, 590.02869);
    }

    SECTION("Elevation 90 deg with 100% support") {
        Model::instance().getForceController()->applyElevationForces();
        RaisingLoweringInfo::instance().fillSupportPercentage();

        M1M3SSPublisher::instance().getInclinometerData()->inclinometerAngle = 90.0;

        runAndCheck(Model::instance().getForceController(), 0, 2.07198, 11000.72559, -77.41071, -19.8329,
                    -10.27149);
    }

    SECTION("Elevation 45 deg with 100% support, progressing load") {
        Model::instance().getForceController()->applyElevationForces();
        RaisingLoweringInfo::instance().fillSupportPercentage();

        M1M3SSPublisher::instance().getInclinometerData()->inclinometerAngle = 45.0;

        runAndCheck(Model::instance().getForceController(), 0, 8157.62793, 8134.57812, 84.48508, -21.52274,
                    -25.1534);
        checkRejectedForces(0, 8157.62793, 8134.57812, 84.48508, -21.52274, -25.1534);

        Model::instance().getForceController()->applyOffsetForcesByMirrorForces(1000, -1000, 200000, 20000,
                                                                                20000, -300000);
        runAndCheck(Model::instance().getForceController(), 6.9610, 16308.28906, 17662.20117, 302.42435,
                    92.18579, -2139.87866);
        checkRejectedForces(6.96104, 16308.28906, 17662.20117, 302.42435, 92.18579, -2139.87866);
        runAndCheck(Model::instance().getForceController(), 13.92207, 24458.94531, 27189.82227, 520.36212,
                    205.89926, -4254.60693);
    }

    SECTION("Elevation 45 deg with 100% support, force sum doesn't support mirror") {
        Model::instance().getForceController()->applyElevationForces();
        RaisingLoweringInfo::instance().fillSupportPercentage();

        M1M3SSPublisher::instance().getInclinometerData()->inclinometerAngle = 45.0;

        runAndCheck(Model::instance().getForceController(), 0, 8157.62793, 8134.57812, 84.48508, -21.52274,
                    -25.1534);

        MTM1M3_appliedElevationForcesC *appliedElevationForces =
                M1M3SSPublisher::instance().getAppliedElevationForces();
        MTM1M3_appliedForcesC *appliedForces = M1M3SSPublisher::instance().getAppliedForces();
        MTM1M3_logevent_errorCodeC *errorCodeData = M1M3SSPublisher::instance().getEventErrorCode();

        REQUIRE(errorCodeData->errorCode == 0);

        // only elevation force is being applied
        REQUIRE(appliedElevationForces->zForces[1] == appliedForces->zForces[1]);
        for (int i = 0; i < FA_Z_COUNT; i++) {
            appliedElevationForces->zForces[i] = -50000;
        }

        SafetyControllerSettings *safetyControllerSettings =
                SettingReader::instance().getSafetyControllerSettings();
        // make sure M1M3 will fail - default settings is now false
        safetyControllerSettings->ForceController.FaultOnForceClipping = true;

        for (int i = 0; i < 2; i++) {
            Model::instance().getForceController()->processAppliedForces();
            CHECK(Model::instance().getSafetyController()->checkSafety(States::ActiveState) ==
                  States::ActiveState);
        }

        Model::instance().getForceController()->processAppliedForces();
        CHECK(Model::instance().getSafetyController()->checkSafety(States::ActiveState) ==
              States::LoweringFaultState);
    }
}
