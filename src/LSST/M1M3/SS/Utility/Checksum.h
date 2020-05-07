/*
 * Checksum.h
 *
 *  Created on: Jan 18, 2018
 *      Author: ccontaxis
 */

#ifndef CHECKSUM_H_
#define CHECKSUM_H_

#include <DataTypes.h>

namespace LSST {
namespace M1M3 {
namespace SS {

class Checksum {
public:
	static uint8_t checksum8(uint8_t* buffer, int32_t startIndex, int32_t length);
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* CHECKSUM_H_ */
