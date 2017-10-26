/*
 * Timestamp.h
 *
 *  Created on: Oct 2, 2017
 *      Author: ccontaxis
 */

#ifndef TIMESTAMP_H_
#define TIMESTAMP_H_

#include <DataTypes.h>

namespace LSST {
namespace M1M3 {
namespace SS {

class Timestamp {
public:
	static double fromRaw(uint64_t raw) { return ((raw & 0xFFFFFFFFFFF00000) >> 20) + ((raw & 0xFFFFF) / 1000000.0); }
	static double currentTime() { return 0; }
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* TIMESTAMP_H_ */
