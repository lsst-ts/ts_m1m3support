/*
 * IAccelerometer.h
 *
 *  Created on: Nov 1, 2017
 *      Author: ccontaxis
 */

#ifndef IACCELEROMETER_H_
#define IACCELEROMETER_H_

namespace LSST {
namespace M1M3 {
namespace SS {

class IAccelerometer {
public:
	virtual ~IAccelerometer();

	virtual void sampleData();
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* IACCELEROMETER_H_ */
