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

struct MTMount_ElevationC;
struct MTMount_AzimuthC;

namespace LSST {
namespace M1M3 {
namespace SS {

class SettingReader;
class M1M3SSPublisher;
class IFPGA;
class IExpansionFPGA;
class Displacement;
class Inclinometer;
class ILC;
class ForceController;
class SafetyController;
class PositionController;
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
class PIDSettings;
class DigitalInputOutput;
class InclinometerSettings;
class PositionControllerSettings;

class Model {
private:
	SettingReader* settingReader;
	M1M3SSPublisher* publisher;
	IFPGA* fpga;
	IExpansionFPGA* expansionFPGA;
	Displacement* displacement;
	Inclinometer* inclinometer;
	ILC* ilc;
	ForceController* forceController;
	SafetyController* safetyController;
	PositionController* positionController;
	Accelerometer* accelerometer;
	PowerController* powerController;
	AutomaticOperationsController* automaticOperationsController;
	Gyro* gyro;
	DigitalInputOutput* digitalInputOutput;
	MTMount_ElevationC* tmaElevation;
	MTMount_AzimuthC* tmaAzimuth;

	pthread_mutex_t mutex;

	double cachedTimestamp;

public:
	Model(SettingReader* settingReader, M1M3SSPublisher* publisher, IFPGA* fpga, IExpansionFPGA* expansionFPGA, DigitalInputOutput* digitalInputOutput, MTMount_ElevationC* tmaAlt, MTMount_AzimuthC* tmaAz);
	virtual ~Model();

	inline SettingReader* getSettingReader() { return this->settingReader; }
	inline M1M3SSPublisher* getPublisher() { return this->publisher; }
	inline IFPGA* getFPGA() { return this->fpga; }
	inline IExpansionFPGA* getExpansionFPGA() { return this->expansionFPGA; }
	inline Displacement* getDisplacement() { return this->displacement; }
	inline Inclinometer* getInclinometer() { return this->inclinometer; }
	inline ILC* getILC() { return this->ilc; }
	inline ForceController* getForceController() { return this->forceController; }
	inline SafetyController* getSafetyController() { return this->safetyController; }
	inline PositionController* getPositionController() { return this->positionController; }
	inline DigitalInputOutput* getDigitalInputOutput() { return this->digitalInputOutput; }
	inline Accelerometer* getAccelerometer() { return this->accelerometer; }
	inline PowerController* getPowerController() { return this->powerController; }
	inline AutomaticOperationsController* getAutomaticOperationsController() { return this->automaticOperationsController; }
	inline Gyro* getGyro() { return this->gyro; }

	void setCachedTimestamp(double timestamp) { this->cachedTimestamp = timestamp; }
	double getCachedTimestamp() { return this->cachedTimestamp; }

	void loadSettings(std::string settingsToApply);

	void queryFPGAData();
	void publishFPGAData();

	void publishStateChange(States::Type newState);
	void publishRecommendedSettings();
	void publishOuterLoop(double executionTime);

	void shutdown();
	void waitForShutdown();

private:
	void populateForceActuatorInfo(ForceActuatorApplicationSettings* forceActuatorApplicationSettings, ForceActuatorSettings* forceActuatorSettings);
	void populateHardpointActuatorInfo(HardpointActuatorApplicationSettings* hardpointActuatorApplicationSettings, HardpointActuatorSettings* hardpointActuatorSettings, PositionControllerSettings* positionControllerSettings);
	void populateHardpointMonitorInfo(HardpointMonitorApplicationSettings* hardpointMonitorApplicationSettings);
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* MODEL_H_ */
