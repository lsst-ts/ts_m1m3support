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
#include <cstring>
#include <getopt.h>
#include <grp.h>
#include <iostream>
#include <pwd.h>
#include <signal.h>
#include <fcntl.h>
#include <chrono>
#include <thread>

#include <spdlog/spdlog.h>

#include <spdlog/async.h>
#include <spdlog/sinks/daily_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/syslog_sink.h>

#include <SAL_MTM1M3.h>
#include <SAL_MTMount.h>

#include <SALSink.h>

#include <Context.h>
#include <ControllerThread.h>
#include <EnterControlCommand.h>
#include <ExitControlCommand.h>
#include <IExpansionFPGA.h>
#include <M1M3SSPublisher.h>
#include <M1M3SSSubscriber.h>
#include <Model.h>
#include <OuterLoopClockThread.h>
#include <PPSThread.h>
#include <RawDCAccelerometersCommands.h>
#include <ReloadConfigurationCommand.h>
#include <SettingReader.h>
#include <SubscriberThread.h>

#ifdef SIMULATOR
#include <SimulatedFPGA.h>
#else
#include <FPGA.h>
#endif

using namespace std::chrono;
using namespace LSST::M1M3::SS;

extern const char *VERSION;

void printHelp() {
    std::cout << "M1M3 Static Support controller. Runs either as simulator or as "
                 "simulator or as "
                 "the real thing on cRIO."
              << std::endl
              << "Version: " << VERSION
#ifdef WITH_SAL_KAFKA
              << "with Kafka middleware" << std::endl
#else
              << "with OpenSplice/DDS middleware" << std::endl
#endif
              << "Options:" << std::endl
              << "  -b runs on background, don't log to stdout" << std::endl
              << "  -c <configuration path> use given configuration directory "
                 "(should be SettingFiles)"
              << std::endl
              << "  -d increases debugging (can be specified multiple times, "
                 "default is info)"
              << std::endl
              << "  -f runs on foreground, don't log to file" << std::endl
              << "  -h prints this help" << std::endl
              << "  -p PID file, started as daemon on background" << std::endl
              << "  -s increases SAL debugging (can be specified multiple times, "
                 "default is 0)"
              << std::endl
              << "  -S don't transmit log messages on SAL/DDS" << std::endl
              << "  -u <user>:<group> run under user & group" << std::endl
              << "  -v prints version and exits" << std::endl
              << "  -V prints SAL, XML and OSPL versions and exits" << std::endl;
}

int debugLevel = 0;
int debugLevelSAL = 0;

const char *pidFile = NULL;
std::string daemonUser("m1m3");
std::string daemonGroup("m1m3");

void sigKill(int signal) {
    SPDLOG_DEBUG("Kill/int signal received");
    ControllerThread::get().enqueue(new ExitControlCommand(-1));
}

void sigUsr1(int signal) { ControllerThread::get().enqueue(new ReloadConfigurationCommand()); }

bool dcAccelerometersRaw = false;

void sigUsr2(int signal) {
    dcAccelerometersRaw = !dcAccelerometersRaw;
    if (dcAccelerometersRaw) {
        ControllerThread::get().enqueue(new RecordRawDCAccelerometersCommand());
    } else {
        ControllerThread::get().enqueue(new StopRawDCAccelerometersCommand());
    }
}

std::vector<spdlog::sink_ptr> sinks;
int enabledSinks = 0x10;

void processArgs(int argc, char *const argv[], const char *&configRoot) {
    int opt;
    while ((opt = getopt(argc, argv, "bc:dfhp:sSu:vV")) != -1) {
        switch (opt) {
            case 'b':
                enabledSinks |= 0x02;
                break;
            case 'c':
                configRoot = optarg;
                break;
            case 'd':
                debugLevel++;
                break;
            case 'f':
                enabledSinks |= 0x01;
                break;
            case 'h':
                printHelp();
                exit(EXIT_SUCCESS);
            case 'p':
                pidFile = optarg;
                enabledSinks |= 0x14;
                break;
            case 's':
                debugLevelSAL++;
                break;
            case 'S':
                enabledSinks &= ~0x10;
                break;
            case 'u': {
                char *sep = strchr(optarg, ':');
                if (sep) {
                    *sep = '\0';
                    daemonUser = optarg;
                    daemonGroup = sep + 1;
                } else {
                    daemonGroup = daemonUser = optarg;
                }
                break;
            }
            case 'v':
                std::cout << VERSION << std::endl;
                exit(EXIT_SUCCESS);
            case 'V':
                std::cout << "SAL " << SAL_MTM1M3::getSALVersion() << " " << SAL_MTMount::getSALVersion()
                          << std::endl
                          << "XML " << SAL_MTM1M3::getXMLVersion() << " " << SAL_MTMount::getXMLVersion()
                          << std::endl
                          << "OSPL " << SAL_MTM1M3::getOSPLVersion() << " " << SAL_MTMount::getOSPLVersion()
                          << std::endl;
                exit(EXIT_SUCCESS);
            default:
                std::cerr << "Unknow option " << opt << std::endl;
                printHelp();
                exit(EXIT_FAILURE);
        }
    }
}

