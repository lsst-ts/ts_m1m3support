/*
 * ISettingReader.h
 *
 *  Created on: Oct 3, 2017
 *      Author: ccontaxis
 */

#ifndef ISETTINGREADER_H_
#define ISETTINGREADER_H_

#include <AliasApplicationSettings.h>
#include <RecommendedApplicationSettings.h>
#include <ILCApplicationSettings.h>
#include <ForceActuatorApplicationSettings.h>
#include <HardpointActuatorApplicationSettings.h>

namespace LSST {
namespace M1M3 {
namespace SS {

class ISettingReader {
public:
	virtual ~ISettingReader();

	virtual void configure(std::string settingsToApply);
	virtual AliasApplicationSettings* loadAliasApplicationSettings();
	virtual RecommendedApplicationSettings* loadRecommendedApplicationSettings();
	virtual ILCApplicationSettings* loadILCApplicationSettings();
	virtual ForceActuatorApplicationSettings* loadForceActuatorApplicationSettings();
	virtual HardpointActuatorApplicationSettings* loadHardpointActuatorApplicationSettings();
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* ISETTINGREADER_H_ */
