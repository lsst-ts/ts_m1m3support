/*
 * SubscriberThread.cpp
 *
 *  Created on: Sep 28, 2017
 *      Author: ccontaxis
 */

#include <SubscriberThread.h>
#include <M1M3SSSubscriber.h>
#include <Controller.h>
#include <M1M3SSPublisher.h>
#include <CommandFactory.h>
#include <Command.h>
#include <unistd.h>
#include <Log.h>

namespace LSST {
namespace M1M3 {
namespace SS {

SubscriberThread::SubscriberThread(M1M3SSSubscriber* subscriber, Controller* controller, M1M3SSPublisher* publisher, CommandFactory* commandFactory) {
    this->subscriber = subscriber;
    this->controller = controller;
    this->publisher = publisher;
    this->commandFactory = commandFactory;
    this->keepRunning = true;
}

void SubscriberThread::run() {
    Log.Info("SubscriberThread: Start");
    while (this->keepRunning) {
        this->enqueueCommandIfAvailable(this->subscriber->tryAcceptCommandStart());
        this->enqueueCommandIfAvailable(this->subscriber->tryAcceptCommandEnable());
        this->enqueueCommandIfAvailable(this->subscriber->tryAcceptCommandDisable());
        this->enqueueCommandIfAvailable(this->subscriber->tryAcceptCommandStandby());
        this->enqueueCommandIfAvailable(this->subscriber->tryAcceptCommandShutdown());
        this->enqueueCommandIfAvailable(this->subscriber->tryAcceptCommandTurnAirOn());
        this->enqueueCommandIfAvailable(this->subscriber->tryAcceptCommandTurnAirOff());
        this->enqueueCommandIfAvailable(this->subscriber->tryAcceptCommandApplyOffsetForces());
        this->enqueueCommandIfAvailable(this->subscriber->tryAcceptCommandClearOffsetForces());
        this->enqueueCommandIfAvailable(this->subscriber->tryAcceptCommandRaiseM1M3());
        this->enqueueCommandIfAvailable(this->subscriber->tryAcceptCommandLowerM1M3());
        this->enqueueCommandIfAvailable(this->subscriber->tryAcceptCommandApplyAberrationForcesByBendingModes());
        this->enqueueCommandIfAvailable(this->subscriber->tryAcceptCommandApplyAberrationForces());
        this->enqueueCommandIfAvailable(this->subscriber->tryAcceptCommandClearAberrationForces());
        this->enqueueCommandIfAvailable(this->subscriber->tryAcceptCommandApplyActiveOpticForcesByBendingModes());
        this->enqueueCommandIfAvailable(this->subscriber->tryAcceptCommandApplyActiveOpticForces());
        this->enqueueCommandIfAvailable(this->subscriber->tryAcceptCommandClearActiveOpticForces());
        this->enqueueCommandIfAvailable(this->subscriber->tryAcceptCommandEnterEngineering());
        this->enqueueCommandIfAvailable(this->subscriber->tryAcceptCommandExitEngineering());
        this->enqueueCommandIfAvailable(this->subscriber->tryAcceptCommandTestAir());
        this->enqueueCommandIfAvailable(this->subscriber->tryAcceptCommandTestHardpoint());
        this->enqueueCommandIfAvailable(this->subscriber->tryAcceptCommandTestForceActuator());
        this->enqueueCommandIfAvailable(this->subscriber->tryAcceptCommandMoveHardpointActuators());
        this->enqueueCommandIfAvailable(this->subscriber->tryAcceptCommandEnableHardpointChase());
        this->enqueueCommandIfAvailable(this->subscriber->tryAcceptCommandDisableHardpointChase());
        this->enqueueCommandIfAvailable(this->subscriber->tryAcceptCommandAbortRaiseM1M3());
        this->enqueueCommandIfAvailable(this->subscriber->tryAcceptCommandTranslateM1M3());
        this->enqueueCommandIfAvailable(this->subscriber->tryAcceptCommandStopHardpointMotion());
        this->enqueueCommandIfAvailable(this->subscriber->tryAcceptCommandPositionM1M3());
        this->enqueueCommandIfAvailable(this->subscriber->tryAcceptCommandTurnLightsOn());
        this->enqueueCommandIfAvailable(this->subscriber->tryAcceptCommandTurnLightsOff());
        this->enqueueCommandIfAvailable(this->subscriber->tryAcceptCommandTurnPowerOn());
        this->enqueueCommandIfAvailable(this->subscriber->tryAcceptCommandTurnPowerOff());
        this->enqueueCommandIfAvailable(this->subscriber->tryAcceptCommandEnableHardpointCorrections());
        this->enqueueCommandIfAvailable(this->subscriber->tryAcceptCommandDisableHardpointCorrections());
        this->enqueueCommandIfAvailable(this->subscriber->tryAcceptCommandApplyOffsetForcesByMirrorForce());
        this->enqueueCommandIfAvailable(this->subscriber->tryAcceptCommandUpdatePID());
        this->enqueueCommandIfAvailable(this->subscriber->tryAcceptCommandResetPID());
        this->enqueueCommandIfAvailable(this->subscriber->tryAcceptCommandProgramILC());
        this->enqueueCommandIfAvailable(this->subscriber->tryAcceptCommandModbusTransmit());
        this->enqueueCommandIfAvailable(this->subscriber->tryGetSampleTMAAzimuth());
        this->enqueueCommandIfAvailable(this->subscriber->tryGetSampleTMAElevation());
        usleep(100);
    }
    Log.Info("SubscriberThread: Completed");
}

void SubscriberThread::stop() {
    this->keepRunning = false;
}

void SubscriberThread::enqueueCommandIfAvailable(Command* command) {
    if (command) {
        if (command->validate()) {
            this->controller->lock();
            this->controller->enqueue(command);
            this->controller->unlock();
        } else {
            this->commandFactory->destroy(command);
        }
    }
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
