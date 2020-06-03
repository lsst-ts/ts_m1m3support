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
    void _printOutBuffer(ModbusBuffer* buf);
    void _printRequestBuffer(ModbusBuffer* buf);
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
    std::cout << "M1M3 command line tool. Access M1M3 FPGA." << std::endl << std::endl;
}

void M1M3cli::processArg(int opt, const char* optarg) {
    switch (opt) {
        case 'h':
            printAppHelp();
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

    _ilcFactory.reportServerID(&mbuf, address);

    mbuf.writeTriggerIRQ();
    mbuf.set(1, mbuf.getIndex() - 2);

    // write ModbusSoftwareTrigger
    mbuf.writeLength(FPGAAddresses::ModbusSoftwareTrigger);

    mbuf.setLength(mbuf.getIndex());

    _printOutBuffer(&mbuf);

    // read response
    ModbusBuffer rbuf;

    rbuf.writeSubnet(FPGAAddresses::ModbusSubnetsRx[subnet]);
    rbuf.setLength(1);

    _printRequestBuffer(&rbuf);
}

void M1M3cli::_printBuffer(ModbusBuffer* buf) {
    uint16_t* b = buf->getBuffer();
    for (int i = 0; i < buf->getLength(); i++, b++) {
        std::cout << " " << std::hex << std::setw(4) << std::setfill('0') << *b;
    }
    std::cout << std::endl;
}

void M1M3cli::_printOutBuffer(ModbusBuffer* buf) {
    std::cout << "> ";
    FPGA::get().writeCommandFIFO(buf->getBuffer(), buf->getLength(), 0);
    _printBuffer(buf);
}

void M1M3cli::_printRequestBuffer(ModbusBuffer* buf) {
    std::cout << "R ";
    FPGA::get().writeRequestFIFO(buf->getBuffer(), buf->getLength(), 0);
    _printBuffer(buf);

    // get response..
    ModbusBuffer response;
    if (FPGA::get().readU16ResponseFIFO(response.getBuffer(), 1, 20)) return;

    uint16_t reportedLength = response.getLength();
    if (reportedLength == 0) {
        spdlog::error("M1M3: Timeout on response");
        return;
    }

    response.reset();
    if (FPGA::get().readU16ResponseFIFO(response.getBuffer(), reportedLength, 10)) {
        spdlog::warn("M1M3: Timeout reading response of length {0}", reportedLength);
    }
    response.setLength(reportedLength);
    _printInBuffer(&response);
}

void M1M3cli::_printInBuffer(ModbusBuffer* buf) {
    std::cout << "< ";
    _printBuffer(buf);
}

int closeFPGA(command_vec) { return IFPGA::get().close(); }

int openFPGA(command_vec) {
    IFPGA::get().initialize();
    return IFPGA::get().open();
}

int serials(command_vec cmds) {
    IFPGA::get().pullHealthAndStatus();
    ModbusPort* p = IFPGA::get().getHealthAndStatusFPGAData()->ports;
    for (int i = 0; i < 5; i++, p++) {
        std::cout << "Bus " << (char)('A' + i) << ": 0x" << std::hex << std::setfill('0') << std::setw(2)
                  << p->errorFlag << std::dec << " " << p->txBytes << " " << p->txFrames << " " << p->rxBytes
                  << " " << p->rxFrames << " " << p->instructionCount << std::endl;
    }
    return 0;
}

M1M3cli cli("M1M3 Command Line Interface");

command_t commands[] = {
        {"close", &closeFPGA, "", 0, NULL, "Close FPGA connection"},
        {"help", [=](command_vec cmds) { return cli.helpCommands(cmds); }, "", 0, NULL, "Print command help"},
        {"info", [=](command_vec cmds) { return cli.info(cmds); }, "S?", 0, "<ID|subnet,address>..",
         "Print ILC info"},
        {"open", &openFPGA, "", 0, NULL, "Open FPGA"},
        {"serials", &serials, "s", 0, NULL, "Report serial port status"},
        {NULL, NULL, NULL, 0, NULL, NULL}};

int main(int argc, char* const argv[]) {
    command_vec cmds = cli.init(commands, "h", argc, argv);

    spdlog::init_thread_pool(8192, 1);
    std::vector<spdlog::sink_ptr> sinks;
    sinks.push_back(std::make_shared<spdlog::sinks::stdout_color_sink_mt>());

    auto logger = std::make_shared<spdlog::async_logger>("m1m3cli", sinks.begin(), sinks.end(),
                                                         spdlog::thread_pool(),
                                                         spdlog::async_overflow_policy::block);
    spdlog::set_default_logger(logger);
    spdlog::set_level(spdlog::level::trace);

    std::cout << "Please type help for more help." << std::endl;
    cli.goInteractive("M1M3 > ");

    return 0;
}
