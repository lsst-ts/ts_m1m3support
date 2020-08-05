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

States::Type EnabledState::storeTMAAzimuthSample(TMAAzimuthSampleCommand* command, Model* model) {
    spdlog::trace("EnabledState: storeTMAAzimuthSample()");
    model->getForceController()->updateAzimuthForces((float)command->getData()->Azimuth_Angle_Actual);
    return model->getSafetyController()->checkSafety(States::NoStateTransition);
}

States::Type EnabledState::storeTMAElevationSample(TMAElevationSampleCommand* command, Model* model) {
    spdlog::trace("EnabledState: storeTMAElevationSample()");
    model->getForceController()->updateTMAElevationData(command->getData());
    return model->getSafetyController()->checkSafety(States::NoStateTransition);
}

States::Type EnabledState::testAir(TestAirCommand* command, Model* model) {
    // TODO: Remove, this is a test command that has been taken for toggling boost valve control
    MTM1M3_logevent_forceActuatorStateC* forceActuatorState =
            M1M3SSPublisher::get().getEventForceActuatorState();
    MTM1M3_outerLoopDataC* outerLoop = M1M3SSPublisher::get().getOuterLoopData();
    spdlog::info("EnabledState: toggleBoostValve to {}", !forceActuatorState->slewFlag);
    forceActuatorState->slewFlag = !forceActuatorState->slewFlag;
    outerLoop->slewFlag = forceActuatorState->slewFlag;

    return model->getSafetyController()->checkSafety(States::NoStateTransition);
}

void EnabledState::runLoop(Model* model) {
    spdlog::trace("EnabledState: runLoop()");
    ILC* ilc = model->getILC();
    model->getForceController()->updateAppliedForces();
    model->getForceController()->processAppliedForces();
    ilc->writeControlListBuffer();
    ilc->triggerModbus();
    model->getDigitalInputOutput()->tryToggleHeartbeat();
    std::this_thread::sleep_for(1ms);
    IFPGA::get().pullTelemetry();
    model->getAccelerometer()->processData();
    model->getDigitalInputOutput()->processData();
    model->getDisplacement()->processData();
    model->getGyro()->processData();
    model->getInclinometer()->processData();
    model->getPowerController()->processData();
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

void EnabledState::sendTelemetry(Model* model) {
    ModelPublisher publishIt(Model);
    runLoop(model);
}

bool EnabledState::raiseCompleted(Model* model) {
    if (model->getAutomaticOperationsController()->checkRaiseOperationComplete()) {
        model->getAutomaticOperationsController()->completeRaiseOperation();
        return true;
    } else if (model->getAutomaticOperationsController()->checkRaiseOperationTimeout()) {
        model->getAutomaticOperationsController()->timeoutRaiseOperation();
    }
    return false;
}

bool EnabledState::lowerCompleted(Model* model) {
    if (model->getAutomaticOperationsController()->checkLowerOperationComplete()) {
        model->getAutomaticOperationsController()->completeLowerOperation();
        return true;
    } else if (model->getAutomaticOperationsController()->checkLowerOperationTimeout()) {
        model->getAutomaticOperationsController()->timeoutLowerOperation();
    }
    return false;
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
