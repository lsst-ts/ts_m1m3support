/*
 * IPowerController.h
 *
 *  Created on: Dec 7, 2017
 *      Author: ccontaxis
 */

#ifndef IPOWERCONTROLLER_H_
#define IPOWERCONTROLLER_H_

namespace LSST {
namespace M1M3 {
namespace SS {

class IPowerController {
public:
	virtual ~IPowerController();

	virtual void checkPowerStatus();
	virtual void samplePowerSupplyDataAndStatus();
	virtual void publishPowerSupplyData();
	virtual void publishPowerSupplyStatusIfRequired();

	virtual void setBothPowerNetworks(bool on);

	virtual void setAllPowerNetworks(bool on);
	virtual void setPowerNetworkA(bool on);
	virtual void setPowerNetworkB(bool on);
	virtual void setPowerNetworkC(bool on);
	virtual void setPowerNetworkD(bool on);

	virtual void setAllAuxPowerNetworks(bool on);
	virtual void setAuxPowerNetworkA(bool on);
	virtual void setAuxPowerNetworkB(bool on);
	virtual void setAuxPowerNetworkC(bool on);
	virtual void setAuxPowerNetworkD(bool on);
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* IPOWERCONTROLLER_H_ */
