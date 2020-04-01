#include <CommandFactory.h>
#include <Context.h>
#include <Controller.h>
#include <ControllerThread.h>
#include <DigitalInputOutput.h>
#include <IExpansionFPGA.h>
#include <M1M3SSPublisher.h>
#include <M1M3SSSubscriber.h>
#include <Model.h>
#include <OuterLoopClockThread.h>
#include <PPSThread.h>
#include <SAL_MTM1M3.h>
#include <SAL_MTMount.h>
#include <SettingReader.h>
#include <StaticStateFactory.h>
#include <SubscriberThread.h>

#include <pthread.h>
#include <getopt.h>
#include <cstring>
#include <iostream>

#include <spdlog/spdlog.h>
#include <spdlog/async.h>
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/sinks/daily_file_sink.h"

#ifdef SIMULATOR
#include <SimulatedFPGA.h>
#else
#include <FPGA.h>
#endif

using namespace std;
using namespace LSST::M1M3::SS;

void* runThread(void* data);

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
              << "  -h prints this help" << std::endl;
}

int main(int argc, char* const argv[]) {
    int opt;
    int enabledSinks = 0x3;
    int debugLevel = 0;
    const char* configRoot = getenv("PWD");
    while ((opt = getopt(argc, argv, "bc:dfh")) != -1) {
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
            default:
                std::cerr << "Unknow option " << opt << std::endl;
                printHelp();
                exit(EXIT_FAILURE);
        }
    }

    spdlog::init_thread_pool(8192, 1);
    std::vector<spdlog::sink_ptr> sinks;
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

    spdlog::info("Main: Creating setting reader");
    SettingReader::get().setRootPath(configRoot);
    spdlog::info("Main: Initializing M1M3 SAL");
    SAL_MTM1M3 m1m3SAL = SAL_MTM1M3();
    m1m3SAL.setDebugLevel(0);
    spdlog::info("Main: Initializing MTMount SAL");
    SAL_MTMount mtMountSAL = SAL_MTMount();
    spdlog::info("Main: Creating publisher");
    M1M3SSPublisher publisher = M1M3SSPublisher(&m1m3SAL);
    spdlog::info("Main: Creating fpga");

    IFPGA* fpga = &IFPGA::get();
#ifdef SIMULATOR
    ((SimulatedFPGA*)fpga)->setPublisher(&publisher);
    ((SimulatedFPGA*)fpga)
            ->setForceActuatorApplicationSettings(
                    SettingReader::get().loadForceActuatorApplicationSettings());
    spdlog::warn("Starting Simulator version!");
