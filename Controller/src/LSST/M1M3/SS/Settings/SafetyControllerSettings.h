/*
 * SafetyControllerSettings.h
 *
 *  Created on: Oct 26, 2017
 *      Author: ccontaxis
 */

#ifndef SAFETYCONTROLLERSETTINGS_H_
#define SAFETYCONTROLLERSETTINGS_H_

#include <AirControllerSafetySettings.h>
#include <DisplacementSafetySettings.h>
#include <InclinometerSafetySettings.h>
#include <InterlockControllerSafetySettings.h>
#include <string>

namespace LSST {
namespace M1M3 {
namespace SS {

class SafetyControllerSettings {
public:
	AirControllerSafetySettings AirController;
	DisplacementSafetySettings Displacement;
	InclinometerSafetySettings Inclinometer;
	InterlockControllerSafetySettings Interlock;

	void load(const std::string &filename);
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* SAFETYCONTROLLERSETTINGS_H_ */
