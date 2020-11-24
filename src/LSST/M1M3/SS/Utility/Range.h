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

#ifndef RANGE_H_
#define RANGE_H_

namespace LSST {
namespace M1M3 {
namespace SS {

class Range {
public:
    static uint64_t Max(uint64_t a, uint64_t b) { return a >= b ? a : b; }

    template <typename T>
    static bool InRange(T min, T max, T value) {
        return value >= min && value <= max;
    }

    template <typename T>
    static T CoerceIntoRange(T min, T max, T value) {
        return (value < min) ? min : (value > max) ? max : value;
    }

    template <typename T>
    static bool InRangeAndCoerce(T min, T max, T value, T* output) {
        if (Range::InRange(min, max, value)) {
            (*output) = value;
            return true;
        } else {
            (*output) = Range::CoerceIntoRange(min, max, value);
            return false;
        }
    }
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* RANGE_H_ */
