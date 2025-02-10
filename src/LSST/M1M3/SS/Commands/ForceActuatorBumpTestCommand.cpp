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
#include <M1M3SSPublisher.h>
#include <Model.h>
#include <SettingReader.h>

using namespace LSST::cRIO::SAL;
using namespace LSST::M1M3::SS;

ForceActuatorBumpTestCommand::ForceActuatorBumpTestCommand(int32_t commandID,
                                                           MTM1M3_command_forceActuatorBumpTestC *data)
        : Command(commandID) {
    memcpy(&_data, data, sizeof(MTM1M3_command_forceActuatorBumpTestC));
}

bool ForceActuatorBumpTestCommand::validate() {
    if (SettingReader::instance().getForceActuatorApplicationSettings()->ActuatorIdToZIndex(
                _data.actuatorId) == -1) {
        M1M3SSPublisher::instance().logCommandRejectionWarning("ForceActuatorBumpTest",
                                                               "Invalid actuatorId.");
        return false;
    }
    if (M1M3SSPublisher::instance().getEventForceActuatorBumpTestStatus()->actuatorId >= 0) {
        M1M3SSPublisher::instance().logCommandRejectionWarning("ForceActuatorBumpTest",
                                                               "Test already in progress.");
        return false;
    }
    if (Model::instance().getILC()->isDisabled(_data.actuatorId)) {
        M1M3SSPublisher::instance().logCommandRejectionWarning(
                "ForceActuatorBumpTest",
                "Cannnot bump test disabled force actuator " + std::to_string(_data.actuatorId));
        return false;
    }
    return true;
}

void ForceActuatorBumpTestCommand::execute() { Context::get().forceActuatorBumpTest(this); }

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
