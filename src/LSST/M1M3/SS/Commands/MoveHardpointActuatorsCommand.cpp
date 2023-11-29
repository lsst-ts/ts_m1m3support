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

#include <cRIO/SAL/Command.h>

#include <Context.h>
#include <MoveHardpointActuatorsCommand.h>
#include <M1M3SSPublisher.h>

using namespace LSST::cRIO::SAL;
using namespace LSST::M1M3::SS;

MoveHardpointActuatorsCommand::MoveHardpointActuatorsCommand(int32_t commandID,
                                                             MTM1M3_command_moveHardpointActuatorsC* data)
        : Command(commandID) {
    for (int i = 0; i < 6; i++) {
        _data.steps[i] = data->steps[i];
    }
}

bool MoveHardpointActuatorsCommand::validate() {
    if (_data.steps[0] == 0 && _data.steps[1] == 0 && _data.steps[2] == 0 && _data.steps[3] == 0 &&
        _data.steps[4] == 0 && _data.steps[5] == 0) {
        M1M3SSPublisher::instance().logCommandRejectionWarning(
                "MoveHardpointActuators",
                "The field Steps must have at least one index with a step value not equal to zero.");
    }
    return _data.steps[0] != 0 || _data.steps[1] != 0 || _data.steps[2] != 0 || _data.steps[3] != 0 ||
           _data.steps[4] != 0 || _data.steps[5] != 0;
}

void MoveHardpointActuatorsCommand::execute() { Context::get().moveHardpointActuators(this); }

void MoveHardpointActuatorsCommand::ackInProgress(const char* description, double timeout) {
    M1M3SSPublisher::instance().ackCommandmoveHardpointActuators(getCommandID(), ACK_INPROGRESS, description,
                                                                 timeout);
}

void MoveHardpointActuatorsCommand::ackComplete() {
    M1M3SSPublisher::instance().ackCommandmoveHardpointActuators(getCommandID(), ACK_COMPLETE, "Completed");
}

void MoveHardpointActuatorsCommand::ackFailed(std::string reason) {
    M1M3SSPublisher::instance().ackCommandmoveHardpointActuators(getCommandID(), ACK_FAILED,
                                                                 "Failed: " + reason);
}
