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

#include <EnabledState.h>
#include <Accelerometer.h>
#include <Displacement.h>
#include <ForceController.h>
#include <ILC.h>
#include <Inclinometer.h>
#include <DigitalInputOutput.h>
#include <Model.h>
#include <PositionController.h>
#include <SafetyController.h>
#include <PowerController.h>
#include <TMAAzimuthSampleCommand.h>
#include <TMAElevationSampleCommand.h>
#include <M1M3SSPublisher.h>
#include <ModelPublisher.h>
#include <Gyro.h>
#include <spdlog/spdlog.h>
#include <FPGA.h>
#include <SAL_MTM1M3C.h>

#include <chrono>
#include <thread>

namespace LSST {
namespace M1M3 {
namespace SS {

EnabledState::EnabledState(std::string name) : State(name) {}

States::Type EnabledState::storeTMAAzimuthSample(TMAAzimuthSampleCommand* command) {
    spdlog::trace("EnabledState: storeTMAAzimuthSample()");
    Model::get().getForceController()->updateAzimuthForces((float)command->getData()->Azimuth_Angle_Actual);
    return Model::get().getSafetyController()->checkSafety(States::NoStateTransition);
}

States::Type EnabledState::storeTMAElevationSample(TMAElevationSampleCommand* command) {
    spdlog::trace("EnabledState: storeTMAElevationSample()");
    Model::get().getForceController()->updateTMAElevationData(command->getData());
    return Model::get().getSafetyController()->checkSafety(States::NoStateTransition);
}

States::Type EnabledState::testAir(TestAirCommand* command) {
    // TODO: Remove, this is a test command that has been taken for toggling boost valve control
    MTM1M3_logevent_forceActuatorStateC* forceActuatorState =
            M1M3SSPublisher::get().getEventForceActuatorState();
    MTM1M3_outerLoopDataC* outerLoop = M1M3SSPublisher::get().getOuterLoopData();
    spdlog::info("EnabledState: toggleBoostValve to {}", !forceActuatorState->slewFlag);
    forceActuatorState->slewFlag = !forceActuatorState->slewFlag;
    outerLoop->slewFlag = forceActuatorState->slewFlag;

    return Model::get().getSafetyController()->checkSafety(States::NoStateTransition);
}

void EnabledState::runLoop() {
    spdlog::trace("EnabledState: runLoop()");
    ILC* ilc = Model::get().getILC();
    Model::get().getForceController()->updateAppliedForces();
    Model::get().getForceController()->processAppliedForces();
    ilc->writeControlListBuffer();
    ilc->triggerModbus();
    Model::get().getDigitalInputOutput()->tryToggleHeartbeat();
    std::this_thread::sleep_for(1ms);
    IFPGA::get().pullTelemetry();
    Model::get().getAccelerometer()->processData();
    Model::get().getDigitalInputOutput()->processData();
    Model::get().getDisplacement()->processData();
    Model::get().getGyro()->processData();
    Model::get().getInclinometer()->processData();
    Model::get().getPowerController()->processData();
    ilc->waitForAllSubnets(5000);
    ilc->readAll();
    ilc->calculateHPPostion();
    ilc->calculateHPMirrorForces();
    ilc->calculateFAMirrorForces();
    ilc->verifyResponses();
    ilc->publishForceActuatorStatus();
    ilc->publishForceActuatorData();
    ilc->publishHardpointStatus();
    ilc->publishHardpointData();
    ilc->publishHardpointMonitorStatus();
    ilc->publishHardpointMonitorData();
    M1M3SSPublisher::get().tryLogHardpointActuatorWarning();
}

void EnabledState::sendTelemetry() {
    ModelPublisher publishIt(Model);
    runLoop();
}

bool EnabledState::raiseCompleted() {
    if (Model::get().getAutomaticOperationsController()->checkRaiseOperationComplete()) {
        Model::get().getAutomaticOperationsController()->completeRaiseOperation();
        return true;
    } else if (Model::get().getAutomaticOperationsController()->checkRaiseOperationTimeout()) {
        Model::get().getAutomaticOperationsController()->timeoutRaiseOperation();
    }
    return false;
}

bool EnabledState::lowerCompleted() {
    if (Model::get().getAutomaticOperationsController()->checkLowerOperationComplete()) {
        Model::get().getAutomaticOperationsController()->completeLowerOperation();
        return true;
    } else if (Model::get().getAutomaticOperationsController()->checkLowerOperationTimeout()) {
        Model::get().getAutomaticOperationsController()->timeoutLowerOperation();
    }
    return false;
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
