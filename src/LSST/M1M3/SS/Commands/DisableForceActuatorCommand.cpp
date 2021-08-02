/*
 * This file is part of LSST M1M3 support system package.
 *
 * Developed for the LSST Data Management System.
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
#include <DisableForceActuatorCommand.h>
#include <M1M3SSPublisher.h>

using namespace LSST::M1M3::SS;

DisableForceActuatorCommand::DisableForceActuatorCommand(int32_t in_commandID,
                                                         MTM1M3_command_disableForceActuatorC* data) {
    commandID = in_commandID;
    actuatorID = data->actuatorId;
}

void DisableForceActuatorCommand::execute() { Context::get().enableForceActuator(this); }

void DisableForceActuatorCommand::ackInProgress() {
    M1M3SSPublisher::get().ackCommanddisableForceActuator(commandID, ACK_INPROGRESS, "In-Progress");
}

void DisableForceActuatorCommand::ackComplete() {
    M1M3SSPublisher::get().ackCommanddisableForceActuator(commandID, ACK_COMPLETE, "Completed");
}

void DisableForceActuatorCommand::ackFailed() {
    M1M3SSPublisher::get().ackCommanddisableForceActuator(commandID, ACK_FAILED, "Failed: " + reason);
}
