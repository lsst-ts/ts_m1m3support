/*
 * IController.h
 *
 *  Created on: Sep 27, 2017
 *      Author: ccontaxis
 */

#ifndef ICONTROLLER_H_
#define ICONTROLLER_H_

namespace LSST {
namespace M1M3 {
namespace SS {

class ICommand;

class IController {
public:
	virtual ~IController();

	virtual void lock();
	virtual void unlock();

	virtual void clear();

	virtual void enqueue(ICommand* command);
	virtual ICommand* dequeue();
	virtual void execute(ICommand* command);
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* ICONTROLLER_H_ */
