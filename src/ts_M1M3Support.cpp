#include <CommandFactory.h>
#include <Context.h>
#include <ControllerThread.h>
#include <IExpansionFPGA.h>
#include <M1M3SSPublisher.h>
#include <M1M3SSSubscriber.h>
#include <Model.h>
#include <OuterLoopClockThread.h>
#include <PPSThread.h>
#include <SAL_MTM1M3.h>
#include <SAL_MTMount.h>
#include <SettingReader.h>
#include <SubscriberThread.h>

#include <getopt.h>
#include <cstring>
#include <iostream>

#include <chrono>
#include <thread>

#include <spdlog/spdlog.h>
#include <spdlog/async.h>
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/sinks/daily_file_sink.h"
#include <SALSink.h>

#ifdef SIMULATOR
#include <SimulatedFPGA.h>
#else
#include <FPGA.h>
#endif
#include <NiError.h>

using namespace std;
using namespace LSST::M1M3::SS;

void printHelp() {
    std::cout << "M1M3 Static Support controller. Runs either as simulator or as simulator or as "
                 "the real think on cRIO."
              << std::endl
              << "Options:" << std::endl
              << "  -b runs on background, don't log to stdout" << std::endl
              << "  -c <configuration path> use given configuration directory (should be SettingFiles)"
              << std::endl
              << "  -d increases debugging (can be specified multiple times, default is info)" << std::endl
              << "  -f runs on foreground, don't log to file" << std::endl
              << "  -h prints this help" << std::endl
              << "  -s increases SAL debugging (can be specified multiple times, default is 0)" << std::endl;
}

int debugLevel = 0;
int debugLevelSAL = 0;

std::vector<spdlog::sink_ptr> sinks;

void processArgs(int argc, char* const argv[], const char*& configRoot) {
    int enabledSinks = 0x3;

    int opt;
    while ((opt = getopt(argc, argv, "bc:dfhs")) != -1) {
        switch (opt) {
            case 'b':
                enabledSinks &= ~0x01;
                break;
            case 'c':
                configRoot = optarg;
                break;
            case 'd':
                debugLevel++;
                break;
            case 'f':
                enabledSinks &= ~0x02;
                break;
            case 'h':
                printHelp();
                exit(EXIT_SUCCESS);
            case 's':
                debugLevelSAL++;
                break;
            default:
                std::cerr << "Unknow option " << opt << std::endl;
                printHelp();
                exit(EXIT_FAILURE);
        }
    }

    spdlog::init_thread_pool(8192, 1);
    if (enabledSinks & 0x01) {
        auto stdout_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
        sinks.push_back(stdout_sink);
    }
    if (enabledSinks & 0x02) {
        auto daily_sink = std::make_shared<spdlog::sinks::daily_file_sink_mt>("MTM1M3", 0, 0);
        sinks.push_back(daily_sink);
    }
    auto logger = std::make_shared<spdlog::async_logger>("loggername", sinks.begin(), sinks.end(),
                                                         spdlog::thread_pool(),
                                                         spdlog::async_overflow_policy::block);
    spdlog::set_default_logger(logger);
    spdlog::set_level((debugLevel == 0 ? spdlog::level::info
                                       : (debugLevel == 1 ? spdlog::level::debug : spdlog::level::trace)));
}

void initializeFPGAs(IFPGA* fpga, IExpansionFPGA* expansionFPGA) {
#ifdef SIMULATOR
    SPDLOG_WARN("Starting Simulator version!");
#endif

    SPDLOG_INFO("Main: Creating fpga");

    fpga->initialize();
    fpga->open();

    SPDLOG_INFO("Main: Load expansion FPGA application settings");
    ExpansionFPGAApplicationSettings* expansionFPGAApplicationSettings =
            SettingReader::get().loadExpansionFPGAApplicationSettings();
    SPDLOG_INFO("Main: Create expansion FPGA");
    expansionFPGA->setExpansionFPGAApplicationSettings(expansionFPGAApplicationSettings);
    expansionFPGA->open();
}

