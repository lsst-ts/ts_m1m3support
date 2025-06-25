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

using namespace LSST::M1M3::SS;

ThermalForceComponent::ThermalForceComponent()
        : ForceComponent("Thermal", &ForceActuatorSettings::instance().ThermalComponentSettings),
          _preclipped_thermal_forces(
                  [](MTM1M3_logevent_preclippedThermalForcesC *data) {
                      M1M3SSPublisher::instance().logPreclippedThermalForces(data);
                  },
                  ForceActuatorSettings::instance().preclippedIgnoreChanges,
                  std::chrono::milliseconds(
                          static_cast<int>(ForceActuatorSettings::instance().preclippedMaxDelay * 1000.0))) {
    _safetyController = Model::instance().getSafetyController();
    _forceSetpointWarning = M1M3SSPublisher::instance().getEventForceSetpointWarning();
    _appliedThermalForces = M1M3SSPublisher::instance().getAppliedThermalForces();
}

void ThermalForceComponent::applyThermalForces(const std::vector<float> &x, const std::vector<float> &y,
                                               const std::vector<float> &z) {
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

    auto &faa_settings = ForceActuatorApplicationSettings::instance();

    std::vector<float> xForces(FA_X_COUNT, 0);
    std::vector<float> yForces(FA_Y_COUNT, 0);
    std::vector<float> zForces(FA_Z_COUNT, 0);
    for (int zIndex = 0; zIndex < FA_COUNT; ++zIndex) {
        int xIndex = faa_settings.ZIndexToXIndex[zIndex];
        int yIndex = faa_settings.ZIndexToYIndex[zIndex];

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

    auto &faa_settings = ForceActuatorApplicationSettings::instance();

    bool notInRange = false;
    bool clippingRequired = false;
    _appliedThermalForces->timestamp = M1M3SSPublisher::instance().getTimestamp();
    _preclipped_thermal_forces.timestamp = _appliedThermalForces->timestamp;
    for (int zIndex = 0; zIndex < FA_COUNT; ++zIndex) {
        int xIndex = faa_settings.ZIndexToXIndex[zIndex];
        int yIndex = faa_settings.ZIndexToYIndex[zIndex];

        _forceSetpointWarning->thermalForceWarning[zIndex] = false;

        if (xIndex != -1) {
            float xLowFault = ForceActuatorSettings::instance().ThermalLimitXTable[xIndex].LowFault;
            float xHighFault = ForceActuatorSettings::instance().ThermalLimitXTable[xIndex].HighFault;
            _preclipped_thermal_forces.xForces[xIndex] = xCurrent[xIndex];
            notInRange = !Range::InRangeAndCoerce(xLowFault, xHighFault,
                                                  _preclipped_thermal_forces.xForces[xIndex],
                                                  _appliedThermalForces->xForces[xIndex]);
            _forceSetpointWarning->thermalForceWarning[zIndex] =
                    notInRange || _forceSetpointWarning->thermalForceWarning[zIndex];
        }

        if (yIndex != -1) {
            float yLowFault = ForceActuatorSettings::instance().ThermalLimitYTable[yIndex].LowFault;
            float yHighFault = ForceActuatorSettings::instance().ThermalLimitYTable[yIndex].HighFault;
            _preclipped_thermal_forces.yForces[yIndex] = yCurrent[yIndex];
            notInRange = !Range::InRangeAndCoerce(yLowFault, yHighFault,
                                                  _preclipped_thermal_forces.yForces[yIndex],
                                                  _appliedThermalForces->yForces[yIndex]);
            _forceSetpointWarning->thermalForceWarning[zIndex] =
                    notInRange || _forceSetpointWarning->thermalForceWarning[zIndex];
        }

        float zLowFault = ForceActuatorSettings::instance().ThermalLimitZTable[zIndex].LowFault;
        float zHighFault = ForceActuatorSettings::instance().ThermalLimitZTable[zIndex].HighFault;
        _preclipped_thermal_forces.zForces[zIndex] = zCurrent[zIndex];
        notInRange =
                !Range::InRangeAndCoerce(zLowFault, zHighFault, _preclipped_thermal_forces.zForces[zIndex],
                                         _appliedThermalForces->zForces[zIndex]);
        _forceSetpointWarning->thermalForceWarning[zIndex] =
                notInRange || _forceSetpointWarning->thermalForceWarning[zIndex];
        clippingRequired = _forceSetpointWarning->thermalForceWarning[zIndex] || clippingRequired;
    }

    ForcesAndMoments fm = ForceActuatorSettings::instance().calculateForcesAndMoments(
            _appliedThermalForces->xForces, _appliedThermalForces->yForces, _appliedThermalForces->zForces);
    _appliedThermalForces->fx = fm.Fx;
    _appliedThermalForces->fy = fm.Fy;
    _appliedThermalForces->fz = fm.Fz;
    _appliedThermalForces->mx = fm.Mx;
    _appliedThermalForces->my = fm.My;
    _appliedThermalForces->mz = fm.Mz;
    _appliedThermalForces->forceMagnitude = fm.ForceMagnitude;

    _safetyController->forceControllerNotifyThermalForceClipping(clippingRequired);

    M1M3SSPublisher::instance().tryLogForceSetpointWarning();
    if (clippingRequired) {
        _preclipped_thermal_forces.calculate_forces_and_moments();
        _preclipped_thermal_forces.check_changes();
    }
    M1M3SSPublisher::instance().logAppliedThermalForces();
}
