/*
 * ControllerThread.h
 *
 *  Created on: Sep 28, 2017
 *      Author: ccontaxis
 */

#ifndef CONTROLLERTHREAD_H_
#define CONTROLLERTHREAD_H_

#include <IThread.h>

namespace LSST {
namespace M1M3 {
namespace SS {

class IController;

/*!
 * The controller thread is responsible for executing commands.
 */
class ControllerThread: public IThread {
private:
	IController* controller;
	bool keepRunning;

public:
	ControllerThread(IController* controller);

	void run();
	void stop();
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* CONTROLLERTHREAD_H_ */
