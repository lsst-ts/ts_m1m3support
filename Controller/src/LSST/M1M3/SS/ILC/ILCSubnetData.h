/*
 * ILCSubnetData.h
 *
 *  Created on: Oct 20, 2017
 *      Author: ccontaxis
 */

#ifndef ILCSUBNETDATA_H_
#define ILCSUBNETDATA_H_

#include <ILCDataTypes.h>
#include <ForceActuatorApplicationSettings.h>
#include <HardpointActuatorApplicationSettings.h>

namespace LSST {
namespace M1M3 {
namespace SS {

class ILCSubnetData {
	struct Container {
		int32_t HPCount;
		std::vector<ILCAddressDataIndexMap> HPIndex;
		int32_t FACount;
		std::vector<ILCAddressDataIndexMap> FAIndex;
		ILCAddressDataMap ILCDataFromAddress[256];
	};
	Container subnetData[5];

public:
	ILCSubnetData(ForceActuatorApplicationSettings* forceActuatorApplicationSettings, HardpointActuatorApplicationSettings* hardpointActuatorApplicationSettings);

	int32_t getHPCount(int32_t subnetIndex) { return this->subnetData[subnetIndex].HPIndex.size(); }
	ILCAddressDataIndexMap getHPIndex(int32_t subnetIndex, int32_t hpIndex) { return this->subnetData[subnetIndex].HPIndex[hpIndex]; }
	int32_t getFACount(int32_t subnetIndex) { return this->subnetData[subnetIndex].FAIndex.size(); }
	ILCAddressDataIndexMap getFAIndex(int32_t subnetIndex, int32_t faIndex) { return this->subnetData[subnetIndex].FAIndex[faIndex]; }
	ILCAddressDataMap getILCDataFromAddress(int32_t subnetIndex, uint8_t address) { return this->subnetData[subnetIndex].ILCDataFromAddress[address]; }
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* ILCSUBNETDATA_H_ */
