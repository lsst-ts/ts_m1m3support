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
#include <SAL_MTMount.h>
#include <Timestamp.h>
#include <FPGAAddresses.h>
#include <SafetyController.h>
#include <InterlockController.h>
#include <cstring>
#include <ExpansionFPGA.h>

#include <Gyro.h>
#include <string>
#include <fstream>
#include <boost/tokenizer.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/format.hpp>
#include <stdio.h>
#include <PID.h>

using namespace std;
using namespace LSST::M1M3::SS;

void* runThread(void* data);

int main() {
	void* status;

	cout << "Creating setting reader" << endl;
	SettingReader settingReader = SettingReader("/usr/ts_M1M3Support/SettingFiles/Base/", "/usr/ts_M1M3Support/SettingFiles/Sets/");
	cout << "Initializing M1M3 SAL" << endl;
	SAL_m1m3 m1m3SAL = SAL_m1m3();
	m1m3SAL.setDebugLevel(0);


//	std::string filename = "/usr/ts_M1M3Support/PID1.csv";
//	typedef boost::tokenizer< boost::escaped_list_separator<char> > tokenizer;
//	std::ifstream inputStream(filename.c_str());
//	std::string lineText;
//	int32_t lineNumber = 0;
//	PID pid = PID(1, 0.02, 0.20431, 3.631165, 0, 0);
//	double startTime = m1m3SAL.getCurrentTime();
//	while(std::getline(inputStream, lineText)) {
//		boost::trim_right(lineText);
//		if (lineNumber != 0 && !lineText.empty()) {
//			tokenizer tok(lineText);
//			tokenizer::iterator i = tok.begin();
//			double targetTimestamp = boost::lexical_cast<double>(*i);
//			++i;
//			double targetDemand = boost::lexical_cast<double>(*i);
//			++i;
//			double targetOutput = boost::lexical_cast<double>(*i);
//			++i;
//			double targetError = boost::lexical_cast<double>(*i);
//			++i;
//			double targetControl = boost::lexical_cast<double>(*i);
//			double myControl = pid.process(targetDemand, targetOutput);
//			cout << targetTimestamp << "," << targetDemand << "," << targetOutput << "," << targetError << "," << boost::str(boost::format("%.9f") % targetControl) << "," << boost::str(boost::format("%.9f") % myControl) << endl;
//		}
//		lineNumber++;
//	}
//	double stopTime = m1m3SAL.getCurrentTime();
//	inputStream.close();
//	cout << boost::str(boost::format("%.9f") % (stopTime - startTime)) << endl;

	cout << "Initializing MTMount SAL" << endl;
	SAL_MTMount mtMountSAL = SAL_MTMount();
	cout << "Creating publisher" << endl;
	M1M3SSPublisher publisher = M1M3SSPublisher(&m1m3SAL);
	cout << "Creating fpga" << endl;
	FPGA fpga = FPGA();
	if (fpga.isErrorCode(fpga.initialize())) {
		cout << "Error initializing FPGA" << endl;
		mtMountSAL.salShutdown();
		m1m3SAL.salShutdown();
		return -1;
	}
	if (fpga.isErrorCode(fpga.open())) {
		cout << "Error opening FPGA" << endl;
		fpga.finalize();
		mtMountSAL.salShutdown();
		m1m3SAL.salShutdown();
		return -1;
	}
	cout << "Load expansion FPGA application settings" << endl;
	ExpansionFPGAApplicationSettings* expansionFPGAApplicationSettings = settingReader.loadExpansionFPGAApplicationSettings();
	cout << "Create expansion FPGA" << endl;
	ExpansionFPGA expansionFPGA = ExpansionFPGA(expansionFPGAApplicationSettings);
	if (expansionFPGA.isErrorCode(expansionFPGA.open())) {
		cout << "Error opening expansion FPGA" << endl;
		fpga.close();
		fpga.finalize();
		mtMountSAL.salShutdown();
		m1m3SAL.salShutdown();
		return -1;
	}

//	settingReader.configure("Default");
//	GyroSettings* gyroSettings = settingReader.loadGyroSettings();
//	Gyro gyro = Gyro(gyroSettings, &fpga, &publisher);
//	while(true) {
//		gyro.disableIgnore();
//		usleep(2000);
//		gyro.read();
//		gyro.enableIgnore();
//		gyro.publishGyroData();
//		gyro.publishGyroWarningIfRequired();
//		usleep(100000);
//	}

	cout << "Creating state factory" << endl;
	StaticStateFactory stateFactory = StaticStateFactory(&publisher);
	cout << "Load interlock application settings" << endl;
	InterlockApplicationSettings* interlockApplicationSettings = settingReader.loadInterlockApplicationSettings();
	cout << "Creating interlock controller" << endl;
	InterlockController interlockController = InterlockController(&publisher, &fpga, interlockApplicationSettings);
	cout << "Creating model" << endl;
	Model model = Model(&settingReader, &publisher, &fpga, &expansionFPGA, &interlockController);
	cout << "Creating context" << endl;
	Context context = Context(&stateFactory, &model);
	cout << "Creating command factory" << endl;
	CommandFactory commandFactory = CommandFactory(&publisher, &context);
	cout << "Creating subscriber" << endl;
	M1M3SSSubscriber subscriber = M1M3SSSubscriber(&m1m3SAL, &mtMountSAL, &commandFactory);
	cout << "Creating controller" << endl;
	Controller controller = Controller(&commandFactory);
	cout << "Creating subscriber thread" << endl;
	SubscriberThread subscriberThread = SubscriberThread(&subscriber, &controller, &publisher, &commandFactory);
	cout << "Creating controller thread" << endl;
	ControllerThread controllerThread = ControllerThread(&controller);
	cout << "Creating outer loop clock thread" << endl;
	OuterLoopClockThread outerLoopClockThread = OuterLoopClockThread(&commandFactory, &controller, &fpga, &publisher);
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

	if (expansionFPGA.isErrorCode(expansionFPGA.close())) {
		cout << "Error closing expansion FPGA" << endl;
	}

	if (fpga.isErrorCode(fpga.close())) {
		cout << "Error closing FPGA" << endl;
	}

	if (fpga.isErrorCode(fpga.finalize())) {
		cout << "Error finalizing FPGA" << endl;
	}

	cout << "Shutting down MTMount SAL" << endl;
	mtMountSAL.salShutdown();

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
