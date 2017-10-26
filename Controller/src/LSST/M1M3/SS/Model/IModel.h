/*
 * IModel.h
 *
 *  Created on: Sep 26, 2017
 *      Author: ccontaxis
 */

#ifndef IMODEL_H_
#define IMODEL_H_

#include <string>
#include <StateTypes.h>

namespace LSST {
namespace M1M3 {
namespace SS {

class ISettingReader;
class IPublisher;
class IFPGA;
class IDisplacement;
class IInclinometer;
class IILC;
class IAirController;
class IForceController;

class IModel {
public:
	virtual ~IModel();

	virtual ISettingReader* getSettingReader();
	virtual IPublisher* getPublisher();
	virtual IFPGA* getFPGA();
	virtual IDisplacement* getDisplacement();
	virtual IInclinometer* getInclinometer();
	virtual IILC* getILC();
	virtual IAirController* getAirController();
	virtual IForceController* getForceController();

	virtual void loadSettings(std::string settingsToApply);

	virtual void queryFPGAData();
	virtual void publishFPGAData();

	virtual void publishStateChange(States::Type newState);
	virtual void publishRecommendedSettings();

	virtual void shutdown();
	virtual void waitForShutdown();
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* IMODEL_H_ */
