/**
 * Command line tool for command line access to M1M3 FPGA.
 */

#include <FPGA.h>
#include <NiStatus.h>

#include <cliapp/CliApp.hpp>
#include <iostream>
#include <iomanip>

using namespace LSST::M1M3::SS;

class M1M3cli : public CliApp {
public:
    M1M3cli(const char* description) : CliApp(description) {}

protected:
    void printUsage() override;
    void processArg(int opt, const char* optarg) override;
};

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

int closeFPGA(command_vec) { return IFPGA::get().close(); }

int info(command_vec cmds) {
    for (auto c : cmds) {
    }

    return 0;
}

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
                  << " " << p->rxFrames << std::endl;
    }
    return 0;
}

M1M3cli cli("M1M3 Command Line Interface");

command_t commands[] = {
        {"close", &closeFPGA, "", 0, NULL, "Close FPGA connection"},
        {"help", [=](command_vec cmds) { return cli.helpCommands(cmds); }, "", 0, NULL, "Print command help"},
        {"info", &info, "I?", 0, "<ID> <ID..>", "Print actuator info"},
        {"open", &openFPGA, "", 0, NULL, "Open FPGA"},
        {"serials", &serials, "s", 0, NULL, "Report serial port status"},
        {NULL, NULL, NULL, 0, NULL, NULL}};

int main(int argc, char* const argv[]) {
    command_vec cmds = cli.init(commands, "h", argc, argv);

    std::cout << "Please type help for more help." << std::endl;
    cli.goInteractive("M1M3 > ");

    return 0;
}
