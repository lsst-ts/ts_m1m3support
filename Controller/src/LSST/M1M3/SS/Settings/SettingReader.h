/*
 * SettingReader.h
 *
 *  Created on: Oct 3, 2017
 *      Author: ccontaxis
 */

#ifndef SETTINGREADER_H_
#define SETTINGREADER_H_

#include <ISettingReader.h>
#include <string>

namespace LSST {
namespace M1M3 {
namespace SS {

class SettingReader: public ISettingReader {
private:
	std::string basePath;
	std::string setsPath;
	std::string currentSet;
	std::string currentVersion;

	std::string getBasePath(std::string file);
	std::string getSetPath(std::string file);

public:
	SettingReader(std::string basePath, std::string setsPath);

	virtual void configure(std::string settingsToApply);
	virtual AliasApplicationSettings loadAliasApplicationSettings();
	virtual RecommendedApplicationSettings loadRecommendedApplicationSettings();
	virtual ILCApplicationSettings loadILCApplicationSettings();
	virtual ForceActuatorApplicationSettings loadForceActuatorApplicationSettings();
	virtual HardpointActuatorApplicationSettings loadHardpointActuatorApplicationSettings();
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* SETTINGREADER_H_ */
