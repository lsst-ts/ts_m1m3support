#ifndef SETTINGREADER_H_
#define SETTINGREADER_H_

#include <SafetyControllerSettings.h>
#include <AliasApplicationSettings.h>
#include <ForceActuatorApplicationSettings.h>
#include <ForceActuatorSettings.h>
#include <HardpointActuatorApplicationSettings.h>
#include <HardpointActuatorSettings.h>
#include <ILCApplicationSettings.h>
#include <RecommendedApplicationSettings.h>
#include <PositionControllerSettings.h>
#include <AccelerometerSettings.h>
#include <DisplacementSensorSettings.h>
#include <HardpointMonitorApplicationSettings.h>
#include <InterlockApplicationSettings.h>
#include <GyroSettings.h>
#include <ExpansionFPGAApplicationSettings.h>
#include <PIDSettings.h>
#include <InclinometerSettings.h>
#include <string>

namespace LSST {
namespace M1M3 {
namespace SS {

/**
 * Reads M1M3 support system configuration.
 */
class SettingReader {
private:
	AliasApplicationSettings aliasApplicationSettings;
	ForceActuatorApplicationSettings forceActuatorApplicationSettings;
	ForceActuatorSettings forceActuatorSettings;
	HardpointActuatorApplicationSettings hardpointActuatorApplicationSettings;
	HardpointActuatorSettings hardpointActuatorSettings;
	ILCApplicationSettings ilcApplicationSettings;
	RecommendedApplicationSettings recommendedApplicationSettings;
	SafetyControllerSettings safetyControllerSettings;
	PositionControllerSettings positionControllerSettings;
	AccelerometerSettings accelerometerSettings;
	DisplacementSensorSettings displacementSensorSettings;
	HardpointMonitorApplicationSettings hardpointMonitorApplicationSettings;
	InterlockApplicationSettings interlockApplicationSettings;
	GyroSettings gyroSettings;
	ExpansionFPGAApplicationSettings expansionFPGAApplicationSettings;
	PIDSettings pidSettings;
	InclinometerSettings inclinometerSettings;

	std::string basePath;
	std::string setsPath;
	std::string currentSet;
	std::string currentVersion;

public:
        /**
         * Construct new SettingReader. Verifies that basePath and setsPath
         * directories exists and are readable, log and exit when not.
         *
         * @param basePath
         * @param setsPath
         */
	SettingReader(std::string basePath, std::string setsPath);

	void configure(std::string settingsToApply);
	AliasApplicationSettings* loadAliasApplicationSettings();
	ForceActuatorApplicationSettings* loadForceActuatorApplicationSettings();
	ForceActuatorSettings* loadForceActuatorSettings();
	HardpointActuatorApplicationSettings* loadHardpointActuatorApplicationSettings();
	HardpointActuatorSettings* loadHardpointActuatorSettings();
	ILCApplicationSettings* loadILCApplicationSettings();
	RecommendedApplicationSettings* loadRecommendedApplicationSettings();
	SafetyControllerSettings* loadSafetyControllerSettings();
	PositionControllerSettings* loadPositionControllerSettings();
	AccelerometerSettings* loadAccelerometerSettings();
	DisplacementSensorSettings* loadDisplacementSensorSettings();
	HardpointMonitorApplicationSettings* loadHardpointMonitorApplicationSettings();
	InterlockApplicationSettings* loadInterlockApplicationSettings();
	GyroSettings* loadGyroSettings();
	ExpansionFPGAApplicationSettings* loadExpansionFPGAApplicationSettings();
	PIDSettings* loadPIDSettings();
	InclinometerSettings* loadInclinometerSettings();

private:
	std::string getBasePath(std::string file);
	std::string getSetPath(std::string file);
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* SETTINGREADER_H_ */
