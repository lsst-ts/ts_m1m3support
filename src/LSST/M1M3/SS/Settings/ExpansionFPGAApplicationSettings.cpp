/*
 * This file is part of LSST M1M3 support system package.
 *
 * Developed for the Vera C. Rubin Telescope and Site System.
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

#include <thread>

#include <spdlog/spdlog.h>
#include <yaml-cpp/yaml.h>

#include <ExpansionFPGAApplicationSettings.h>
#include <Heartbeat.h>
#include <IExpansionFPGA.h>

using namespace LSST::M1M3::SS;

ExpansionFPGAApplicationSettings::ExpansionFPGAApplicationSettings(token) {}

void ExpansionFPGAApplicationSettings::load(YAML::Node doc) {
    SPDLOG_INFO("Loading ExpansionFPGAApplicationSettings");
    Enabled = doc["Enabled"].as<bool>();
    Resource = doc["Resource"].as<std::string>();
}

void ExpansionFPGAApplicationSettings::initialize(StartCommand *command) {
    IExpansionFPGA::get().setResource(Enabled, Resource);
    if (Enabled) {
        auto &heartbeat = Heartbeat::instance();

        heartbeat.tryToggle();
        IExpansionFPGA::get().close();
        heartbeat.tryToggle();

        SPDLOG_INFO("Opening expansion FPGA: {}", Resource);
        command->ackInProgress("Opening expansion FPGA", 12);
        IExpansionFPGA::get().open();

        // TODO replace that with wait for IRQ from the expansion FPGA, being raised
        // in FPGA after it finish initialization

        for (int i = 0; i < 50; i++) {
            heartbeat.tryToggle();
            command->ackInProgress("Waiting for Expansion FPGA", 11.5 - (i * 0.2f));
            std::this_thread::sleep_for(std::chrono::milliseconds(200));
        }

        command->ackInProgress("Expansion FPGA opened", 1.4);

        heartbeat.tryToggle();
    }
}
