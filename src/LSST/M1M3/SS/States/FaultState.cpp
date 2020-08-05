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

#include <FaultState.h>
#include <DigitalInputOutput.h>
#include <Model.h>
#include <ILC.h>
#include <Displacement.h>
#include <Inclinometer.h>
#include <PowerController.h>
#include <SafetyController.h>
#include <ForceController.h>
#include <unistd.h>
#include <M1M3SSPublisher.h>
#include <ModelPublisher.h>
#include <Accelerometer.h>
#include <spdlog/spdlog.h>
#include <Gyro.h>
#include <FPGA.h>
#include <chrono>
#include <thread>

namespace LSST {
namespace M1M3 {
namespace SS {

FaultState::FaultState() : State("FaultState") {}
FaultState::FaultState(std::string name) : State(name) {}

States::Type FaultState::update(UpdateCommand* command, Model* model) {
    ModelPublisher publishIt(model);
    spdlog::trace("FaultState: update()");
    ILC* ilc = model->getILC();
    ilc->writeFreezeSensorListBuffer();
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
    return States::NoStateTransition;
}

States::Type FaultState::standby(StandbyCommand* command, Model* model) {
    spdlog::trace("FaultState: standby()");
    model->getILC()->writeSetModeStandbyBuffer();
    model->getILC()->triggerModbus();
    model->getILC()->waitForAllSubnets(5000);
    model->getILC()->readAll();
    model->getILC()->verifyResponses();
    model->getPowerController()->setAllPowerNetworks(false);
    model->getSafetyController()->clearErrorCode();
    return States::StandbyState;
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
