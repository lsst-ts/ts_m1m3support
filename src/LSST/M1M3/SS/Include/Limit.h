/*
 * Limit.h
 *
 *  Created on: Mar 1, 2018
 *      Author: ccontaxis
 */

#ifndef LIMIT_H_
#define LIMIT_H_

namespace LSST {
namespace M1M3 {
namespace SS {

struct Limit {
	float LowFault;
	float LowWarning;
	float HighWarning;
	float HighFault;
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* LIMIT_H_ */