#endif

    if (fpga->isErrorCode(fpga->initialize())) {
        spdlog::critical("Main: Error initializing FPGA");
        mtMountSAL.salShutdown();
        m1m3SAL.salShutdown();
        return -1;
    }
    if (fpga->isErrorCode(fpga->open())) {
        spdlog::critical("Main: Error opening FPGA");
        fpga->finalize();
        mtMountSAL.salShutdown();
        m1m3SAL.salShutdown();
        return -1;
    }
    spdlog::info("Main: Load expansion FPGA application settings");
    ExpansionFPGAApplicationSettings* expansionFPGAApplicationSettings =
            SettingReader::get().loadExpansionFPGAApplicationSettings();
    spdlog::info("Main: Create expansion FPGA");
    IExpansionFPGA* expansionFPGA = &IExpansionFPGA::get();
    expansionFPGA->setExpansionFPGAApplicationSettings(expansionFPGAApplicationSettings);
    if (expansionFPGA->isErrorCode(expansionFPGA->open())) {
        spdlog::critical("Main: Error opening expansion FPGA");
        fpga->close();
        fpga->finalize();
        mtMountSAL.salShutdown();
        m1m3SAL.salShutdown();
        return -1;
    }
    spdlog::info("Main: Creating state factory");
    StaticStateFactory stateFactory = StaticStateFactory(&publisher);
    spdlog::info("Main: Load interlock application settings");
    InterlockApplicationSettings* interlockApplicationSettings =
            SettingReader::get().loadInterlockApplicationSettings();
    spdlog::info("Main: Creating digital input output");
    DigitalInputOutput digitalInputOutput = DigitalInputOutput(interlockApplicationSettings, &publisher);
    spdlog::info("Main: Creating model");
    Model model = Model(&publisher, &digitalInputOutput);
    spdlog::info("Main: Creating context");
    Context context = Context(&stateFactory, &model);
    spdlog::info("Main: Creating command factory");
    CommandFactory commandFactory = CommandFactory(&publisher, &context);
    spdlog::info("Main: Creating subscriber");
    M1M3SSSubscriber subscriber = M1M3SSSubscriber(&m1m3SAL, &mtMountSAL, &commandFactory);
    spdlog::info("Main: Creating controller");
    Controller controller = Controller(&commandFactory);
    spdlog::info("Main: Creating subscriber thread");
    SubscriberThread subscriberThread =
            SubscriberThread(&subscriber, &controller, &publisher, &commandFactory);
    spdlog::info("Main: Creating controller thread");
    ControllerThread controllerThread = ControllerThread(&controller);
    spdlog::info("Main: Creating outer loop clock thread");
    OuterLoopClockThread outerLoopClockThread =
            OuterLoopClockThread(&commandFactory, &controller, &publisher);
    spdlog::info("Main: Creating pps thread");
    PPSThread ppsThread = PPSThread(&publisher);
    spdlog::info("Main: Queuing boot command");
    controller.enqueue(commandFactory.create(Commands::BootCommand));

    pthread_t subscriberThreadId;
    pthread_t controllerThreadId;
    pthread_t outerLoopClockThreadId;
    pthread_t ppsThreadId;
    pthread_attr_t threadAttribute;
    pthread_attr_init(&threadAttribute);
    pthread_attr_setdetachstate(&threadAttribute, PTHREAD_CREATE_JOINABLE);

    void* status;
    spdlog::info("Main: Starting pps thread");
    int rc = pthread_create(&ppsThreadId, &threadAttribute, runThread, (void*)(&ppsThread));
    usleep(1500000);
    if (!rc) {
        spdlog::info("Main: Starting subscriber thread");
        int rc = pthread_create(&subscriberThreadId, &threadAttribute, runThread, (void*)(&subscriberThread));
        if (!rc) {
            spdlog::info("Main: Starting controller thread");
            rc = pthread_create(&controllerThreadId, &threadAttribute, runThread, (void*)(&controllerThread));
            if (!rc) {
                spdlog::info("Main: Starting outer loop clock thread");
                rc = pthread_create(&outerLoopClockThreadId, &threadAttribute, runThread,
                                    (void*)(&outerLoopClockThread));
                if (!rc) {
                    spdlog::info("Main: Waiting for shutdown");
                    model.waitForShutdown();
                    spdlog::info("Main: Shutdown received");
                    spdlog::info("Main: Stopping pps thread");
                    ppsThread.stop();
                    spdlog::info("Main: Stopping subscriber thread");
                    subscriberThread.stop();
                    spdlog::info("Main: Stopping controller thread");
                    controllerThread.stop();
                    spdlog::info("Main: Stopping outer loop clock thread");
                    outerLoopClockThread.stop();
                    usleep(100000);
                    controller.clear();
                    spdlog::info("Main: Joining pps thread");
                    pthread_join(ppsThreadId, &status);
                    spdlog::info("Main: Joining subscriber thread");
                    pthread_join(subscriberThreadId, &status);
                    spdlog::info("Main: Joining controller thread");
                    pthread_join(controllerThreadId, &status);
                    spdlog::info("Main: Joining outer loop clock thread");
                    pthread_join(outerLoopClockThreadId, &status);
                } else {
                    spdlog::critical("Main: Failed to start outer loop clock thread");
                    spdlog::info("Main: Stopping pps thread");
                    ppsThread.stop();
                    spdlog::info("Main: Stopping subscriber thread");
                    subscriberThread.stop();
                    spdlog::info("Main: Stopping controller thread");
                    controllerThread.stop();
                    spdlog::info("Main: Joining pps thread");
                    pthread_join(ppsThreadId, &status);
                    spdlog::info("Main: Joining subscriber thread");
                    pthread_join(subscriberThreadId, &status);
                    spdlog::info("Main: Joining controller thread");
                    pthread_join(controllerThreadId, &status);
                }
            } else {
                spdlog::critical("Main: Failed to start controller thread");
                spdlog::info("Main: Stopping pps thread");
                ppsThread.stop();
                spdlog::info("Main: Stopping subscriber thread");
                subscriberThread.stop();
                spdlog::info("Main: Joining pps thread");
                pthread_join(ppsThreadId, &status);
                spdlog::info("Main: Joining subscriber thread");
                pthread_join(subscriberThreadId, &status);
            }
        } else {
            spdlog::critical("Main: Failed to start subscriber thread");
            spdlog::info("Main: Stopping pps thread");
            ppsThread.stop();
            spdlog::info("Main: Joining pps thread");
            pthread_join(ppsThreadId, &status);
        }
    } else {
        spdlog::critical("Main: Failed to start pps thread");
    }

    pthread_attr_destroy(&threadAttribute);

    if (expansionFPGA->isErrorCode(expansionFPGA->close())) {
        spdlog::error("Main: Error closing expansion FPGA");
    }

    if (fpga->isErrorCode(fpga->close())) {
        spdlog::error("Main: Error closing FPGA");
    }

    if (fpga->isErrorCode(fpga->finalize())) {
        spdlog::error("Main: Error finalizing FPGA");
    }

    spdlog::info("Main: Shutting down MTMount SAL");
    mtMountSAL.salShutdown();

    spdlog::info("Main: Shutting down M1M3 SAL");
    m1m3SAL.salShutdown();

    spdlog::info("Main: Shutdown complete");

    return 0;
}

void* runThread(void* data) {
    IThread* thread = (IThread*)data;
    thread->run();
    return 0;
}
