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

#include <ForceActuatorData.h>
#include <ForceActuatorFollowingErrorCounter.h>
#include <ForceActuatorForceWarning.h>
#include <ForceActuatorSettings.h>
#include <M1M3SSPublisher.h>
#include <SafetyController.h>
#include <SettingReader.h>

using namespace LSST::M1M3::SS;

ForceActuatorForceWarning::ForceActuatorForceWarning(token) {
    reset();
    _shouldSend = true;
    _measuredForceWarningRatio = 0.9;
}

void ForceActuatorForceWarning::reset() {
    timestamp = 0;
    anyWarning = false;
    anyFault = false;
    anyPrimaryAxisFollowingErrorWarning = false;
    anyPrimaryAxisFollowingErrorCountingFault = false;
    anyPrimaryAxisFollowingErrorImmediateFault = false;

    for (int i = 0; i < FA_COUNT; i++) {
        primaryAxisFollowingErrorWarning[i] = false;
        primaryAxisFollowingErrorCountingFault[i] = false;
        primaryAxisFollowingErrorImmediateFault[i] = false;
    }

    anySecondaryAxisFollowingErrorWarning = false;
    anySecondaryAxisFollowingErrorCountingFault = false;
    anySecondaryAxisFollowingErrorImmediateFault = false;

    for (int i = 0; i < FA_S_COUNT; i++) {
        secondaryAxisFollowingErrorWarning[i] = false;
        secondaryAxisFollowingErrorCountingFault[i] = false;
        secondaryAxisFollowingErrorImmediateFault[i] = false;
    }

    anyMeasuredZForceWarning = false;
    for (int i = 0; i < FA_Z_COUNT; i++) {
        measuredZForceWarning[i] = false;
    }

    anyMeasuredYForceWarning = false;
    for (int i = 0; i < FA_Y_COUNT; i++) {
        measuredYForceWarning[i] = false;
    }

    anyMeasuredXForceWarning = false;
    for (int i = 0; i < FA_X_COUNT; i++) {
        measuredXForceWarning[i] = false;
    }
}

void ForceActuatorForceWarning::setTimestamp(double globalTimestamp) { timestamp = globalTimestamp; }

void ForceActuatorForceWarning::checkPrimary(int dataIndex, int actuatorId, float primaryForce,
                                             float primarySetpoint) {
    float followingError = primaryForce - primarySetpoint;
    ForceActuatorData::instance().primaryCylinderFollowingError[dataIndex] = followingError;

    followingError = std::abs(followingError);

    float pFEWarningThreshold =
            ForceActuatorSettings::instance().primaryFollowingErrorWarningThreshold[dataIndex];
    float pFECountingThreshold =
            ForceActuatorSettings::instance().primaryFollowingErrorCountingFaultThreshold[dataIndex];
    float pFEImmediateLimit =
            ForceActuatorSettings::instance().primaryFollowingErrorImmediateFaultThreshold[dataIndex];

    bool warningTriggered = followingError > pFEWarningThreshold;
    bool countingTriggered = followingError > pFECountingThreshold;
    bool immediateTriggered = followingError > pFEImmediateLimit;

    if (warningTriggered != primaryAxisFollowingErrorWarning[dataIndex]) {
        if (warningTriggered) {
            anyWarning = true;
            anyPrimaryAxisFollowingErrorWarning = true;
        }
        primaryAxisFollowingErrorWarning[dataIndex] = warningTriggered;
        _shouldSend = true;
    }

    if (countingTriggered != primaryAxisFollowingErrorCountingFault[dataIndex]) {
        if (countingTriggered) {
            anyFault = true;
            anyPrimaryAxisFollowingErrorCountingFault = true;
        }
        primaryAxisFollowingErrorCountingFault[dataIndex] = countingTriggered;
        _shouldSend = true;
    }

    if (immediateTriggered != primaryAxisFollowingErrorImmediateFault[dataIndex]) {
        if (immediateTriggered) {
            anyFault = true;
            anyPrimaryAxisFollowingErrorImmediateFault = true;
        }
        primaryAxisFollowingErrorImmediateFault[dataIndex] = immediateTriggered;
        _shouldSend = true;
    }

    ForceActuatorFollowingErrorCounter::instance().updatePrimaryCounts(dataIndex, warningTriggered,
                                                                       countingTriggered);
}

void ForceActuatorForceWarning::checkSecondary(int dataIndex, int actuatorId, float secondaryForce,
                                               float secondarySetpoint) {
    float followingError = secondaryForce - secondarySetpoint;
    ForceActuatorData::instance().secondaryCylinderFollowingError[dataIndex] = followingError;

    followingError = std::abs(followingError);

    float sFEWarningThreshold =
            ForceActuatorSettings::instance().secondaryFollowingErrorWarningThreshold[dataIndex];
    float sFECountingThreshold =
            ForceActuatorSettings::instance().secondaryFollowingErrorCountingFaultThreshold[dataIndex];
    float sFEImmediateLimit =
            ForceActuatorSettings::instance().secondaryFollowingErrorImmediateFaultThreshold[dataIndex];

    bool warningTriggered = followingError > sFEWarningThreshold;
    bool countingTriggered = followingError > sFECountingThreshold;
    bool immediateTriggered = followingError > sFEImmediateLimit;

    if (warningTriggered != secondaryAxisFollowingErrorWarning[dataIndex]) {
        if (warningTriggered) {
            anyWarning = true;
            anySecondaryAxisFollowingErrorWarning = true;
        }
        secondaryAxisFollowingErrorWarning[dataIndex] = warningTriggered;
        _shouldSend = true;
    }

    if (countingTriggered != secondaryAxisFollowingErrorCountingFault[dataIndex]) {
        if (countingTriggered) {
            anyFault = true;
            anySecondaryAxisFollowingErrorCountingFault = true;
        }
        secondaryAxisFollowingErrorCountingFault[dataIndex] = countingTriggered;
        _shouldSend = true;
    }

    if (immediateTriggered != secondaryAxisFollowingErrorImmediateFault[dataIndex]) {
        if (immediateTriggered) {
            anyFault = true;
            anySecondaryAxisFollowingErrorImmediateFault = true;
        }
        secondaryAxisFollowingErrorImmediateFault[dataIndex] = immediateTriggered;
        _shouldSend = true;
    }

    ForceActuatorFollowingErrorCounter::instance().updateSecondaryCounts(dataIndex, warningTriggered,
                                                                         countingTriggered);
}

