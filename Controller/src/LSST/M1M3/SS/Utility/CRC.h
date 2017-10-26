/*
 * CRC.h
 *
 *  Created on: Oct 2, 2017
 *      Author: ccontaxis
 */

#ifndef CRC_H_
#define CRC_H_

#include <DataTypes.h>

namespace LSST {
namespace M1M3 {
namespace SS {

class CRC {
public:
	static uint16_t modbus(uint8_t* buffer, int32_t startIndex, int32_t length);
	static uint16_t modbus(uint16_t* buffer, int32_t startIndex, int32_t length);
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* CRC_H_ */
