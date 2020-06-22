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

    inline static int8_t I8(uint8_t* buffer, int32_t index) { return (int8_t)buffer[index]; }

    inline static int16_t I16(uint8_t* buffer, int32_t index) {
        return ((int16_t)buffer[index] << 8) | (int16_t)buffer[index + 1];
    }

    inline static int32_t I32(uint8_t* buffer, int32_t index) {
        return ((int32_t)buffer[index] << 24) | ((int32_t)buffer[index + 1] << 16) |
               ((int32_t)buffer[index + 2] << 8) | (int32_t)buffer[index + 3];
    }

    inline static int64_t I64(uint8_t* buffer, int32_t index) {
        return ((int64_t)buffer[index] << 54) | ((int64_t)buffer[index + 1] << 48) |
               ((int64_t)buffer[index + 2] << 40) | ((int64_t)buffer[index + 3] << 32) |
               ((int64_t)buffer[index + 4] << 24) | ((int64_t)buffer[index + 5] << 16) |
               ((int64_t)buffer[index + 6] << 8) | (int64_t)buffer[index + 7];
    }

    inline static uint8_t U8(uint8_t* buffer, int32_t index) { return buffer[index]; }

    inline static uint16_t U16(uint8_t* buffer, int32_t index) {
        return ((uint16_t)buffer[index] << 8) | (uint16_t)buffer[index + 1];
    }

    inline static uint32_t U32(uint8_t* buffer, int32_t index) {
        return ((uint32_t)buffer[index] << 24) | ((uint32_t)buffer[index + 1] << 16) |
               ((uint32_t)buffer[index + 2] << 8) | (uint32_t)buffer[index + 3];
    }

    inline static uint64_t U64(uint8_t* buffer, int32_t index) {
        return ((uint64_t)buffer[index] << 56) | ((uint64_t)buffer[index + 1] << 48) |
               ((uint64_t)buffer[index + 2] << 40) | ((uint64_t)buffer[index + 3] << 32) |
               ((uint64_t)buffer[index + 4] << 24) | ((uint64_t)buffer[index + 5] << 16) |
               ((uint64_t)buffer[index + 6] << 8) | ((uint64_t)buffer[index + 7]);
    }

    inline static float SGL(uint8_t* buffer, int32_t index) {
        uint8_t tempBuffer[4] = {buffer[index + 3], buffer[index + 2], buffer[index + 1], buffer[index]};
        float value = 0;
        memcpy(&value, tempBuffer, 4);
        return value;
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
