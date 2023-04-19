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
    anyPrimaryAxisMeasuredForceWarning = false;
    anyPrimaryAxisFollowingErrorWarning = false;
    anyPrimaryAxisFollowingErrorCountingFault = false;
    anyPrimaryAxisFollowingErrorImmediateFault = false;

    for (int i = 0; i < FA_COUNT; i++) {
        primaryAxisMeasuredForceWarning[i] = false;
        primaryAxisFollowingErrorWarning[i] = false;
        primaryAxisFollowingErrorCountingFault[i] = false;
        primaryAxisFollowingErrorImmediateFault[i] = false;
    }

    anySecondaryAxisMeasuredForceWarning = false;
    anySecondaryAxisFollowingErrorWarning = false;
    anySecondaryAxisFollowingErrorCountingFault = false;
    anySecondaryAxisFollowingErrorImmediateFault = false;

    for (int i = 0; i < FA_S_COUNT; i++) {
        secondaryAxisMeasuredForceWarning[i] = false;
        secondaryAxisFollowingErrorWarning[i] = false;
        secondaryAxisFollowingErrorCountingFault[i] = false;
        secondaryAxisFollowingErrorImmediateFault[i] = false;
    }
}

void ForceActuatorForceWarning::setTimestamp(double globalTimestamp) { timestamp = globalTimestamp; }

bool ForceActuatorForceWarning::checkPrimary(int dataIndex, int actuatorId, float primaryForce,
                                             float primarySetpoint) {
    _checkPrimaryFollowingError(dataIndex, primaryForce, primarySetpoint);
    return _checkPrimaryMeasuredForce(dataIndex, actuatorId, primaryForce);
}

bool ForceActuatorForceWarning::checkSecondary(int dataIndex, int actuatorId, float secondaryForce,
                                               float secondarySetpoint) {
    _checkSecondaryFollowingError(dataIndex, secondaryForce, secondarySetpoint);
    return _checkSecondaryMeasuredForce(dataIndex, actuatorId, secondaryForce);
}

bool ForceActuatorForceWarning::_checkPrimaryMeasuredForce(int dataIndex, int actuatorId,
                                                           float primaryForce) {
    float primaryLowLimit = ForceActuatorSettings::instance().primaryCylinderMeasuredForceLowLimit[dataIndex];
    float primaryHighLimit =
            ForceActuatorSettings::instance().primaryCylinderMeasuredForceHighLimit[dataIndex];

    if (primaryForce < primaryLowLimit || primaryForce > primaryHighLimit) {
        return true;
    }

    auto primaryLowWarning = primaryLowLimit * _measuredForceWarningRatio;
    auto primaryHighWarning = primaryHighLimit * _measuredForceWarningRatio;

    bool primaryLimitWarning = primaryForce < primaryLowWarning || primaryForce > primaryHighWarning;
    if (primaryLimitWarning != primaryAxisMeasuredForceWarning[dataIndex]) {
        if (primaryLimitWarning) {
            anyWarning = true;
            anyPrimaryAxisMeasuredForceWarning = true;
            SPDLOG_WARN(
                    "Primary measured force for actuator {}: measured {:.1f} N isn't between {:.1f} N and "
                    "{:.1f} N",
                    actuatorId, primaryForce, primaryLowWarning, primaryHighWarning);
        } else {
            SPDLOG_INFO("Primary measured force for actuator {} in limits", actuatorId);
        }
        primaryAxisMeasuredForceWarning[dataIndex] = primaryLimitWarning;
        _shouldSend = true;
    }

    return false;
}

void ForceActuatorForceWarning::_checkPrimaryFollowingError(int dataIndex, float primaryForce,
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

bool ForceActuatorForceWarning::_checkSecondaryMeasuredForce(int dataIndex, int actuatorId,
                                                             float secondaryForce) {
    float secondaryLowLimit =
            ForceActuatorSettings::instance().secondaryCylinderMeasuredForceLowLimit[dataIndex];
    float secondaryHighLimit =
            ForceActuatorSettings::instance().secondaryCylinderMeasuredForceHighLimit[dataIndex];

    if (secondaryForce < secondaryLowLimit || secondaryForce > secondaryHighLimit) {
        return true;
    }

    auto secondaryLowWarning = secondaryLowLimit * _measuredForceWarningRatio;
    auto secondaryHighWarning = secondaryHighLimit * _measuredForceWarningRatio;

    bool secondaryLimitWarning =
            secondaryForce < secondaryLowWarning || secondaryForce > secondaryHighWarning;
    if (secondaryLimitWarning != secondaryAxisMeasuredForceWarning[dataIndex]) {
        if (secondaryLimitWarning) {
            anyWarning = true;
            anySecondaryAxisMeasuredForceWarning = true;
            SPDLOG_WARN(
                    "Secondary measured force for actuator {}: measured {:.1f} N isn't between {:.1f} N and "
                    "{:.1f} N",
                    actuatorId, secondaryForce, secondaryLowWarning, secondaryHighWarning);
        } else {
            SPDLOG_INFO("Primary measured force for actuator {} in limits", actuatorId);
        }
    }
    secondaryAxisMeasuredForceWarning[dataIndex] = secondaryLimitWarning;
    _shouldSend = true;

    return false;
}

void ForceActuatorForceWarning::_checkSecondaryFollowingError(int dataIndex, float secondaryForce,
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

void ForceActuatorForceWarning::send() {
    if (_shouldSend == false) {
        return;
    }
    anyWarning = anyPrimaryAxisMeasuredForceWarning || anySecondaryAxisMeasuredForceWarning ||
                 anyPrimaryAxisFollowingErrorWarning || anySecondaryAxisFollowingErrorWarning;
    anyFault = anyPrimaryAxisFollowingErrorCountingFault || anySecondaryAxisFollowingErrorCountingFault ||
               anyPrimaryAxisFollowingErrorImmediateFault || anySecondaryAxisFollowingErrorImmediateFault;

    M1M3SSPublisher::instance().logForceActuatorForceWarning(this);

    _shouldSend = false;
}
