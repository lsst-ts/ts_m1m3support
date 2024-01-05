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
#include <DisableHardpointChaseCommand.h>
#include <M1M3SSPublisher.h>

using namespace LSST::cRIO::SAL;
using namespace LSST::M1M3::SS;

DisableHardpointChaseCommand::DisableHardpointChaseCommand(int32_t commandID,
                                                           MTM1M3_command_disableHardpointChaseC* data)
        : Command(commandID) {}

void DisableHardpointChaseCommand::execute() { Context::get().disableHardpointChase(this); }

void DisableHardpointChaseCommand::ackInProgress(const char* description, double timeout) {
    M1M3SSPublisher::instance().ackCommanddisableHardpointChase(getCommandID(), ACK_INPROGRESS, description,
                                                                timeout);
}

void DisableHardpointChaseCommand::ackComplete() {
    M1M3SSPublisher::instance().ackCommanddisableHardpointChase(getCommandID(), ACK_COMPLETE, "Completed");
}

void DisableHardpointChaseCommand::ackFailed(std::string reason) {
    M1M3SSPublisher::instance().ackCommanddisableHardpointChase(getCommandID(), ACK_FAILED,
                                                                "Failed: " + reason);
}
