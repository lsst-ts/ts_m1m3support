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

#include <InterlockStatus.h>
#include <M1M3SSPublisher.h>

using namespace LSST::M1M3::SS;

InterlockStatus::InterlockStatus() {
    heartbeatCommandedState = false;
    heartbeatOutputState = false;
}

void InterlockStatus::set_heartbeat_output(double _timestamp, bool _heartbeat) {
    if (heartbeatOutputState != _heartbeat) {
        timestamp = _timestamp;
        heartbeatOutputState = _heartbeat;
        send();
    }
}

void InterlockStatus::send() { M1M3SSPublisher::instance().logInterlockStatus(this); }