void initializeFPGA(IFPGA *fpga) {
#ifdef WITH_SAL_KAFKA
    const char *kafkaOSPL = "with Kafka middleware";
#else
    const char *kafkaOSPL = "with OpenSplice/DDS middleware";
#endif

#ifdef SIMULATOR
    SPDLOG_WARN("Starting Simulator version! Version {} {}", VERSION, kafkaOSPL);
#else
    SPDLOG_INFO("Starting cRIO/real HW version. Version {} {}", VERSION, kafkaOSPL);
#endif

    SPDLOG_INFO("Main: Creating fpga");

    fpga->initialize();
    fpga->open();
}

void runFPGAs(std::shared_ptr<SAL_MTM1M3> m1m3SAL, std::shared_ptr<SAL_MTMount> mtMountSAL, int retPipe) {
    SPDLOG_INFO("Main: Creating subscriber");
    M1M3SSSubscriber::get().setSAL(m1m3SAL, mtMountSAL);
    SPDLOG_INFO("Main: Creating subscriber thread");
    SubscriberThread subscriberThread;
    SPDLOG_INFO("Main: Creating outer loop clock thread");
    OuterLoopClockThread outerLoopClockThread;
    SPDLOG_INFO("Main: Creating pps thread");
    PPSThread ppsThread;
    SPDLOG_INFO("Main: Queuing EnterControl command");
    ControllerThread::get().enqueue(new EnterControlCommand());

    signal(SIGKILL, sigKill);
    signal(SIGINT, sigKill);
    signal(SIGTERM, sigKill);

    signal(SIGUSR1, sigUsr1);
    signal(SIGUSR2, sigUsr2);

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

        if (retPipe >= 0) {
            write(retPipe, "OK", 2);
            close(retPipe);
        }

        Model::instance().waitForExitControl();
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
    } catch (std::exception &ex) {
        if (retPipe >= 0) {
            write(retPipe, ex.what(), strlen(ex.what()));
            close(retPipe);
        }
        SPDLOG_CRITICAL("Error starting.stopping or joining threads: {)", ex.what());
    }
}

spdlog::level::level_enum getSpdLogLogLevel() {
    return debugLevel == 0 ? spdlog::level::info
                           : (debugLevel == 1 ? spdlog::level::debug : spdlog::level::trace);
}

void setSinks() {
    auto logger = std::make_shared<spdlog::async_logger>("M1M3support", sinks.begin(), sinks.end(),
                                                         spdlog::thread_pool(),
                                                         spdlog::async_overflow_policy::block);
    spdlog::set_default_logger(logger);
    spdlog::set_level(getSpdLogLogLevel());
}

void startLog() {
    spdlog::init_thread_pool(8192, 1);
    if (enabledSinks & 0x01) {
        auto stdout_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
        sinks.push_back(stdout_sink);
    }
    if (enabledSinks & 0x02) {
        auto daily_sink = std::make_shared<spdlog::sinks::daily_file_sink_mt>("M1M3support", 0, 0);
        sinks.push_back(daily_sink);
    }
    if (enabledSinks & 0x04) {
        auto syslog_sink = std::make_shared<spdlog::sinks::syslog_sink_mt>("M1M3support", LOG_PID | LOG_CONS,
                                                                           LOG_USER, false);
        sinks.push_back(syslog_sink);
    }

    setSinks();
}

