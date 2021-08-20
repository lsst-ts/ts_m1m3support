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

#include <Gyro.h>
#include <GyroSettings.h>
#include <IFPGA.h>
#include <FPGAAddresses.h>
#include <SupportFPGAData.h>
#include <M1M3SSPublisher.h>
#include <Timestamp.h>
#include <SAL_MTM1M3C.h>
#include <spdlog/spdlog.h>

#include <cstring>
#include <thread>
#include <chrono>

namespace LSST {
namespace M1M3 {
namespace SS {

Gyro::Gyro(GyroSettings* gyroSettings) {
    SPDLOG_DEBUG("Gyro: Gyro()");
    _gyroSettings = gyroSettings;

    _gyroData = M1M3SSPublisher::get().getGyroData();
    _gyroWarning = M1M3SSPublisher::get().getEventGyroWarning();

    _lastBITTimestamp = 0;
    _lastErrorTimestamp = 0;
    _lastSampleTimestamp = 0;

    _errorCleared = false;

    memset(_gyroData, 0, sizeof(MTM1M3_gyroDataC));
    memset(_gyroWarning, 0, sizeof(MTM1M3_logevent_gyroWarningC));
}

void Gyro::bit() {
    SPDLOG_INFO("Gyro: bit()");
    this->writeCommand("?BIT,2\r\n");
    std::this_thread::sleep_for(10ms);
}

void Gyro::enterConfigurationMode() {
    SPDLOG_INFO("Gyro: enterConfigurationMode()");
    this->writeCommand("=CONFIG,1\r\n");
    std::this_thread::sleep_for(10ms);
}

void Gyro::exitConfigurationMode() {
    SPDLOG_INFO("Gyro: exitConfigurationMode()");
    this->writeCommand("=CONFIG,0\r\n");
    std::this_thread::sleep_for(10ms);
}

void Gyro::resetConfiguration() {
    SPDLOG_INFO("Gyro: resetConfiguration()");
    this->writeCommand("=RSTCFG\r\n");
    std::this_thread::sleep_for(10ms);
}

void Gyro::setRotationFormatRate() {
    SPDLOG_INFO("Gyro: setRotationFormatRate()");
    this->writeCommand("=ROTFMT,RATE\r\n");
    std::this_thread::sleep_for(10ms);
}

void Gyro::setRotationUnitsRadians() {
    SPDLOG_INFO("Gyro: setRotationUnitsRadians()");
    this->writeCommand("=ROTUNITS,RAD\r\n");
    std::this_thread::sleep_for(10ms);
}

void Gyro::setAxis() {
    SPDLOG_INFO("Gyro: setAxis()");
    std::string command = "=AXES";
    for (unsigned int i = 0; i < _gyroSettings->AxesMatrix.size(); ++i) {
        command = command + "," + std::to_string(_gyroSettings->AxesMatrix[i]);
    }
    this->writeCommand(command + "\r\n");
    std::this_thread::sleep_for(10ms);
}

void Gyro::setDataRate() {
    SPDLOG_INFO("Gyro: setDataRate()");
    this->writeCommand("=DR," + std::to_string(_gyroSettings->DataRate) + "\r\n");
    std::this_thread::sleep_for(10ms);
}

void Gyro::processData() {
    // TODO: Handle no data available
    // TODO: Handle limits, push to safety controller
    SPDLOG_TRACE("Gyro: processData()");
    bool tryLogWarning = false;
    SupportFPGAData* fpgaData = IFPGA::get().getSupportFPGAData();
    if (fpgaData->GyroErrorTimestamp > _lastErrorTimestamp) {
        _lastErrorTimestamp = fpgaData->GyroErrorTimestamp;
        _errorCleared = false;
        _gyroWarning->timestamp = Timestamp::fromFPGA(fpgaData->GyroErrorTimestamp);
        _gyroWarning->invalidHeaderWarning = fpgaData->GyroErrorCode == 1;
        _gyroWarning->crcMismatchWarning = fpgaData->GyroErrorCode == 2 || fpgaData->GyroErrorCode == 4;
        _gyroWarning->incompleteFrameWarning = fpgaData->GyroErrorCode == 3;
        // TODO: Add Checksum Error
        tryLogWarning = true;
    }
    if (fpgaData->GyroBITTimestamp > _lastBITTimestamp) {
        _lastBITTimestamp = fpgaData->GyroBITTimestamp;
        _gyroWarning->timestamp = Timestamp::fromFPGA(fpgaData->GyroBITTimestamp);
        uint8_t word0 = fpgaData->GyroBIT0;
        _gyroWarning->gyroXSLDWarning = (word0 & 0x01) == 0;
        _gyroWarning->gyroXMODDACWarning = (word0 & 0x02) == 0;
        _gyroWarning->gyroXPhaseWarning = (word0 & 0x04) == 0;
        _gyroWarning->gyroXFlashWarning = (word0 & 0x08) == 0;
        _gyroWarning->gyroYSLDWarning = (word0 & 0x10) == 0;
        _gyroWarning->gyroYMODDACWarning = (word0 & 0x20) == 0;
        _gyroWarning->gyroYPhaseWarning = (word0 & 0x40) == 0;
        uint8_t word1 = fpgaData->GyroBIT1;
        _gyroWarning->gyroYFlashWarning = (word1 & 0x01) == 0;
        _gyroWarning->gyroZSLDWarning = (word1 & 0x02) == 0;
        _gyroWarning->gyroZMODDACWarning = (word1 & 0x04) == 0;
        _gyroWarning->gyroZPhaseWarning = (word1 & 0x08) == 0;
        uint8_t word2 = fpgaData->GyroBIT2;
        _gyroWarning->gyroXSLDTemperatureStatusWarning = (word2 & 0x04) == 0;
        _gyroWarning->gyroYSLDTemperatureStatusWarning = (word2 & 0x10) == 0;
        _gyroWarning->gyroZSLDTemperatureStatusWarning = (word2 & 0x40) == 0;
        uint8_t word3 = fpgaData->GyroBIT3;
        _gyroWarning->gcbTemperatureStatusWarning = (word3 & 0x08) == 0;
        _gyroWarning->temperatureStatusWarning = (word3 & 0x10) == 0;
        _gyroWarning->gcbDSPSPIFlashStatusWarning = (word3 & 0x20) == 0;
        _gyroWarning->gcbFPGASPIFlashStatusWarning = (word3 & 0x40) == 0;
        uint8_t word4 = fpgaData->GyroBIT4;
        _gyroWarning->dspSPIFlashStatusWarning = (word4 & 0x01) == 0;
        _gyroWarning->fpgaSPIFlashStatusWarning = (word4 & 0x02) == 0;
        _gyroWarning->gcb1_2VStatusWarning = (word4 & 0x04) == 0;
        _gyroWarning->gcb3_3VStatusWarning = (word4 & 0x08) == 0;
        _gyroWarning->gcb5VStatusWarning = (word4 & 0x10) == 0;
        _gyroWarning->v1_2StatusWarning = (word4 & 0x20) == 0;
        _gyroWarning->v3_3StatusWarning = (word4 & 0x40) == 0;
        uint8_t word5 = fpgaData->GyroBIT5;
        _gyroWarning->v5StatusWarning = (word5 & 0x01) == 0;
        _gyroWarning->gcbFPGAStatusWarning = (word5 & 0x04) == 0;
        _gyroWarning->fpgaStatusWarning = (word5 & 0x08) == 0;
        _gyroWarning->hiSpeedSPORTStatusWarning = (word5 & 0x10) == 0;
        _gyroWarning->auxSPORTStatusWarning = (word5 & 0x20) == 0;
        _gyroWarning->sufficientSoftwareResourcesWarning = (word5 & 0x40) == 0;
        uint8_t word6 = fpgaData->GyroBIT6;
        _gyroWarning->gyroEOVoltsPositiveWarning = (word6 & 0x01) == 0;
        _gyroWarning->gyroEOVoltsNegativeWarning = (word6 & 0x02) == 0;
        _gyroWarning->gyroXVoltsWarning = (word6 & 0x04) == 0;
        _gyroWarning->gyroYVoltsWarning = (word6 & 0x08) == 0;
        _gyroWarning->gyroZVoltsWarning = (word6 & 0x10) == 0;
        uint8_t word7 = fpgaData->GyroBIT7;
        _gyroWarning->gcbADCCommsWarning = (word7 & 0x01) == 0;
        _gyroWarning->mSYNCExternalTimingWarning = (word7 & 0x02) == 0;
        tryLogWarning = true;
    }
    if (fpgaData->GyroSampleTimestamp > _lastSampleTimestamp) {
        _lastSampleTimestamp = fpgaData->GyroSampleTimestamp;
        _gyroData->timestamp = Timestamp::fromFPGA(fpgaData->GyroSampleTimestamp);
        _gyroData->angularVelocityX = fpgaData->GyroRawX + _gyroSettings->AngularVelocityXOffset;
        _gyroData->angularVelocityY = fpgaData->GyroRawY + _gyroSettings->AngularVelocityYOffset;
        _gyroData->angularVelocityZ = fpgaData->GyroRawZ + _gyroSettings->AngularVelocityZOffset;
        _gyroData->sequenceNumber = fpgaData->GyroSequenceNumber;
        _gyroData->temperature = fpgaData->GyroTemperature;
        uint8_t status = fpgaData->GyroStatus;
        _gyroWarning->gyroXStatusWarning = (status & 0x01) == 0;
        _gyroWarning->gyroYStatusWarning = (status & 0x02) == 0;
        _gyroWarning->gyroZStatusWarning = (status & 0x04) == 0;
        M1M3SSPublisher::get().putGyroData();
        tryLogWarning = true;
        if (!_errorCleared && fpgaData->GyroSampleTimestamp > _lastErrorTimestamp) {
            _lastErrorTimestamp = fpgaData->GyroErrorTimestamp;
            _errorCleared = true;
            _gyroWarning->timestamp = Timestamp::fromFPGA(fpgaData->GyroSampleTimestamp);
            _gyroWarning->invalidHeaderWarning = false;
            _gyroWarning->crcMismatchWarning = false;
            _gyroWarning->incompleteFrameWarning = false;
            // TODO: Add Checksum Error
            tryLogWarning = true;
        }
    }
    if (tryLogWarning) {
        M1M3SSPublisher::get().tryLogGyroWarning();
    }
}

void Gyro::writeCommand(std::string command) {
    uint16_t buffer[256];
    memset(buffer, 0, sizeof(buffer));
    int32_t size = (int32_t)command.size();
    buffer[0] = FPGAAddresses::GyroTx;
    buffer[1] = (uint16_t)size;
    for (int i = 0; i < size; ++i) {
        buffer[i + 2] = (uint16_t)command[i];
    }
    IFPGA::get().writeCommandFIFO(buffer, size + 2, 0);
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
