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

#include <thread>
#include <chrono>
#include <spdlog/spdlog.h>

#include <BoosterValveStatus.h>
#include <DigitalInputOutput.h>
#include <Gyro.h>
#include <ILC.h>
#include <Model.h>
#include <PowerController.h>
#include <SafetyController.h>
#include <SettingReader.h>
#include <StandbyState.h>

namespace LSST {
namespace M1M3 {
namespace SS {

StandbyState::StandbyState() : State("StandbyState") {}

States::Type StandbyState::update(UpdateCommand* command) {
    SPDLOG_TRACE("StandbyState: update()");
    Model::get().getDigitalInputOutput()->tryToggleHeartbeat();
    return States::NoStateTransition;
}

States::Type StandbyState::start(StartCommand* command) {
    SPDLOG_INFO("StandbyState: start()");
    SettingReader::instance().getSafetyControllerSettings()->ForceController.exitBumpTesting();

    Model::get().loadSettings(command->getConfigurationOverride());
    M1M3SSPublisher::instance().getEventConfigurationApplied()->version =
            SettingReader::instance().getSettingsVersion();
    M1M3SSPublisher::instance().getEventConfigurationApplied()->otherInfo = "";
    M1M3SSPublisher::instance().logConfigurationApplied();

    PowerController* powerController = Model::get().getPowerController();
    ILC* ilc = Model::get().getILC();
    DigitalInputOutput* digitalInputOutput = Model::get().getDigitalInputOutput();
    Gyro* gyro = Model::get().getGyro();

    digitalInputOutput->tryToggleHeartbeat();
    std::this_thread::sleep_for(1ms);  // wait for GIS to sense heartbeat

    IFPGA::get().pullTelemetry();
    digitalInputOutput->processData();
    powerController->processData();

    powerController->setAllAuxPowerNetworks(false);
    powerController->setAllPowerNetworks(true);

    for (int i = 0; i < 2; i++) {
        digitalInputOutput->tryToggleHeartbeat();
        std::this_thread::sleep_for(500ms);  // wait 2*0.5=1 second for ILCs to power on
    }
    digitalInputOutput->tryToggleHeartbeat();

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
    M1M3SSPublisher::instance().getEnabledForceActuators()->log();
    M1M3SSPublisher::instance().tryLogForceActuatorState();
    std::this_thread::sleep_for(20ms);
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
    BoosterValveStatus::instance().setSlewFlag(false);
    BoosterValveStatus::instance().log(true);
    digitalInputOutput->tryToggleHeartbeat();
    return Model::get().getSafetyController()->checkSafety(States::DisabledState);
}

States::Type StandbyState::exitControl(ExitControlCommand* command) {
    SPDLOG_INFO("StandbyState: ExitControl()");
    Model::get().exitControl();
    return States::OfflineState;
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
