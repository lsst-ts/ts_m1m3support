/*
 * Command line Thermal System client.
 *
 * Developed for the Vera C. Rubin Observatory Telescope & Site Software Systems.
 * This product includes software developed by the Vera C.Rubin Observatory Project
 * (https://www.lsst.org). See the COPYRIGHT file at the top-level directory of
 * this distribution for details of code ownership.
 *
 * This program is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation, either version 3 of the License, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program. If not, see <https://www.gnu.org/licenses/>.
 */

#include <FPGA.h>

#include <cRIO/ElectromechanicalPneumaticILC.h>

#include <CliApp.hpp>
#include <iostream>
#include <iomanip>

#include <spdlog/async.h>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>

using namespace LSST::cRIO;
using namespace LSST::M1M3::SS;

class PrintILC : public ElectromechanicalPneumaticILC {
public:
    PrintILC(uint8_t bus) : _bus(bus) {}
    uint8_t getBus() { return _bus; }

protected:
    void processServerID(uint8_t address, uint64_t uniqueID, uint8_t ilcAppType, uint8_t networkNodeType,
                         uint8_t ilcSelectedOptions, uint8_t networkNodeOptions, uint8_t majorRev,
                         uint8_t minorRev, std::string firmwareName);

    void processServerStatus(uint8_t address, uint8_t mode, uint16_t status, uint16_t faults) override {}

    void processChangeILCMode(uint8_t address, uint16_t mode) override {}

    void processSetTempILCAddress(uint8_t address, uint8_t newAddress) override {}

    void processResetServer(uint8_t address) override;

    virtual void processCalibrationData(uint8_t address, float mainADCK[4], float mainOffset[4],
                                        float mainSensitivity[4], float backupADCK[4], float backupOffset[4],
                                        float backupSensitivity[4]) override;

private:
    uint8_t _bus;
};

void PrintILC::processServerID(uint8_t address, uint64_t uniqueID, uint8_t ilcAppType,
                               uint8_t networkNodeType, uint8_t ilcSelectedOptions,
                               uint8_t networkNodeOptions, uint8_t majorRev, uint8_t minorRev,
                               std::string firmwareName) {
    std::cout << "Address: " << std::to_string(address) << std::endl
              << "UniqueID: " << std::hex << std::setw(8) << std::setfill('0') << (uniqueID) << std::endl
              << "ILC application type: " << std::to_string(ilcAppType) << std::endl
              << "Network node type: " << std::to_string(networkNodeType) << std::endl
              << "ILC selected options: " << std::to_string(ilcSelectedOptions) << std::endl
              << "Network node options: " << std::to_string(networkNodeOptions) << std::endl
              << "Firmware revision: " << std::to_string(majorRev) << "." << std::to_string(minorRev)
              << std::endl
              << "Firmware name: " << firmwareName << std::endl
              << std::endl;
}

void PrintILC::processResetServer(uint8_t address) {
    std::cout << "Reseted " << _bus << "/" << address << std::endl;
}

template <typename t>
void print4(const char* name, t a[4]) {
    std::cout << std::setfill(' ') << std::setw(18) << name;
    for (int i = 0; i < 4; i++) {
        std::cout << " " << std::setw(15) << std::setprecision(10) << a[i];
    }
    std::cout << std::endl;
};

void PrintILC::processCalibrationData(uint8_t address, float mainADCK[4], float mainOffset[4],
                                      float mainSensitivity[4], float backupADCK[4], float backupOffset[4],
                                      float backupSensitivity[4]) {
    std::cout << "Calibration data " << std::to_string(_bus) << "/" << std::to_string(address) << std::endl
              << std::endl;

    int vi[4] = {1, 2, 3, 4};
    print4("Values", vi);

    print4("Main ADC Kn", mainADCK);
    print4("Main Offset", mainOffset);
    print4("Main Sensitivity", mainSensitivity);

    print4("Backup ADC Kn", backupADCK);
    print4("Backup Offset", backupOffset);
    print4("Backup Sensitivity", backupSensitivity);
}

