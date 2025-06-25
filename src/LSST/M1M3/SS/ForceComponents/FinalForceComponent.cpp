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

#include <algorithm>

#include <spdlog/spdlog.h>

#include "DistributedForces.h"
#include "FinalForceComponent.h"
#include "ForceActuatorApplicationSettings.h"
#include "ForceActuatorSettings.h"
#include "ForcesAndMoments.h"
#include "M1M3SSPublisher.h"
#include "Model.h"
#include "Range.h"
#include "SafetyController.h"
#include "SettingReader.h"

using namespace LSST::M1M3::SS;

FinalForceComponent::FinalForceComponent()
        : ForceComponent("Final", &ForceActuatorSettings::instance().FinalComponentSettings),
          _preclipped_forces(
                  [](MTM1M3_logevent_preclippedForcesC *data) {
                      M1M3SSPublisher::instance().logPreclippedForces(data);
                  },
                  ForceActuatorSettings::instance().preclippedIgnoreChanges,
                  std::chrono::milliseconds(
                          static_cast<int>(ForceActuatorSettings::instance().preclippedMaxDelay * 1000.0))) {
    _safetyController = Model::instance().getSafetyController();
    _enabledForceActuators = M1M3SSPublisher::instance().getEnabledForceActuators();
    _forceActuatorState = M1M3SSPublisher::instance().getEventForceActuatorState();
    _forceSetpointWarning = M1M3SSPublisher::instance().getEventForceSetpointWarning();
    _appliedForces = M1M3SSPublisher::instance().getAppliedForces();

    _appliedAccelerationForces = M1M3SSPublisher::instance().getAppliedAccelerationForces();
    _appliedActiveOpticForces = M1M3SSPublisher::instance().getEventAppliedActiveOpticForces();
    _appliedAzimuthForces = M1M3SSPublisher::instance().getAppliedAzimuthForces();
    _appliedBalanceForces = M1M3SSPublisher::instance().getAppliedBalanceForces();
    _appliedElevationForces = M1M3SSPublisher::instance().getAppliedElevationForces();
    _appliedOffsetForces = M1M3SSPublisher::instance().getEventAppliedOffsetForces();
    _appliedStaticForces = M1M3SSPublisher::instance().getEventAppliedStaticForces();
    _appliedThermalForces = M1M3SSPublisher::instance().getAppliedThermalForces();
    _appliedVelocityForces = M1M3SSPublisher::instance().getAppliedVelocityForces();

    enable();
}

void FinalForceComponent::applyForcesByComponents() {
    SPDLOG_TRACE("FinalForceComponent: applyForcesByComponents()");

    if (!isEnabled()) {
        enable();
    }

    for (int i = 0; i < FA_COUNT; ++i) {
        if (i < 12) {
            xTarget[i] = (_appliedAccelerationForces->xForces[i] + _appliedAzimuthForces->xForces[i] +
                          _appliedBalanceForces->xForces[i] + _appliedElevationForces->xForces[i] +
                          _appliedOffsetForces->xForces[i] + _appliedStaticForces->xForces[i] +
                          _appliedThermalForces->xForces[i] + _appliedVelocityForces->xForces[i]);
        }

        if (i < 100) {
            yTarget[i] = (_appliedAccelerationForces->yForces[i] + _appliedAzimuthForces->yForces[i] +
                          _appliedBalanceForces->yForces[i] + _appliedElevationForces->yForces[i] +
                          _appliedOffsetForces->yForces[i] + _appliedStaticForces->yForces[i] +
                          _appliedThermalForces->yForces[i] + _appliedVelocityForces->yForces[i]);
        }

        zTarget[i] = (_appliedAccelerationForces->zForces[i] + _appliedActiveOpticForces->zForces[i] +
                      _appliedAzimuthForces->zForces[i] + _appliedBalanceForces->zForces[i] +
                      _appliedElevationForces->zForces[i] + _appliedOffsetForces->zForces[i] +
                      _appliedStaticForces->zForces[i] + _appliedThermalForces->zForces[i] +
                      _appliedVelocityForces->zForces[i]);
    }

    auto &faa_settings = ForceActuatorApplicationSettings::instance();

    // find disabled FAs quadrants
    std::vector<int> disabledInQuadrants[4];

    for (int i = 0; i < FA_COUNT; i++) {
        if (_enabledForceActuators->forceActuatorEnabled[i] == false) {
            disabledInQuadrants[int(faa_settings.ZIndexToActuatorId(i) / 100) - 1].push_back(i);
        }
    }

    // 2nd pass - distribute any disabled FA force per quadrants
    for (int q = 0; q < 4; q++) {
        auto qd = disabledInQuadrants[q];

        if (disabledInQuadrants[q].size() == 0) {
            continue;
        }

        float excess_z = 0;
        float excess_y = 0;
        float excess_x = 0;

        int z_disabled = 0;
        int y_disabled = 0;
        int x_disabled = 0;

        for (auto disabled : qd) {
            excess_z += zTarget[disabled];
            zTarget[disabled] = 0;
            z_disabled++;

            int yIndex = faa_settings.ZIndexToYIndex[disabled];
            if (yIndex >= 0) {
                excess_y += yTarget[yIndex];
                yTarget[yIndex] = 0;
                y_disabled++;
            }

            int xIndex = faa_settings.ZIndexToXIndex[disabled];
            if (xIndex >= 0) {
                excess_x += xTarget[xIndex];
                xTarget[xIndex] = 0;
                x_disabled++;
            }
        }

        auto qz = faa_settings.QuadrantZ[q];

        for (auto fa : qz) {
            if (std::find(qd.begin(), qd.end(), fa) != std::end(qd)) {
                continue;
            }

            zTarget[fa] += excess_z / (qz.size() - z_disabled);
        }

        auto qy = faa_settings.QuadrantY[q];

        for (auto fa : qy) {
            if (std::find(qd.begin(), qd.end(), fa) != std::end(qd)) {
                continue;
            }

            yTarget[fa] += excess_y / (qy.size() - y_disabled);
        }

        auto qx = faa_settings.QuadrantX[q];

        for (auto fa : qx) {
            if (std::find(qd.begin(), qd.end(), fa) != std::end(qd)) {
                continue;
            }

            xTarget[fa] += excess_x / (qx.size() - x_disabled);
        }
    }
}

