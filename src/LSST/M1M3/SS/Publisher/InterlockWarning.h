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

#ifndef LSST_INTERLOCKWARNING_H
#define LSST_INTERLOCKWARNING_H

#include <SAL_MTM1M3.h>

#include <cRIO/Singleton.h>
#include <M1M3SSPublisher.h>

namespace LSST {
namespace M1M3 {
namespace SS {

/**
 * Wrapper object for MTM1M3_logevent_interlockWarningC.
 */
class InterlockWarning : public MTM1M3_logevent_interlockWarningC, public cRIO::Singleton<InterlockWarning> {
public:
    /**
     * Construct new InterlockWarning.
     */
    InterlockWarning(token) {
        timestamp = NAN;
        _currentInput = 0xFFFF;
        _heartbeatMismatch = -1;
    }

    /**
     * Sets interlock data. Publish changed data if change is detected.
     *
     * @param globalTimestamp actual timestamp
     * @param inputStates input states of slot 5 DIO module
     */
    void setData(double globalTimestamp, uint16_t inputStates) {
        timestamp = globalTimestamp;
        if (_currentInput != inputStates) {
            anyWarning = (inputStates & 0x00DB) != 0x00DB;  // this needs to be changed if new bits are added
            auxPowerNetworksOff = (inputStates & 0x0001) == 0;
            thermalEquipmentOff = (inputStates & 0x0002) == 0;
            airSupplyOff = (inputStates & 0x0008) == 0;
            cabinetDoorOpen = (inputStates & 0x0010) == 0;
            tmaMotionStop = (inputStates & 0x0040) == 0;
            gisHeartbeatLost = (inputStates & 0x0080) == 0;
            _currentInput = inputStates;
            M1M3SSPublisher::get().logInterlockWarning(this);
        }
    }

    /**
     *
     *
     * @param globalTimestamp actual timestamp
     * @param mismatched if true, heartbeat is
     */
    void setHearbeatOutputMismatch(double globalTimestamp, bool mismatched) {
        if (_heartbeatMismatch < 0 || _heartbeatMismatch != mismatched) {
            timestamp = globalTimestamp;
            _heartbeatMismatch = mismatched;
            heartbeatStateOutputMismatch = mismatched;
            M1M3SSPublisher::get().logInterlockWarning(this);
        }
    }

private:
    uint16_t _currentInput;
    int _heartbeatMismatch;
};

}  // namespace SS
}  // namespace M1M3
}  // namespace LSST

#endif  // LSST_INTERLOCKWARNING_H
