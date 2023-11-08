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

#include "ILCWarning.h"
#include "M1M3SSPublisher.h"

using namespace LSST::M1M3::SS;

ILCWarningTimeout::ILCWarningTimeout() {
    memset(hpILCs, 0, sizeof(hpILCs));
    memset(monitorILCs, 0, sizeof(monitorILCs));
    memset(faILCs, 0, sizeof(faILCs));
}

ILCWarning::ILCWarning(token) {
    timestamp = 0;
    actuatorId = -1;
    responseTimeout = false;
    invalidCRC = false;
    illegalFunction = false;
    illegalDataValue = false;
    invalidLength = false;
    unknownSubnet = false;
    unknownAddress = false;
    unknownFunction = false;
    unknownProblem = false;
}

void ILCWarning::warnResponseTimeout(double _timestamp, int32_t _actuatorId) {
    if (_responseTimeout[_actuatorId]
    timestamp = _timestamp;
    actuatorId = _actuatorId;
    responseTimeout = true;
    invalidCRC = false;
    illegalFunction = false;
    illegalDataValue = false;
    invalidLength = false;
    unknownSubnet = false;
    unknownAddress = false;
    unknownFunction = false;
    unknownProblem = false;
    M1M3SSPublisher::instance().logILCWarning(this);
}

void ILCWarning::warnInvalidCRC(double _timestamp) {
    timestamp = _timestamp;
    actuatorId = -1;
    responseTimeout = false;
    invalidCRC = true;
    illegalFunction = false;
    illegalDataValue = false;
    invalidLength = false;
    unknownSubnet = false;
    unknownAddress = false;
    unknownFunction = false;
    unknownProblem = false;
    M1M3SSPublisher::instance().logILCWarning(this);
}

void ILCWarning::warnIllegalFunction(double _timestamp, int32_t _actuatorId) {
    timestamp = _timestamp;
    actuatorId = _actuatorId;
    responseTimeout = false;
    invalidCRC = false;
    illegalFunction = true;
    illegalDataValue = false;
    invalidLength = false;
    unknownSubnet = false;
    unknownAddress = false;
    unknownFunction = false;
    unknownProblem = false;
    M1M3SSPublisher::instance().logILCWarning(this);
}

void ILCWarning::warnIllegalDataValue(double _timestamp, int32_t _actuatorId) {
    timestamp = _timestamp;
    actuatorId = _actuatorId;
    responseTimeout = false;
    invalidCRC = false;
    illegalFunction = false;
    illegalDataValue = true;
    invalidLength = false;
    unknownSubnet = false;
    unknownAddress = false;
    unknownFunction = false;
    unknownProblem = false;
    M1M3SSPublisher::instance().logILCWarning(this);
}

void ILCWarning::warnInvalidLength(double _timestamp, int32_t _actuatorId) {
    timestamp = _timestamp;
    actuatorId = _actuatorId;
    responseTimeout = false;
    invalidCRC = false;
    illegalFunction = false;
    illegalDataValue = false;
    invalidLength = true;
    unknownSubnet = false;
    unknownAddress = false;
    unknownFunction = false;
    unknownProblem = false;
    M1M3SSPublisher::instance().logILCWarning(this);
}

void ILCWarning::warnUnknownSubnet(double _timestamp) {
    timestamp = _timestamp;
    actuatorId = -1;
    responseTimeout = false;
    invalidCRC = false;
    illegalFunction = false;
    illegalDataValue = false;
    invalidLength = false;
    unknownSubnet = true;
    unknownAddress = false;
    unknownFunction = false;
    unknownProblem = false;
    M1M3SSPublisher::instance().logILCWarning(this);
}

void ILCWarning::warnUnknownAddress(double _timestamp, int32_t _actuatorId) {
    timestamp = _timestamp;
    actuatorId = _actuatorId;
    responseTimeout = false;
    invalidCRC = false;
    illegalFunction = false;
    illegalDataValue = false;
    invalidLength = false;
    unknownSubnet = false;
    unknownAddress = true;
    unknownFunction = false;
    unknownProblem = false;
    M1M3SSPublisher::instance().logILCWarning(this);
}

void ILCWarning::warnUnknownFunction(double _timestamp, int32_t _actuatorId) {
    timestamp = _timestamp;
    actuatorId = _actuatorId;
    responseTimeout = false;
    invalidCRC = false;
    illegalFunction = false;
    illegalDataValue = false;
    invalidLength = false;
    unknownSubnet = false;
    unknownAddress = false;
    unknownFunction = true;
    unknownProblem = false;
    M1M3SSPublisher::instance().logILCWarning(this);
}

void ILCWarning::warnUnknownProblem(double _timestamp, int32_t _actuatorId) {
    timestamp = _timestamp;
    actuatorId = _actuatorId;
    responseTimeout = false;
    invalidCRC = false;
    illegalFunction = false;
    illegalDataValue = false;
    invalidLength = false;
    unknownSubnet = false;
    unknownAddress = false;
    unknownFunction = false;
    unknownProblem = true;
    M1M3SSPublisher::instance().logILCWarning(this);
}