constexpr int NEED_FPGA = 0x01;

class M1M3TScli : public CliApp {
public:
    M1M3TScli(const char* description) : CliApp(description) {}

protected:
    void printUsage() override;
    void processArg(int opt, const char* optarg) override;
    int processCommand(const command_t* cmd, const command_vec& args) override;
};

void M1M3TScli::printUsage() {
    std::cout << "M1M3 Thermal System command line tool. Access M1M3 Thermal System FPGA." << std::endl
              << "Options: " << std::endl
              << "  -h   help" << std::endl
              << "  -O   don't auto open (and run) FPGA" << std::endl
              << "  -v   increase verbosity" << std::endl;
    command_vec cmds;
    helpCommands(cmds);
}

bool _autoOpen = true;
int _verbose = 0;

void M1M3TScli::processArg(int opt, const char* optarg) {
    switch (opt) {
        case 'h':
            printAppHelp();
            exit(EXIT_SUCCESS);
            break;

        case 'O':
            _autoOpen = false;
            break;

        case 'v':
            _verbose++;
            break;

        default:
            std::cerr << "Unknown command: " << (char)(opt) << std::endl;
            exit(EXIT_FAILURE);
    }
}

void _printBuffer(std::string prefix, uint16_t* buf, size_t len) {
    if (_verbose == 0) {
        return;
    }

    std::cout << prefix;
    for (size_t i = 0; i < len; i++) {
        std::cout << std::hex << std::setfill('0') << std::setw(4) << buf[i] << " ";
    }
    std::cout << std::endl;
}

class PrintSSFPGA : public LSST::M1M3::SS::FPGA {
public:
    PrintSSFPGA() : FPGA() {}

    void writeCommandFIFO(uint16_t* data, size_t length, uint32_t timeout) override {
        _printBuffer("C> ", data, length);
        FPGA::writeCommandFIFO(data, length, timeout);
    }

    void writeRequestFIFO(uint16_t* data, size_t length, uint32_t timeout) {
        _printBuffer("R> ", data, length);
        FPGA::writeRequestFIFO(data, length, timeout);
    }

    void readU16ResponseFIFO(uint16_t* data, size_t length, uint32_t timeout) override {
        FPGA::readU16ResponseFIFO(data, length, timeout);
        _printBuffer("R< ", data, length);
    }
};

PrintSSFPGA* fpga = NULL;
PrintILC ilc(1);

int M1M3TScli::processCommand(const command_t* cmd, const command_vec& args) {
    if ((cmd->flags & NEED_FPGA) && fpga == NULL) {
        std::cerr << "Command " << cmd->command << " needs opened FPGA. Please call open command first"
                  << std::endl;
        return -1;
    }
    return CliApp::processCommand(cmd, args);
}

int closeFPGA() {
    fpga->close();
    delete fpga;
    fpga = NULL;
    return 0;
}

int callFunction(command_vec cmds, std::function<void(uint8_t)> call) {
    int ret = -2;
    ilc.clear();
    for (auto c : cmds) {
        try {
            int address = std::stoi(c);
            if (address <= 0 || address > 100) {
                std::cerr << "Invalid address " << c << std::endl;
                ret = -1;
                continue;
            }
            call(address);
            ret = 0;
        } catch (std::logic_error& e) {
            std::cerr << "Non-numeric address: " << c << std::endl;
            ret = -1;
        }
    }

    if (ret == -2) {
        std::cout << "Info for all ILC" << std::endl;
        for (int i = 1; i <= 96; i++) {
            call(i);
        }
        ret = 0;
    }

    if (ilc.getLength() > 0) {
        fpga->ilcCommands(ilc.getBus() + 8, ilc);
    }

    return ret;
}

