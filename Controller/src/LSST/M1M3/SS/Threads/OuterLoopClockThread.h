/*
 * OuterLoopClockThread.h
 *
 *  Created on: Oct 2, 2017
 *      Author: ccontaxis
 */

#ifndef OUTERLOOPCLOCKTHREAD_H_
#define OUTERLOOPCLOCKTHREAD_H_

#include <IThread.h>
#include <pthread.h>

namespace LSST {
namespace M1M3 {
namespace SS {

class ICommandFactory;
class IController;
class IFPGA;

/*!
 * The outer loop clock thread is responsible for controlling
 * the outer loop execution timing.
 */
class OuterLoopClockThread: public IThread {
private:
	ICommandFactory* commandFactory;
	IController* controller;
	IFPGA* fpga;
	bool keepRunning;
	pthread_mutex_t updateMutex;

public:
	OuterLoopClockThread(ICommandFactory* commandFactory, IController* controller, IFPGA* fpga);
	~OuterLoopClockThread();

	void run();
	void stop();
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* OUTERLOOPCLOCKTHREAD_H_ */
