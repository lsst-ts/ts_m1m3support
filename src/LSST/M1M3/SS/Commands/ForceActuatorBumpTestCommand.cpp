/*
 * This file is part of LSST M1M3 support system package.
 *
 * Developed for the LSST Telescope & Site Software Systems.
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

#include <cRIO/SAL/Command.h>

#include <Context.h>
#include <ForceActuatorBumpTestStatus.h>
#include <M1M3SSPublisher.h>
#include <Model.h>
#include <SettingReader.h>

using namespace LSST::cRIO::SAL;
using namespace LSST::M1M3::SS;

ForceActuatorBumpTestCommand::ForceActuatorBumpTestCommand(int32_t commandID,
                                                           MTM1M3_command_forceActuatorBumpTestC *data)
        : Command(commandID), index(0), cylinders(0) {
    memcpy(&_data, data, sizeof(MTM1M3_command_forceActuatorBumpTestC));
}

bool ForceActuatorBumpTestCommand::validate() {
    if (_data.actuatorId < 0) {
        _data.actuatorId *= -1;
        cylinders = true;
    } else {
        cylinders = false;
    }

    index = SettingReader::instance().getForceActuatorApplicationSettings()->ActuatorIdToZIndex(
            _data.actuatorId);

    if (index == -1) {
        M1M3SSPublisher::instance().logCommandRejectionWarning("ForceActuatorBumpTest",
                                                               "Invalid actuatorId.");
        return false;
    }
    auto min_distance = ForceActuatorSettings::instance().bumpTestMinimalDistance;
    int min_actuator_id;
    auto measured_distance =
            ForceActuatorBumpTestStatus::instance().minimal_tested_distance(index, min_actuator_id);
    if (measured_distance <= min_distance) {
        M1M3SSPublisher::instance().logCommandRejectionWarning(
                "ForceActuatorBumpTest",
                fmt::format(
                        "Actuator with ID {} cannot be tested  - {:0.2}m is closer then {:0.2}m to already "
                        "tested FA {}.",
                        _data.actuatorId, measured_distance, min_distance, min_actuator_id));
        return false;
    }
    if (Model::instance().getILC()->isDisabled(_data.actuatorId)) {
        M1M3SSPublisher::instance().logCommandRejectionWarning(
                "ForceActuatorBumpTest",
                fmt::format("Cannnot bump test disabled force actuator {}", _data.actuatorId));
        return false;
    }
    return true;
}

void ForceActuatorBumpTestCommand::execute() { Context::instance().forceActuatorBumpTest(this); }

void ForceActuatorBumpTestCommand::ackInProgress(const char *description, double timeout) {
    M1M3SSPublisher::instance().ackCommandforceActuatorBumpTest(getCommandID(), ACK_INPROGRESS, description,
                                                                timeout);
}

void ForceActuatorBumpTestCommand::ackComplete() {
    M1M3SSPublisher::instance().ackCommandforceActuatorBumpTest(getCommandID(), ACK_COMPLETE, "Completed");
}

void ForceActuatorBumpTestCommand::ackFailed(std::string reason) {
    M1M3SSPublisher::instance().ackCommandforceActuatorBumpTest(getCommandID(), ACK_FAILED,
                                                                "Failed: " + reason);
}
