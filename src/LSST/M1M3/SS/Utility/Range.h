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

    static bool InRange(float min, float max, float value) { return value >= min && value <= max; }
    static bool InRange(double min, double max, double value) { return value >= min && value <= max; }
    static bool InRange(int32_t min, int32_t max, int32_t value) { return value >= min && value <= max; }

    static int32_t CoerceIntoRange(int32_t min, int32_t max, int32_t value) {
        return (value < min) ? min : (value > max) ? max : value;
    }
    static double CoerceIntoRange(double min, double max, double value) {
        return (value < min) ? min : (value > max) ? max : value;
    }
    static float CoerceIntoRange(float min, float max, float value) {
        return (value < min) ? min : (value > max) ? max : value;
    }

    static bool InRangeAndCoerce(float min, float max, float value, float* output) {
        if (Range::InRange(min, max, value)) {
            (*output) = value;
            return true;
        } else {
            (*output) = Range::CoerceIntoRange(min, max, value);
            return false;
        }
    }

    static bool InRangeAndCoerce(int32_t min, int32_t max, int32_t value, int32_t* output) {
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
