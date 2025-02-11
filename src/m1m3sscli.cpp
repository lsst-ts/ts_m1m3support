/*
 * Command line Thermal System client.
 *
 * Developed for the Vera C. Rubin Observatory Telescope & Site Software
 * Systems. This product includes software developed by the Vera C.Rubin
 * Observatory Project (https://www.lsst.org). See the COPYRIGHT file at the
 * top-level directory of this distribution for details of code ownership.
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

#include <csignal>
#include <fstream>
#include <iomanip>
#include <iostream>

#include <spdlog/async.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>

#include <cRIO/ElectromechanicalPneumaticILC.h>
#include <cRIO/FPGACliApp.h>
#include <cRIO/PrintILC.h>
#include <cRIO/Thread.h>

#ifdef SIMULATOR
#include <SimulatedFPGA.h>
#define FPGAClass SimulatedFPGA
#else
#include <FPGA.h>
#define FPGAClass LSST::M1M3::SS::FPGA
#endif

#include <FPGA.h>
#include <FPGAAddresses.h>
#include <ForceActuatorApplicationSettings.h>
#include <NiFpga_M1M3SupportFPGA.h>

using namespace LSST::cRIO;
using namespace LSST::M1M3::SS;
using namespace LSST::M1M3::SS::FPGAAddresses;
using namespace std::chrono_literals;

#define MAX_FORCE 250

std::ofstream *_test_output = nullptr;

class M1M3SScli : public FPGACliApp {
public:
    M1M3SScli(const char *name, const char *description);
    virtual ~M1M3SScli();

    int setPower(command_vec cmds);
    int setAir(command_vec cmds);
    int setLights(command_vec cmds);
    int setDCAGain(command_vec cmds);
    int setSAAOffset(command_vec cmds);
    int setDAAOffset(command_vec cmds);
    int setCalibration(command_vec cmds);
    int testOutput(command_vec cmds);
    int testSettings(command_vec cmds);
    int testSAA(command_vec cmds);
    int testDAA(command_vec cmds);

    int dumpAccelerometer(command_vec cmds);

protected:
    virtual LSST::cRIO::FPGA *newFPGA(const char *dir, bool &fpga_singleton) override;
    virtual ILCUnits getILCs(command_vec cmds) override;

private:
    void _printSupportData();
    void _report_pressure_forces(ILCUnits &ilcs);

    std::chrono::milliseconds _test_duration;
    std::chrono::milliseconds _test_pause_force;
    std::chrono::milliseconds _test_pause_pressure;
};

class PrintElectromechanical : public ElectromechanicalPneumaticILC, public PrintILC {
public:
    PrintElectromechanical(uint8_t bus)
            : ILCBusList(bus), ElectromechanicalPneumaticILC(bus), PrintILC(bus) {}

protected:
    void processStepperForceStatus(uint8_t address, uint8_t status, int32_t encoderPostion,
                                   float loadCellForce) override;

    void processDCAGain(uint8_t address, float primaryGain, float secondaryGain) override;

    void processHardpointLVDT(uint8_t address, float breakawayLVDT, float displacementLVDT) override;

    void processSAAForceStatus(uint8_t address, uint8_t status, float primaryLoadCellForce) override;
    void processDAAForceStatus(uint8_t address, uint8_t status, float primaryLoadCellForce,
                               float secondaryLoadCellForce) override;

    void processCalibrationData(uint8_t address, float mainADCK[4], float mainOffset[4],
                                float mainSensitivity[4], float backupADCK[4], float backupOffset[4],
                                float backupSensitivity[4]) override;

    void processMezzaninePressure(uint8_t address, float primaryPush, float primaryPull, float secondaryPush,
                                  float secondaryPull) override;
};

class PrintSSFPGA : public FPGAClass {
public:
#ifdef SIMULATOR
    PrintSSFPGA() : SimulatedFPGA() {}
#else
    PrintSSFPGA() : LSST::M1M3::SS::FPGA() {}
#endif

    void writeCommandFIFO(uint16_t *data, size_t length, uint32_t timeout) override;
    void writeRequestFIFO(uint16_t *data, size_t length, uint32_t timeout) override;
    void readU16ResponseFIFO(uint16_t *data, size_t length, uint32_t timeout) override;
};

M1M3SScli::M1M3SScli(const char *name, const char *description)
        : FPGACliApp(name, description),
          _test_duration(std::chrono::milliseconds(3000)),
          _test_pause_force(std::chrono::milliseconds(20)),
          _test_pause_pressure(std::chrono::milliseconds(20)) {
    addCommand("power", std::bind(&M1M3SScli::setPower, this, std::placeholders::_1), "i", NEED_FPGA, "<0|1>",
               "Power off/on ILC bus");

    addCommand("air", std::bind(&M1M3SScli::setAir, this, std::placeholders::_1), "b", NEED_FPGA, "<0|1>",
               "Turns air valve off/on");

    addCommand("accelerometer", std::bind(&M1M3SScli::dumpAccelerometer, this, std::placeholders::_1), "",
               NEED_FPGA, NULL, "Dumps raw accelerometer data");

    addCommand("lights", std::bind(&M1M3SScli::setLights, this, std::placeholders::_1), "b", NEED_FPGA,
               "<0|1>", "Turns lights off/on");

    addILCCommand(
            "calibration",
            [](ILCUnit u) {
                std::dynamic_pointer_cast<PrintElectromechanical>(u.first)->reportCalibrationData(u.second);
            },
            "Read calibration data");

    addILCCommand(
            "measured",
            [](ILCUnit u) {
                std::dynamic_pointer_cast<PrintElectromechanical>(u.first)->reportForceActuatorForceStatus(
                        u.second);
            },
            "Report measured load cell force");

    addILCCommand(
            "pressure",
            [](ILCUnit u) {
                std::dynamic_pointer_cast<PrintElectromechanical>(u.first)->reportMezzaninePressure(u.second);
            },
            "Read mezzanine pressure");

    addILCCommand(
            "hardpoint-force",
            [](ILCUnit u) {
                std::dynamic_pointer_cast<PrintElectromechanical>(u.first)->reportStepperForceStatus(
                        u.second);
            },
            "Read hardpoint info");

    addILCCommand(
            "dca-gain",
            [](ILCUnit u) {
                std::dynamic_pointer_cast<PrintElectromechanical>(u.first)->reportDCAGain(u.second);
            },
            "Read booster valve DCA Gain");

    addILCCommand(
            "lvdt",
            [](ILCUnit u) {
                std::dynamic_pointer_cast<PrintElectromechanical>(u.first)->reportHardpointLVDT(u.second);
            },
            "Read LVDT info");

    addCommand("set-dca-gain", std::bind(&M1M3SScli::setDCAGain, this, std::placeholders::_1), "DDs?",
               NEED_FPGA, "<axial gain> <lateral gain> <ILC..>", "Set DCA gain");

    addCommand("@test-output", std::bind(&M1M3SScli::testOutput, this, std::placeholders::_1), "S", 0,
               "<filename>", "Set filename to record progress of actuators tests.");
    addCommand("@test-settings", std::bind(&M1M3SScli::testSettings, this, std::placeholders::_1), "III", 0,
               "<duration> <force-pause> <pressure-pause>",
               "Sets test parameters. Duration specifies for how long will data be recorded, -pause tells "
               "CLI delays to take after force or pressure telemetry.");

    addCommand("saa-offset", std::bind(&M1M3SScli::setSAAOffset, this, std::placeholders::_1), "Ds?",
               NEED_FPGA, "<primary> <ILC..>", "Set ILC primary force offset");
    addCommand("saa-test", std::bind(&M1M3SScli::testSAA, this, std::placeholders::_1), "Ds?", NEED_FPGA,
               "<primary> <ILC..>", "Set force, readout force and pressure for 3 seconds");

    addCommand("daa-offset", std::bind(&M1M3SScli::setDAAOffset, this, std::placeholders::_1), "DDs?",
               NEED_FPGA, "<primary> <secondary> <ILC..>", "Set ILC primary and secondary force offsets");
    addCommand("daa-test", std::bind(&M1M3SScli::testDAA, this, std::placeholders::_1), "DDs?", NEED_FPGA,
               "<primary> <secondary> <ILC..>", "Set force, readout force and pressure for 3 seconds");

    addCommand("set-calibration", std::bind(&M1M3SScli::setCalibration, this, std::placeholders::_1), "IDDS",
               NEED_FPGA, "<channel> <offset> <sensitivity> <ILC>", "Write calibration data");

    addILC(std::make_shared<PrintElectromechanical>(1));
    addILC(std::make_shared<PrintElectromechanical>(2));
    addILC(std::make_shared<PrintElectromechanical>(3));
    addILC(std::make_shared<PrintElectromechanical>(4));
    addILC(std::make_shared<PrintElectromechanical>(5));
}

M1M3SScli::~M1M3SScli() {
    if (_test_output != nullptr) {
        _test_output->close();
    }
    delete _test_output;
    _test_output = NULL;
}

int M1M3SScli::setPower(command_vec cmds) {
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
    uint16_t pa[16] = {FPGAAddresses::DCAuxPowerNetworkAOn, aux, FPGAAddresses::DCAuxPowerNetworkBOn, aux,
                       FPGAAddresses::DCAuxPowerNetworkCOn, aux, FPGAAddresses::DCAuxPowerNetworkDOn, aux,
                       FPGAAddresses::DCPowerNetworkAOn,    net, FPGAAddresses::DCPowerNetworkBOn,    net,
                       FPGAAddresses::DCPowerNetworkCOn,    net, FPGAAddresses::DCPowerNetworkDOn,    net};
    getFPGA()->writeCommandFIFO(pa, 16, 0);

    _printSupportData();
    return 0;
}

int M1M3SScli::setAir(command_vec cmds) {
    if (cmds.size() == 1) {
        uint16_t on = CliApp::onOff(cmds[0]);
        uint16_t aa[2] = {FPGAAddresses::AirSupplyValveControl, on};
        getFPGA()->writeCommandFIFO(aa, 2, 0);
    }

    _printSupportData();
    return 0;
}

int M1M3SScli::setLights(command_vec cmds) {
    if (cmds.size() == 1) {
        uint16_t on = CliApp::onOff(cmds[0]);
        uint16_t aa[2] = {FPGAAddresses::MirrorCellLightControl, on};
        getFPGA()->writeCommandFIFO(aa, 2, 0);
    }

    _printSupportData();
    return 0;
}

int M1M3SScli::setDCAGain(command_vec cmds) {
    float primary = stof(cmds[0]);
    float secondary = stof(cmds[1]);

    cmds.erase(cmds.begin(), cmds.begin() + 2);

    clearILCs();
    ILCUnits ilcs = getILCs(cmds);
    for (auto u : ilcs) {
        std::dynamic_pointer_cast<PrintElectromechanical>(u.first)->setDCAGain(u.second, primary, secondary);
    }
    runILCCommands(500);
    return 0;
}

int M1M3SScli::testOutput(command_vec cmds) {
    if (_test_output != nullptr) {
        _test_output->close();
        delete _test_output;
    }
    _test_output = new std::ofstream(cmds[0], std::ofstream::out);
    if (_test_output->good()) {
        std::cout << "Telemery will be written to: " << cmds[0] << std::endl;
    } else {
        std::cerr << "Cannot open " << cmds[0] << " as telemetry log file: " << strerror(errno) << std::endl;
        delete _test_output;
        _test_output = nullptr;
    }
    return 0;
}

int M1M3SScli::testSettings(command_vec cmds) {
    _test_duration = std::chrono::milliseconds(std::stoi(cmds[0]));
    _test_pause_force = std::chrono::milliseconds(std::stoi(cmds[1]));
    _test_pause_pressure = std::chrono::milliseconds(std::stoi(cmds[2]));

    std::cout << "Duration: " << _test_duration.count() << " Force pause: " << _test_pause_force.count()
              << " Pressure pause: " << _test_pause_pressure.count() << std::endl;
    return 0;
}

int M1M3SScli::setSAAOffset(command_vec cmds) {
    float primary = stof(cmds[0]);

    if (fabs(primary) > MAX_FORCE) {
        std::cerr << "Force offset must be below " << MAX_FORCE << " N, received " << primary << " N "
                  << std::endl;
        return -1;
    }

    cmds.erase(cmds.begin(), cmds.begin() + 1);

    clearILCs();
    ILCUnits ilcs = getILCs(cmds);
    for (auto u : ilcs) {
        std::dynamic_pointer_cast<PrintElectromechanical>(u.first)->setSAAForceOffset(u.second, false,
                                                                                      primary);
    }
    runILCCommands(500);
    return 0;
}

int M1M3SScli::setDAAOffset(command_vec cmds) {
    float primary = stof(cmds[0]);
    float secondary = stof(cmds[1]);

    if (fabs(primary) > MAX_FORCE || fabs(secondary) > MAX_FORCE) {
        std::cerr << "Force offset must be below " << MAX_FORCE << " N, received " << primary << " N and "
                  << secondary << " N" << std::endl;
        return -1;
    }

    cmds.erase(cmds.begin(), cmds.begin() + 2);

    clearILCs();
    ILCUnits ilcs = getILCs(cmds);
    for (auto u : ilcs) {
        std::dynamic_pointer_cast<PrintElectromechanical>(u.first)->setDAAForceOffset(u.second, false,
                                                                                      primary, secondary);
    }
    runILCCommands(500);
    return 0;
}

int M1M3SScli::setCalibration(command_vec cmds) {
    uint8_t channel = stod(cmds[0]);
    float offset = stof(cmds[1]);
    float sensitivity = stof(cmds[2]);

    if (channel > 3) {
        std::cerr << "Channel should be 0-3, was commanded as " << +channel << std::endl;
        return -1;
    }

    cmds.erase(cmds.begin(), cmds.begin() + 3);

    clearILCs();
    ILCUnits ilcs = getILCs(cmds);
    if (ilcs.size() != 1) {
        std::cerr << "Calibration data can be set only for a single ILC." << std::endl;
        return -1;
    }
    for (auto u : ilcs) {
        std::dynamic_pointer_cast<PrintElectromechanical>(u.first)->setOffsetAndSensitivity(
                u.second, channel, offset, sensitivity);
    }
    runILCCommands(500);
    return 0;
}

int M1M3SScli::testSAA(command_vec cmds) {
    float primary = stof(cmds[0]);

    if (fabs(primary) > MAX_FORCE) {
        std::cerr << "Force offset must be below " << MAX_FORCE << " N, received " << primary << " N "
                  << std::endl;
        return -1;
    }

    cmds.erase(cmds.begin(), cmds.begin() + 1);

    clearILCs();
    ILCUnits ilcs = getILCs(cmds);
    for (auto u : ilcs) {
        std::dynamic_pointer_cast<PrintElectromechanical>(u.first)->setSAAForceOffset(u.second, false,
                                                                                      primary);
    }
    runILCCommands(500);

    _report_pressure_forces(ilcs);

    return 0;
}

int M1M3SScli::testDAA(command_vec cmds) {
    float primary = stof(cmds[0]);
    float secondary = stof(cmds[1]);

    if (fabs(primary) > MAX_FORCE || fabs(secondary) > MAX_FORCE) {
        std::cerr << "Force offset must be below " << MAX_FORCE << " N, received " << primary << " N and "
                  << secondary << " N" << std::endl;
        return -1;
    }

    cmds.erase(cmds.begin(), cmds.begin() + 2);

    clearILCs();
    ILCUnits ilcs = getILCs(cmds);
    for (auto u : ilcs) {
        std::dynamic_pointer_cast<PrintElectromechanical>(u.first)->setDAAForceOffset(u.second, false,
                                                                                      primary, secondary);
    }
    runILCCommands(500);

    _report_pressure_forces(ilcs);

    return 0;
}

class ReadRawAccelerometer : public Thread {
public:
    ReadRawAccelerometer(FPGAClass *_fpga) { fpga = _fpga; }
    void run(std::unique_lock<std::mutex> &lock) override;

    FPGAClass *fpga;
};

static ReadRawAccelerometer *rawThread = NULL;

void signal_stop_dump(int signal) {
    if (rawThread == NULL) {
        std::cerr << "Invalid call to stop dump signal handle." << std::endl;
        return;
    }
    rawThread->stop(10s);
    std::cerr << "End dumping data." << std::endl;
}

void ReadRawAccelerometer::run(std::unique_lock<std::mutex> &lock) {
    uint64_t raw[8];
    while (keepRunning) {
        runCondition.wait_for(lock, 1ms);
        for (int meas = 0; meas < 100; meas++) {
            fpga->readRawAccelerometerFIFO(raw, 1);
            for (int i = 0; i < 8; i++) {
                std::cout << std::fixed << std::setw(10) << std::setprecision(2)
                          << NiFpga_ConvertFromFxpToFloat(
                                     NiFpga_M1M3SupportFPGA_TargetToHostFifoFxp_RawAccelerometer_TypeInfo,
                                     raw[i]) *
                                     500;
            }
            std::cout << std::endl;
        }
    }
}

int M1M3SScli::dumpAccelerometer(command_vec cmds) {
    std::cerr << "Press Ctr+c to end dump." << std::endl;

    rawThread = new ReadRawAccelerometer(dynamic_cast<FPGAClass *>(getFPGA()));
    std::signal(SIGINT, &signal_stop_dump);

    std::this_thread::sleep_for(500ms);
    rawThread->start();

    while (rawThread->isRunning()) {
        std::unique_lock<std::mutex> lock;
        std::this_thread::sleep_for(100ms);
    }

    std::signal(SIGINT, SIG_DFL);

    delete rawThread;
    rawThread = NULL;

    return 0;
}

LSST::cRIO::FPGA *M1M3SScli::newFPGA(const char *dir, bool &fpga_singleton) {
    fpga_singleton = false;
    return new PrintSSFPGA();
}

constexpr int ILC_BUS = 5;

ForceActuatorApplicationSettings forceActuators;

ILCUnits M1M3SScli::getILCs(command_vec cmds) {
    ILCUnits units;
    int ret = -2;

    auto getBus = [](std::string b) -> int {
        if (b.length() != 1) {
            throw std::runtime_error("Invalid bus length - expected one character, got " + b);
        }
        if (b[0] >= 'A' && b[0] <= 'E') {
            return b[0] - 'A';
        }
        if (b[0] >= 'a' && b[0] <= 'e') {
            return b[0] - 'a';
        }
        if (b[0] >= '1' && b[0] <= '5') {
            return b[0] - '1';
        }
        throw std::runtime_error("Invalid bus name: " + b);
    };

    for (auto c : cmds) {
        size_t division = c.find('/');
        int bus = -1;
        int address = -1;
        try {
            if (division != std::string::npos) {
                bus = getBus(c.substr(0, division));
                std::string add_s = c.substr(division + 1);
                if (add_s == "*") {
                    for (int i = 0; i < 156; i++) {
                        ForceActuatorTableRow row = forceActuators.Table[i];
                        if (row.Subnet == bus + 1) {
                            units.push_back(ILCUnit(getILC(row.Subnet - 1), row.Address));
                        }
                    }
                    continue;
                }
                address = std::stoi(add_s);
                if ((bus != 4 && (address <= 0 || address > 46)) ||
                    (address <= 0 || address > 89 || (address > 6 && address < 84))) {
                    std::cerr << "Invalid address " << c << std::endl;
                    units.clear();
                    return units;
                }
            }
            // try to find bus & address by FA/HP ID
            else {
                int id = std::stoi(c);
                if (id >= 101 && id <= 443) {
                    if (getDebugLevel() > 1) {
                        std::cout << "FA ID: " << std::dec << +id;
                    }
                    id = forceActuators.ActuatorIdToZIndex(id);
                    if (id < 0) {
                        std::cerr << "Unknown actuator ID " << c << std::endl;
                        ret = -1;
                        continue;
                    }
                    ForceActuatorTableRow row = forceActuators.Table[id];
                    bus = row.Subnet - 1;
                    address = row.Address;
                    if (getDebugLevel() > 1) {
                        std::cout << " Index: " << id << " address: " << std::to_string(bus) << "/"
                                  << std::to_string(address) << std::endl;
                    }
                    // HP actuator and monitor ILCs
                } else if (id >= 1 && id <= 6) {
                    bus = 4;
                    address = id;
                } else if (id >= 11 && id <= 16) {
                    bus = 4;
                    address = 73 + id;  // 84 + 0-based ID
                } else {
                    std::cerr << "Invalid ID (1-6 for HP, 11-16 for HPmon, 101-443): " << id << std::endl;
                    units.clear();
                    return units;
                }
            }
        } catch (std::logic_error &e) {
            std::cerr << "Non-numeric address: " << c << std::endl;
        }

        if (bus < 0 || bus >= ILC_BUS || address == -1) {
            std::cerr << "Invalid ILC address: " << c << std::endl;
            ret = -1;
            continue;
        }

        units.push_back(ILCUnit(getILC(bus), address));
    }

    if (ret == -2 && units.empty()) {
        std::cout << "Command for all ILC" << std::endl;
        for (int i = 0; i < 156; i++) {
            ForceActuatorTableRow row = forceActuators.Table[i];
            units.push_back(ILCUnit(getILC(row.Subnet - 1), row.Address));
        }
        for (int i = 1; i < 7; i++) {
            units.push_back(ILCUnit(getILC(4), i));
        }
        ret = 0;
    }

    return units;
}

void M1M3SScli::_printSupportData() {
    dynamic_cast<FPGAClass *>(getFPGA())->pullTelemetry();
    SupportFPGAData *fpgaData = dynamic_cast<FPGAClass *>(getFPGA())->getSupportFPGAData();

    auto printOnOff = [](bool on) { return (on ? "on" : "off"); };
    auto printNeg = [printOnOff](bool neg) { return printOnOff(!neg); };

    std::cout << "Air Valve Command Open: "
              << printOnOff(fpgaData->DigitalOutputStates & FPGAAddresses::DigitalOutputs::AirCommandOutputOn)
              << std::endl
              << "Air Valve Open: "
              << printNeg(fpgaData->DigitalInputStates & FPGAAddresses::DigitalInputs::AirValveOpened)
              << std::endl
              << "Air Valve Closed: "
              << printNeg(fpgaData->DigitalInputStates & FPGAAddresses::DigitalInputs::AirValveClosed)
              << std::endl
              << "Lights Commanded On: "
              << printOnOff(fpgaData->DigitalOutputStates & FPGAAddresses::DigitalOutputs::CellLightsOutputOn)
              << std::endl
              << "Lights On: "
              << printOnOff(fpgaData->DigitalInputStates & FPGAAddresses::DigitalInputs::CellLightsOn)
              << std::endl;
}

void M1M3SScli::_report_pressure_forces(ILCUnits &ilcs) {
    auto end = std::chrono::steady_clock::now() + _test_duration;

    bool show_press = true;

    while (std::chrono::steady_clock::now() < end) {
        clearILCs();
        for (auto u : ilcs) {
            if (show_press) {
                std::this_thread::sleep_for(_test_pause_pressure);
                std::dynamic_pointer_cast<PrintElectromechanical>(u.first)->reportMezzaninePressure(u.second);
            } else {
                std::this_thread::sleep_for(_test_pause_force);
                std::dynamic_pointer_cast<PrintElectromechanical>(u.first)->reportForceActuatorForceStatus(
                        u.second);
            }
        }
        runILCCommands(500);

        show_press = !show_press;
    }

    clearILCs();
}

unsigned int _printout = 0;

void _printSepline() {
    if (_printout > 0) {
        std::cout << std::endl;
    }
    _printout++;
}

template <typename t>
void print4(const char *name, t a[4]) {
    std::cout << std::setfill(' ') << std::setw(18) << name;
    for (int i = 0; i < 4; i++) {
        std::cout << " " << std::setw(15) << std::setprecision(10) << a[i];
    }
    std::cout << std::endl;
}

void PrintElectromechanical::processStepperForceStatus(uint8_t address, uint8_t status,
                                                       int32_t encoderPostion, float loadCellForce) {
    _printSepline();

    auto limitSwitch = [](bool sw) { return (sw ? "CLOSED" : "OPEN"); };

    std::cout << "Status: 0x" << std::setfill('0') << std::setw(2) << std::hex << status << std::dec
              << " (ILC: " << (status & 0x80 ? "FAULT" : "OK") << ", Limit Switch CW "
              << limitSwitch(status & 0x20) << ", Limit Switch CCW " << limitSwitch(status & 0x10) << ")"
              << std::endl;
    std::cout << "Encoder Position: " << encoderPostion << std::endl;
    std::cout << "Load Cell Force: " << std::setprecision(2) << std::fixed << loadCellForce << " N"
              << std::endl;
}

void PrintElectromechanical::processDCAGain(uint8_t address, float primaryGain, float secondaryGain) {
    _printSepline();

    std::cout << "Primary (axial) gain: " << primaryGain << std::endl;
    std::cout << "Secondary (lateral) gain: " << secondaryGain << std::endl;
}

void PrintElectromechanical::processHardpointLVDT(uint8_t address, float breakawayLVDT,
                                                  float displacementLVDT) {
    _printSepline();

    std::cout << "Breakaway LVDT: " << std::setprecision(5) << std::fixed << breakawayLVDT << std::endl
              << "Displacement LVDT: " << std::setprecision(5) << std::fixed << displacementLVDT << std::endl;
}

void PrintElectromechanical::processSAAForceStatus(uint8_t address, uint8_t status,
                                                   float primaryLoadCellForce) {
    _printSepline();
    std::cout << "Status: 0x" << std::setfill('0') << std::setw(2) << std::hex << status << std::dec
              << " (ILC: " << (status & 0x80 ? "FAULT" : "OK") << " DCA: " << (status & 0x40 ? "FAULT" : "OK")
              << ")" << std::endl;
    std::cout << "Primary force: " << std::setprecision(2) << std::fixed << primaryLoadCellForce << " N"
              << std::endl;

    if (_test_output != nullptr) {
        time_t now;
        time(&now);
        char time_buf[80];
        strftime(time_buf, 80, "%x %X", gmtime(&now));

        *_test_output << time_buf << ", saa-force, " << +address << ", " << std::fixed << primaryLoadCellForce
                      << std::endl;
    }
}

void PrintElectromechanical::processDAAForceStatus(uint8_t address, uint8_t status,
                                                   float primaryLoadCellForce, float secondaryLoadCellForce) {
    _printSepline();
    std::cout << "Status: 0x" << std::setfill('0') << std::setw(2) << std::hex << status << std::dec
              << " (ILC: " << (status & 0x80 ? "FAULT" : "OK") << " DCA: " << (status & 0x40 ? "FAULT" : "OK")
              << ")" << std::endl;
    std::cout << "Primary force: " << std::setprecision(2) << std::fixed << primaryLoadCellForce << " N"
              << std::endl;
    std::cout << "Secondary force: " << std::setprecision(2) << std::fixed << secondaryLoadCellForce << " N"
              << std::endl;

    if (_test_output != nullptr) {
        time_t now;
        time(&now);
        char time_buf[80];
        strftime(time_buf, 80, "%x %X", gmtime(&now));

        *_test_output << time_buf << ", daa-force, " << +address << ", " << std::fixed << primaryLoadCellForce
                      << ", " << secondaryLoadCellForce << std::endl;
    }
}

void PrintElectromechanical::processCalibrationData(uint8_t address, float mainADCK[4], float mainOffset[4],
                                                    float mainSensitivity[4], float backupADCK[4],
                                                    float backupOffset[4], float backupSensitivity[4]) {
    _printSepline();
    std::cout << "Calibration data " << std::to_string(getBus()) << "/" << std::to_string(address)
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

void PrintElectromechanical::processMezzaninePressure(uint8_t address, float primaryPush, float primaryPull,
                                                      float secondaryPush, float secondaryPull) {
    _printSepline();
    std::cout << "Pressure data " << std::to_string(getBus()) << "/" << std::to_string(address) << std::endl;

    auto printPushPull = [](const char *name, float push, float pull) {
        std::cout << std::setfill(' ') << std::setw(10) << name << ": " << std::setw(8)
                  << std::setprecision(2) << std::fixed << push << " | " << std::setw(8)
                  << std::setprecision(2) << std::fixed << push << std::endl;
    };

    printPushPull("Primary", primaryPush, primaryPull);
    printPushPull("Secondary", secondaryPush, secondaryPull);

    if (_test_output != nullptr) {
        time_t now;
        time(&now);
        char time_buf[80];
        strftime(time_buf, 80, "%x %X", gmtime(&now));

        *_test_output << time_buf << ", pressure, " << +address << ", " << std::fixed << primaryPush << ", "
                      << primaryPull << ", " << secondaryPush << ", " << secondaryPull << std::endl;
    }
}

M1M3SScli cli("M1M3SS", "M1M3 Support System Command Line Interface");

void _printBuffer(std::string prefix, uint16_t *buf, size_t len) {
    if (cli.getDebugLevel() == 0) {
        return;
    }

    std::cout << prefix;
    for (size_t i = 0; i < len; i++) {
        std::cout << std::hex << std::setfill('0') << std::setw(4) << buf[i] << " ";
    }
    std::cout << std::endl;
}

void PrintSSFPGA::writeCommandFIFO(uint16_t *data, size_t length, uint32_t timeout) {
    _printBuffer("C> ", data, length);
    FPGAClass::writeCommandFIFO(data, length, timeout);
}

void PrintSSFPGA::writeRequestFIFO(uint16_t *data, size_t length, uint32_t timeout) {
    _printBuffer("R> ", data, length);
    FPGAClass::writeRequestFIFO(data, length, timeout);
}

void PrintSSFPGA::readU16ResponseFIFO(uint16_t *data, size_t length, uint32_t timeout) {
    FPGAClass::readU16ResponseFIFO(data, length, timeout);
    _printBuffer("R< ", data, length);
}

int main(int argc, char *const argv[]) { return cli.run(argc, argv); }
