/*
 * Controller.h
 *
 *  Created on: Sep 27, 2017
 *      Author: ccontaxis
 */

#ifndef CONTROLLER_H_
#define CONTROLLER_H_

#include <queue>
#include <pthread.h>

namespace LSST {
namespace M1M3 {
namespace SS {

class Command;
class CommandFactory;

/*!
 * The main controller for the application.
 */
class Controller {
private:
    CommandFactory* commandFactory;
    pthread_mutex_t mutex;
    std::queue<Command*> queue;

public:
    Controller(CommandFactory* commandFactory);
    ~Controller();

    void lock();
    void unlock();

    void clear();

    void enqueue(Command* command);
    Command* dequeue();
    void execute(Command* command);
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* CONTROLLER_H_ */
