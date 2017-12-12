/*
 * ISettingReader.cpp
 *
 *  Created on: Oct 3, 2017
 *      Author: ccontaxis
 */

#include <ISettingReader.h>

namespace LSST {
namespace M1M3 {
namespace SS {

ISettingReader::~ISettingReader() { }

void ISettingReader::configure(std::string settingsToApply) { }
AliasApplicationSettings* ISettingReader::loadAliasApplicationSettings() { return 0; }
ForceActuatorApplicationSettings* ISettingReader::loadForceActuatorApplicationSettings() { return 0; }
ForceActuatorSettings* ISettingReader::loadForceActuatorSettings() { return 0; }
HardpointActuatorApplicationSettings* ISettingReader::loadHardpointActuatorApplicationSettings() { return 0; }
HardpointActuatorSettings* ISettingReader::loadHardpointActuatorSettings() { return 0; }
ILCApplicationSettings* ISettingReader::loadILCApplicationSettings() { return 0; }
RecommendedApplicationSettings* ISettingReader::loadRecommendedApplicationSettings() { return 0; }
SafetyControllerSettings* ISettingReader::loadSafetyControllerSettings() { return 0; }
PositionControllerSettings* ISettingReader::loadPositionControllerSettings() { return 0; }
AccelerometerSettings* ISettingReader::loadAccelerometerSettings() { return 0; }
DisplacementSensorSettings* ISettingReader::loadDisplacementSensorSettings() { return 0; }
HardpointMonitorApplicationSettings* ISettingReader::loadHardpointMonitorApplicationSettings() { return 0; }
InterlockApplicationSettings* ISettingReader::loadInterlockApplicationSettings() { return 0; }

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
