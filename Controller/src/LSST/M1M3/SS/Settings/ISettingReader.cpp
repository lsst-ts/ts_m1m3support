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

RecommendedApplicationSettings* ISettingReader::loadRecommendedApplicationSettings() { return 0; }

ILCApplicationSettings* ISettingReader::loadILCApplicationSettings() { return 0; }

ForceActuatorApplicationSettings* ISettingReader::loadForceActuatorApplicationSettings() { return 0; }

HardpointActuatorApplicationSettings* ISettingReader::loadHardpointActuatorApplicationSettings() { return 0; }

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