void FinalForceComponent::postEnableDisableActions() {
    SPDLOG_TRACE("FinalForceComponent: postEnableDisableActions()");
}

void FinalForceComponent::postUpdateActions() {
    SPDLOG_TRACE("FinalForceController: postUpdateActions()");

    auto &faa_settings = ForceActuatorApplicationSettings::instance();

    bool notInRange = false;
    bool clippingRequired = false;
    _appliedForces->timestamp = M1M3SSPublisher::instance().getTimestamp();
    _preclipped_forces.timestamp = _appliedForces->timestamp;
    for (int zIndex = 0; zIndex < FA_COUNT; ++zIndex) {
        int xIndex = faa_settings.ZIndexToXIndex[zIndex];
        int yIndex = faa_settings.ZIndexToYIndex[zIndex];

        _forceSetpointWarning->forceWarning[zIndex] = false;

        if (xIndex != -1) {
            float xLow = ForceActuatorSettings::instance().appliedXForceLowLimit[xIndex];
            float xHigh = ForceActuatorSettings::instance().appliedXForceHighLimit[xIndex];
            _preclipped_forces.xForces[xIndex] = xCurrent[xIndex];
            notInRange = !Range::InRangeAndCoerce(xLow, xHigh, _preclipped_forces.xForces[xIndex],
                                                  _appliedForces->xForces[xIndex]);
            _forceSetpointWarning->forceWarning[zIndex] =
                    notInRange || _forceSetpointWarning->forceWarning[zIndex];
        }

        if (yIndex != -1) {
            float yLow = ForceActuatorSettings::instance().appliedYForceLowLimit[yIndex];
            float yHigh = ForceActuatorSettings::instance().appliedYForceHighLimit[yIndex];
            _preclipped_forces.yForces[yIndex] = yCurrent[yIndex];
            notInRange = !Range::InRangeAndCoerce(yLow, yHigh, _preclipped_forces.yForces[yIndex],
                                                  _appliedForces->yForces[yIndex]);
            _forceSetpointWarning->forceWarning[zIndex] =
                    notInRange || _forceSetpointWarning->forceWarning[zIndex];
        }

        float zLow = ForceActuatorSettings::instance().appliedZForceLowLimit[zIndex];
        float zHigh = ForceActuatorSettings::instance().appliedZForceHighLimit[zIndex];
        _preclipped_forces.zForces[zIndex] = zCurrent[zIndex];
        notInRange = !Range::InRangeAndCoerce(zLow, zHigh, _preclipped_forces.zForces[zIndex],
                                              _appliedForces->zForces[zIndex]);
        _forceSetpointWarning->forceWarning[zIndex] =
                notInRange || _forceSetpointWarning->forceWarning[zIndex];
        clippingRequired = _forceSetpointWarning->forceWarning[zIndex] || clippingRequired;
    }

    ForcesAndMoments fm = ForceActuatorSettings::instance().calculateForcesAndMoments(
            _appliedForces->xForces, _appliedForces->yForces, _appliedForces->zForces);
    _appliedForces->fx = fm.Fx;
    _appliedForces->fy = fm.Fy;
    _appliedForces->fz = fm.Fz;
    _appliedForces->mx = fm.Mx;
    _appliedForces->my = fm.My;
    _appliedForces->mz = fm.Mz;
    _appliedForces->forceMagnitude = fm.ForceMagnitude;

    _safetyController->forceControllerNotifyForceClipping(clippingRequired);

    M1M3SSPublisher::instance().tryLogForceSetpointWarning();
    if (clippingRequired) {
        _preclipped_forces.calculate_forces_and_moments();
        _preclipped_forces.check_changes();
    }
    M1M3SSPublisher::instance().logAppliedForces();
}
