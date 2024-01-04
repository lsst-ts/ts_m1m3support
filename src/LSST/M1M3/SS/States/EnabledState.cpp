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

#include <chrono>
#include <thread>

#include <spdlog/spdlog.h>

#include <SAL_MTM1M3C.h>

#include <Accelerometer.h>
#include <BoosterValveController.h>
#include <DigitalInputOutput.h>
#include <EnabledState.h>
#include <ForceActuatorData.h>
#include <HardpointActuatorWarning.h>
#include <Heartbeat.h>
#include <ILC.h>
#include <Model.h>
#include <TMA.h>
#include <TMAAzimuthSampleCommand.h>
#include <TMAElevationSampleCommand.h>
#include <M1M3SSPublisher.h>
#include <ModelPublisher.h>

namespace LSST {
namespace M1M3 {
namespace SS {

EnabledState::EnabledState(std::string name) : State(name) {}

States::Type EnabledState::storeTMAAzimuthSample(TMAAzimuthSampleCommand* command) {
    SPDLOG_TRACE("EnabledState: storeTMAAzimuthSample()");
    TMA::instance().updateTMAAzimuth(command->getData());
    return Model::instance().getSafetyController()->checkSafety(States::NoStateTransition);
}

States::Type EnabledState::storeTMAElevationSample(TMAElevationSampleCommand* command) {
    SPDLOG_TRACE("EnabledState: storeTMAElevationSample()");
    TMA::instance().updateTMAElevation(command->getData());
    return Model::instance().getSafetyController()->checkSafety(States::NoStateTransition);
}

void EnabledState::runLoop() {
    SPDLOG_TRACE("EnabledState: runLoop()");
    IFPGA::get().pullTelemetry();
    Model::instance().getAccelerometer()->processData();
    DigitalInputOutput::instance().processData();
    Model::instance().getDisplacement()->processData();
    Model::instance().getGyro()->processData();
    Model::instance().getInclinometer()->processData();
    Model::instance().getPowerController()->processData();

    Heartbeat::instance().tryToggle();

    ILC* ilc = Model::instance().getILC();
    Model::instance().getForceController()->updateAppliedForces();
    Model::instance().getForceController()->processAppliedForces();
    ilc->writeControlListBuffer();
    ilc->triggerModbus();
    std::this_thread::sleep_for(1ms);
    ilc->waitForAllSubnets(5000);
    ilc->readAll();
    ilc->calculateHPPostion();
    ilc->calculateHPMirrorForces();
    ilc->calculateFAMirrorForces();
    ilc->verifyResponses();
    ilc->publishForceActuatorStatus();
    ForceActuatorData::instance().send();
    ilc->publishHardpointStatus();
    ilc->publishHardpointData();
    ilc->publishHardpointMonitorStatus();
    ilc->publishHardpointMonitorData();
    BoosterValveController::instance().checkTriggers();
    HardpointActuatorWarning::instance().send();
    M1M3SSPublisher::instance().getEnabledForceActuators()->log();
}

void EnabledState::sendTelemetry() {
    ModelPublisher publishIt{};
    runLoop();
}

bool EnabledState::raiseCompleted() {
    if (Model::instance().getMirrorRaiseController()->checkComplete()) {
        Model::instance().getMirrorRaiseController()->complete();
        return true;
    } else if (Model::instance().getMirrorRaiseController()->checkTimeout()) {
        Model::instance().getMirrorRaiseController()->timeout();
    }
    return false;
}

bool EnabledState::lowerCompleted() {
    if (Model::instance().getMirrorLowerController()->checkComplete()) {
        Model::instance().getMirrorLowerController()->complete();
        return true;
    } else if (Model::instance().getMirrorLowerController()->checkTimeout()) {
        Model::instance().getMirrorLowerController()->timeout();
    }
    return false;
}

States::Type EnabledState::disableMirror() {
    Model::instance().getILC()->writeSetModeDisableBuffer();
    Model::instance().getILC()->triggerModbus();
    Model::instance().getILC()->waitForAllSubnets(5000);
    Model::instance().getILC()->readAll();
    Model::instance().getILC()->verifyResponses();
    Model::instance().getForceController()->reset();
    DigitalInputOutput::instance().turnAirOff();
    Model::instance().getPowerController()->setAllAuxPowerNetworks(false);
    return Model::instance().getSafetyController()->checkSafety(States::DisabledState);
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
