/*
 * ByteArrayUtilities.h
 *
 *  Created on: Oct 2, 2017
 *      Author: ccontaxis
 */

#ifndef U8ARRAYUTILITIES_H_
#define U8ARRAYUTILITIES_H_

#include <DataTypes.h>
#include <string>
#include <cstring>

namespace LSST {
namespace M1M3 {
namespace SS {

class U8ArrayUtilities {
public:
	virtual ~U8ArrayUtilities();

	static uint16_t u16(uint8_t* buffer, int32_t index) {
		return ((uint16_t)buffer[index] << 8) |
				(uint16_t)buffer[index + 1];
	}

	static uint64_t u64(uint8_t* buffer, int32_t index) {
		return ((uint64_t)buffer[index] << 56) |
				((uint64_t)buffer[index + 1] << 48) |
				((uint64_t)buffer[index + 2] << 40) |
				((uint64_t)buffer[index + 3] << 32) |
				((uint64_t)buffer[index + 4] << 24) |
				((uint64_t)buffer[index + 5] << 16) |
				((uint64_t)buffer[index + 6] << 8) |
				((uint64_t)buffer[index + 7]);
	}

	static std::string toString(uint8_t* buffer, int32_t index, int32_t length) {
		char tmp[256];
		memset(tmp, 0, 256);
		memcpy(tmp, buffer + index, length);
		return std::string(tmp, length);
	}
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* U8ARRAYUTILITIES_H_ */