int calData(command_vec cmds) {
    return callFunction(cmds, [](uint8_t a) { return ilc.reportCalibrationData(a); });
}

int info(command_vec cmds) {
    return callFunction(cmds, [](uint8_t a) { return ilc.reportServerID(a); });
}

int openFPGA(command_vec cmds) {
    if (fpga != NULL) {
        std::cerr << "FPGA already opened!" << std::endl;
        return 1;
    }
    char dir[255];
    if (cmds.size() == 0) {
        getcwd(dir, 255);
    } else {
        memcpy(dir, cmds[0].c_str(), cmds[0].length() + 1);
    }
    fpga = new PrintSSFPGA();
    fpga->initialize();
    fpga->open();
    return 0;
}

M1M3TScli cli("M1M3 Thermal System Command Line Interface");

void _updateVerbosity(int newVerbose) {
    _verbose = newVerbose;
    spdlog::level::level_enum logLevel = spdlog::level::trace;

    switch (_verbose) {
        case 0:
            logLevel = spdlog::level::info;
        case 1:
            logLevel = spdlog::level::debug;
            break;
    }
    spdlog::set_level(logLevel);
}

int setPower(command_vec cmds) {
    uint16_t net = 1;
    uint16_t aux = 0;
    switch (cmds.size()) {
        case 0:
            break;
        case 1:
            net = aux = CliApp::onOff(cmds[0]);
            break;
        case 2:
            net = CliApp::onOff(cmds[0]);
            aux = CliApp::onOff(cmds[1]);
            break;
        default:
            std::cerr << "Invalid number of arguments to power command." << std::endl;
            return -1;
    }
    uint16_t pa[16] = {65, aux, 66, aux, 67, aux, 68, aux, 69, net, 70, net, 71, net, 72, net};
    fpga->writeCommandFIFO(pa, 16, 0);
    return 0;
}

int verbose(command_vec cmds) {
    switch (cmds.size()) {
        case 1:
            _updateVerbosity(std::stoi(cmds[0]));
        case 0:
            std::cout << "Verbosity level: " << _verbose << std::endl;
            break;
    }
    return 0;
}

command_t commands[] = {
        {"caldata", &calData, "s?", NEED_FPGA, "<address>..", "Print ILC calibration data"},
        {"close", [=](command_vec) { return closeFPGA(); }, "", NEED_FPGA, NULL, "Close FPGA connection"},
        {"help", [=](command_vec cmds) { return cli.helpCommands(cmds); }, "", 0, NULL,
         "Print commands help"},
        {"info", &info, "s?", NEED_FPGA, "<address>..", "Print ILC info"},
        {"open", &openFPGA, "", 0, NULL, "Open FPGA"},
        {"power", &setPower, "i", NEED_FPGA, "<0|1>", "Power off/on ILC bus"},
        {"verbose", &verbose, "?", 0, "<new level>", "Report/set verbosity level"},
        {NULL, NULL, NULL, 0, NULL, NULL}};

int main(int argc, char* const argv[]) {
    command_vec cmds = cli.init(commands, "hOv", argc, argv);

    spdlog::init_thread_pool(8192, 1);
    std::vector<spdlog::sink_ptr> sinks;
    sinks.push_back(std::make_shared<spdlog::sinks::stdout_color_sink_mt>());

    auto logger = std::make_shared<spdlog::async_logger>("m1m3tscli", sinks.begin(), sinks.end(),
                                                         spdlog::thread_pool(),
                                                         spdlog::async_overflow_policy::block);
    spdlog::set_default_logger(logger);
    _updateVerbosity(_verbose);

    if (_autoOpen) {
        command_vec cmds;
        openFPGA(cmds);
        // IFPGA::get().setPower(false, true);
    }

    if (cmds.empty()) {
        std::cout << "Please type help for more help." << std::endl;
        cli.goInteractive("M1M3TS > ");
        closeFPGA();
        return 0;
    }

    return cli.processCmdVector(cmds);
}
