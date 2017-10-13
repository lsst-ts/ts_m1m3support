/*
 * RS232.h
 *
 *  Created on: Oct 2, 2017
 *      Author: ccontaxis
 */

#ifndef RS232_H_
#define RS232_H_

#include <IRS232.h>
#include <Displacement.h>
#include <Inclinometer.h>

namespace LSST {
namespace M1M3 {
namespace SS {

class IPublisher;
class IFPGA;

class RS232: public IRS232{
private:
	Displacement displacement;
	Inclinometer inclinometer;

public:
	RS232(IFPGA* fpga, IPublisher* publisher);

	void writeDisplacementRequest();
	void readDisplacementResponse();

	void writeInclinometerRequest();
	void readInclinometerResponse();
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* RS232_H_ */
