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

#ifndef LSST_FORCEACTUATORINFO_H
#define LSST_FORCEACTUATORINFO_H

#include <SAL_MTM1M3.h>

#include <cRIO/Singleton.h>

#include <ForceActuatorApplicationSettings.h>
#include <M1M3SSPublisher.h>
#include <ModbusBuffer.h>

namespace LSST {
namespace M1M3 {
namespace SS {

class ForceActuatorInfo : public MTM1M3_logevent_forceActuatorInfoC,
                          public cRIO::Singleton<ForceActuatorInfo> {
public:
    ForceActuatorInfo(token);

    void log() { M1M3SSPublisher::instance().logForceActuatorInfo(this); }

    void populate(ForceActuatorApplicationSettings* forceActuatorApplicationSettings);

    void serverIDResponse(int32_t dataIndex, uint64_t _ilcUniqueId, uint8_t _ilcApplicationType,
                          uint8_t _networkNodeType, uint8_t _ilcSelectedOptions, uint8_t _networkNodeOptions,
                          uint8_t _majorRevision, uint8_t _minorRevision);

    void boosterValveDCAGains(int32_t dataIndex, float _mezzaninePrimaryCylinderGain,
                              float _mezzanineSecondaryCylinderGain);

    void setFACalibration(int32_t dataIndex, uint8_t _adcScanRate);
    void parseFAADCScanRate(int32_t dataIndex, ModbusBuffer* buffer);
    void setDCAID(int32_t dataIndex, uint32_t _mezzanineUniqueId, uint8_t _mezzanineFirmwareType,
                  uint8_t _mezzanineMajorRevision, uint8_t _mezzanineMinorRevision);
};

}  // namespace SS
}  // namespace M1M3
}  // namespace LSST

#endif  // !LSST_FORCEACTUATORINFO_H
