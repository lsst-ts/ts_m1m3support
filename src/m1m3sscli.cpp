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
#include <ForceActuatorApplicationSettings.h>

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
    PrintILC(uint8_t bus) : ElectromechanicalPneumaticILC(bus) { setAlwaysTrigger(true); }

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
    std::cout << "Reseted " << std::to_string(getBus()) << "/" << std::to_string(address) << std::endl;
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
    std::cout << "Calibration data " << std::to_string(getBus()) << "/" << std::to_string(address)
              << std::endl
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
constexpr int ILC_BUS = 5;
PrintILC ilcs[ILC_BUS] = {PrintILC(1), PrintILC(2), PrintILC(3), PrintILC(4), PrintILC(5)};

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

ForceActuatorApplicationSettings forceActuators;

/**
 * Process command arguments, call given function. Function accepts three
 * arguments - bus number (1-5), FA address, command arguments begin and
 * command arguments. It returns <0 on error.
 *
 * Address is either <bus>/<address>, where bus is bus number (1-5) and address
 * is the address (1-46). Or a number >= 101 and <= 443, denoting force
 * actuator ID. Single number 1-6 is for hardpoints.
 *
 * @param cmds commands array
 * @param call function to call
 *
 * @return
 */
int callFunction(command_vec cmds,
                 std::function<void(uint8_t, uint8_t, command_vec::iterator&, command_vec)> call) {
    for (int i = 0; i < ILC_BUS; i++) {
        ilcs[i].clear();
    }

    if (cmds.empty()) {
        std::cout << "Info for all ILC" << std::endl;
        command_vec::iterator beg = cmds.begin();
        for (int i = 0; i < 156; i++) {
            ForceActuatorTableRow row = forceActuators.Table[i];
            call(row.Subnet, row.Address, beg, cmds);
        }
        return 0;
    }

    for (command_vec::iterator c = cmds.begin(); c != cmds.end(); c++) {
        std::size_t division = c->find('/');
        int bus = -1;
        int address = -1;
        try {
            if (division != std::string::npos) {
                bus = std::stoi(c->substr(0, division));
                address = std::stoi(c->substr(division + 1));
                if (address <= 0 || address > 46) {
                    std::cerr << "Invalid address " << *c << std::endl;
                    return -1;
                }
            }
            // try to find bus & address by FA/HP ID
            else {
                int id = std::stoi(*c);
                if (id >= 101 && id <= 443) {
                    id = forceActuators.ActuatorIdToZIndex(id);
                    if (id < 0) {
                        std::cerr << "Unknown actuator ID " << id << std::endl;
                        return -1;
                    }
                    ForceActuatorTableRow row = forceActuators.Table[id];
                    if (_verbose) {
                        std::cout << "Id: " << id << " address: " << std::to_string(bus) << "/"
                                  << std::to_string(address) << std::endl;
                    }
                    bus = row.Subnet;
                    address = row.Address;
                }
            }
        } catch (std::logic_error& e) {
            std::cerr << "Non-numeric address: " << *c << std::endl;
            return -1;
        }

        call(bus, address, c, cmds);
    }

    for (int i = 0; i < ILC_BUS; i++) {
        if (ilcs[i].getLength() > 0) {
            fpga->ilcCommands(ilcs[i]);
        }
    }

    return 0;
}

int calData(command_vec cmds) {
    return callFunction(cmds, [](uint8_t bus, uint8_t address, command_vec::iterator& c, command_vec cmds) {
        return ilcs[bus].reportCalibrationData(address);
    });
}

int calSet(command_vec cmds) {
    return callFunction(cmds, [](uint8_t bus, uint8_t address, command_vec::iterator& c, command_vec cmds) {
        uint8_t channel = std::stoi(*(++c));
        float offset = std::stof(*(++c));
        float sensitivity = std::stof(*(++c));
        return ilcs[bus].setOffsetAndSensitivity(address, channel, offset, sensitivity);
    });
}

int info(command_vec cmds) {
    return callFunction(cmds, [](uint8_t bus, uint8_t address, command_vec::iterator& c, command_vec cmds) {
        return ilcs[bus].reportServerID(address);
    });
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
        {"calset", &calSet, "siff", NEED_FPGA, "<address> <channel> <ADC Kn> <offset> <setpoint>",
         "Set calibration data for given channel"},
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
