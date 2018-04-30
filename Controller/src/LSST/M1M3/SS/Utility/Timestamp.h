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
	static double fromRaw(uint64_t raw) { return ((double)raw) / 1000000000.0; }
	static uint64_t toRaw(double timestamp) { return (uint64_t)(timestamp * 1000000000.0); }
	static uint64_t toFPGA(double timestamp) { return (uint64_t)(timestamp * 1000000000.0); }
	static double fromFPGA(uint64_t timestamp) { return ((double)timestamp) / 1000000000.0; }
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* TIMESTAMP_H_ */
