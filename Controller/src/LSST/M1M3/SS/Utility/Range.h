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
	static bool InRange(double min, double max, double value) { return value >= min && value <= max; }
	static bool InRange(int32_t min, int32_t max, int32_t value) { return value >= min && value <= max; }

	static int32_t CoerceIntoRange(int32_t min, int32_t max, int32_t value) { return (value < min) ? min : (value > max) ? max : value; }
	static double CoerceIntoRange(double min, double max, double value) { return (value < min) ? min : (value > max) ? max : value; }
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* RANGE_H_ */
