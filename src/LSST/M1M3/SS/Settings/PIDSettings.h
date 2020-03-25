/*
 * PIDSettings.h
 *
 *  Created on: Feb 13, 2018
 *      Author: ccontaxis
 */

#ifndef PIDSETTINGS_H_
#define PIDSETTINGS_H_

#include <PIDParameters.h>
#include <string>

namespace LSST {
namespace M1M3 {
namespace SS {

struct PIDSettings {
	PIDParameters Fx;
	PIDParameters Fy;
	PIDParameters Fz;
	PIDParameters Mx;
	PIDParameters My;
	PIDParameters Mz;

	void load(const std::string &filename);
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* PIDSETTINGS_H_ */