int main(int argc, char *const argv[]) {
    const char *configRoot = "/var/lib/M1M3support";

    processArgs(argc, argv, configRoot);

    int startPipe[2] = {-1, -1};

    if (pidFile) {
        struct passwd *runAs = getpwnam(daemonUser.c_str());
        if (runAs == NULL) {
            std::cerr << "Error: Cannot find user " << daemonUser << std::endl;
            exit(EXIT_FAILURE);
        }
        struct group *runGroup = getgrnam(daemonGroup.c_str());
        if (runGroup == NULL) {
            std::cerr << "Error: Cannot find group " << daemonGroup << std::endl;
            exit(EXIT_FAILURE);
        }

        if (pipe(startPipe) == -1) {
            std::cerr << "Error: Cannot create pipe for child/start process: " << strerror(errno)
                      << std::endl;
            exit(EXIT_FAILURE);
        }

        pid_t child = fork();
        if (child < 0) {
            std::cerr << "Error: Cannot fork:" << strerror(errno) << std::endl;
            exit(EXIT_FAILURE);
        }
        if (child > 0) {
            close(startPipe[1]);
            std::ofstream pidf(pidFile, std::ofstream::out);
            pidf << child;
            pidf.close();
            if (pidf.fail()) {
                std::cerr << "Error: Cannot write to PID file " << pidFile << ": " << strerror(errno)
                          << std::endl;
                exit(EXIT_FAILURE);
            }
            if (chown(pidFile, runAs->pw_uid, runGroup->gr_gid)) {
                std::cerr << "Error: Cannot change owner of " << pidFile << ":" << strerror(errno)
                          << std::endl;
                exit(EXIT_FAILURE);
            }
            char retbuf[2000];
            memset(retbuf, 0, sizeof(retbuf));
            signal(SIGALRM, [](int) {
                std::cerr << "Error: Start timeouted, see syslog for details." << std::endl;
                exit(EXIT_FAILURE);
            });
            alarm(30);
            read(startPipe[0], retbuf, 2000);
            if (strcmp(retbuf, "OK") == 0) {
                return EXIT_SUCCESS;
            }
            std::cerr << retbuf << std::endl;
            return EXIT_FAILURE;
        }
        close(startPipe[0]);
        startLog();
        setuid(runAs->pw_uid);
        setgid(runGroup->gr_gid);
        SPDLOG_DEBUG("Running as {}:{}", daemonUser, daemonGroup);
        if (!(enabledSinks & 0x01)) {
            close(0);
            close(1);
            close(2);
            int nf = open("/dev/null", O_RDWR);
            dup(nf);
            dup(nf);
            dup(nf);
        }
    } else {
        startLog();
    }

    SettingReader::instance().setRootPath(configRoot);
    SPDLOG_INFO("Main: Initializing M1M3 SAL");
    std::shared_ptr<SAL_MTM1M3> m1m3SAL = std::make_shared<SAL_MTM1M3>();
    m1m3SAL->setDebugLevel(debugLevelSAL);

    if (enabledSinks & 0x10) {
        sinks.push_back(std::make_shared<SALSink_mt>(m1m3SAL));
        setSinks();
        SPDLOG_INFO("Enabled SAL logger");
    }

    if (SAL_MTM1M3::getSALVersion() != SAL_MTMount::getSALVersion()) {
        SPDLOG_WARN("SAL version mismatch: MTM1M3 {} MTMount {}", SAL_MTM1M3::getSALVersion(),
                    SAL_MTMount::getSALVersion());
    }
    if (SAL_MTM1M3::getXMLVersion() != SAL_MTMount::getXMLVersion()) {
        SPDLOG_WARN("XML mismatch: MTM1M3 {} MTMount {}", SAL_MTM1M3::getXMLVersion(),
                    SAL_MTMount::getXMLVersion());
    }
    if (SAL_MTM1M3::getOSPLVersion() != SAL_MTMount::getOSPLVersion()) {
        SPDLOG_WARN("OSPL version mismatch: MTM1M3 {} MTMount {}", SAL_MTM1M3::getOSPLVersion(),
                    SAL_MTMount::getOSPLVersion());
    }

    SPDLOG_INFO("Main: Initializing MTMount SAL");
    std::shared_ptr<SAL_MTMount> mtMountSAL = std::make_shared<SAL_MTMount>();
    mtMountSAL->setDebugLevel(debugLevelSAL);
    SPDLOG_INFO("Main: Creating publisher");
    M1M3SSPublisher::instance().setSAL(m1m3SAL);
    M1M3SSPublisher::instance().newLogLevel(getSpdLogLogLevel() * 10);

    IFPGA *fpga = &IFPGA::get();
    IExpansionFPGA *expansionFPGA = &IExpansionFPGA::get();

    try {
        initializeFPGA(fpga);

        spdlog::apply_all([&](std::shared_ptr<spdlog::logger> l) { l->flush(); });

        runFPGAs(m1m3SAL, mtMountSAL, startPipe[1]);

        expansionFPGA->close();
        fpga->close();
        fpga->finalize();
    } catch (std::runtime_error &nie) {
        if (startPipe[1] >= 0) {
            write(startPipe[1], nie.what(), strlen(nie.what()));
            close(startPipe[1]);
        }
        SPDLOG_CRITICAL("Main: Error initializing FPGA: {}", nie.what());
        expansionFPGA->close();
        fpga->close();
        fpga->finalize();
        expansionFPGA->close();
        mtMountSAL->salShutdown();
        m1m3SAL->salShutdown();
        return -1;
    }
    SPDLOG_INFO("Main: Shutting down MTMount SAL");
    mtMountSAL->salShutdown();

    SPDLOG_INFO("Main: Shutting down M1M3 SAL");
    if (enabledSinks & 0x10) {
        sinks.pop_back();
        setSinks();
        std::this_thread::sleep_for(1000us);
    }
    m1m3SAL->salShutdown();

    std::this_thread::sleep_for(2s);

    SPDLOG_INFO("Main: Shutdown complete");

    return 0;
}
