/*
 * Displacement.h
 *
 *  Created on: Oct 6, 2017
 *      Author: ccontaxis
 */

#ifndef DISPLACEMENT_H_
#define DISPLACEMENT_H_

#include <DataTypes.h>

namespace LSST {
namespace M1M3 {
namespace SS {

class Displacement {
private:
	uint8_t txBuffer[4];
	uint8_t rxBuffer[128];

public:
	Displacement();
	virtual ~Displacement();

private:
	void createTxBuffer();
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* DISPLACEMENT_H_ */
