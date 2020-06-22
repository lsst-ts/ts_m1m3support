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

#include <StandbyState.h>
#include <ILC.h>
#include <DigitalInputOutput.h>
#include <Model.h>
#include <SafetyController.h>
#include <PowerController.h>
#include <StartCommand.h>
#include <Gyro.h>
#include <M1M3SSPublisher.h>
#include <unistd.h>

#include <spdlog/spdlog.h>

namespace LSST {
namespace M1M3 {
namespace SS {

StandbyState::StandbyState(M1M3SSPublisher* publisher) : State(publisher, "StandbyState") {}

States::Type StandbyState::update(UpdateCommand* command, Model* model) {
    spdlog::trace("StandbyState: update()");
    model->getDigitalInputOutput()->tryToggleHeartbeat();
    return States::NoStateTransition;
}

States::Type StandbyState::start(StartCommand* command, Model* model) {
    spdlog::info("StandbyState: start()");
    States::Type newState = States::DisabledState;
    model->loadSettings(command->getData()->settingsToApply);
    PowerController* powerController = model->getPowerController();
    ILC* ilc = model->getILC();
    DigitalInputOutput* digitalInputOutput = model->getDigitalInputOutput();
    Gyro* gyro = model->getGyro();

    powerController->setAllAuxPowerNetworks(false);
    powerController->setAllPowerNetworks(true);
    for (int i = 0; i < 2; ++i) {
        digitalInputOutput->tryToggleHeartbeat();
        usleep(500000);
    }

    ilc->flushAll();
    ilc->writeSetModeClearFaultsBuffer();
    ilc->triggerModbus();
    ilc->waitForAllSubnets(5000);
    ilc->readAll();
    digitalInputOutput->tryToggleHeartbeat();
    ilc->writeReportServerIDBuffer();
    ilc->triggerModbus();
    ilc->waitForAllSubnets(5000);
    ilc->readAll();
    digitalInputOutput->tryToggleHeartbeat();
    ilc->writeReportServerStatusBuffer();
    ilc->triggerModbus();
    ilc->waitForAllSubnets(5000);
    ilc->readAll();
    digitalInputOutput->tryToggleHeartbeat();
    ilc->writeReportADCScanRateBuffer();
    ilc->triggerModbus();
    ilc->waitForAllSubnets(5000);
    ilc->readAll();
    digitalInputOutput->tryToggleHeartbeat();
    ilc->writeReadCalibrationDataBuffer();
    ilc->triggerModbus();
    ilc->waitForAllSubnets(5000);
    ilc->readAll();
    digitalInputOutput->tryToggleHeartbeat();
    ilc->writeReadBoostValveDCAGainBuffer();
    ilc->triggerModbus();
    ilc->waitForAllSubnets(5000);
    ilc->readAll();
    digitalInputOutput->tryToggleHeartbeat();
    ilc->writeReportDCAIDBuffer();
    ilc->triggerModbus();
    ilc->waitForAllSubnets(5000);
    ilc->readAll();
    digitalInputOutput->tryToggleHeartbeat();
    ilc->writeReportDCAStatusBuffer();
    ilc->triggerModbus();
    ilc->waitForAllSubnets(5000);
    ilc->readAll();
    digitalInputOutput->tryToggleHeartbeat();
    ilc->writeSetModeDisableBuffer();
    ilc->triggerModbus();
    ilc->waitForAllSubnets(5000);
    ilc->readAll();
    digitalInputOutput->tryToggleHeartbeat();
    model->getPublisher()->tryLogForceActuatorState();
    usleep(20000);
    ilc->verifyResponses();
    ilc->publishForceActuatorInfo();
    ilc->publishHardpointActuatorInfo();
    ilc->publishHardpointMonitorInfo();
    gyro->enterConfigurationMode();
    gyro->resetConfiguration();
    gyro->setAxis();
    gyro->setDataRate();
    gyro->setRotationFormatRate();
    gyro->setRotationUnitsRadians();
    gyro->exitConfigurationMode();
    gyro->bit();
    digitalInputOutput->tryToggleHeartbeat();
    return model->getSafetyController()->checkSafety(newState);
}

States::Type StandbyState::shutdown(ShutdownCommand* command, Model* model) {
    spdlog::info("StandbyState: shutdown()");
    States::Type newState = States::OfflineState;
    model->shutdown();
    return newState;
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
