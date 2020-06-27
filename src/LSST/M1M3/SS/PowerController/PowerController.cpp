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

#include <PowerController.h>
#include <FPGAAddresses.h>
#include <SupportFPGAData.h>
#include <M1M3SSPublisher.h>
#include <SafetyController.h>
#include <Timestamp.h>
#include <SAL_MTM1M3C.h>
#include <spdlog/spdlog.h>

#include <cstring>

namespace LSST {
namespace M1M3 {
namespace SS {

PowerController::PowerController(M1M3SSPublisher* publisher, SafetyController* safetyController) {
    spdlog::debug("PowerController: PowerController()");
    _publisher = publisher;
    _safetyController = safetyController;

    _powerSupplyData = _publisher->getPowerSupplyData();
    _powerStatus = _publisher->getEventPowerStatus();
    _powerSupplyStatus = _publisher->getEventPowerSupplyStatus();
    _powerWarning = _publisher->getEventPowerWarning();

    _lastPowerTimestamp = 0;

    memset(_powerSupplyData, 0, sizeof(MTM1M3_powerSupplyDataC));
    memset(_powerStatus, 0, sizeof(MTM1M3_logevent_powerStatusC));
    memset(_powerSupplyStatus, 0, sizeof(MTM1M3_logevent_powerSupplyStatusC));
    memset(_powerWarning, 0, sizeof(MTM1M3_logevent_powerWarningC));
}

void PowerController::processData() {
    // TODO: Handle no data available
    // TODO: Handle limits, push to safety controller
    spdlog::trace("PowerController: processData()");
    SupportFPGAData* fpgaData = IFPGA::get().getSupportFPGAData();

    if (fpgaData->PowerSupplyTimestamp != _lastPowerTimestamp) {
        _lastPowerTimestamp = fpgaData->PowerSupplyTimestamp;
        double timestamp = Timestamp::fromFPGA(fpgaData->PowerSupplyTimestamp);
        _powerStatus->timestamp = timestamp;
        _powerStatus->auxPowerNetworkAOutputOn = (fpgaData->PowerSupplyStates & 0x01) != 0;
        _powerStatus->auxPowerNetworkBOutputOn = (fpgaData->PowerSupplyStates & 0x02) != 0;
        _powerStatus->auxPowerNetworkCOutputOn = (fpgaData->PowerSupplyStates & 0x04) != 0;
        _powerStatus->auxPowerNetworkDOutputOn = (fpgaData->PowerSupplyStates & 0x08) != 0;
        _powerStatus->powerNetworkAOutputOn = (fpgaData->PowerSupplyStates & 0x10) != 0;
        _powerStatus->powerNetworkBOutputOn = (fpgaData->PowerSupplyStates & 0x20) != 0;
        _powerStatus->powerNetworkCOutputOn = (fpgaData->PowerSupplyStates & 0x40) != 0;
        _powerStatus->powerNetworkDOutputOn = (fpgaData->PowerSupplyStates & 0x80) != 0;
        _publisher->tryLogPowerStatus();
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
        _publisher->tryLogPowerWarning();
    }
    double timestamp = _publisher->getTimestamp();
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
    _publisher->putPowerSupplyData();
}

void PowerController::setBothPowerNetworks(bool on) {
    spdlog::info("PowerController: setBothPowerNetworks({:d})", on);
    _powerStatus->powerNetworkACommandedOn = on;
    _powerStatus->powerNetworkBCommandedOn = on;
    _powerStatus->powerNetworkCCommandedOn = on;
    _powerStatus->powerNetworkDCommandedOn = on;
    _powerStatus->auxPowerNetworkACommandedOn = on;
    _powerStatus->auxPowerNetworkBCommandedOn = on;
    _powerStatus->auxPowerNetworkCCommandedOn = on;
    _powerStatus->auxPowerNetworkDCommandedOn = on;
    uint16_t buffer[16] = {
            FPGAAddresses::DCPowerNetworkAOn,    (uint16_t)_powerStatus->powerNetworkACommandedOn,
            FPGAAddresses::DCPowerNetworkBOn,    (uint16_t)_powerStatus->powerNetworkBCommandedOn,
            FPGAAddresses::DCPowerNetworkCOn,    (uint16_t)_powerStatus->powerNetworkCCommandedOn,
            FPGAAddresses::DCPowerNetworkDOn,    (uint16_t)_powerStatus->powerNetworkDCommandedOn,
            FPGAAddresses::DCAuxPowerNetworkAOn, (uint16_t)_powerStatus->auxPowerNetworkACommandedOn,
            FPGAAddresses::DCAuxPowerNetworkBOn, (uint16_t)_powerStatus->auxPowerNetworkBCommandedOn,
            FPGAAddresses::DCAuxPowerNetworkCOn, (uint16_t)_powerStatus->auxPowerNetworkCCommandedOn,
            FPGAAddresses::DCAuxPowerNetworkDOn, (uint16_t)_powerStatus->auxPowerNetworkDCommandedOn};
    IFPGA::get().writeCommandFIFO(buffer, 16, 0);
    _publisher->tryLogPowerStatus();
}

void PowerController::setAllPowerNetworks(bool on) {
    spdlog::info("PowerController: setAllPowerNetworks({:d})", on);
    _powerStatus->powerNetworkACommandedOn = on;
    _powerStatus->powerNetworkBCommandedOn = on;
    _powerStatus->powerNetworkCCommandedOn = on;
    _powerStatus->powerNetworkDCommandedOn = on;
    uint16_t buffer[8] = {
            FPGAAddresses::DCPowerNetworkAOn, (uint16_t)_powerStatus->powerNetworkACommandedOn,
            FPGAAddresses::DCPowerNetworkBOn, (uint16_t)_powerStatus->powerNetworkBCommandedOn,
            FPGAAddresses::DCPowerNetworkCOn, (uint16_t)_powerStatus->powerNetworkCCommandedOn,
            FPGAAddresses::DCPowerNetworkDOn, (uint16_t)_powerStatus->powerNetworkDCommandedOn};
    IFPGA::get().writeCommandFIFO(buffer, 8, 0);
    _publisher->tryLogPowerStatus();
}

void PowerController::setPowerNetworkA(bool on) {
    spdlog::info("PowerController: setPowerNetworkA({:d})", on);
    _powerStatus->powerNetworkACommandedOn = on;
    uint16_t buffer[2] = {FPGAAddresses::DCPowerNetworkAOn,
                          (uint16_t)_powerStatus->powerNetworkACommandedOn};
    IFPGA::get().writeCommandFIFO(buffer, 2, 0);
    _publisher->tryLogPowerStatus();
}

void PowerController::setPowerNetworkB(bool on) {
    spdlog::info("PowerController: setPowerNetworkB({:d})", on);
    _powerStatus->powerNetworkBCommandedOn = on;
    uint16_t buffer[2] = {FPGAAddresses::DCPowerNetworkBOn,
                          (uint16_t)_powerStatus->powerNetworkBCommandedOn};
    IFPGA::get().writeCommandFIFO(buffer, 2, 0);
    _publisher->tryLogPowerStatus();
}

void PowerController::setPowerNetworkC(bool on) {
    spdlog::info("PowerController: setPowerNetworkC({:d})", on);
    _powerStatus->powerNetworkCCommandedOn = on;
    uint16_t buffer[2] = {FPGAAddresses::DCPowerNetworkCOn,
                          (uint16_t)_powerStatus->powerNetworkCCommandedOn};
    IFPGA::get().writeCommandFIFO(buffer, 2, 0);
    _publisher->tryLogPowerStatus();
}

void PowerController::setPowerNetworkD(bool on) {
    spdlog::info("PowerController: setPowerNetworkD({:d})", on);
    _powerStatus->powerNetworkDCommandedOn = on;
    uint16_t buffer[2] = {FPGAAddresses::DCPowerNetworkDOn,
                          (uint16_t)_powerStatus->powerNetworkDCommandedOn};
    IFPGA::get().writeCommandFIFO(buffer, 2, 0);
    _publisher->tryLogPowerStatus();
}

void PowerController::setAllAuxPowerNetworks(bool on) {
    spdlog::info("PowerController: setAllAuxPowerNetworks({:d})", on);
    _powerStatus->auxPowerNetworkACommandedOn = on;
    _powerStatus->auxPowerNetworkBCommandedOn = on;
    _powerStatus->auxPowerNetworkCCommandedOn = on;
    _powerStatus->auxPowerNetworkDCommandedOn = on;
    uint16_t buffer[8] = {
            FPGAAddresses::DCAuxPowerNetworkAOn, (uint16_t)_powerStatus->auxPowerNetworkACommandedOn,
            FPGAAddresses::DCAuxPowerNetworkBOn, (uint16_t)_powerStatus->auxPowerNetworkBCommandedOn,
            FPGAAddresses::DCAuxPowerNetworkCOn, (uint16_t)_powerStatus->auxPowerNetworkCCommandedOn,
            FPGAAddresses::DCAuxPowerNetworkDOn, (uint16_t)_powerStatus->auxPowerNetworkDCommandedOn};
    IFPGA::get().writeCommandFIFO(buffer, 8, 0);
    _publisher->tryLogPowerStatus();
}

void PowerController::setAuxPowerNetworkA(bool on) {
    spdlog::info("PowerController: setAuxPowerNetworkA({:d})", on);
    _powerStatus->auxPowerNetworkACommandedOn = on;
    uint16_t buffer[2] = {FPGAAddresses::DCAuxPowerNetworkAOn,
                          (uint16_t)_powerStatus->auxPowerNetworkACommandedOn};
    IFPGA::get().writeCommandFIFO(buffer, 2, 0);
    _publisher->tryLogPowerStatus();
}

void PowerController::setAuxPowerNetworkB(bool on) {
    spdlog::info("PowerController: setAuxPowerNetworkB({:d})", on);
    _powerStatus->auxPowerNetworkBCommandedOn = on;
    uint16_t buffer[2] = {FPGAAddresses::DCAuxPowerNetworkBOn,
                          (uint16_t)_powerStatus->auxPowerNetworkBCommandedOn};
    IFPGA::get().writeCommandFIFO(buffer, 2, 0);
    _publisher->tryLogPowerStatus();
}

void PowerController::setAuxPowerNetworkC(bool on) {
    spdlog::info("PowerController: setAuxPowerNetworkC({:d})", on);
    _powerStatus->auxPowerNetworkCCommandedOn = on;
    uint16_t buffer[2] = {FPGAAddresses::DCAuxPowerNetworkCOn,
                          (uint16_t)_powerStatus->auxPowerNetworkCCommandedOn};
    IFPGA::get().writeCommandFIFO(buffer, 2, 0);
    _publisher->tryLogPowerStatus();
}

void PowerController::setAuxPowerNetworkD(bool on) {
    spdlog::info("PowerController: setAuxPowerNetworkD({:d})", on);
    _powerStatus->auxPowerNetworkDCommandedOn = on;
    uint16_t buffer[2] = {FPGAAddresses::DCAuxPowerNetworkDOn,
                          (uint16_t)_powerStatus->auxPowerNetworkDCommandedOn};
    IFPGA::get().writeCommandFIFO(buffer, 2, 0);
    _publisher->tryLogPowerStatus();
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
