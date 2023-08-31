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
#include <DisableForceActuatorCommand.h>
#include <M1M3SSPublisher.h>
#include <Model.h>
#include <SettingReader.h>

using namespace LSST::M1M3::SS;

DisableForceActuatorCommand::DisableForceActuatorCommand(int32_t commandID,
                                                         MTM1M3_command_disableForceActuatorC* data)
        : Command(commandID) {
    actuatorId = data->actuatorId;
    actuatorIndex = -1;
}

bool DisableForceActuatorCommand::validate() {
    actuatorIndex =
            SettingReader::instance().getForceActuatorApplicationSettings()->ActuatorIdToZIndex(actuatorId);
    if (actuatorIndex < 0) {
        M1M3SSPublisher::instance().commandFailed("DisableForceActuator", "Invalid actuatorId for FA ID {}",
                                                  actuatorId);
        return false;
    }

    // neighbor check
    uint32_t farID = Model::instance().getILC()->hasDisabledFarNeighbor(actuatorIndex);
    if (farID > 0) {
        M1M3SSPublisher::instance().commandFailed(
                "DisableForceActuatorCommand", "Actuator ID {} is far neighbor of already disabled FA ID {}",
                actuatorId, farID);
        return false;
    }

    return true;
}

void DisableForceActuatorCommand::execute() { Context::get().disableForceActuator(this); }

void DisableForceActuatorCommand::ackInProgress() {
    M1M3SSPublisher::instance().ackCommanddisableForceActuator(getCommandID(), ACK_INPROGRESS, "In-Progress");
}

void DisableForceActuatorCommand::ackComplete() {
    M1M3SSPublisher::instance().ackCommanddisableForceActuator(getCommandID(), ACK_COMPLETE, "Completed");
}

void DisableForceActuatorCommand::ackFailed(std::string reason) {
    M1M3SSPublisher::instance().ackCommanddisableForceActuator(getCommandID(), ACK_FAILED,
                                                               "Failed: " + reason);
}
