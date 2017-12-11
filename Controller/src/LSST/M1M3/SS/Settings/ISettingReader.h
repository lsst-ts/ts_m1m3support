/*
 * ISettingReader.h
 *
 *  Created on: Oct 3, 2017
 *      Author: ccontaxis
 */

#ifndef ISETTINGREADER_H_
#define ISETTINGREADER_H_

#include <string>

namespace LSST {
namespace M1M3 {
namespace SS {

class AliasApplicationSettings;
class ILCApplicationSettings;
class ForceActuatorApplicationSettings;
class ForceActuatorSettings;
class HardpointActuatorApplicationSettings;
class HardpointActuatorSettings;
class RecommendedApplicationSettings;
class SafetyControllerSettings;
class PositionControllerSettings;
class AccelerometerSettings;
class DisplacementSensorSettings;
class HardpointMonitorApplicationSettings;
class InterlockApplicationSettings;

class ISettingReader {
public:
	virtual ~ISettingReader();

	virtual void configure(std::string settingsToApply);
	virtual AliasApplicationSettings* loadAliasApplicationSettings();
	virtual ForceActuatorApplicationSettings* loadForceActuatorApplicationSettings();
	virtual ForceActuatorSettings* loadForceActuatorSettings();
	virtual HardpointActuatorApplicationSettings* loadHardpointActuatorApplicationSettings();
	virtual HardpointActuatorSettings* loadHardpointActuatorSettings();
	virtual ILCApplicationSettings* loadILCApplicationSettings();
	virtual RecommendedApplicationSettings* loadRecommendedApplicationSettings();
	virtual SafetyControllerSettings* loadSafetyControllerSettings();
	virtual PositionControllerSettings* loadPositionControllerSettings();
	virtual AccelerometerSettings* loadAccelerometerSettings();
	virtual DisplacementSensorSettings* loadDisplacementSensorSettings();
	virtual HardpointMonitorApplicationSettings* loadHardpointMonitorApplicationSettings();
	virtual InterlockApplicationSettings* loadInterlockApplicationSettings();
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* ISETTINGREADER_H_ */
