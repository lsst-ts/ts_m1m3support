#include <CommandFactory.h>
#include <CommandTypes.h>
#include <Context.h>
#include <Controller.h>
#include <ControllerThread.h>
#include <DigitalInputOutput.h>
#include <IExpansionFPGA.h>
#include <FPGAAddresses.h>
#include <M1M3SSPublisher.h>
#include <M1M3SSSubscriber.h>
#include <Model.h>
#include <OuterLoopClockThread.h>
#include <PPSThread.h>
#include <SAL_MTM1M3.h>
#include <SAL_MTMount.h>
#include <SafetyController.h>
#include <SettingReader.h>
#include <StaticStateFactory.h>
#include <SubscriberThread.h>
#include <Timestamp.h>
#include <pthread.h>
#include <cstring>
#include <iostream>

#ifdef SIMULATOR
#include <SimulatedFPGA.h>
#else
#include <FPGA.h>
#endif

#include <Log.h>

using namespace std;
using namespace LSST::M1M3::SS;

void* runThread(void* data);

LSST::M1M3::SS::Logger Log;

int main() {
    Log.SetLevel(Levels::Debug);
    Log.Info("Main: Creating setting reader");
    SettingReader::get().setRootPath("/home/petr/ts_m1m3support/SettingFiles/");
    Log.Info("Main: Initializing M1M3 SAL");
    SAL_MTM1M3 m1m3SAL = SAL_MTM1M3();
    m1m3SAL.setDebugLevel(0);
    Log.Info("Main: Initializing MTMount SAL");
    SAL_MTMount mtMountSAL = SAL_MTMount();
    Log.Info("Main: Creating publisher");
    M1M3SSPublisher publisher = M1M3SSPublisher(&m1m3SAL);
    Log.Info("Main: Creating fpga");

    IFPGA* fpga = &IFPGA::get();
#ifdef SIMULATOR
    ((SimulatedFPGA*)fpga)->setPublisher(&publisher);
    ((SimulatedFPGA*)fpga)
            ->setForceActuatorApplicationSettings(SettingReader::get().loadForceActuatorApplicationSettings());
#endif

    if (fpga->isErrorCode(fpga->initialize())) {
        Log.Fatal("Main: Error initializing FPGA");
        mtMountSAL.salShutdown();
        m1m3SAL.salShutdown();
        return -1;
    }
    if (fpga->isErrorCode(fpga->open())) {
        Log.Fatal("Main: Error opening FPGA");
        fpga->finalize();
        mtMountSAL.salShutdown();
        m1m3SAL.salShutdown();
        return -1;
    }
    Log.Info("Main: Load expansion FPGA application settings");
    ExpansionFPGAApplicationSettings* expansionFPGAApplicationSettings =
            SettingReader::get().loadExpansionFPGAApplicationSettings();
    Log.Info("Main: Create expansion FPGA");
    IExpansionFPGA* expansionFPGA = &IExpansionFPGA::get();
    expansionFPGA->setExpansionFPGAApplicationSettings(expansionFPGAApplicationSettings);
    if (expansionFPGA->isErrorCode(expansionFPGA->open())) {
        Log.Fatal("Main: Error opening expansion FPGA");
        fpga->close();
        fpga->finalize();
        mtMountSAL.salShutdown();
        m1m3SAL.salShutdown();
        return -1;
    }
    Log.Info("Main: Creating state factory");
    StaticStateFactory stateFactory = StaticStateFactory(&publisher);
    Log.Info("Main: Load interlock application settings");
    InterlockApplicationSettings* interlockApplicationSettings =
            SettingReader::get().loadInterlockApplicationSettings();
    Log.Info("Main: Creating digital input output");
    DigitalInputOutput digitalInputOutput = DigitalInputOutput(interlockApplicationSettings, &publisher);
    Log.Info("Main: Creating model");
    Model model = Model(&publisher, &digitalInputOutput);
    Log.Info("Main: Creating context");
    Context context = Context(&stateFactory, &model);
    Log.Info("Main: Creating command factory");
    CommandFactory commandFactory = CommandFactory(&publisher, &context);
    Log.Info("Main: Creating subscriber");
    M1M3SSSubscriber subscriber = M1M3SSSubscriber(&m1m3SAL, &mtMountSAL, &commandFactory);
    Log.Info("Main: Creating controller");
    Controller controller = Controller(&commandFactory);
    Log.Info("Main: Creating subscriber thread");
    SubscriberThread subscriberThread =
            SubscriberThread(&subscriber, &controller, &publisher, &commandFactory);
    Log.Info("Main: Creating controller thread");
    ControllerThread controllerThread = ControllerThread(&controller);
    Log.Info("Main: Creating outer loop clock thread");
    OuterLoopClockThread outerLoopClockThread =
            OuterLoopClockThread(&commandFactory, &controller, &publisher);
    Log.Info("Main: Creating pps thread");
    PPSThread ppsThread = PPSThread(&publisher);
    Log.Info("Main: Queuing boot command");
    controller.enqueue(commandFactory.create(Commands::BootCommand));

    pthread_t subscriberThreadId;
    pthread_t controllerThreadId;
    pthread_t outerLoopClockThreadId;
    pthread_t ppsThreadId;
    pthread_attr_t threadAttribute;
    pthread_attr_init(&threadAttribute);
    pthread_attr_setdetachstate(&threadAttribute, PTHREAD_CREATE_JOINABLE);

    void* status;
    Log.Info("Main: Starting pps thread");
    int rc = pthread_create(&ppsThreadId, &threadAttribute, runThread, (void*)(&ppsThread));
    usleep(1500000);
    if (!rc) {
        Log.Info("Main: Starting subscriber thread");
        int rc = pthread_create(&subscriberThreadId, &threadAttribute, runThread, (void*)(&subscriberThread));
        if (!rc) {
            Log.Info("Main: Starting controller thread");
            rc = pthread_create(&controllerThreadId, &threadAttribute, runThread, (void*)(&controllerThread));
            if (!rc) {
                Log.Info("Main: Starting outer loop clock thread");
                rc = pthread_create(&outerLoopClockThreadId, &threadAttribute, runThread,
                                    (void*)(&outerLoopClockThread));
                if (!rc) {
                    Log.Info("Main: Waiting for shutdown");
                    model.waitForShutdown();
                    Log.Info("Main: Shutdown received");
                    Log.Info("Main: Stopping pps thread");
                    ppsThread.stop();
                    Log.Info("Main: Stopping subscriber thread");
                    subscriberThread.stop();
                    Log.Info("Main: Stopping controller thread");
                    controllerThread.stop();
                    Log.Info("Main: Stopping outer loop clock thread");
                    outerLoopClockThread.stop();
                    usleep(100000);
                    controller.clear();
                    Log.Info("Main: Joining pps thread");
                    pthread_join(ppsThreadId, &status);
                    Log.Info("Main: Joining subscriber thread");
                    pthread_join(subscriberThreadId, &status);
                    Log.Info("Main: Joining controller thread");
                    pthread_join(controllerThreadId, &status);
                    Log.Info("Main: Joining outer loop clock thread");
                    pthread_join(outerLoopClockThreadId, &status);
                } else {
                    Log.Fatal("Main: Failed to start outer loop clock thread");
                    Log.Info("Main: Stopping pps thread");
                    ppsThread.stop();
                    Log.Info("Main: Stopping subscriber thread");
                    subscriberThread.stop();
                    Log.Info("Main: Stopping controller thread");
                    controllerThread.stop();
                    Log.Info("Main: Joining pps thread");
                    pthread_join(ppsThreadId, &status);
                    Log.Info("Main: Joining subscriber thread");
                    pthread_join(subscriberThreadId, &status);
                    Log.Info("Main: Joining controller thread");
                    pthread_join(controllerThreadId, &status);
                }
            } else {
                Log.Fatal("Main: Failed to start controller thread");
                Log.Info("Main: Stopping pps thread");
                ppsThread.stop();
                Log.Info("Main: Stopping subscriber thread");
                subscriberThread.stop();
                Log.Info("Main: Joining pps thread");
                pthread_join(ppsThreadId, &status);
                Log.Info("Main: Joining subscriber thread");
                pthread_join(subscriberThreadId, &status);
            }
        } else {
            Log.Fatal("Main: Failed to start subscriber thread");
            Log.Info("Main: Stopping pps thread");
            ppsThread.stop();
            Log.Info("Main: Joining pps thread");
            pthread_join(ppsThreadId, &status);
        }
    } else {
        Log.Fatal("Main: Failed to start pps thread");
    }

    pthread_attr_destroy(&threadAttribute);

    if (expansionFPGA->isErrorCode(expansionFPGA->close())) {
        Log.Error("Main: Error closing expansion FPGA");
    }

    if (fpga->isErrorCode(fpga->close())) {
        Log.Error("Main: Error closing FPGA");
    }

    if (fpga->isErrorCode(fpga->finalize())) {
        Log.Error("Main: Error finalizing FPGA");
    }

    Log.Info("Main: Shutting down MTMount SAL");
    mtMountSAL.salShutdown();

    Log.Info("Main: Shutting down M1M3 SAL");
    m1m3SAL.salShutdown();

    Log.Info("Main: Shutdown complete");

    return 0;
}

void* runThread(void* data) {
    IThread* thread = (IThread*)data;
    thread->run();
    return 0;
}
