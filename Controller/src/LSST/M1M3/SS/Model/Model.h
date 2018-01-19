/*
 * Model.h
 *
 *  Created on: Sep 26, 2017
 *      Author: ccontaxis
 */

#ifndef MODEL_H_
#define MODEL_H_

#include <StateTypes.h>
#include <pthread.h>
#include <string>

namespace LSST {
namespace M1M3 {
namespace SS {

class SettingReader;
class M1M3SSPublisher;
class FPGA;
class ExpansionFPGA;
class Displacement;
class Inclinometer;
class ILC;
class AirController;
class ForceController;
class SafetyController;
class PositionController;
class InterlockController;
class Accelerometer;
class PowerController;
class AutomaticOperationsController;
class Gyro;
class SettingReader;
class ForceCalculator;
class ForceActuatorApplicationSettings;
class ForceActuatorSettings;
class HardpointActuatorApplicationSettings;
class HardpointActuatorSettings;
class HardpointMonitorApplicationSettings;

class Model {
private:
	SettingReader* settingReader;
	M1M3SSPublisher* publisher;
	FPGA* fpga;
	ExpansionFPGA* expansionFPGA;
	Displacement* displacement;
	Inclinometer* inclinometer;
	ILC* ilc;
	AirController* airController;
	ForceController* forceController;
	SafetyController* safetyController;
	PositionController* positionController;
	InterlockController* interlockController;
	Accelerometer* accelerometer;
	PowerController* powerController;
	AutomaticOperationsController* automaticOperationsController;
	Gyro* gyro;

	pthread_mutex_t mutex;

	double cachedTimestamp;

public:
	Model(SettingReader* settingReader, M1M3SSPublisher* publisher, FPGA* fpga, ExpansionFPGA* expansionFPGA, InterlockController* interlockController);
	virtual ~Model();

	SettingReader* getSettingReader() { return this->settingReader; }
	M1M3SSPublisher* getPublisher() { return this->publisher; }
	FPGA* getFPGA() { return this->fpga; }
	ExpansionFPGA* getExpansionFPGA() { return this->expansionFPGA; }
	Displacement* getDisplacement() { return this->displacement; }
	Inclinometer* getInclinometer() { return this->inclinometer; }
	ILC* getILC() { return this->ilc; }
	AirController* getAirController() { return this->airController; }
	ForceController* getForceController() { return this->forceController; }
	SafetyController* getSafetyController() { return this->safetyController; }
	PositionController* getPositionController() { return this->positionController; }
	InterlockController* getInterlockController() { return this->interlockController; }
	Accelerometer* getAccelerometer() { return this->accelerometer; }
	PowerController* getPowerController() { return this->powerController; }
	AutomaticOperationsController* getAutomaticOperationsController() { return this->automaticOperationsController; }
	Gyro* getGyro() { return this->gyro; }

	void setCachedTimestamp(double timestamp) { this->cachedTimestamp = timestamp; }
	double getCachedTimestamp() { return this->cachedTimestamp; }

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
	void populateHardpointMonitorInfo(HardpointMonitorApplicationSettings* hardpointMonitorApplicationSettings);
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* MODEL_H_ */
