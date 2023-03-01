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

#ifndef LSST_HARDPOINTACTUATORSETTINGS_H
#define LSST_HARDPOINTACTUATORSETTINGS_H

#include <SAL_MTM1M3.h>

#include <cRIO/Singleton.h>
#include <M1M3SSPublisher.h>

#include "DataTypes.h"

namespace LSST {
namespace M1M3 {
namespace SS {

class HardpointActuatorWarning : public MTM1M3_logevent_hardpointActuatorWarningC,
                                 public cRIO::Singleton<HardpointActuatorWarning> {
public:
    HardpointActuatorWarning(token);

    void send();

    void setStatus(int32_t hpIndex, double _timestamp, uint8_t status, int broadcastCounter);
    void setIlcStatus(int32_t hpIndex, uint16_t ilcStatus, uint16_t ilcFaults);
    void setProximityWarning(int32_t hpIndex, bool lowWarning, bool highWarning);
    void setAirPressure(int32_t hpIndex, bool lowWarning, bool highWarning, float airPressure);

    bool anyAirLowPressureFault;
    bool anyAirHighPressureFault;
    bool waitingForAirPressureBeforeRaise;

private:
    bool _updated;

    uint16_t _ilcOldStatus[HP_COUNT];
    uint16_t _ilcOldFaults[HP_COUNT];
    bool _airPressureLowWarning[HP_COUNT];
    bool _airPressureHighWarning[HP_COUNT];
};

}  // namespace SS
}  // namespace M1M3
}  // namespace LSST

#endif  // LSST_HARDPOINTACTUATORSETTINGS_H
