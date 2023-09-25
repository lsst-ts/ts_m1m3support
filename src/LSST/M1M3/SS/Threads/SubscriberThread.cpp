/*
 * This file is part of LSST M1M3 support system package.
 *
 * Developed for the Vera C. Rubin Telescope and Site System.
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

#include <ControllerThread.h>
#include <SubscriberThread.h>
#include <M1M3SSPublisher.h>
#include <M1M3SSSubscriber.h>
#include <chrono>
#include <thread>
#include <spdlog/spdlog.h>

namespace LSST {
namespace M1M3 {
namespace SS {

SubscriberThread::SubscriberThread() { _keepRunning = true; }

void SubscriberThread::run() {
    SPDLOG_INFO("SubscriberThread: Start");
    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
    while (_keepRunning) {
        _enqueueCommandIfAvailable(M1M3SSSubscriber::get().tryAcceptCommandSetLogLevel());
        _enqueueCommandIfAvailable(M1M3SSSubscriber::get().tryAcceptCommandStart());
        _enqueueCommandIfAvailable(M1M3SSSubscriber::get().tryAcceptCommandEnable());
        _enqueueCommandIfAvailable(M1M3SSSubscriber::get().tryAcceptCommandDisable());
        _enqueueCommandIfAvailable(M1M3SSSubscriber::get().tryAcceptCommandStandby());
        _enqueueCommandIfAvailable(M1M3SSSubscriber::get().tryAcceptCommandExitControl());
        _enqueueCommandIfAvailable(M1M3SSSubscriber::get().tryAcceptCommandPanic());
        _enqueueCommandIfAvailable(M1M3SSSubscriber::get().tryAcceptCommandSetSlewFlag());
        _enqueueCommandIfAvailable(M1M3SSSubscriber::get().tryAcceptCommandClearSlewFlag());
        _enqueueCommandIfAvailable(M1M3SSSubscriber::get().tryAcceptCommandTurnAirOn());
        _enqueueCommandIfAvailable(M1M3SSSubscriber::get().tryAcceptCommandTurnAirOff());
        _enqueueCommandIfAvailable(M1M3SSSubscriber::get().tryAcceptCommandBoosterValveOpen());
        _enqueueCommandIfAvailable(M1M3SSSubscriber::get().tryAcceptCommandBoosterValveClose());
        _enqueueCommandIfAvailable(M1M3SSSubscriber::get().tryAcceptCommandApplyOffsetForces());
        _enqueueCommandIfAvailable(M1M3SSSubscriber::get().tryAcceptCommandClearOffsetForces());
        _enqueueCommandIfAvailable(M1M3SSSubscriber::get().tryAcceptCommandRaiseM1M3());
        _enqueueCommandIfAvailable(M1M3SSSubscriber::get().tryAcceptCommandLowerM1M3());
        _enqueueCommandIfAvailable(M1M3SSSubscriber::get().tryAcceptCommandPauseM1M3RaisingLowering());
        _enqueueCommandIfAvailable(M1M3SSSubscriber::get().tryAcceptCommandResumeM1M3RaisingLowering());
        _enqueueCommandIfAvailable(M1M3SSSubscriber::get().tryAcceptCommandApplyActiveOpticForces());
        _enqueueCommandIfAvailable(M1M3SSSubscriber::get().tryAcceptCommandClearActiveOpticForces());
        _enqueueCommandIfAvailable(M1M3SSSubscriber::get().tryAcceptCommandEnterEngineering());
        _enqueueCommandIfAvailable(M1M3SSSubscriber::get().tryAcceptCommandExitEngineering());
        _enqueueCommandIfAvailable(M1M3SSSubscriber::get().tryAcceptCommandTestHardpoint());
        _enqueueCommandIfAvailable(M1M3SSSubscriber::get().tryAcceptCommandKillHardpointTest());
        _enqueueCommandIfAvailable(M1M3SSSubscriber::get().tryAcceptCommandMoveHardpointActuators());
        _enqueueCommandIfAvailable(M1M3SSSubscriber::get().tryAcceptCommandEnableHardpointChase());
        _enqueueCommandIfAvailable(M1M3SSSubscriber::get().tryAcceptCommandDisableHardpointChase());
        _enqueueCommandIfAvailable(M1M3SSSubscriber::get().tryAcceptCommandAbortRaiseM1M3());
        _enqueueCommandIfAvailable(M1M3SSSubscriber::get().tryAcceptCommandTranslateM1M3());
        _enqueueCommandIfAvailable(M1M3SSSubscriber::get().tryAcceptCommandStopHardpointMotion());
        _enqueueCommandIfAvailable(M1M3SSSubscriber::get().tryAcceptCommandPositionM1M3());
        _enqueueCommandIfAvailable(M1M3SSSubscriber::get().tryAcceptCommandTurnLightsOn());
        _enqueueCommandIfAvailable(M1M3SSSubscriber::get().tryAcceptCommandTurnLightsOff());
        _enqueueCommandIfAvailable(M1M3SSSubscriber::get().tryAcceptCommandTurnPowerOn());
        _enqueueCommandIfAvailable(M1M3SSSubscriber::get().tryAcceptCommandTurnPowerOff());
        _enqueueCommandIfAvailable(M1M3SSSubscriber::get().tryAcceptCommandEnableHardpointCorrections());
        _enqueueCommandIfAvailable(M1M3SSSubscriber::get().tryAcceptCommandDisableHardpointCorrections());
        _enqueueCommandIfAvailable(M1M3SSSubscriber::get().tryAcceptCommandRunMirrorForceProfile());
        _enqueueCommandIfAvailable(M1M3SSSubscriber::get().tryAcceptCommandAbortProfile());
        _enqueueCommandIfAvailable(M1M3SSSubscriber::get().tryAcceptCommandApplyOffsetForcesByMirrorForce());
        _enqueueCommandIfAvailable(M1M3SSSubscriber::get().tryAcceptCommandUpdatePID());
        _enqueueCommandIfAvailable(M1M3SSSubscriber::get().tryAcceptCommandResetPID());
        _enqueueCommandIfAvailable(M1M3SSSubscriber::get().tryAcceptCommandForceActuatorBumpTest());
        _enqueueCommandIfAvailable(M1M3SSSubscriber::get().tryAcceptCommandKillForceActuatorBumpTest());
        _enqueueCommandIfAvailable(M1M3SSSubscriber::get().tryAcceptCommandDisableForceActuator());
        _enqueueCommandIfAvailable(M1M3SSSubscriber::get().tryAcceptCommandEnableForceActuator());
        _enqueueCommandIfAvailable(M1M3SSSubscriber::get().tryAcceptCommandEnableAllForceActuators());
        _enqueueCommandIfAvailable(M1M3SSSubscriber::get().tryAcceptCommandEnableDisableForceComponent());
        _enqueueCommandIfAvailable(M1M3SSSubscriber::get().tryAcceptCommandSetSlewControllerSettings());
        _enqueueCommandIfAvailable(M1M3SSSubscriber::get().tryGetSampleTMAAzimuth());
        _enqueueCommandIfAvailable(M1M3SSSubscriber::get().tryGetSampleTMAElevation());
        std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
        long executionTime = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count();
        if (executionTime > 100) {
            SPDLOG_WARN("SubscriberThread executing for too long: {} ms", executionTime);
        }
        begin = end;
        std::this_thread::sleep_for(100us);
    }
    SPDLOG_INFO("SubscriberThread: Completed");
}

void SubscriberThread::stop() { _keepRunning = false; }

void SubscriberThread::_enqueueCommandIfAvailable(Command* command) {
    if (command) {
        try {
            if (command->validate()) {
                ControllerThread::get().enqueue(command);
            } else {
                auto info = M1M3SSPublisher::instance().getEventCommandRejectionWarning();
                command->ackFailed(
                        fmt::format("Command \"{}\" validation failed: {} ", info->command, info->reason));
                delete command;
            }
        } catch (std::exception& ex) {
            command->ackFailed(ex.what());
            delete command;
        }
    }
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
