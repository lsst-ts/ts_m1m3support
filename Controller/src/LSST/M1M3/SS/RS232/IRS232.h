/*
 * IRS232.h
 *
 *  Created on: Oct 2, 2017
 *      Author: ccontaxis
 */

#ifndef IRS232_H_
#define IRS232_H_

#include <DataTypes.h>

namespace LSST {
namespace M1M3 {
namespace SS {

class IRS232 {
public:
	virtual ~IRS232();

	virtual void writeDisplacementRequest();
	virtual void writeInclinometerRequest();

	virtual void read(int32_t port);
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* IRS232_H_ */
