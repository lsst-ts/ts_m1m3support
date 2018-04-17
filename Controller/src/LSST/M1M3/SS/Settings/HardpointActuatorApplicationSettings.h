/*
 * HardpointActuatorApplicationSettings.h
 *
 *  Created on: Oct 3, 2017
 *      Author: ccontaxis
 */

#ifndef HARDPOINTACTUATORAPPLICATIONSETTINGS_H_
#define HARDPOINTACTUATORAPPLICATIONSETTINGS_H_

#include <HardpointActuatorTableRow.h>
#include <vector>
#include <string>

namespace LSST {
namespace M1M3 {
namespace SS {

class HardpointActuatorApplicationSettings {
public:
	std::vector<HardpointActuatorTableRow> Table;

	void load(const std::string &filename);
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* HARDPOINTACTUATORAPPLICATIONSETTINGS_H_ */
