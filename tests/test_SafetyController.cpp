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

#include <string>

#include "spdlog/sinks/base_sink.h"
#include <spdlog/spdlog.h>

#include <catch2/catch_test_macros.hpp>

#include <SAL_MTM1M3.h>

#include <Model.h>
#include <HardpointActuatorSettings.h>
#include <M1M3SSPublisher.h>
#include <SafetyController.h>
#include <SafetyControllerSettings.h>
#include <SettingReader.h>

int lowCount = 0;
int highCount = 0;

// custom sink, checks for message content
template <typename Mutex>
class check_sink : public spdlog::sinks::base_sink<Mutex> {
protected:
    void sink_it_(const spdlog::details::log_msg& msg) override {
        spdlog::memory_buf_t formatted;
        spdlog::sinks::base_sink<Mutex>::formatter_->format(msg, formatted);

        std::string message = fmt::to_string(formatted);
        std::cout << "Message: " << message << std::endl;

        if (message.find("Air Pressure High") != std::string::npos) {
            highCount++;
        }
        if (message.find("Air Pressure Low") != std::string::npos) {
            lowCount++;
        }
    }

    void flush_() override { std::cout << std::flush; }
};

#include "spdlog/details/null_mutex.h"
#include <mutex>
using check_sink_st = check_sink<spdlog::details::null_mutex>;

using namespace LSST::M1M3::SS;

void init_logger() {
    std::shared_ptr<SAL_MTM1M3> m1m3SAL = std::make_shared<SAL_MTM1M3>();
    m1m3SAL->setDebugLevel(2);
    M1M3SSPublisher::instance().setSAL(m1m3SAL);

    SettingReader::instance().setRootPath("../SettingFiles/");
    REQUIRE_NOTHROW(Model::get().loadSettings("Default"));

    auto logger = std::make_shared<spdlog::logger>("Test", std::make_shared<check_sink_st>());
    spdlog::set_default_logger(logger);
    spdlog::set_level(spdlog::level::trace);
}

TEST_CASE("Failure on low air pressure", "[SafetyController]") {
    init_logger();

    SafetyControllerSettings* safetyControllerSettings =
            SettingReader::instance().getSafetyControllerSettings();

    SafetyController safetyController(safetyControllerSettings);

    lowCount = highCount = 0;

    for (int i = 0; i < safetyControllerSettings->ILC.AirPressureCountThreshold - 1; i++) {
        safetyController.hardpointActuatorAirPressure(
                0, -1, HardpointActuatorSettings::instance().airPressureFaultLow - 1);
        safetyController.checkSafety(States::ActiveState);
        REQUIRE(lowCount == 0);
        REQUIRE(highCount == 0);
    }

    safetyController.hardpointActuatorAirPressure(
            0, -1, HardpointActuatorSettings::instance().airPressureFaultLow - 1);
    safetyController.checkSafety(States::ActiveState);
    REQUIRE(lowCount == 1);
    REQUIRE(highCount == 0);
}

TEST_CASE("Failure on high air pressure", "[SafetyController]") {
    init_logger();

    SafetyControllerSettings* safetyControllerSettings =
            SettingReader::instance().getSafetyControllerSettings();

    SafetyController safetyController(safetyControllerSettings);

    lowCount = highCount = 0;

    for (int i = 0; i < safetyControllerSettings->ILC.AirPressureCountThreshold - 1; i++) {
        safetyController.hardpointActuatorAirPressure(
                0, 1, HardpointActuatorSettings::instance().airPressureFaultHigh + 1);
        safetyController.checkSafety(States::ActiveState);
        REQUIRE(lowCount == 0);
        REQUIRE(highCount == 0);
    }

    safetyController.hardpointActuatorAirPressure(
            0, 1, HardpointActuatorSettings::instance().airPressureFaultHigh + 1);
    safetyController.checkSafety(States::ActiveState);
    REQUIRE(lowCount == 0);
    REQUIRE(highCount == 1);
}
