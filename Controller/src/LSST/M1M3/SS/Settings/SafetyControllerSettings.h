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
#include <ForceControllerSafetySettings.h>
#include <CellLightsSafetySettings.h>
#include <PowerControllerSafetySettings.h>
#include <RaiseOperationSafetySettings.h>
#include <LowerOperationSafetySettings.h>
#include <ILCSafetySettings.h>
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
    ForceControllerSafetySettings ForceController;
    CellLightsSafetySettings CellLights;
    PowerControllerSafetySettings PowerController;
    RaiseOperationSafetySettings RaiseOperation;
    LowerOperationSafetySettings LowerOperation;
    ILCSafetySettings ILC;

    void load(const std::string& filename);
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* SAFETYCONTROLLERSETTINGS_H_ */
