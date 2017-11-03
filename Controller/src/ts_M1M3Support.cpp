//============================================================================
// Name        : ts_M1M3Support.cpp
// Author      : LSST
// Version     :
// Copyright   : LSST
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <pthread.h>
#include <CommandFactory.h>
#include <M1M3SSSubscriber.h>
#include <M1M3SSPublisher.h>
#include <StaticStateFactory.h>
#include <Model.h>
#include <Context.h>
#include <Controller.h>
#include <SubscriberThread.h>
#include <ControllerThread.h>
#include <OuterLoopClockThread.h>
#include <FPGA.h>
#include <CommandTypes.h>
#include <SettingReader.h>
#include <SAL_m1m3.h>
//#include <SAL_MTMount.h>
#include <Timestamp.h>
#include <FPGAAddresses.h>
#include <SafetyController.h>
#include <cstring>

using namespace std;
using namespace LSST::M1M3::SS;

void* runThread(void* data);

int main() {
	void* status;

	cout << "Creating setting reader" << endl;
	SettingReader settingReader = SettingReader("/usr/ts_M1M3Support/SettingFiles/Base/", "/usr/ts_M1M3Support/SettingFiles/Sets/");
	cout << "Initializing M1M3 SAL" << endl;
	SAL_m1m3 m1m3SAL = SAL_m1m3();
	//cout << "Initializing MTMount SAL" << endl;
	//SAL_MTMount mtMountSAL = SAL_MTMount();
	cout << "Creating publisher" << endl;
	M1M3SSPublisher publisher = M1M3SSPublisher(&m1m3SAL);
	cout << "Creating fpga" << endl;
	FPGA fpga = FPGA();
	if (fpga.isErrorCode(fpga.initialize())) {
		cout << "Error initializing FPGA" << endl;
		m1m3SAL.salShutdown();
		//mtMountSAL.salShutdown();
		return -1;
	}
	if (fpga.isErrorCode(fpga.open())) {
		cout << "Error opening FPGA" << endl;
		m1m3SAL.salShutdown();
		//mtMountSAL.salShutdown();
		return -1;
	}
	cout << "Creating state factory" << endl;
	StaticStateFactory stateFactory = StaticStateFactory();
	cout << "Creating model" << endl;
	Model model = Model(&settingReader, &publisher, &fpga);
	cout << "Creating context" << endl;
	Context context = Context(&stateFactory, &model);
	cout << "Creating command factory" << endl;
	CommandFactory commandFactory = CommandFactory(&publisher, &context);
	cout << "Creating subscriber" << endl;
	M1M3SSSubscriber subscriber = M1M3SSSubscriber(&m1m3SAL, 0/*&mtMountSAL*/, &commandFactory);
	cout << "Creating controller" << endl;
	Controller controller = Controller(&commandFactory);
	cout << "Creating subscriber thread" << endl;
	SubscriberThread subscriberThread = SubscriberThread(&subscriber, &controller, &publisher, &commandFactory);
	cout << "Creating controller thread" << endl;
	ControllerThread controllerThread = ControllerThread(&controller);
	cout << "Creating outer loop clock thread" << endl;
	OuterLoopClockThread outerLoopClockThread = OuterLoopClockThread(&commandFactory, &controller, &fpga);
	cout << "Queuing boot command" << endl;
	controller.enqueue(commandFactory.create(Commands::BootCommand));

	pthread_t subscriberThreadId;
	pthread_t controllerThreadId;
	pthread_t outerLoopClockThreadId;
	pthread_attr_t threadAttribute;
	pthread_attr_init(&threadAttribute);
	pthread_attr_setdetachstate(&threadAttribute, PTHREAD_CREATE_JOINABLE);

	cout << "Starting subscriber thread" << endl;
	int rc = pthread_create(&subscriberThreadId, &threadAttribute, runThread, (void*)(&subscriberThread));
	if (!rc) {
		cout << "Starting controller thread" << endl;
		rc = pthread_create(&controllerThreadId, &threadAttribute, runThread, (void*)(&controllerThread));
		if (!rc) {
			cout << "Starting outer loop clock thread" << endl;
			rc = pthread_create(&outerLoopClockThreadId, &threadAttribute, runThread, (void*)(&outerLoopClockThread));
			if (!rc) {
				cout << "Waiting for shutdown" << endl;
				model.waitForShutdown();
				cout << "Shutdown received" << endl;
				cout << "Stopping subscriber thread" << endl;
				subscriberThread.stop();
				cout << "Stopping controller thread" << endl;
				controllerThread.stop();
				cout << "Stopping outer loop clock thread" << endl;
				outerLoopClockThread.stop();
				usleep(100000);
				controller.clear();
				cout << "Joining subscriber thread" << endl;
				pthread_join(subscriberThreadId, &status);
				cout << "Joining controller thread" << endl;
				pthread_join(controllerThreadId, &status);
				cout << "Joining outer loop clock thread" << endl;
				pthread_join(outerLoopClockThreadId, &status);
			}
			else {
				cout << "Failed to start outer loop clock thread" << endl;
				cout << "Stopping subscriber thread" << endl;
				subscriberThread.stop();
				cout << "Stopping controller thread" << endl;
				controllerThread.stop();
				cout << "Joining subscriber thread" << endl;
				pthread_join(subscriberThreadId, &status);
				cout << "Joining controller thread" << endl;
				pthread_join(controllerThreadId, &status);
			}
		}
		else {
			cout << "Failed to start controller thread" << endl;
			cout << "Stopping subscriber thread" << endl;
			subscriberThread.stop();
			cout << "Joining subscriber thread" << endl;
			pthread_join(subscriberThreadId, &status);
		}
	}
	else {
		cout << "Failed to start subscriber thread" << endl;
	}

	pthread_attr_destroy(&threadAttribute);

	if (fpga.isErrorCode(fpga.close())) {
		cout << "Error closing fpga" << endl;
	}

	if (fpga.isErrorCode(fpga.finalize())) {
		cout << "Error finalizing fpga" << endl;
	}

	//cout << "Shutting down MTMount SAL" << endl;
	//mtMountSAL.salShutdown();

	cout << "Shutting down M1M3 SAL" << endl;
	m1m3SAL.salShutdown();

	cout << "Shutdown complete" << endl;

	return 0;
}

void* runThread(void* data) {
	cout << "Thread started" << endl;
	IThread* thread = (IThread*)data;
	thread->run();
	cout << "Thread completed" << endl;
	return 0;
}
