/*
 * This file is part of LSST M1M3 support system package.
 *
 * Developed for the Vera C. Rubin Telescope and Site System.
 * This product includes software developed by the LSST Project
 * (https://www.lsst.org).
 * See the COPYRIGHT file at the top-level directory of this distribution
 * for details of code ownership.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef SAFETYCONTROLLERSETTINGS_H_
#define SAFETYCONTROLLERSETTINGS_H_

#include <string>

#include <yaml-cpp/yaml.h>

#include <AirControllerSafetySettings.h>
#include <CellLightsSafetySettings.h>
#include <DisplacementSafetySettings.h>
#include <ForceControllerSafetySettings.h>
#include <ILCSafetySettings.h>
#include <InclinometerSafetySettings.h>
#include <InterlockControllerSafetySettings.h>
#include <LowerOperationSafetySettings.h>
#include <PositionControllerSafetySettings.h>
#include <PowerControllerSafetySettings.h>
#include <RaiseOperationSafetySettings.h>
#include <TMASafetySettings.h>

namespace LSST {
namespace M1M3 {
namespace SS {

/**
 * Safety controller class.
 */
class SafetyControllerSettings {
public:
    AirControllerSafetySettings AirController;
    DisplacementSafetySettings Displacement;
    InclinometerSafetySettings Inclinometer;
    InterlockControllerSafetySettings Interlock;

    ForceControllerSafetySettings ForceController;
    CellLightsSafetySettings CellLights;
    PositionControllerSafetySettings PositionController;
    PowerControllerSafetySettings PowerController;
    RaiseOperationSafetySettings RaiseOperation;
    LowerOperationSafetySettings LowerOperation;
    ILCSafetySettings ILC;
    TMASafetySettings TMA;

    /**
     * Loads safety settings.
     *
     * @param filename filename to load
     *
     * @throws std::runtime_error on error
     */
    void load(YAML::Node doc);
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* SAFETYCONTROLLERSETTINGS_H_ */
