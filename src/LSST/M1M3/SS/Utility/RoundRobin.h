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

#ifndef ROUNDROBIN_H_
#define ROUNDROBIN_H_

#include <ILCDataTypes.h>

namespace LSST {
namespace M1M3 {
namespace SS {

class RoundRobin {
public:
    static int32_t Inc(int32_t index, int32_t limit) { return (index + 1) < limit ? (index + 1) : 0; }

    static uint8_t BroadcastCounter(uint8_t broadcastCounter) {
        return (broadcastCounter + 1) < 16 ? (broadcastCounter + 1) : 0;
    }
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* ROUNDROBIN_H_ */
