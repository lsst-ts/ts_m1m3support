#ifndef CRC_H_
#define CRC_H_

#include <DataTypes.h>

namespace LSST {
namespace M1M3 {
namespace SS {

/**
 * CRC utility functions.
 */
class CRC {
public:
    /**
     * Calculates 16 bit Modbus CRC. See (CRC calculator)[https://crccalc.com]
     * for checks.
     *
     * @param buffer data buffer
     * @param startIndex start index
     * @param length data lenght
     *
     * @return 16 bits Modbus CRC
     */
    static uint16_t modbus(uint8_t* buffer, int32_t startIndex, int32_t length);

    /**
     * Calculates 16 bit Modbus CRC. See (CRC calculator)[https://crccalc.com]
     * for checks.
     *
     * @param buffer data buffer
     * @param startIndex start index
     * @param length data lenght
     *
     * @return 16 bits Modbus CRC
     */
    static uint16_t modbus(uint16_t* buffer, int32_t startIndex, int32_t length);
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* CRC_H_ */
