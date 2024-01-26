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

#include <filesystem>
#include <fstream>
#include <time.h>

#include <spdlog/spdlog.h>

#include <cRIO/Thread.h>

#include <AccelerometerSettings.h>
#include <IFPGA.h>
#include <RawDCAccelerometersCommands.h>

using namespace std::chrono_literals;
using namespace LSST::M1M3::SS;

class DumpRawAccelerometer : public LSST::cRIO::Thread {
public:
    void open(std::filesystem::path dump_path);
    void close();
    void run(std::unique_lock<std::mutex> &lock) override;

private:
    std::ofstream _file;
    size_t _sync_counter = 0;
};

void DumpRawAccelerometer::open(std::filesystem::path dump_path) {
    try {
        if (_file.is_open()) {
            close();
        }
        std::filesystem::create_directories(dump_path.parent_path());
        _file.open(dump_path, std::ofstream::out | std::ofstream::binary | std::ofstream::trunc);
        _file.exceptions(std::ios::badbit | std::ios::failbit);
        _sync_counter = 0;
        SPDLOG_INFO("Recording raw DC accelerometer data to {}", dump_path.string());
    } catch (const std::ios_base::failure &e) {
        SPDLOG_ERROR("Cannot open raw DC accelerometer file {}: {}", dump_path.string(), e.what());
    }
}

void DumpRawAccelerometer::close() {
    try {
        _file.close();
    } catch (const std::ios_base::failure &e) {
        SPDLOG_ERROR("Cannot close raw DC accelerometer file: {}", e.what());
    }
}

void DumpRawAccelerometer::run(std::unique_lock<std::mutex> &lock) {
    while (keepRunning) {
        runCondition.wait_for(lock, 1ms);
        try {
            constexpr size_t samples = 100;
            uint64_t raw[samples * 8];
            char data[samples * 8 * 3];
            IFPGA::get().readRawAccelerometerFIFO(raw, samples);
            for (size_t i = 0; i < samples * 8; i++) {
                raw[i] = htobe64(raw[i]);
                memcpy(data + (i * 3), (reinterpret_cast<char *>(raw + i)) + 5, 3);
            }
            _file.write(data, samples * 8 * 3);
            if (_sync_counter > 10) {
                _file.flush();
                _sync_counter = 0;
            } else {
                _sync_counter++;
            }
        } catch (const std::ios_base::failure &e) {
            SPDLOG_ERROR("Cannot record raw DC accelerometer file: {}", e.what());
            close();
            break;
        }
    }
}

DumpRawAccelerometer rawThread;

RecordRawDCAccelerometersCommand::RecordRawDCAccelerometersCommand() : Command(-1) {}

void RecordRawDCAccelerometersCommand::execute() {
    SPDLOG_INFO("Starting Raw DC Accelerometers recording");
    char buf[200];
    time_t now;
    time(&now);
    strftime(buf, 200, AccelerometerSettings::instance().dump_path.c_str(), gmtime(&now));
    rawThread.open(buf);
    rawThread.start();
}

StopRawDCAccelerometersCommand::StopRawDCAccelerometersCommand() : Command(-1) {}

void StopRawDCAccelerometersCommand::execute() {
    rawThread.stop(10ms);
    rawThread.close();
    SPDLOG_INFO("Stopped Raw DC Accelerometers recording");
}
