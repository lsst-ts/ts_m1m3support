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
#include <ForceActuatorSettings.h>
#include <M1M3SSPublisher.h>
#include <SafetyController.h>
#include <SettingReader.h>

using namespace LSST::M1M3::SS;

ForceActuatorFollowingErrorCounter::ForceActuatorFollowingErrorCounter(token) {
    _shouldSend = true;
    _wasSend = false;
    _lastSendCounter = 0;

    counter = 0;

    memset(primaryAxisFollowingErrorWarningCounter, 0, sizeof(primaryAxisFollowingErrorWarningCounter));
    memset(primaryAxisFollowingErrorCountingCounter, 0, sizeof(primaryAxisFollowingErrorCountingCounter));
    memset(secondaryAxisFollowingErrorWarningCounter, 0, sizeof(secondaryAxisFollowingErrorWarningCounter));
    memset(secondaryAxisFollowingErrorCountingCounter, 0, sizeof(secondaryAxisFollowingErrorCountingCounter));
}

void ForceActuatorFollowingErrorCounter::updatePrimaryCounts(int dataIndex, bool warningTriggered,
                                                             bool countingTriggered) {
    if (warningTriggered) {
        primaryAxisFollowingErrorWarningCounter[dataIndex]++;
        _shouldSend = true;
    }
    if (countingTriggered) {
        primaryAxisFollowingErrorCountingCounter[dataIndex]++;
        _shouldSend = true;
    }
}

void ForceActuatorFollowingErrorCounter::updateSecondaryCounts(int dataIndex, bool warningTriggered,
                                                               bool countingTriggered) {
    if (warningTriggered) {
        secondaryAxisFollowingErrorWarningCounter[dataIndex]++;
        _shouldSend = true;
    }
    if (countingTriggered) {
        secondaryAxisFollowingErrorCountingCounter[dataIndex]++;
        _shouldSend = true;
    }
}

void ForceActuatorFollowingErrorCounter::send() {
    increaseCounter();

    if (_shouldSend == false && _wasSend == true) {
        return;
    }

    if (counter < _lastSendCounter) {
        return;
    }

    M1M3SSPublisher::instance().logForceActuatorFollowingErrorCounter(this);

    _lastSendCounter = counter + 50;
    _shouldSend = false;
    _wasSend = true;
}
