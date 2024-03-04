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

#include <spdlog/spdlog.h>

#include <DistributedForces.h>
#include <ForceActuatorApplicationSettings.h>
#include <ForceActuatorSettings.h>
#include <ForceControllerState.h>
#include <ForcesAndMoments.h>
#include <M1M3SSPublisher.h>
#include <Model.h>
#include <Range.h>
#include <SafetyController.h>
#include <ThermalForceComponent.h>

namespace LSST {
namespace M1M3 {
namespace SS {

ThermalForceComponent::ThermalForceComponent(
        ForceActuatorApplicationSettings *forceActuatorApplicationSettings)
        : ForceComponent("Thermal", &ForceActuatorSettings::instance().ThermalComponentSettings) {
    _safetyController = Model::instance().getSafetyController();
    _forceActuatorApplicationSettings = forceActuatorApplicationSettings;
    _forceSetpointWarning = M1M3SSPublisher::instance().getEventForceSetpointWarning();
    _appliedThermalForces = M1M3SSPublisher::instance().getAppliedThermalForces();
    _preclippedThermalForces = M1M3SSPublisher::instance().getEventPreclippedThermalForces();
}

#ifdef WITH_SAL_KAFKA
void ThermalForceComponent::applyThermalForces(std::vector<float> x, std::vector<float> y, std::vector<float> z) {
#else
void ThermalForceComponent::applyThermalForces(float *x, float *y, float *z) {
#endif
    SPDLOG_TRACE("ThermalForceComponent: applyThermalForces()");

    if (!isEnabled()) {
        SPDLOG_ERROR(
                "ThermalForceComponent: applyThermalForces() called when the "
                "component is not applied");
        return;
    }

    for (int i = 0; i < FA_COUNT; ++i) {
        if (i < FA_X_COUNT) {
            xTarget[i] = x[i];
        }

        if (i < FA_Y_COUNT) {
            yTarget[i] = y[i];
        }

        zTarget[i] = z[i];
    }
}  // namespace SS

void ThermalForceComponent::applyThermalForcesByMirrorTemperature(float temperature) {
    SPDLOG_TRACE("ThermalForceComponent: applyThermalForcesByMirrorForces({:.1f})", temperature);
    DistributedForces forces = ForceActuatorSettings::instance().calculateForceFromTemperature(temperature);
#ifdef WITH_SAL_KAFKA
    std::vector<float> xForces(FA_X_COUNT,0);
    std::vector<float> yForces(FA_Y_COUNT,0);
    std::vector<float> zForces(FA_Z_COUNT,0);
#else
    float xForces[FA_X_COUNT];
    float yForces[FA_Y_COUNT];
    float zForces[FA_Z_COUNT];
#endif
    for (int zIndex = 0; zIndex < FA_COUNT; ++zIndex) {
        int xIndex = _forceActuatorApplicationSettings->ZIndexToXIndex[zIndex];
        int yIndex = _forceActuatorApplicationSettings->ZIndexToYIndex[zIndex];

        if (xIndex != -1) {
            xForces[xIndex] = forces.XForces[zIndex];
        }
        if (yIndex != -1) {
            yForces[yIndex] = forces.YForces[zIndex];
        }
        zForces[zIndex] = forces.ZForces[zIndex];
    }
    applyThermalForces(xForces, yForces, zForces);
}

void ThermalForceComponent::postEnableDisableActions() {
    SPDLOG_DEBUG("ThermalForceComponent: postEnableDisableActions()");

    ForceControllerState::instance().set_thermalForcesApplied(isEnabled());
}

void ThermalForceComponent::postUpdateActions() {
    SPDLOG_TRACE("ThermalForceController: postUpdateActions()");

    bool notInRange = false;
    bool clippingRequired = false;
    _appliedThermalForces->timestamp = M1M3SSPublisher::instance().getTimestamp();
    _preclippedThermalForces->timestamp = _appliedThermalForces->timestamp;
    for (int zIndex = 0; zIndex < FA_COUNT; ++zIndex) {
        int xIndex = _forceActuatorApplicationSettings->ZIndexToXIndex[zIndex];
        int yIndex = _forceActuatorApplicationSettings->ZIndexToYIndex[zIndex];

        _forceSetpointWarning->thermalForceWarning[zIndex] = false;

        if (xIndex != -1) {
            float xLowFault = ForceActuatorSettings::instance().ThermalLimitXTable[xIndex].LowFault;
            float xHighFault = ForceActuatorSettings::instance().ThermalLimitXTable[xIndex].HighFault;
            _preclippedThermalForces->xForces[xIndex] = xCurrent[xIndex];
            notInRange =
                    !Range::InRangeAndCoerce(xLowFault, xHighFault, _preclippedThermalForces->xForces[xIndex],
                                             _appliedThermalForces->xForces[xIndex]);
            _forceSetpointWarning->thermalForceWarning[zIndex] =
                    notInRange || _forceSetpointWarning->thermalForceWarning[zIndex];
        }

        if (yIndex != -1) {
            float yLowFault = ForceActuatorSettings::instance().ThermalLimitYTable[yIndex].LowFault;
            float yHighFault = ForceActuatorSettings::instance().ThermalLimitYTable[yIndex].HighFault;
            _preclippedThermalForces->yForces[yIndex] = yCurrent[yIndex];
            notInRange =
                    !Range::InRangeAndCoerce(yLowFault, yHighFault, _preclippedThermalForces->yForces[yIndex],
                                             _appliedThermalForces->yForces[yIndex]);
            _forceSetpointWarning->thermalForceWarning[zIndex] =
                    notInRange || _forceSetpointWarning->thermalForceWarning[zIndex];
        }

        float zLowFault = ForceActuatorSettings::instance().ThermalLimitZTable[zIndex].LowFault;
        float zHighFault = ForceActuatorSettings::instance().ThermalLimitZTable[zIndex].HighFault;
        _preclippedThermalForces->zForces[zIndex] = zCurrent[zIndex];
        notInRange =
                !Range::InRangeAndCoerce(zLowFault, zHighFault, _preclippedThermalForces->zForces[zIndex],
                                         _appliedThermalForces->zForces[zIndex]);
        _forceSetpointWarning->thermalForceWarning[zIndex] =
                notInRange || _forceSetpointWarning->thermalForceWarning[zIndex];
        clippingRequired = _forceSetpointWarning->thermalForceWarning[zIndex] || clippingRequired;
    }

    ForcesAndMoments fm = ForceActuatorSettings::instance().calculateForcesAndMoments(
            _forceActuatorApplicationSettings, _appliedThermalForces->xForces, _appliedThermalForces->yForces,
            _appliedThermalForces->zForces);
    _appliedThermalForces->fx = fm.Fx;
    _appliedThermalForces->fy = fm.Fy;
    _appliedThermalForces->fz = fm.Fz;
    _appliedThermalForces->mx = fm.Mx;
    _appliedThermalForces->my = fm.My;
    _appliedThermalForces->mz = fm.Mz;
    _appliedThermalForces->forceMagnitude = fm.ForceMagnitude;

    fm = ForceActuatorSettings::instance().calculateForcesAndMoments(
            _forceActuatorApplicationSettings, _preclippedThermalForces->xForces,
            _preclippedThermalForces->yForces, _preclippedThermalForces->zForces);
    _preclippedThermalForces->fx = fm.Fx;
    _preclippedThermalForces->fy = fm.Fy;
    _preclippedThermalForces->fz = fm.Fz;
    _preclippedThermalForces->mx = fm.Mx;
    _preclippedThermalForces->my = fm.My;
    _preclippedThermalForces->mz = fm.Mz;
    _preclippedThermalForces->forceMagnitude = fm.ForceMagnitude;

    _safetyController->forceControllerNotifyThermalForceClipping(clippingRequired);

    M1M3SSPublisher::instance().tryLogForceSetpointWarning();
    if (clippingRequired) {
        M1M3SSPublisher::instance().logPreclippedThermalForces();
    }
    M1M3SSPublisher::instance().logAppliedThermalForces();
}

}  // namespace SS
}  // namespace M1M3
} /* namespace LSST */
