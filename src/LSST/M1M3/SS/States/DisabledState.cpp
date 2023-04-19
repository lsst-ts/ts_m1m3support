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

#include <Accelerometer.h>
#include <DigitalInputOutput.h>
#include <DisabledState.h>
#include <Displacement.h>
#include <ForceActuatorData.h>
#include <ILC.h>
#include <Inclinometer.h>
#include <HardpointActuatorWarning.h>
#include <Model.h>
#include <SafetyController.h>
#include <PowerController.h>
#include <Gyro.h>
#include <ForceController.h>
#include <M1M3SSPublisher.h>
#include <ModelPublisher.h>
#include <spdlog/spdlog.h>
#include <FPGA.h>
#include <chrono>
#include <thread>

namespace LSST {
namespace M1M3 {
namespace SS {

DisabledState::DisabledState() : State("DisabledState") {}

States::Type DisabledState::update(UpdateCommand* command) {
    ModelPublisher publishIt{};
    SPDLOG_TRACE("DisabledState::update()");
    ILC* ilc = Model::get().getILC();
    ilc->writeFreezeSensorListBuffer();
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
    ForceActuatorData::instance().send();
    ilc->publishHardpointStatus();
    ilc->publishHardpointData();
    ilc->publishHardpointMonitorStatus();
    ilc->publishHardpointMonitorData();
    HardpointActuatorWarning::instance().send();
    return Model::get().getSafetyController()->checkSafety(States::NoStateTransition);
}

States::Type DisabledState::enable(EnableCommand* command) {
    SPDLOG_INFO("DisabledState: enable()");
    Model::get().getILC()->writeSetModeEnableBuffer();
    Model::get().getILC()->triggerModbus();
    Model::get().getILC()->waitForAllSubnets(5000);
    Model::get().getILC()->readAll();
    Model::get().getILC()->verifyResponses();
    Model::get().getDigitalInputOutput()->turnAirOn();
    Model::get().getPowerController()->setAllAuxPowerNetworks(true);
    return Model::get().getSafetyController()->checkSafety(States::ParkedState);
}

States::Type DisabledState::standby(StandbyCommand* command) {
    SPDLOG_INFO("DisabledState: standby()");
    Model::get().getILC()->writeSetModeStandbyBuffer();
    Model::get().getILC()->triggerModbus();
    Model::get().getILC()->waitForAllSubnets(5000);
    Model::get().getILC()->readAll();
    Model::get().getILC()->verifyResponses();
    M1M3SSPublisher::instance().tryLogForceActuatorState();
    Model::get().getPowerController()->setBothPowerNetworks(false);
    return Model::get().getSafetyController()->checkSafety(States::StandbyState);
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
