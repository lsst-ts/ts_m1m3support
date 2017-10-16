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

AliasApplicationSettings ISettingReader::loadAliasApplicationSettings() { return AliasApplicationSettings(); }

RecommendedApplicationSettings ISettingReader::loadRecommendedApplicationSettings() { return RecommendedApplicationSettings(); }

ILCApplicationSettings ISettingReader::loadILCApplicationSettings() { return ILCApplicationSettings(); }

ForceActuatorApplicationSettings ISettingReader::loadForceActuatorApplicationSettings() { return ForceActuatorApplicationSettings(); }

HardpointActuatorApplicationSettings ISettingReader::loadHardpointActuatorApplicationSettings() { return HardpointActuatorApplicationSettings(); }

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