void runFPGAs(std::shared_ptr<SAL_MTM1M3> m1m3SAL, std::shared_ptr<SAL_MTMount> mtMountSAL) {
    SPDLOG_INFO("Main: Creating subscriber");
    M1M3SSSubscriber::get().setSAL(m1m3SAL, mtMountSAL);
    SPDLOG_INFO("Main: Creating subscriber thread");
    SubscriberThread subscriberThread;
    SPDLOG_INFO("Main: Creating outer loop clock thread");
    OuterLoopClockThread outerLoopClockThread;
    SPDLOG_INFO("Main: Creating pps thread");
    PPSThread ppsThread;
    SPDLOG_INFO("Main: Queuing EnterControl command");
    ControllerThread::get().enqueue(CommandFactory::create(Commands::EnterControlCommand));

    try {
        SPDLOG_INFO("Main: Starting pps thread");
        std::thread pps([&ppsThread] { ppsThread.run(); });
        std::this_thread::sleep_for(1500ms);
        SPDLOG_INFO("Main: Starting subscriber thread");
        std::thread subscriber([&subscriberThread] { subscriberThread.run(); });
        SPDLOG_INFO("Main: Starting controller thread");
        std::thread controller([] { ControllerThread::get().run(); });
        SPDLOG_INFO("Main: Starting outer loop clock thread");
        std::thread outerLoopClock([&outerLoopClockThread] { outerLoopClockThread.run(); });

        SPDLOG_INFO("Main: Waiting for ExitControl");
        Model::get().waitForExitControl();
        SPDLOG_INFO("Main: ExitControl received");

        SPDLOG_INFO("Main: Stopping pps thread");
        ppsThread.stop();
        SPDLOG_INFO("Main: Stopping subscriber thread");
        subscriberThread.stop();
        SPDLOG_INFO("Main: Stopping controller thread");
        ControllerThread::get().stop();
        SPDLOG_INFO("Main: Stopping outer loop clock thread");
        outerLoopClockThread.stop();
        std::this_thread::sleep_for(100ms);
        SPDLOG_INFO("Main: Joining pps thread");
        pps.join();
        SPDLOG_INFO("Main: Joining subscriber thread");
        subscriber.join();
        SPDLOG_INFO("Main: Joining controller thread");
        controller.join();
        SPDLOG_INFO("Main: Joining outer loop clock thread");
        outerLoopClock.join();
    } catch (std::exception& ex) {
        SPDLOG_CRITICAL("Error starting.stopping or joining threads: {)", ex.what());
        exit(1);
    }
}

int main(int argc, char* const argv[]) {
    const char* configRoot = getenv("PWD");

    processArgs(argc, argv, configRoot);

    SPDLOG_INFO("Main: Creating setting reader, root {}", configRoot);
    SettingReader::get().setRootPath(configRoot);
    SPDLOG_INFO("Main: Initializing M1M3 SAL");
    std::shared_ptr<SAL_MTM1M3> m1m3SAL = std::make_shared<SAL_MTM1M3>();
    m1m3SAL->setDebugLevel(debugLevelSAL);

    sinks.push_back(std::make_shared<SALSink_mt>(m1m3SAL));
    auto logger = std::make_shared<spdlog::async_logger>("loggername", sinks.begin(), sinks.end(),
                                                         spdlog::thread_pool(),
                                                         spdlog::async_overflow_policy::block);
    spdlog::set_default_logger(logger);
    spdlog::level::level_enum logLevel =
            (debugLevel == 0 ? spdlog::level::info
                             : (debugLevel == 1 ? spdlog::level::debug : spdlog::level::trace));
    spdlog::set_level(logLevel);

    SPDLOG_INFO("Main: Initializing MTMount SAL");
    std::shared_ptr<SAL_MTMount> mtMountSAL = std::make_shared<SAL_MTMount>();
    mtMountSAL->setDebugLevel(debugLevelSAL);
    SPDLOG_INFO("Main: Creating publisher");
    M1M3SSPublisher::get().setSAL(m1m3SAL);
    M1M3SSPublisher::get().newLogLevel(logLevel * 10);

    IFPGA* fpga = &IFPGA::get();
    IExpansionFPGA* expansionFPGA = &IExpansionFPGA::get();

    try {
        initializeFPGAs(fpga, expansionFPGA);
        runFPGAs(m1m3SAL, mtMountSAL);

        expansionFPGA->close();

        fpga->close();
        fpga->finalize();
    } catch (NiError& nie) {
        SPDLOG_CRITICAL("Main: Error initializing FPGA: {}", nie.what());
        fpga->finalize();
        mtMountSAL->salShutdown();
        m1m3SAL->salShutdown();
        return -1;
    }
    SPDLOG_INFO("Main: Shutting down MTMount SAL");
    mtMountSAL->salShutdown();

    SPDLOG_INFO("Main: Shutting down M1M3 SAL");
    m1m3SAL->salShutdown();

    SPDLOG_INFO("Main: Shutdown complete");

    return 0;
}
