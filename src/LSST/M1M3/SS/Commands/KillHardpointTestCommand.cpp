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

#include <Context.h>
#include <KillHardpointTestCommand.h>
#include <M1M3SSPublisher.h>

namespace LSST {
namespace M1M3 {
namespace SS {

KillHardpointTestCommand::KillHardpointTestCommand(int32_t commandID, MTM1M3_command_killHardpointTestC* data)
        : Command(commandID) {
    _data.hardpointActuator = data->hardpointActuator;
}

bool KillHardpointTestCommand::validate() {
    if (!(_data.hardpointActuator >= 1 && _data.hardpointActuator <= 6)) {
        M1M3SSPublisher::get().logCommandRejectionWarning(
                "KillHardpointTest", "The field HardpointActuator must be in range [1, 6].");
    } else if (Model::get().getHardpointTestController()->isTested(_data.hardpointActuator - 1) == false) {
        M1M3SSPublisher::get().logCommandRejectionWarning("KillHardpointTest",
                                                          "Hardpoint is not being tested.");
    } else {
        return true;
    }
    return false;
}

void KillHardpointTestCommand::execute() { Context::get().killHardpointTest(this); }

void KillHardpointTestCommand::ackInProgress() {
    M1M3SSPublisher::get().ackCommandkillHardpointTest(getCommandID(), ACK_INPROGRESS, "In-Progress");
}

void KillHardpointTestCommand::ackComplete() {
    M1M3SSPublisher::get().ackCommandkillHardpointTest(getCommandID(), ACK_COMPLETE, "Completed");
}

void KillHardpointTestCommand::ackFailed(std::string reason) {
    M1M3SSPublisher::get().ackCommandkillHardpointTest(getCommandID(), ACK_FAILED, "Failed: " + reason);
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */