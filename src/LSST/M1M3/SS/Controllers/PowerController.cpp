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

#include <PowerController.h>
#include <FPGAAddresses.h>
#include <SupportFPGAData.h>
#include <M1M3SSPublisher.h>
#include <SafetyController.h>
#include <Timestamp.h>
#include <SAL_MTM1M3C.h>
#include <spdlog/spdlog.h>

#include <cstring>

using namespace LSST::M1M3::SS;
using namespace LSST::M1M3::SS::FPGAAddresses;

PowerController::PowerController(SafetyController* safetyController) {
    SPDLOG_DEBUG("PowerController: PowerController()");
    _safetyController = safetyController;

    _powerSupplyData = M1M3SSPublisher::get().getPowerSupplyData();
    _powerStatus = M1M3SSPublisher::get().getEventPowerStatus();
    _powerWarning = M1M3SSPublisher::get().getEventPowerWarning();

    _lastPowerTimestamp = 0;

    memset(_powerSupplyData, 0, sizeof(MTM1M3_powerSupplyDataC));
    memset(_powerStatus, 0, sizeof(MTM1M3_logevent_powerStatusC));
    memset(_powerWarning, 0, sizeof(MTM1M3_logevent_powerWarningC));
}

void PowerController::processData() {
    // TODO: Handle no data available
    // TODO: Handle limits, push to safety controller
    SPDLOG_TRACE("PowerController: processData()");
    SupportFPGAData* fpgaData = IFPGA::get().getSupportFPGAData();

    if (fpgaData->PowerSupplyTimestamp != _lastPowerTimestamp) {
        _lastPowerTimestamp = fpgaData->PowerSupplyTimestamp;
        double timestamp = Timestamp::fromFPGA(fpgaData->PowerSupplyTimestamp);
        _powerStatus->timestamp = timestamp;
        _powerStatus->auxPowerNetworkAOutputOn = (fpgaData->PowerSupplyStates & PowerSupply::AuxA) != 0;
        _powerStatus->auxPowerNetworkBOutputOn = (fpgaData->PowerSupplyStates & PowerSupply::AuxB) != 0;
        _powerStatus->auxPowerNetworkCOutputOn = (fpgaData->PowerSupplyStates & PowerSupply::AuxC) != 0;
        _powerStatus->auxPowerNetworkDOutputOn = (fpgaData->PowerSupplyStates & PowerSupply::AuxD) != 0;
        _powerStatus->powerNetworkAOutputOn = (fpgaData->PowerSupplyStates & PowerSupply::A) != 0;
        _powerStatus->powerNetworkBOutputOn = (fpgaData->PowerSupplyStates & PowerSupply::B) != 0;
        _powerStatus->powerNetworkCOutputOn = (fpgaData->PowerSupplyStates & PowerSupply::C) != 0;
        _powerStatus->powerNetworkDOutputOn = (fpgaData->PowerSupplyStates & PowerSupply::D) != 0;
        M1M3SSPublisher::get().tryLogPowerStatus();
        _powerWarning->timestamp = timestamp;
        _powerWarning->auxPowerNetworkAOutputMismatch =
                _powerStatus->auxPowerNetworkACommandedOn != _powerStatus->auxPowerNetworkAOutputOn;
        _powerWarning->auxPowerNetworkBOutputMismatch =
                _powerStatus->auxPowerNetworkBCommandedOn != _powerStatus->auxPowerNetworkBOutputOn;
        _powerWarning->auxPowerNetworkCOutputMismatch =
                _powerStatus->auxPowerNetworkCCommandedOn != _powerStatus->auxPowerNetworkCOutputOn;
        _powerWarning->auxPowerNetworkDOutputMismatch =
                _powerStatus->auxPowerNetworkDCommandedOn != _powerStatus->auxPowerNetworkDOutputOn;
        _powerWarning->powerNetworkAOutputMismatch =
                _powerStatus->powerNetworkACommandedOn != _powerStatus->powerNetworkAOutputOn;
        _powerWarning->powerNetworkBOutputMismatch =
                _powerStatus->powerNetworkBCommandedOn != _powerStatus->powerNetworkBOutputOn;
        _powerWarning->powerNetworkCOutputMismatch =
                _powerStatus->powerNetworkCCommandedOn != _powerStatus->powerNetworkCOutputOn;
        _powerWarning->powerNetworkDOutputMismatch =
                _powerStatus->powerNetworkDCommandedOn != _powerStatus->powerNetworkDOutputOn;
        M1M3SSPublisher::get().tryLogPowerWarning();
    }
    double timestamp = M1M3SSPublisher::get().getTimestamp();
    IExpansionFPGA::get().sample();
    float sample[6] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0};

    IExpansionFPGA::get().readSlot1(sample);
    _powerSupplyData->timestamp = timestamp;
    _powerSupplyData->powerNetworkACurrent = sample[0];
    _powerSupplyData->powerNetworkBCurrent = sample[1];
    _powerSupplyData->powerNetworkCCurrent = sample[2];
    _powerSupplyData->powerNetworkDCurrent = sample[3];
    _powerSupplyData->lightPowerNetworkCurrent = sample[4];
    _powerSupplyData->controlsPowerNetworkCurrent = sample[5];
    M1M3SSPublisher::get().putPowerSupplyData();

    uint32_t powerStatus;
    IExpansionFPGA::get().readSlot2(&powerStatus);
    M1M3SSPublisher::get().getPowerSupplyStatus()->setPowerSupplyStatus(powerStatus);
}

void PowerController::setBothPowerNetworks(bool on) {
    SPDLOG_INFO("PowerController: setBothPowerNetworks({:d})", on);
    _powerStatus->powerNetworkACommandedOn = on;
    _powerStatus->powerNetworkBCommandedOn = on;
    _powerStatus->powerNetworkCCommandedOn = on;
    _powerStatus->powerNetworkDCommandedOn = on;
    _powerStatus->auxPowerNetworkACommandedOn = on;
    _powerStatus->auxPowerNetworkBCommandedOn = on;
    _powerStatus->auxPowerNetworkCCommandedOn = on;
    _powerStatus->auxPowerNetworkDCommandedOn = on;
    uint16_t buffer[16] = {DCPowerNetworkAOn,    (uint16_t)_powerStatus->powerNetworkACommandedOn,
                           DCPowerNetworkBOn,    (uint16_t)_powerStatus->powerNetworkBCommandedOn,
                           DCPowerNetworkCOn,    (uint16_t)_powerStatus->powerNetworkCCommandedOn,
                           DCPowerNetworkDOn,    (uint16_t)_powerStatus->powerNetworkDCommandedOn,
                           DCAuxPowerNetworkAOn, (uint16_t)_powerStatus->auxPowerNetworkACommandedOn,
                           DCAuxPowerNetworkBOn, (uint16_t)_powerStatus->auxPowerNetworkBCommandedOn,
                           DCAuxPowerNetworkCOn, (uint16_t)_powerStatus->auxPowerNetworkCCommandedOn,
                           DCAuxPowerNetworkDOn, (uint16_t)_powerStatus->auxPowerNetworkDCommandedOn};
    IFPGA::get().writeCommandFIFO(buffer, 16, 0);
    M1M3SSPublisher::get().tryLogPowerStatus();
}

void PowerController::setAllPowerNetworks(bool on) {
    SPDLOG_INFO("PowerController: setAllPowerNetworks({:d})", on);
    _powerStatus->powerNetworkACommandedOn = on;
    _powerStatus->powerNetworkBCommandedOn = on;
    _powerStatus->powerNetworkCCommandedOn = on;
    _powerStatus->powerNetworkDCommandedOn = on;
    uint16_t buffer[8] = {DCPowerNetworkAOn, (uint16_t)_powerStatus->powerNetworkACommandedOn,
                          DCPowerNetworkBOn, (uint16_t)_powerStatus->powerNetworkBCommandedOn,
                          DCPowerNetworkCOn, (uint16_t)_powerStatus->powerNetworkCCommandedOn,
                          DCPowerNetworkDOn, (uint16_t)_powerStatus->powerNetworkDCommandedOn};
    IFPGA::get().writeCommandFIFO(buffer, 8, 0);
    M1M3SSPublisher::get().tryLogPowerStatus();
}

void PowerController::setPowerNetworkA(bool on) {
    SPDLOG_INFO("PowerController: setPowerNetworkA({:d})", on);
    _powerStatus->powerNetworkACommandedOn = on;
    uint16_t buffer[2] = {DCPowerNetworkAOn, (uint16_t)_powerStatus->powerNetworkACommandedOn};
    IFPGA::get().writeCommandFIFO(buffer, 2, 0);
    M1M3SSPublisher::get().tryLogPowerStatus();
}

