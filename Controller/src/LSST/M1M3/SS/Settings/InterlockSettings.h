/*
 * InterlockSettings.h
 *
 *  Created on: Nov 29, 2017
 *      Author: ccontaxis
 */

#ifndef INTERLOCKSETTINGS_H_
#define INTERLOCKSETTINGS_H_

#include <string>

namespace LSST {
namespace M1M3 {
namespace SS {

class InterlockSettings {
public:
	double HeartbeatPeriodInSeconds;

	void load(const std::string &filename);
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* INTERLOCKSETTINGS_H_ */
