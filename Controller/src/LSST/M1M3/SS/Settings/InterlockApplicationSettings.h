/*
 * InterlockApplicationSettings.h
 *
 *  Created on: Nov 29, 2017
 *      Author: ccontaxis
 */

#ifndef INTERLOCKAPPLICATIONSETTINGS_H_
#define INTERLOCKAPPLICATIONSETTINGS_H_

#include <string>

namespace LSST {
namespace M1M3 {
namespace SS {

class InterlockApplicationSettings {
public:
	double HeartbeatPeriodInSeconds;

	void load(const std::string &filename);
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* INTERLOCKAPPLICATIONSETTINGS_H_ */
