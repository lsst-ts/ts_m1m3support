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
#include <M1M3SSPublisher.h>
#include <ResetPIDCommand.h>

using namespace LSST::cRIO::SAL;
using namespace LSST::M1M3::SS;

ResetPIDCommand::ResetPIDCommand(int32_t commandID, MTM1M3_command_resetPIDC* data) : Command(commandID) {
    _data.pid = data->pid;
}

bool ResetPIDCommand::validate() {
    if (_data.pid < 1 || _data.pid > 6) {
        M1M3SSPublisher::instance().logCommandRejectionWarning("ResetPID",
                                                               "The field PID must be in range [1, 6].");
    }
    return _data.pid >= 1 && _data.pid <= 6;
}

void ResetPIDCommand::execute() { Context::instance().resetPID(this); }

void ResetPIDCommand::ackInProgress(const char* description, double timeout) {
    M1M3SSPublisher::instance().ackCommandresetPID(getCommandID(), ACK_INPROGRESS, description, timeout);
}

void ResetPIDCommand::ackComplete() {
    M1M3SSPublisher::instance().ackCommandresetPID(getCommandID(), ACK_COMPLETE, "Complete");
}

void ResetPIDCommand::ackFailed(std::string reason) {
    M1M3SSPublisher::instance().ackCommandresetPID(getCommandID(), ACK_FAILED, "Failed: " + reason);
}