bool ForceActuatorForceWarning::checkXMeasuredForce(int xIndex, int actuatorId, float xForce) {
    float xLowLimit = ForceActuatorSettings::instance().measuredXForceLowLimit[xIndex];
    float xHighLimit = ForceActuatorSettings::instance().measuredXForceHighLimit[xIndex];

    if (xForce < xLowLimit || xForce > xHighLimit) {
        return true;
    }

    auto xLowWarning = xLowLimit * _measuredForceWarningRatio;
    auto xHighWarning = xHighLimit * _measuredForceWarningRatio;

    bool xLimitWarning = xForce < xLowWarning || xForce > xHighWarning;
    if (xLimitWarning != measuredXForceWarning[xIndex]) {
        if (xLimitWarning) {
            anyWarning = true;
            anyMeasuredXForceWarning = true;
            SPDLOG_WARN(
                    "Measured X force for actuator {}: measured {:.1f} N isn't "
                    "between {:.1f} N and "
                    "{:.1f} N",
                    actuatorId, xForce, xLowWarning, xHighWarning);
        } else {
            SPDLOG_INFO("Measured X force for actuator {} in limits", actuatorId);
        }
        measuredXForceWarning[xIndex] = xLimitWarning;
        _shouldSend = true;
    }

    return false;
}

bool ForceActuatorForceWarning::checkYMeasuredForce(int yIndex, int actuatorId, float yForce) {
    float yLowLimit = ForceActuatorSettings::instance().measuredYForceLowLimit[yIndex];
    float yHighLimit = ForceActuatorSettings::instance().measuredYForceHighLimit[yIndex];

    if (yForce < yLowLimit || yForce > yHighLimit) {
        return true;
    }

    auto yLowWarning = yLowLimit * _measuredForceWarningRatio;
    auto yHighWarning = yHighLimit * _measuredForceWarningRatio;

    bool yLimitWarning = yForce < yLowWarning || yForce > yHighWarning;
    if (yLimitWarning != measuredYForceWarning[yIndex]) {
        if (yLimitWarning) {
            anyWarning = true;
            anyMeasuredYForceWarning = true;
            SPDLOG_WARN(
                    "Measured Y force for actuator {}: measured {:.1f} N isn't "
                    "between {:.1f} N and "
                    "{:.1f} N",
                    actuatorId, yForce, yLowWarning, yHighWarning);
        } else {
            SPDLOG_INFO("Measured Y force for actuator {} in limits", actuatorId);
        }
        measuredYForceWarning[yIndex] = yLimitWarning;
        _shouldSend = true;
    }

    return false;
}

bool ForceActuatorForceWarning::checkZMeasuredForce(int zIndex, int actuatorId, float zForce) {
    float zLowLimit = ForceActuatorSettings::instance().measuredZForceLowLimit[zIndex];
    float zHighLimit = ForceActuatorSettings::instance().measuredZForceHighLimit[zIndex];

    if (zForce < zLowLimit || zForce > zHighLimit) {
        return true;
    }

    auto zLowWarning = zLowLimit * _measuredForceWarningRatio;
    auto zHighWarning = zHighLimit * _measuredForceWarningRatio;

    bool zLimitWarning = zForce < zLowWarning || zForce > zHighWarning;
    if (zLimitWarning != measuredZForceWarning[zIndex]) {
        if (zLimitWarning) {
            anyWarning = true;
            anyMeasuredZForceWarning = true;
            SPDLOG_WARN(
                    "Measured Z force for actuator {}: measured {:.1f} N isn't "
                    "between {:.1f} N and "
                    "{:.1f} N",
                    actuatorId, zForce, zLowWarning, zHighWarning);
        } else {
            SPDLOG_INFO("Measured Z force for actuator {} in limits", actuatorId);
        }
        measuredZForceWarning[zIndex] = zLimitWarning;
        _shouldSend = true;
    }

    return false;
}

void ForceActuatorForceWarning::send() {
    if (_shouldSend == false) {
        return;
    }
    anyWarning = anyMeasuredZForceWarning || anyMeasuredYForceWarning || anyMeasuredXForceWarning ||
                 anyPrimaryAxisFollowingErrorWarning || anySecondaryAxisFollowingErrorWarning;
    anyFault = anyPrimaryAxisFollowingErrorCountingFault || anySecondaryAxisFollowingErrorCountingFault ||
               anyPrimaryAxisFollowingErrorImmediateFault || anySecondaryAxisFollowingErrorImmediateFault;

    M1M3SSPublisher::instance().logForceActuatorForceWarning(this);

    _shouldSend = false;
}
