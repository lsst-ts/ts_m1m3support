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

#include <CommandFactory.h>
#include <Controller.h>
#include <SubscriberThread.h>
#include <M1M3SSSubscriber.h>
#include <chrono>
#include <thread>
#include <spdlog/spdlog.h>

namespace LSST {
namespace M1M3 {
namespace SS {

SubscriberThread::SubscriberThread() { _keepRunning = true; }

void SubscriberThread::run() {
    spdlog::info("SubscriberThread: Start");
    M1M3SSSubscriber subscriber = M1M3SSSubscriber::get();
    while (_keepRunning) {
        _enqueueCommandIfAvailable(subscriber.tryAcceptCommandStart());
        _enqueueCommandIfAvailable(subscriber.tryAcceptCommandEnable());
        _enqueueCommandIfAvailable(subscriber.tryAcceptCommandDisable());
        _enqueueCommandIfAvailable(subscriber.tryAcceptCommandStandby());
        _enqueueCommandIfAvailable(subscriber.tryAcceptCommandExitControl());
        _enqueueCommandIfAvailable(subscriber.tryAcceptCommandTurnAirOn());
        _enqueueCommandIfAvailable(subscriber.tryAcceptCommandTurnAirOff());
        _enqueueCommandIfAvailable(subscriber.tryAcceptCommandApplyOffsetForces());
        _enqueueCommandIfAvailable(subscriber.tryAcceptCommandClearOffsetForces());
        _enqueueCommandIfAvailable(subscriber.tryAcceptCommandRaiseM1M3());
        _enqueueCommandIfAvailable(subscriber.tryAcceptCommandLowerM1M3());
        _enqueueCommandIfAvailable(subscriber.tryAcceptCommandApplyAberrationForcesByBendingModes());
        _enqueueCommandIfAvailable(subscriber.tryAcceptCommandApplyAberrationForces());
        _enqueueCommandIfAvailable(subscriber.tryAcceptCommandClearAberrationForces());
        _enqueueCommandIfAvailable(subscriber.tryAcceptCommandApplyActiveOpticForcesByBendingModes());
        _enqueueCommandIfAvailable(subscriber.tryAcceptCommandApplyActiveOpticForces());
        _enqueueCommandIfAvailable(subscriber.tryAcceptCommandClearActiveOpticForces());
        _enqueueCommandIfAvailable(subscriber.tryAcceptCommandEnterEngineering());
        _enqueueCommandIfAvailable(subscriber.tryAcceptCommandExitEngineering());
        _enqueueCommandIfAvailable(subscriber.tryAcceptCommandTestAir());
        _enqueueCommandIfAvailable(subscriber.tryAcceptCommandTestHardpoint());
        _enqueueCommandIfAvailable(subscriber.tryAcceptCommandTestForceActuator());
        _enqueueCommandIfAvailable(subscriber.tryAcceptCommandMoveHardpointActuators());
        _enqueueCommandIfAvailable(subscriber.tryAcceptCommandEnableHardpointChase());
        _enqueueCommandIfAvailable(subscriber.tryAcceptCommandDisableHardpointChase());
        _enqueueCommandIfAvailable(subscriber.tryAcceptCommandAbortRaiseM1M3());
        _enqueueCommandIfAvailable(subscriber.tryAcceptCommandTranslateM1M3());
        _enqueueCommandIfAvailable(subscriber.tryAcceptCommandStopHardpointMotion());
        _enqueueCommandIfAvailable(subscriber.tryAcceptCommandPositionM1M3());
        _enqueueCommandIfAvailable(subscriber.tryAcceptCommandTurnLightsOn());
        _enqueueCommandIfAvailable(subscriber.tryAcceptCommandTurnLightsOff());
        _enqueueCommandIfAvailable(subscriber.tryAcceptCommandTurnPowerOn());
        _enqueueCommandIfAvailable(subscriber.tryAcceptCommandTurnPowerOff());
        _enqueueCommandIfAvailable(subscriber.tryAcceptCommandEnableHardpointCorrections());
        _enqueueCommandIfAvailable(subscriber.tryAcceptCommandDisableHardpointCorrections());
        _enqueueCommandIfAvailable(subscriber.tryAcceptCommandRunMirrorForceProfile());
        _enqueueCommandIfAvailable(subscriber.tryAcceptCommandAbortProfile());
        _enqueueCommandIfAvailable(subscriber.tryAcceptCommandApplyOffsetForcesByMirrorForce());
        _enqueueCommandIfAvailable(subscriber.tryAcceptCommandUpdatePID());
        _enqueueCommandIfAvailable(subscriber.tryAcceptCommandResetPID());
        _enqueueCommandIfAvailable(subscriber.tryAcceptCommandProgramILC());
        _enqueueCommandIfAvailable(subscriber.tryAcceptCommandModbusTransmit());
        _enqueueCommandIfAvailable(subscriber.tryGetSampleTMAAzimuth());
        _enqueueCommandIfAvailable(subscriber.tryGetSampleTMAElevation());
        std::this_thread::sleep_for(100us);
    }
    spdlog::info("SubscriberThread: Completed");
}

void SubscriberThread::stop() { _keepRunning = false; }

void SubscriberThread::_enqueueCommandIfAvailable(Command* command) {
    if (command) {
        if (command->validate()) {
            Controller controller = Controller::get();
            controller.lock();
            controller.enqueue(command);
            controller.unlock();
        } else {
            command->ackFailed("Validation");
            delete command;
        }
    }
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
