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
class IExpansionFPGA;
class IDisplacement;
class IInclinometer;
class IILC;
class IAirController;
class IForceController;
class ISafetyController;
class IPositionController;
class IInterlockController;
class IAccelerometer;
class IPowerController;
class IAutomaticOperationsController;
class IGyro;

class IModel {
public:
	virtual ~IModel();

	virtual ISettingReader* getSettingReader();
	virtual IPublisher* getPublisher();
	virtual IFPGA* getFPGA();
	virtual IExpansionFPGA* getExpansionFPGA();
	virtual IDisplacement* getDisplacement();
	virtual IInclinometer* getInclinometer();
	virtual IILC* getILC();
	virtual IAirController* getAirController();
	virtual IForceController* getForceController();
	virtual ISafetyController* getSafetyController();
	virtual IPositionController* getPositionController();
	virtual IInterlockController* getInterlockController();
	virtual IAccelerometer* getAccelerometer();
	virtual IPowerController* getPowerController();
	virtual IAutomaticOperationsController* getAutomaticOperationsController();
	virtual IGyro* getGyro();

	virtual void setCachedTimestamp(double timestamp);
	virtual double getCachedTimestamp();

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
