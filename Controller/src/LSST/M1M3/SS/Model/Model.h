/*
 * Model.h
 *
 *  Created on: Sep 26, 2017
 *      Author: ccontaxis
 */

#ifndef MODEL_H_
#define MODEL_H_

#include <IModel.h>
#include <AirController.h>
#include <pthread.h>
#include <string>

namespace LSST {
namespace M1M3 {
namespace SS {

class ISettingReader;
class ForceCalculator;
class ForceActuatorApplicationSettings;
class ForceActuatorSettings;
class HardpointActuatorApplicationSettings;
class HardpointActuatorSettings;

class Model : public IModel {
private:
	ISettingReader* settingReader;
	IPublisher* publisher;
	IFPGA* fpga;
	IDisplacement* displacement;
	IInclinometer* inclinometer;
	IILC* ilc;
	IAirController* airController;
	IForceController* forceController;
	ISafetyController* safetyController;
	IPositionController* positionController;
	IInterlockController* interlockController;

	pthread_mutex_t mutex;

public:
	Model(ISettingReader* settingReader, IPublisher* publisher, IFPGA* fpga);
	virtual ~Model();

	ISettingReader* getSettingReader() { return this->settingReader; }
	IPublisher* getPublisher() { return this->publisher; }
	IFPGA* getFPGA() { return this->fpga; }
	IDisplacement* getDisplacement() { return this->displacement; }
	IInclinometer* getInclinometer() { return this->inclinometer; }
	IILC* getILC() { return this->ilc; }
	IAirController* getAirController() { return this->airController; }
	IForceController* getForceController() { return this->forceController; }
	ISafetyController* getSafetyController() { return this->safetyController; }
	IPositionController* getPositionController() { return this->positionController; }
	IInterlockController* getInterlockController() { return this->interlockController; }

	void loadSettings(std::string settingsToApply);

	void queryFPGAData();
	void publishFPGAData();

	void publishStateChange(States::Type newState);
	void publishRecommendedSettings();

	void shutdown();
	void waitForShutdown();

private:
	void populateForceActuatorInfo(ForceActuatorApplicationSettings* forceActuatorApplicationSettings, ForceActuatorSettings* forceActuatorSettings);
	void populateHardpointActuatorInfo(HardpointActuatorApplicationSettings* hardpointActuatorApplicationSettings, HardpointActuatorSettings* hardpointActuatorSettings);
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* MODEL_H_ */
