/*
 * RoundRobin.h
 *
 *  Created on: Oct 20, 2017
 *      Author: ccontaxis
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

	static uint8_t BroadcastCounter(uint8_t broadcastCounter) { return (broadcastCounter + 1) < 16 ? (broadcastCounter + 1) : 0; }
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* ROUNDROBIN_H_ */
