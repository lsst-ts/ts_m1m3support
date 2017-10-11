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

class IThread {
public:
	virtual ~IThread();

	virtual void run();
	virtual void stop();
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* ITHREAD_H_ */
