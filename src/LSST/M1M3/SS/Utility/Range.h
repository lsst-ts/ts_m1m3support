/*
 * Range.h
 *
 *  Created on: Oct 31, 2017
 *      Author: ccontaxis
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