void PowerController::setPowerNetworkB(bool on) {
    SPDLOG_INFO("PowerController: setPowerNetworkB({:d})", on);
    _powerStatus->powerNetworkBCommandedOn = on;
    uint16_t buffer[2] = {DCPowerNetworkBOn, (uint16_t)_powerStatus->powerNetworkBCommandedOn};
    IFPGA::get().writeCommandFIFO(buffer, 2, 0);
    M1M3SSPublisher::get().tryLogPowerStatus();
}

void PowerController::setPowerNetworkC(bool on) {
    SPDLOG_INFO("PowerController: setPowerNetworkC({:d})", on);
    _powerStatus->powerNetworkCCommandedOn = on;
    uint16_t buffer[2] = {DCPowerNetworkCOn, (uint16_t)_powerStatus->powerNetworkCCommandedOn};
    IFPGA::get().writeCommandFIFO(buffer, 2, 0);
    M1M3SSPublisher::get().tryLogPowerStatus();
}

void PowerController::setPowerNetworkD(bool on) {
    SPDLOG_INFO("PowerController: setPowerNetworkD({:d})", on);
    _powerStatus->powerNetworkDCommandedOn = on;
    uint16_t buffer[2] = {DCPowerNetworkDOn, (uint16_t)_powerStatus->powerNetworkDCommandedOn};
    IFPGA::get().writeCommandFIFO(buffer, 2, 0);
    M1M3SSPublisher::get().tryLogPowerStatus();
}

void PowerController::setAllAuxPowerNetworks(bool on) {
    SPDLOG_INFO("PowerController: setAllAuxPowerNetworks({:d})", on);
    _powerStatus->auxPowerNetworkACommandedOn = on;
    _powerStatus->auxPowerNetworkBCommandedOn = on;
    _powerStatus->auxPowerNetworkCCommandedOn = on;
    _powerStatus->auxPowerNetworkDCommandedOn = on;
    uint16_t buffer[8] = {DCAuxPowerNetworkAOn, (uint16_t)_powerStatus->auxPowerNetworkACommandedOn,
                          DCAuxPowerNetworkBOn, (uint16_t)_powerStatus->auxPowerNetworkBCommandedOn,
                          DCAuxPowerNetworkCOn, (uint16_t)_powerStatus->auxPowerNetworkCCommandedOn,
                          DCAuxPowerNetworkDOn, (uint16_t)_powerStatus->auxPowerNetworkDCommandedOn};
    IFPGA::get().writeCommandFIFO(buffer, 8, 0);
    M1M3SSPublisher::get().tryLogPowerStatus();
}

void PowerController::setAuxPowerNetworkA(bool on) {
    SPDLOG_INFO("PowerController: setAuxPowerNetworkA({:d})", on);
    _powerStatus->auxPowerNetworkACommandedOn = on;
    uint16_t buffer[2] = {DCAuxPowerNetworkAOn, (uint16_t)_powerStatus->auxPowerNetworkACommandedOn};
    IFPGA::get().writeCommandFIFO(buffer, 2, 0);
    M1M3SSPublisher::get().tryLogPowerStatus();
}

void PowerController::setAuxPowerNetworkB(bool on) {
    SPDLOG_INFO("PowerController: setAuxPowerNetworkB({:d})", on);
    _powerStatus->auxPowerNetworkBCommandedOn = on;
    uint16_t buffer[2] = {DCAuxPowerNetworkBOn, (uint16_t)_powerStatus->auxPowerNetworkBCommandedOn};
    IFPGA::get().writeCommandFIFO(buffer, 2, 0);
    M1M3SSPublisher::get().tryLogPowerStatus();
}

void PowerController::setAuxPowerNetworkC(bool on) {
    SPDLOG_INFO("PowerController: setAuxPowerNetworkC({:d})", on);
    _powerStatus->auxPowerNetworkCCommandedOn = on;
    uint16_t buffer[2] = {DCAuxPowerNetworkCOn, (uint16_t)_powerStatus->auxPowerNetworkCCommandedOn};
    IFPGA::get().writeCommandFIFO(buffer, 2, 0);
    M1M3SSPublisher::get().tryLogPowerStatus();
}

void PowerController::setAuxPowerNetworkD(bool on) {
    SPDLOG_INFO("PowerController: setAuxPowerNetworkD({:d})", on);
    _powerStatus->auxPowerNetworkDCommandedOn = on;
    uint16_t buffer[2] = {DCAuxPowerNetworkDOn, (uint16_t)_powerStatus->auxPowerNetworkDCommandedOn};
    IFPGA::get().writeCommandFIFO(buffer, 2, 0);
    M1M3SSPublisher::get().tryLogPowerStatus();
}
