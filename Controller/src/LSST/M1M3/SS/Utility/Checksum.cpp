/*
 * Checksum.cpp
 *
 *  Created on: Jan 18, 2018
 *      Author: ccontaxis
 */

#include <Checksum.h>

namespace LSST {
namespace M1M3 {
namespace SS {

uint8_t Checksum::checksum8(uint8_t* buffer, int32_t startIndex, int32_t length) {
    uint32_t checksum = 0;
    for (int i = 0; i < length; ++i) {
        checksum += buffer[startIndex + i];
    }
    return (uint8_t)(checksum & 0xFF);
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
