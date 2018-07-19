/*
 * ILCSafetySettings.h
 *
 *  Created on: Jun 28, 2018
 *      Author: ccontaxis
 */

#ifndef ILCSAFETYSETTINGS_H_
#define ILCSAFETYSETTINGS_H_

namespace LSST {
namespace M1M3 {
namespace SS {

struct ILCSafetySettings {
	bool FaultOnCommunicationTimeout;
	int CommunicationTimeoutCountThreshold;
	int CommunicationTimeoutPeriod;
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* ILCSAFETYSETTINGS_H_ */
