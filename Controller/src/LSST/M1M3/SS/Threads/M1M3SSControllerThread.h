/*
 * M1M3SSControllerThread.h
 *
 *  Created on: Sep 28, 2017
 *      Author: ccontaxis
 */

#ifndef M1M3SSCONTROLLERTHREAD_H_
#define M1M3SSCONTROLLERTHREAD_H_

#include <IThread.h>

namespace LSST {
namespace M1M3 {
namespace SS {

class IController;

class M1M3SSControllerThread: public IThread {
private:
	IController* controller;
	bool keepRunning;

public:
	M1M3SSControllerThread(IController* controller);
	virtual ~M1M3SSControllerThread();

	virtual void run();
	virtual void stop();
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* M1M3SSCONTROLLERTHREAD_H_ */
