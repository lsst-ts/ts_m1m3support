/**
 * Command line tool for command line access to M1M3 FPGA.
 */

#include <FPGA.h>
#include <FPGAAddresses.h>
#include <NiStatus.h>
#include <ILCApplicationSettings.h>
#include <ILCMessageFactory.h>
#include <ModbusBuffer.h>

#include <cliapp/CliApp.hpp>
#include <iostream>
#include <iomanip>

#include <spdlog/spdlog.h>
#include <spdlog/async.h>
#include <spdlog/sinks/stdout_color_sinks.h>

using namespace LSST::M1M3::SS;

class M1M3cli : public CliApp {
public:
    M1M3cli(const char* description);

    /**
     * Prints info.
     *
     * @params cmds vector of strings. Expects <subnet>,<address>
     */
    int info(command_vec cmds);

protected:
    void printUsage() override;
    void processArg(int opt, const char* optarg) override;

private:
    ILCApplicationSettings _ilcApplicationSettings;
    ILCMessageFactory _ilcFactory;

    /**
     * Triggers Modbus send. When command 8 is passed to Modbus, this must be
     * called to start processing data send after the command. By triggering
     * ModbusSoftwareTrigger register, clears wait for trigger flag.
     */
    void _triggerModbus();

    void _printILCInfo(uint8_t subnet, uint8_t address);
    void _printBuffer(ModbusBuffer* buf);
    void _writeRequestBuffer(ModbusBuffer* buf);
    void _printInBuffer(ModbusBuffer* buf);
};

M1M3cli::M1M3cli(const char* description) : CliApp(description), _ilcFactory(&_ilcApplicationSettings) {
    _ilcApplicationSettings.ReportServerID = 1000;
}

int M1M3cli::info(command_vec cmds) {
    for (auto c : cmds) {
        size_t i = c.find(',');
        if (i == std::string::npos || i != 1) {
            std::cerr << "Unknown actuator " << c << std::endl;
            return -1;
        }

        uint8_t subnet = 0xFF;

        if (('A' <= c[0]) && (c[0] <= 'E')) {
            subnet = c[0] - 'A';
        } else if (('a' <= c[0]) && (c[0] <= 'e')) {
            subnet = c[0] - 'a';
        } else if (('1' <= c[0]) && (c[0] <= '5')) {
            subnet = c[0] - '1';
        } else {
            std::cerr << "Invalid subnet: " << c[0] << ", expected A-E,a-e or 1-5" << std::endl;
            return -1;
        }

        _printILCInfo(subnet, std::stoi(c.substr(i + 1)));
    }

    return 0;
}

void M1M3cli::printUsage() {
    std::cout << "M1M3 command line tool. Access M1M3 FPGA." << std::endl
              << std::endl
              << "Options: " << std::endl
              << "  -h   help" << std::endl
              << "  -O   don't auto open (and run) FPGA" << std::endl
              << "  -v   increase verbosity" << std::endl;
    command_vec cmds;
    helpCommands(cmds);
}

bool _autoOpen = true;
int _verbose = 0;

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

void M1M3cli::processArg(int opt, const char* optarg) {
    switch (opt) {
        case 'h':
            printAppHelp();
            exit(EXIT_SUCCESS);
            break;

        case 'v':
            _verbose++;
            break;

        case 'O':
            _autoOpen = false;
            break;

        default:
            std::cerr << "Unknow command: " << (char)(opt) << std::endl;
            exit(EXIT_FAILURE);
    }
}

void M1M3cli::_triggerModbus() {
    spdlog::debug("M1M3 triggerModbus()");
    IFPGA::get().writeCommandFIFO(FPGAAddresses::ModbusSoftwareTrigger, 0);
}

