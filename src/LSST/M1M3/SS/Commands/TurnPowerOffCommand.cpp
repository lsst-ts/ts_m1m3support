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
#include <TurnPowerOffCommand.h>
#include <M1M3SSPublisher.h>

namespace LSST {
namespace M1M3 {
namespace SS {

TurnPowerOffCommand::TurnPowerOffCommand(int32_t commandID, MTM1M3_command_turnPowerOffC* data)
        : Command(commandID) {
    _data.turnPowerNetworkAOff = data->turnPowerNetworkAOff;
    _data.turnPowerNetworkBOff = data->turnPowerNetworkBOff;
    _data.turnPowerNetworkCOff = data->turnPowerNetworkCOff;
    _data.turnPowerNetworkDOff = data->turnPowerNetworkDOff;
    _data.turnAuxPowerNetworkAOff = data->turnAuxPowerNetworkAOff;
    _data.turnAuxPowerNetworkBOff = data->turnAuxPowerNetworkBOff;
    _data.turnAuxPowerNetworkCOff = data->turnAuxPowerNetworkCOff;
    _data.turnAuxPowerNetworkDOff = data->turnAuxPowerNetworkDOff;
}

bool TurnPowerOffCommand::validate() {
    if (!(_data.turnPowerNetworkAOff || _data.turnPowerNetworkBOff || _data.turnPowerNetworkCOff ||
          _data.turnPowerNetworkDOff || _data.turnAuxPowerNetworkAOff || _data.turnAuxPowerNetworkBOff ||
          _data.turnAuxPowerNetworkCOff || _data.turnAuxPowerNetworkDOff)) {
        M1M3SSPublisher::get().logCommandRejectionWarning("TurnPowerOff", "At least one field is not TRUE.");
    }
    return _data.turnPowerNetworkAOff || _data.turnPowerNetworkBOff || _data.turnPowerNetworkCOff ||
           _data.turnPowerNetworkDOff || _data.turnAuxPowerNetworkAOff || _data.turnAuxPowerNetworkBOff ||
           _data.turnAuxPowerNetworkCOff || _data.turnAuxPowerNetworkDOff;
}

void TurnPowerOffCommand::execute() { Context::get().turnPowerOff(this); }

void TurnPowerOffCommand::ackInProgress() {
    M1M3SSPublisher::get().ackCommandturnPowerOff(getCommandID(), ACK_INPROGRESS, "In-Progress");
}

void TurnPowerOffCommand::ackComplete() {
    M1M3SSPublisher::get().ackCommandturnPowerOff(getCommandID(), ACK_COMPLETE, "Complete");
}

void TurnPowerOffCommand::ackFailed(std::string reason) {
    M1M3SSPublisher::get().ackCommandturnPowerOff(getCommandID(), ACK_FAILED, "Failed: " + reason);
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
