/*
 * IDisplacement.h
 *
 *  Created on: Oct 25, 2017
 *      Author: ccontaxis
 */

#ifndef IDISPLACEMENT_H_
#define IDISPLACEMENT_H_

namespace LSST {
namespace M1M3 {
namespace SS {

class IDisplacement {
public:
	virtual ~IDisplacement();

	virtual void writeDataRequest();
	virtual void readDataResponse();
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* IDISPLACEMENT_H_ */
