/*
 * U16ArrayUtilities.h
 *
 *  Created on: Oct 12, 2017
 *      Author: ccontaxis
 */

#ifndef U16ARRAYUTILITIES_H_
#define U16ARRAYUTILITIES_H_

#include <DataTypes.h>

namespace LSST {
namespace M1M3 {
namespace SS {

class U16ArrayUtilities {
public:
	virtual ~U16ArrayUtilities();

	static uint64_t u64(uint16_t* buffer, int32_t index) {
		return ((uint64_t)buffer[index] << 48) |
				((uint64_t)buffer[index + 1] << 32) |
				((uint64_t)buffer[index + 2] << 16) |
				((uint64_t)buffer[index + 3]);
	}
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* U16ARRAYUTILITIES_H_ */
