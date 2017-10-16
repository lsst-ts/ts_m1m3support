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

/*!
 * The main controller for the application.
 */
class Controller: public IController {
private:
	ICommandFactory* commandFactory;
	pthread_mutex_t mutex;
	std::queue<ICommand*> queue;

public:
	Controller(ICommandFactory* commandFactory);
	~Controller();

	void lock();
	void unlock();

	void clear();

	void enqueue(ICommand* command);
	ICommand* dequeue();
	void execute(ICommand* command);
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* CONTROLLER_H_ */
