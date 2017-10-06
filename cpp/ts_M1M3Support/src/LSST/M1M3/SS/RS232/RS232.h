/*
 * RS232.h
 *
 *  Created on: Oct 2, 2017
 *      Author: ccontaxis
 */

#ifndef RS232_H_
#define RS232_H_

#include <IRS232.h>
#include <SAL_m1m3C.h>
#include <DataTypes.h>
#include <Inclinometer.h>

namespace LSST {
namespace M1M3 {
namespace SS {

class IPublisher;
class IFPGA;

class RS232: public IRS232{
private:
	IPublisher* publisher;
	IFPGA* fpga;

	Inclinometer inclinometer;

	uint8_t displacementTxBuffer[7];
	uint8_t rxBuffer[128];
	int32_t rxBufferIndex;

	void createDisplacementTxBuffer();

public:
	RS232(IFPGA* fpga, IPublisher* publisher);

	virtual void writeInclinometerRequest();

	virtual void read();
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* RS232_H_ */
