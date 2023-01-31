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

#include <EnabledState.h>
#include <Accelerometer.h>
#include <Displacement.h>
#include <ForceController.h>
#include <HardpointActuatorWarning.h>
#include <ILC.h>
#include <Inclinometer.h>
#include <DigitalInputOutput.h>
#include <Model.h>
#include <PositionController.h>
#include <SafetyController.h>
#include <PowerController.h>
#include <TMA.h>
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
    SPDLOG_TRACE("EnabledState: storeTMAAzimuthSample()");
    TMA::instance().updateTMAAzimuth(command->getData());
    return Model::get().getSafetyController()->checkSafety(States::NoStateTransition);
}

States::Type EnabledState::storeTMAElevationSample(TMAElevationSampleCommand* command) {
    SPDLOG_TRACE("EnabledState: storeTMAElevationSample()");
    TMA::instance().updateTMAElevation(command->getData());
    return Model::get().getSafetyController()->checkSafety(States::NoStateTransition);
}

States::Type EnabledState::setAirSlewFlag(SetAirSlewFlagCommand* command) {
    MTM1M3_logevent_forceActuatorStateC* forceActuatorState =
            M1M3SSPublisher::instance().getEventForceActuatorState();
    MTM1M3_outerLoopDataC* outerLoop = M1M3SSPublisher::instance().getOuterLoopData();
    SPDLOG_INFO("EnabledState: setAirSlewFlag to {}", command->slewFlag);
    forceActuatorState->slewFlag = command->slewFlag;
    outerLoop->slewFlag = command->slewFlag;

    M1M3SSPublisher::instance().tryLogForceActuatorState();

    return Model::get().getSafetyController()->checkSafety(States::NoStateTransition);
}

void EnabledState::runLoop() {
    SPDLOG_TRACE("EnabledState: runLoop()");
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
    HardpointActuatorWarning::instance().send();
    M1M3SSPublisher::instance().getEnabledForceActuators()->log();
}

void EnabledState::sendTelemetry() {
    ModelPublisher publishIt{};
    runLoop();
}

bool EnabledState::raiseCompleted() {
    if (Model::get().getMirrorRaiseController()->checkComplete()) {
        Model::get().getMirrorRaiseController()->complete();
        return true;
    } else if (Model::get().getMirrorRaiseController()->checkTimeout()) {
        Model::get().getMirrorRaiseController()->timeout();
    }
    return false;
}

bool EnabledState::lowerCompleted() {
    if (Model::get().getMirrorLowerController()->checkComplete()) {
        Model::get().getMirrorLowerController()->complete();
        return true;
    } else if (Model::get().getMirrorLowerController()->checkTimeout()) {
        Model::get().getMirrorLowerController()->timeout();
    }
    return false;
}

States::Type EnabledState::disableMirror() {
    Model::get().getILC()->writeSetModeDisableBuffer();
    Model::get().getILC()->triggerModbus();
    Model::get().getILC()->waitForAllSubnets(5000);
    Model::get().getILC()->readAll();
    Model::get().getILC()->verifyResponses();
    Model::get().getForceController()->reset();
    Model::get().getDigitalInputOutput()->turnAirOff();
    Model::get().getPowerController()->setAllAuxPowerNetworks(false);
    return Model::get().getSafetyController()->checkSafety(States::DisabledState);
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
