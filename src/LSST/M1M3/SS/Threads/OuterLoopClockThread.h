/*
 * OuterLoopClockThread.h
 *
 *  Created on: Oct 2, 2017
 *      Author: ccontaxis
 */

#ifndef OUTERLOOPCLOCKTHREAD_H_
#define OUTERLOOPCLOCKTHREAD_H_

#include <IThread.h>
#include <DataTypes.h>
#include <pthread.h>

namespace LSST {
namespace M1M3 {
namespace SS {

class CommandFactory;
class Controller;
class FPGA;
class M1M3SSPublisher;

/*!
 * The outer loop clock thread is responsible for controlling
 * the outer loop execution timing.
 */
class OuterLoopClockThread : public IThread {
private:
    CommandFactory* commandFactory;
    Controller* controller;
    M1M3SSPublisher* publisher;
    uint16_t timestampUpdateBuffer[5];
    bool keepRunning;
    pthread_mutex_t updateMutex;

public:
    OuterLoopClockThread(CommandFactory* commandFactory, Controller* controller, M1M3SSPublisher* publisher);
    ~OuterLoopClockThread();

    void run();
    void stop();
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* OUTERLOOPCLOCKTHREAD_H_ */
