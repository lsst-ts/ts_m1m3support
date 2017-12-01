/*
 * SubscriberThread.cpp
 *
 *  Created on: Sep 28, 2017
 *      Author: ccontaxis
 */

#include <SubscriberThread.h>
#include <ISubscriber.h>
#include <IController.h>
#include <IPublisher.h>
#include <ICommandFactory.h>
#include <ICommand.h>
#include <unistd.h>

namespace LSST {
namespace M1M3 {
namespace SS {

SubscriberThread::SubscriberThread(ISubscriber* subscriber, IController* controller, IPublisher* publisher, ICommandFactory* commandFactory) {
	this->subscriber = subscriber;
	this->controller = controller;
	this->publisher = publisher;
	this->commandFactory = commandFactory;
	this->keepRunning = true;
}

void SubscriberThread::run() {
	while(this->keepRunning) {
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
		this->enqueueCommandIfAvailable(this->subscriber->tryAcceptCommandApplyAberrationByBendingModes());
		this->enqueueCommandIfAvailable(this->subscriber->tryAcceptCommandApplyAberrationByForces());
		this->enqueueCommandIfAvailable(this->subscriber->tryAcceptCommandClearAberration());
		this->enqueueCommandIfAvailable(this->subscriber->tryAcceptCommandApplyAOSCorrectionByBendingModes());
		this->enqueueCommandIfAvailable(this->subscriber->tryAcceptCommandApplyAOSCorrectionByForces());
		this->enqueueCommandIfAvailable(this->subscriber->tryAcceptCommandClearAOSCorrection());
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
		this->enqueueCommandIfAvailable(this->subscriber->tryGetSampleTMAAzimuth());
		this->enqueueCommandIfAvailable(this->subscriber->tryGetSampleTMAElevation());
		usleep(100);
	}
}

void SubscriberThread::stop() {
	this->keepRunning = false;
}

void SubscriberThread::enqueueCommandIfAvailable(ICommand* command) {
	if (command) {
		if(command->validate()) {
			this->controller->lock();
			this->controller->enqueue(command);
			this->controller->unlock();
		}
		else {
			command->ackFailed("Validation");
			this->commandFactory->destroy(command);
		}
	}
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
