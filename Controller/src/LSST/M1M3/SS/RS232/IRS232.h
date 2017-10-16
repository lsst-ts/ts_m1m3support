/*
 * IRS232.h
 *
 *  Created on: Oct 2, 2017
 *      Author: ccontaxis
 */

#ifndef IRS232_H_
#define IRS232_H_

namespace LSST {
namespace M1M3 {
namespace SS {

class IRS232 {
public:
	virtual ~IRS232();

	virtual void writeDisplacementRequest();
	virtual void writeInclinometerRequest();

	virtual void readDisplacementResponse();
	virtual void readInclinometerResponse();
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* IRS232_H_ */
