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

#include <DisabledState.h>
#include <Accelerometer.h>
#include <Displacement.h>
#include <ILC.h>
#include <Inclinometer.h>
#include <DigitalInputOutput.h>
#include <Model.h>
#include <SafetyController.h>
#include <PowerController.h>
#include <Gyro.h>
#include <ForceController.h>
#include <M1M3SSPublisher.h>
#include <ProgramILCCommand.h>
#include <ModbusTransmitCommand.h>
#include <spdlog/spdlog.h>
#include <unistd.h>
#include <FPGA.h>

namespace LSST {
namespace M1M3 {
namespace SS {

DisabledState::DisabledState(M1M3SSPublisher* publisher) : State(publisher, "DisabledState") {}

States::Type DisabledState::update(UpdateCommand* command, Model* model) {
    spdlog::trace("DisabledState::update()");
    this->startTimer();
    ILC* ilc = model->getILC();
    ilc->writeFreezeSensorListBuffer();
    ilc->triggerModbus();
    model->getDigitalInputOutput()->tryToggleHeartbeat();
    usleep(1000);
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
    model->getPublisher()->tryLogHardpointActuatorWarning();
    this->stopTimer();
    model->publishOuterLoop(this->getTimer());
    return model->getSafetyController()->checkSafety(States::NoStateTransition);
}

States::Type DisabledState::enable(EnableCommand* command, Model* model) {
    spdlog::info("DisabledState: enable()");
    States::Type newState = States::ParkedState;
    model->getILC()->writeSetModeEnableBuffer();
    model->getILC()->triggerModbus();
    model->getILC()->waitForAllSubnets(5000);
    model->getILC()->readAll();
    model->getILC()->verifyResponses();
    model->getDigitalInputOutput()->turnAirOn();
    model->getPowerController()->setAllAuxPowerNetworks(true);
    return model->getSafetyController()->checkSafety(newState);
}

States::Type DisabledState::standby(StandbyCommand* command, Model* model) {
    spdlog::info("DisabledState: standby()");
    States::Type newState = States::StandbyState;
    model->getILC()->writeSetModeStandbyBuffer();
    model->getILC()->triggerModbus();
    model->getILC()->waitForAllSubnets(5000);
    model->getILC()->readAll();
    model->getILC()->verifyResponses();
    model->getPublisher()->tryLogForceActuatorState();
    model->getPowerController()->setBothPowerNetworks(false);
    return model->getSafetyController()->checkSafety(newState);
}

States::Type DisabledState::programILC(ProgramILCCommand* command, Model* model) {
    spdlog::info("DisabledState: programILC()");
    model->getILC()->programILC(command->getData()->actuatorId, command->getData()->filePath);
    return model->getSafetyController()->checkSafety(States::NoStateTransition);
}

States::Type DisabledState::modbusTransmit(ModbusTransmitCommand* command, Model* model) {
    spdlog::info("DisabledState: modbusTransmit()");
    model->getILC()->modbusTransmit(command->getData()->actuatorId, command->getData()->functionCode,
                                    command->getData()->dataLength, command->getData()->data);
    return model->getSafetyController()->checkSafety(States::NoStateTransition);
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
