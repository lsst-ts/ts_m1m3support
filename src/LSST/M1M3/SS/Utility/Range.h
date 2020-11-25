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

#include <spdlog/spdlog.h>

namespace LSST {
namespace M1M3 {
namespace SS {

/**
 * Utility range checking functions.
 */
class Range {
public:
    static uint64_t Max(uint64_t a, uint64_t b) { return a >= b ? a : b; }

    /**
     * Template function to check if a value is inside a given range.
     *
     * @tparam T value type. All parameters shall have the same type.
     * @param min range minimal value
     * @param max range maximal value
     * @param value value to test
     *
     * @return true if value >= min and value <= max
     */
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

    /**
     * Tests if value is in given range. Produces warning when the value is not
     * in range.
     *
     * @tparam T test value type
     * @tparam TArgs variadic arguments for SPDLOG_WARN macro
     * @param min range minimal value
     * @param max range maximal value
     * @param value test value
     * @param counter counter of failed tests. Reset to 0 if value is in range
     * @param reportAfter report warning only after this numbers of failed tests
     * @param format format for log message
     * @param FArgs parameters passed to SPDLOG_WARN
     *
     * @return true if value >= min and value <= max. Prints warning if conter >= reportAfter
     */
    template <typename T, typename... TArgs>
    static bool InRangeWithWarning(T min, T max, T value, int& counter, int reportAfter, const char* format,
                                   TArgs... FArgs) {
        bool inRange = InRange(min, max, value);
        if (inRange == false) {
            counter++;
            if (counter >= reportAfter) {
                SPDLOG_WARN(format, FArgs...);
            }
        } else {
            counter = 0;
        }
        return inRange;
    }
};

}  // namespace SS
}  // namespace M1M3
}  // namespace LSST
#endif /* RANGE_H_ */
