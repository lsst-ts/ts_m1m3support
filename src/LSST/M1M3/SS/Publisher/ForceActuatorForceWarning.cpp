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
#include <ForceActuatorForceWarning.h>
#include <ForceActuatorSettings.h>
#include <M1M3SSPublisher.h>
#include <SafetyController.h>
#include <SettingReader.h>

using namespace LSST::M1M3::SS;

ForceActuatorForceWarning::ForceActuatorForceWarning(token) { reset(); }

void ForceActuatorForceWarning::reset() {
    _shouldSend = false;
    _wasSend = false;

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

void ForceActuatorForceWarning::checkPrimary(int dataIndex, float primaryForce, float primarySetpoint) {
    _checkPrimaryMeasuredForce(dataIndex, primaryForce);
    _checkPrimaryFollowingError(dataIndex, primaryForce, primarySetpoint);
}

void ForceActuatorForceWarning::checkSecondary(int dataIndex, float secondaryForce, float secondarySetpoint) {
    _checkSecondaryMeasuredForce(dataIndex, secondaryForce);
    _checkSecondaryFollowingError(dataIndex, secondaryForce, secondarySetpoint);
}

void ForceActuatorForceWarning::_checkPrimaryMeasuredForce(int dataIndex, float primaryForce) {
    float primaryLowLimit =
            ForceActuatorSettings::instance().MeasuredPrimaryCylinderLimitTable[dataIndex].LowFault;
    float primaryHighLLimit =
            ForceActuatorSettings::instance().MeasuredPrimaryCylinderLimitTable[dataIndex].HighFault;
    bool primaryLimitWarning = primaryForce < primaryLowLimit || primaryForce > primaryHighLLimit;
    if (primaryLimitWarning != primaryAxisMeasuredForceWarning[dataIndex]) {
        int actuatorId = SettingReader::instance().getForceActuatorApplicationSettings()->ZIndexToActuatorId(
                dataIndex);
        if (primaryLimitWarning) {
            anyWarning = true;
            anyPrimaryAxisMeasuredForceWarning = true;
            SPDLOG_WARN(
                    "Primary measured force for actuator {}: measured {:.1f} isn't between {:.1f} and {:.1f}",
                    actuatorId, primaryForce, primaryLowLimit, primaryHighLLimit);
        } else {
            SPDLOG_INFO("Primary measured force for actuator {} in limits", actuatorId);
        }
        primaryAxisMeasuredForceWarning[dataIndex] = primaryLimitWarning;
        _shouldSend = true;
    }
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
}

void ForceActuatorForceWarning::_checkSecondaryMeasuredForce(int dataIndex, float secondaryForce) {
    float secondaryLowLimit =
            ForceActuatorSettings::instance().MeasuredSecondaryCylinderLimitTable[dataIndex].LowFault;
    float secondaryHighLLimit =
            ForceActuatorSettings::instance().MeasuredSecondaryCylinderLimitTable[dataIndex].HighFault;
    bool secondaryLimitWarning = secondaryForce < secondaryLowLimit || secondaryForce > secondaryHighLLimit;
    if (secondaryLimitWarning != secondaryAxisMeasuredForceWarning[dataIndex]) {
        if (secondaryLimitWarning) {
            anyWarning = true;
            anySecondaryAxisMeasuredForceWarning = true;
        }
        secondaryAxisMeasuredForceWarning[dataIndex] = secondaryLimitWarning;
        _shouldSend = true;
    }
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
}

void ForceActuatorForceWarning::send() {
    if (_shouldSend == false && _wasSend == true) {
        return;
    }
    anyWarning = anyPrimaryAxisMeasuredForceWarning || anySecondaryAxisMeasuredForceWarning ||
                 anyPrimaryAxisFollowingErrorWarning || anySecondaryAxisFollowingErrorWarning;
    anyFault = anyPrimaryAxisFollowingErrorCountingFault || anySecondaryAxisFollowingErrorCountingFault ||
               anyPrimaryAxisFollowingErrorImmediateFault || anySecondaryAxisFollowingErrorImmediateFault;

    M1M3SSPublisher::instance().logForceActuatorForceWarning(this);

    _shouldSend = false;
    _wasSend = true;
}
