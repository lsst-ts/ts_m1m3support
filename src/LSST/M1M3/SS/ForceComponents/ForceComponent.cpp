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

#include <ForceComponent.h>
#include <cstring>
#include <cmath>
#include <spdlog/spdlog.h>

namespace LSST {
namespace M1M3 {
namespace SS {

ForceComponent::ForceComponent(const char *name, const ForceComponentSettings &forceComponentSettings)
        : _name(name),
          _maxRateOfChange(forceComponentSettings.MaxRateOfChange),
          _nearZeroValue(forceComponentSettings.NearZeroValue) {
    _state = INITIALISING;

    _zeroAll();
}

ForceComponent::~ForceComponent() {}

void ForceComponent::enable() {
    // Enable and set the target to 0N
    SPDLOG_DEBUG("{}ForceComponent: enable()", _name);
    _state = ENABLED;
    _zeroTarget();
    postEnableDisableActions();
}

void ForceComponent::disable() {
    // Start disabling and driving to 0N
    SPDLOG_DEBUG("{}ForceComponent: disable()", _name);
    _state = DISABLING;
    _zeroTarget();
}

void ForceComponent::update() {
    if (isDisabling()) {
        // If we are disabling we need to keep driving this force component to 0N
        // Once we are near zero we consider our action complete and that the force
        // component is actually disabled
        bool nearZero = true;
        for (int i = 0; i < FA_COUNT && nearZero; ++i) {
            if (i < FA_X_COUNT) {
                nearZero = nearZero && fabs(xCurrent[i]) < _nearZeroValue;
            }

            if (i < FA_Y_COUNT) {
                nearZero = nearZero && fabs(yCurrent[i]) < _nearZeroValue;
            }

            nearZero = nearZero && fabs(zCurrent[i]) < _nearZeroValue;
        }
        if (nearZero) {
            SPDLOG_DEBUG("{}ForceComponent: disabled()", _name);
            _state = DISABLED;
            memset(xCurrent, 0, sizeof(xCurrent));
            memset(yCurrent, 0, sizeof(yCurrent));
            memset(zCurrent, 0, sizeof(zCurrent));
            postEnableDisableActions();
            postUpdateActions();
        }
    }
    if (isInitialising()) {
        postUpdateActions();
        _state = DISABLED;
    } else if (isEnabled() || isDisabling()) {
        // If this force component is enabled then we need to keep trying
        // to drive this force component to it's target value.
        // To do this we need to find the vector with the largest delta
        // and scale all other vectors based off how long it will take to
        // drive that delta to 0N.
        float largestDelta = 0.0;
        for (int i = 0; i < FA_COUNT; ++i) {
            if (i < FA_X_COUNT) {
                xOffset[i] = xTarget[i] - xCurrent[i];
                if (std::abs(xOffset[i]) > largestDelta) {
                    largestDelta = std::abs(xOffset[i]);
                }
            }

            if (i < FA_Y_COUNT) {
                yOffset[i] = yTarget[i] - yCurrent[i];
                if (std::abs(yOffset[i]) > largestDelta) {
                    largestDelta = std::abs(yOffset[i]);
                }
            }

            zOffset[i] = zTarget[i] - zCurrent[i];
            if (std::abs(zOffset[i]) > largestDelta) {
                largestDelta = std::abs(zOffset[i]);
            }
        }
        // Determine how many outer loop cycles it will take to drive the
        // largest delta to 0N and use that as a scalar for all other
        // actuator deltas.
        float scalar = largestDelta / _maxRateOfChange;
        if (scalar > 1) {
            // If it is more than 1 outer loop cycle keep working, we aren't
            // then we need to keep working!
            for (int i = 0; i < FA_COUNT; ++i) {
                if (i < FA_X_COUNT) {
                    xOffset[i] /= scalar;
                    xCurrent[i] += xOffset[i];
                }

                if (i < FA_Y_COUNT) {
                    yOffset[i] /= scalar;
                    yCurrent[i] += yOffset[i];
                }

                zOffset[i] /= scalar;
                zCurrent[i] += zOffset[i];
            }
        } else {
            // If it is less than 1 outer loop cycle just set current as the target
            // we do this to prevent rounding errors from making it so when we
            // request 100N we don't put 99.998N and claim that is what we where asked
            // to produce.
            for (int i = 0; i < FA_COUNT; ++i) {
                if (i < FA_X_COUNT) {
                    xCurrent[i] = xTarget[i];
                }

                if (i < FA_Y_COUNT) {
                    yCurrent[i] = yTarget[i];
                }

                zCurrent[i] = zTarget[i];
            }
        }
        postUpdateActions();
    }
}

void ForceComponent::reset() {
    _state = DISABLED;
    postEnableDisableActions();
    postUpdateActions();
}

void ForceComponent::_zeroTarget() {
    memset(xTarget, 0, sizeof(xTarget));
    memset(yTarget, 0, sizeof(yTarget));
    memset(zTarget, 0, sizeof(zTarget));
}

void ForceComponent::_zeroAll() {
    memset(xCurrent, 0, sizeof(xCurrent));
    memset(yCurrent, 0, sizeof(yCurrent));
    memset(zCurrent, 0, sizeof(zCurrent));
    _zeroTarget();
    memset(xOffset, 0, sizeof(xOffset));
    memset(yOffset, 0, sizeof(yOffset));
    memset(zOffset, 0, sizeof(zOffset));
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
