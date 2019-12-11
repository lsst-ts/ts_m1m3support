/*
 * CRC.cpp
 *
 *  Created on: Oct 2, 2017
 *      Author: ccontaxis
 */

#include <CRC.h>
#include <boost/crc.hpp>
#include <Log.h>

namespace LSST {
namespace M1M3 {
namespace SS {

uint16_t CRC::modbus(uint8_t* buffer, int32_t startIndex, int32_t length) {
    uint16_t crc = 0xFFFF;
    for (int i = startIndex; i < startIndex + length; i++) {
        crc = crc ^ ((uint16_t)buffer[i]);
        for (int j = 0; j < 8; j++) {
            if (crc & 0x0001) {
                crc = crc >> 1;
                crc = crc ^ 0xA001;
            } else {
                crc = crc >> 1;
            }
        }
    }
    return crc;
}

uint16_t CRC::modbus(uint16_t* buffer, int32_t startIndex, int32_t length) {
    uint16_t crc = 0xFFFF;
    for (int i = startIndex; i < startIndex + length; i++) {
        crc = crc ^ buffer[i];
        for (int j = 0; j < 8; j++) {
            if (crc & 0x0001) {
                crc = crc >> 1;
                crc = crc ^ 0xA001;
            } else {
                crc = crc >> 1;
            }
        }
    }
    return crc;
}

uint32_t CRC::crc32(uint8_t* buffer, int32_t startIndex, int32_t length) {
    boost::crc_optimal<32, 0x04C11DB7, 0xFFFFFFFF, 0x00000000, false, false> result;
    result.process_block(buffer + startIndex, buffer + startIndex + length);
    uint32_t crc = result.checksum();
    //	Log.Info("CRC32: %d %d %d", buffer[startIndex], buffer[length], crc);
    return crc;
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
