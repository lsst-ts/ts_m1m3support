/*
 * This file is part of LSST M1M3 support system package.
 *
 * Developed for the LSST Data Management System.
 * This product includes software developed by the LSST Project
 * (https://www.lsst.org).
 * See the COPYRIGHT file at the top-level directory of this distribution
 * for details of code ownership.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include <SubscriberThread.h>
#include <M1M3SSSubscriber.h>
#include <Controller.h>
#include <M1M3SSPublisher.h>
#include <CommandFactory.h>
#include <Command.h>
#include <unistd.h>
#include <spdlog/spdlog.h>

namespace LSST {
namespace M1M3 {
namespace SS {

SubscriberThread::SubscriberThread(M1M3SSSubscriber* subscriber, Controller* controller,
                                   M1M3SSPublisher* publisher, CommandFactory* commandFactory) {
    this->subscriber = subscriber;
    this->controller = controller;
    this->publisher = publisher;
    this->commandFactory = commandFactory;
    this->keepRunning = true;
}

void SubscriberThread::run() {
    spdlog::info("SubscriberThread: Start");
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
        this->enqueueCommandIfAvailable(
                this->subscriber->tryAcceptCommandApplyAberrationForcesByBendingModes());
        this->enqueueCommandIfAvailable(this->subscriber->tryAcceptCommandApplyAberrationForces());
        this->enqueueCommandIfAvailable(this->subscriber->tryAcceptCommandClearAberrationForces());
        this->enqueueCommandIfAvailable(
                this->subscriber->tryAcceptCommandApplyActiveOpticForcesByBendingModes());
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
        this->enqueueCommandIfAvailable(this->subscriber->tryAcceptCommandRunMirrorForceProfile());
        this->enqueueCommandIfAvailable(this->subscriber->tryAcceptCommandAbortProfile());
        this->enqueueCommandIfAvailable(this->subscriber->tryAcceptCommandApplyOffsetForcesByMirrorForce());
        this->enqueueCommandIfAvailable(this->subscriber->tryAcceptCommandUpdatePID());
        this->enqueueCommandIfAvailable(this->subscriber->tryAcceptCommandResetPID());
        this->enqueueCommandIfAvailable(this->subscriber->tryAcceptCommandProgramILC());
        this->enqueueCommandIfAvailable(this->subscriber->tryAcceptCommandModbusTransmit());
        this->enqueueCommandIfAvailable(this->subscriber->tryGetSampleTMAAzimuth());
        this->enqueueCommandIfAvailable(this->subscriber->tryGetSampleTMAElevation());
        usleep(100);
    }
    spdlog::info("SubscriberThread: Completed");
}

void SubscriberThread::stop() { this->keepRunning = false; }

void SubscriberThread::enqueueCommandIfAvailable(Command* command) {
    if (command) {
        if (command->validate()) {
            this->controller->lock();
            this->controller->enqueue(command);
            this->controller->unlock();
        } else {
            command->ackFailed("Validation");
            this->commandFactory->destroy(command);
        }
    }
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
