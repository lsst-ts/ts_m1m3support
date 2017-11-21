/*
 * HardpointMonitorApplicationSettings.h
 *
 *  Created on: Nov 17, 2017
 *      Author: ccontaxis
 */

#ifndef HARDPOINTMONITORAPPLICATIONSETTINGS_H_
#define HARDPOINTMONITORAPPLICATIONSETTINGS_H_

#include <HardpointMonitorTableRow.h>
#include <vector>
#include <string>

namespace LSST {
namespace M1M3 {
namespace SS {

class HardpointMonitorApplicationSettings {
public:
	std::vector<HardpointMonitorTableRow> Table;

	void load(const std::string &filename);
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* HARDPOINTMONITORAPPLICATIONSETTINGS_H_ */