void M1M3cli::_printILCInfo(uint8_t subnet, uint8_t address) {
    ModbusBuffer mbuf;

    mbuf.writeSubnet(FPGAAddresses::ModbusSubnetsTx[subnet]);
    mbuf.writeLength(0);
    mbuf.writeSoftwareTrigger();
    mbuf.writeTimestamp();

    _ilcFactory.reportServerID(&mbuf, address);

    mbuf.writeTriggerIRQ();
    mbuf.set(1, mbuf.getIndex() - 2);

    // write ModbusSoftwareTrigger
    mbuf.writeLength(FPGAAddresses::ModbusSoftwareTrigger);

    mbuf.setLength(mbuf.getIndex());

    _writeRequestBuffer(&mbuf);

    FPGA::get().waitForModbusIRQ(subnet + 1, 5000);
    FPGA::get().ackModbusIRQ(subnet + 1);

    // read response
    ModbusBuffer rbuf;

    uint64_t ts1, ts2;
    std::vector<uint8_t> data;
    rbuf.pullModbusResponse(FPGAAddresses::ModbusSubnetsRx[subnet], ts1, ts2, data);
    _printInBuffer(&rbuf);
    std::cout << "TimeStamp 1: " << ts1 << " TimeStamp 2: " << ts2 << " Diff: " << std::dec << (ts2 - ts1)
              << std::endl;
    std::cout << "Data:";
    for (auto d : data) std::cout << " " << std::hex << std::setfill('0') << std::setw(2) << (unsigned int)d;
    std::cout << std::dec << std::endl;
}

void M1M3cli::_printBuffer(ModbusBuffer* buf) {
    if (_verbose == 0) return;
    uint16_t* b = buf->getBuffer();
    for (int i = 0; i < buf->getLength(); i++, b++) {
        std::cout << " " << std::hex << std::setw(4) << std::setfill('0') << *b;
    }
    std::cout << std::endl;
}

void M1M3cli::_writeRequestBuffer(ModbusBuffer* buf) {
    if (_verbose > 0) std::cout << "> ";
    FPGA::get().writeCommandFIFO(buf->getBuffer(), buf->getLength(), 0);
    _printBuffer(buf);
}

void M1M3cli::_printInBuffer(ModbusBuffer* buf) {
    if (_verbose == 0) return;
    std::cout << "< ";
    _printBuffer(buf);
}

int serials(command_vec cmds) {
    IFPGA::get().pullHealthAndStatus();
    ModbusPort* p = IFPGA::get().getHealthAndStatusFPGAData()->ports;
    std::cout << "Bus Error TxBytes TxFrames  RxBytes RxFrames InsCount" << std::endl;
    for (int i = 0; i < 5; i++, p++) {
        std::cout << "  " << (char)('A' + i) << " 0x" << std::hex << std::setfill('0') << std::setw(2)
                  << p->errorFlag << " " << std::dec << std::setfill(' ') << std::setw(8) << p->txBytes << " "
                  << std::setw(8) << p->txFrames << " " << std::setw(8) << p->rxBytes << " " << std::setw(8)
                  << p->rxFrames << " " << std::setw(8) << p->instructionCount << std::endl;
    }
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

void closeFPGA() { IFPGA::get().close(); }

void openFPGA() {
    IFPGA::get().initialize();
    IFPGA::get().open();
}

M1M3cli cli("M1M3 Command Line Interface");

command_t commands[] = {{"close",
                         [=](command_vec cmds) {
                             closeFPGA();
                             return 0;
                         },
                         "", 0, NULL, "Close FPGA connection"},
                        {"help", [=](command_vec cmds) { return cli.helpCommands(cmds); }, "", 0, NULL,
                         "Print commands help"},
                        {"info", [=](command_vec cmds) { return cli.info(cmds); }, "S?", 0,
                         "<ID|subnet,address>..", "Print ILC info"},
                        {"open",
                         [=](command_vec) {
                             openFPGA();
                             return 0;
                         },
                         "", 0, NULL, "Open FPGA"},
                        {"serials", &serials, "s", 0, NULL, "Report serial port status"},
                        {"verbose", &verbose, "?", 0, "<new level>", "Rebort/set verbosity level"},
                        {NULL, NULL, NULL, 0, NULL, NULL}};

int main(int argc, char* const argv[]) {
    command_vec cmds = cli.init(commands, "hOv", argc, argv);

    spdlog::init_thread_pool(8192, 1);
    std::vector<spdlog::sink_ptr> sinks;
    sinks.push_back(std::make_shared<spdlog::sinks::stdout_color_sink_mt>());

    auto logger = std::make_shared<spdlog::async_logger>("m1m3cli", sinks.begin(), sinks.end(),
                                                         spdlog::thread_pool(),
                                                         spdlog::async_overflow_policy::block);
    spdlog::set_default_logger(logger);
    _updateVerbosity(_verbose);

    if (_autoOpen) openFPGA();

    if (cmds.empty()) {
        std::cout << "Please type help for more help." << std::endl;
        cli.goInteractive("M1M3 > ");
        closeFPGA();
        return 0;
    }

    return cli.processCmdVector(cmds);
}
