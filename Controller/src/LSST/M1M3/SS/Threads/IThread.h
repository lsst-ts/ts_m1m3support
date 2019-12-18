/*
 * IThread.h
 *
 *  Created on: Sep 28, 2017
 *      Author: ccontaxis
 */

#ifndef ITHREAD_H_
#define ITHREAD_H_

namespace LSST {
namespace M1M3 {
namespace SS {

/*!
 * Interface used to represent a task that needs to run
 * on its own thread.
 */
class IThread {
public:
    virtual ~IThread();

    /*!
	 * Run the task.
	 */
    virtual void run();
    /*!
	 * Stop the task.
	 */
    virtual void stop();
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* ITHREAD_H_ */
