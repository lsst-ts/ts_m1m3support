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
    _subscriber = subscriber;
    _controller = controller;
    _publisher = publisher;
    _commandFactory = commandFactory;
    _keepRunning = true;
}

void SubscriberThread::run() {
    spdlog::info("SubscriberThread: Start");
    while (_keepRunning) {
        _enqueueCommandIfAvailable(_subscriber->tryAcceptCommandStart());
        _enqueueCommandIfAvailable(_subscriber->tryAcceptCommandEnable());
        _enqueueCommandIfAvailable(_subscriber->tryAcceptCommandDisable());
        _enqueueCommandIfAvailable(_subscriber->tryAcceptCommandStandby());
        _enqueueCommandIfAvailable(_subscriber->tryAcceptCommandShutdown());
        _enqueueCommandIfAvailable(_subscriber->tryAcceptCommandTurnAirOn());
        _enqueueCommandIfAvailable(_subscriber->tryAcceptCommandTurnAirOff());
        _enqueueCommandIfAvailable(_subscriber->tryAcceptCommandApplyOffsetForces());
        _enqueueCommandIfAvailable(_subscriber->tryAcceptCommandClearOffsetForces());
        _enqueueCommandIfAvailable(_subscriber->tryAcceptCommandRaiseM1M3());
        _enqueueCommandIfAvailable(_subscriber->tryAcceptCommandLowerM1M3());
        _enqueueCommandIfAvailable(_subscriber->tryAcceptCommandApplyAberrationForcesByBendingModes());
        _enqueueCommandIfAvailable(_subscriber->tryAcceptCommandApplyAberrationForces());
        _enqueueCommandIfAvailable(_subscriber->tryAcceptCommandClearAberrationForces());
        _enqueueCommandIfAvailable(_subscriber->tryAcceptCommandApplyActiveOpticForcesByBendingModes());
        _enqueueCommandIfAvailable(_subscriber->tryAcceptCommandApplyActiveOpticForces());
        _enqueueCommandIfAvailable(_subscriber->tryAcceptCommandClearActiveOpticForces());
        _enqueueCommandIfAvailable(_subscriber->tryAcceptCommandEnterEngineering());
        _enqueueCommandIfAvailable(_subscriber->tryAcceptCommandExitEngineering());
        _enqueueCommandIfAvailable(_subscriber->tryAcceptCommandTestAir());
        _enqueueCommandIfAvailable(_subscriber->tryAcceptCommandTestHardpoint());
        _enqueueCommandIfAvailable(_subscriber->tryAcceptCommandTestForceActuator());
        _enqueueCommandIfAvailable(_subscriber->tryAcceptCommandMoveHardpointActuators());
        _enqueueCommandIfAvailable(_subscriber->tryAcceptCommandEnableHardpointChase());
        _enqueueCommandIfAvailable(_subscriber->tryAcceptCommandDisableHardpointChase());
        _enqueueCommandIfAvailable(_subscriber->tryAcceptCommandAbortRaiseM1M3());
        _enqueueCommandIfAvailable(_subscriber->tryAcceptCommandTranslateM1M3());
        _enqueueCommandIfAvailable(_subscriber->tryAcceptCommandStopHardpointMotion());
        _enqueueCommandIfAvailable(_subscriber->tryAcceptCommandPositionM1M3());
        _enqueueCommandIfAvailable(_subscriber->tryAcceptCommandTurnLightsOn());
        _enqueueCommandIfAvailable(_subscriber->tryAcceptCommandTurnLightsOff());
        _enqueueCommandIfAvailable(_subscriber->tryAcceptCommandTurnPowerOn());
        _enqueueCommandIfAvailable(_subscriber->tryAcceptCommandTurnPowerOff());
        _enqueueCommandIfAvailable(_subscriber->tryAcceptCommandEnableHardpointCorrections());
        _enqueueCommandIfAvailable(_subscriber->tryAcceptCommandDisableHardpointCorrections());
        _enqueueCommandIfAvailable(_subscriber->tryAcceptCommandRunMirrorForceProfile());
        _enqueueCommandIfAvailable(_subscriber->tryAcceptCommandAbortProfile());
        _enqueueCommandIfAvailable(_subscriber->tryAcceptCommandApplyOffsetForcesByMirrorForce());
        _enqueueCommandIfAvailable(_subscriber->tryAcceptCommandUpdatePID());
        _enqueueCommandIfAvailable(_subscriber->tryAcceptCommandResetPID());
        _enqueueCommandIfAvailable(_subscriber->tryAcceptCommandProgramILC());
        _enqueueCommandIfAvailable(_subscriber->tryAcceptCommandModbusTransmit());
        _enqueueCommandIfAvailable(_subscriber->tryGetSampleTMAAzimuth());
        _enqueueCommandIfAvailable(_subscriber->tryGetSampleTMAElevation());
        usleep(100);
    }
    spdlog::info("SubscriberThread: Completed");
}

void SubscriberThread::stop() { _keepRunning = false; }

void SubscriberThread::_enqueueCommandIfAvailable(Command* command) {
    if (command) {
        if (command->validate()) {
            _controller->lock();
            _controller->enqueue(command);
            _controller->unlock();
        } else {
            command->ackFailed("Validation");
            _commandFactory->destroy(command);
        }
    }
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
