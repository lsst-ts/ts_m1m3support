/*
 * IInclinometer.h
 *
 *  Created on: Oct 25, 2017
 *      Author: ccontaxis
 */

#ifndef IINCLINOMETER_H_
#define IINCLINOMETER_H_

namespace LSST {
namespace M1M3 {
namespace SS {

class IInclinometer {
public:
	virtual ~IInclinometer();

	virtual void writeDataRequest();
	virtual void readDataResponse();
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* IINCLINOMETER_H_ */
