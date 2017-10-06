/*
 * Controller.h
 *
 *  Created on: Sep 27, 2017
 *      Author: ccontaxis
 */

#ifndef CONTROLLER_H_
#define CONTROLLER_H_

#include <IController.h>
#include <queue>
#include <pthread.h>

namespace LSST {
namespace M1M3 {
namespace SS {

class ICommandFactory;

class Controller: public IController {
private:
	ICommandFactory* commandFactory;
	pthread_mutex_t mutex;
	std::queue<ICommand*> queue;

public:
	Controller(ICommandFactory* commandFactory);
	virtual ~Controller();

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

#endif /* CONTROLLER_H_ */
