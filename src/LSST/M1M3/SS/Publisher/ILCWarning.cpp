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

#include <math.h>

#include "ILCWarning.h"
#include "M1M3SSPublisher.h"

using namespace LSST::M1M3::SS;

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

    ignore_time = 60;
}

bool ILCWarning::ignore_warning(std::map<int, double> &_timeouts, double _timestamp, int _actuator_id,
                                bool _active) {
    auto entry = _timeouts.find(_actuator_id);

    if (entry != _timeouts.end() && _timestamp > entry->second) {
        return true;
    }

    if (_active == true) {
        if (entry == _timeouts.end() || _timestamp > entry->second) {
            _timeouts[_actuator_id] = _timestamp + ignore_time;
            return false;
        }
    } else {
        if (entry != _timeouts.end() && _timestamp > entry->second) {
            _timeouts[_actuator_id] = INFINITY;
            return false;
        }
    }

    return true;
}

void ILCWarning::warnResponseTimeout(double _timestamp, int32_t _actuator_id, bool _active) {
    if (ignore_warning(_last_response_timeout, _timestamp, _actuator_id, _active)) {
        return;
    }
    timestamp = _timestamp;
    actuatorId = _actuator_id;
    responseTimeout = _active;
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

void ILCWarning::warnInvalidCRC(double _timestamp, bool _active) {
    if (_active == false) {
        return;
    }
    timestamp = _timestamp;
    actuatorId = -1;
    responseTimeout = false;
    invalidCRC = _active;
    illegalFunction = false;
    illegalDataValue = false;
    invalidLength = false;
    unknownSubnet = false;
    unknownAddress = false;
    unknownFunction = false;
    unknownProblem = false;
    M1M3SSPublisher::instance().logILCWarning(this);
}

void ILCWarning::warnIllegalFunction(double _timestamp, int32_t _actuator_id, bool _active) {
    if (ignore_warning(_last_illegal_function, _timestamp, _actuator_id, _active)) {
        return;
    }
    timestamp = _timestamp;
    actuatorId = _actuator_id;
    responseTimeout = false;
    invalidCRC = false;
    illegalFunction = _active;
    illegalDataValue = false;
    invalidLength = false;
    unknownSubnet = false;
    unknownAddress = false;
    unknownFunction = false;
    unknownProblem = false;
    M1M3SSPublisher::instance().logILCWarning(this);
}

void ILCWarning::warnIllegalDataValue(double _timestamp, int32_t _actuator_id, bool _active) {
    if (ignore_warning(_last_illegal_date_value, _timestamp, _actuator_id, _active)) {
        return;
    }
    timestamp = _timestamp;
    actuatorId = _actuator_id;
    responseTimeout = false;
    invalidCRC = false;
    illegalFunction = false;
    illegalDataValue = _active;
    invalidLength = false;
    unknownSubnet = false;
    unknownAddress = false;
    unknownFunction = false;
    unknownProblem = false;
    M1M3SSPublisher::instance().logILCWarning(this);
}

void ILCWarning::warnInvalidLength(double _timestamp, int32_t _actuator_id, bool _active) {
    if (ignore_warning(_last_invalid_length, _timestamp, _actuator_id, _active)) {
        return;
    }
    timestamp = _timestamp;
    actuatorId = _actuator_id;
    responseTimeout = false;
    invalidCRC = false;
    illegalFunction = false;
    illegalDataValue = false;
    invalidLength = _active;
    unknownSubnet = false;
    unknownAddress = false;
    unknownFunction = false;
    unknownProblem = false;
    M1M3SSPublisher::instance().logILCWarning(this);
}

void ILCWarning::warnUnknownSubnet(double _timestamp, bool _active) {
    if (_active == false) {
        return;
    }
    timestamp = _timestamp;
    actuatorId = -1;
    responseTimeout = false;
    invalidCRC = false;
    illegalFunction = false;
    illegalDataValue = false;
    invalidLength = false;
    unknownSubnet = _active;
    unknownAddress = false;
    unknownFunction = false;
    unknownProblem = false;
    M1M3SSPublisher::instance().logILCWarning(this);
}

void ILCWarning::warnUnknownAddress(double _timestamp, int32_t _actuator_id, bool _active) {
    if (ignore_warning(_last_unknown_address, _timestamp, _actuator_id, _active)) {
        return;
    }
    timestamp = _timestamp;
    actuatorId = _actuator_id;
    responseTimeout = false;
    invalidCRC = false;
    illegalFunction = false;
    illegalDataValue = false;
    invalidLength = false;
    unknownSubnet = false;
    unknownAddress = _active;
    unknownFunction = false;
    unknownProblem = false;
    M1M3SSPublisher::instance().logILCWarning(this);
}

void ILCWarning::warnUnknownFunction(double _timestamp, int32_t _actuator_id, bool _active) {
    if (ignore_warning(_last_unknown_function, _timestamp, _actuator_id, _active)) {
        return;
    }
    timestamp = _timestamp;
    actuatorId = _actuator_id;
    responseTimeout = false;
    invalidCRC = false;
    illegalFunction = false;
    illegalDataValue = false;
    invalidLength = false;
    unknownSubnet = false;
    unknownAddress = false;
    unknownFunction = _active;
    unknownProblem = false;
    M1M3SSPublisher::instance().logILCWarning(this);
}

void ILCWarning::warnUnknownProblem(double _timestamp, int32_t _actuator_id, bool _active) {
    if (ignore_warning(_last_invalid_length, _timestamp, _actuator_id, _active)) {
        return;
    }
    timestamp = _timestamp;
    actuatorId = _actuator_id;
    responseTimeout = false;
    invalidCRC = false;
    illegalFunction = false;
    illegalDataValue = false;
    invalidLength = false;
    unknownSubnet = false;
    unknownAddress = false;
    unknownFunction = false;
    unknownProblem = _active;
    M1M3SSPublisher::instance().logILCWarning(this);
}
