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

#include <TurnPowerOnCommand.h>
#include <Context.h>
#include <M1M3SSPublisher.h>

namespace LSST {
namespace M1M3 {
namespace SS {

TurnPowerOnCommand::TurnPowerOnCommand(Context* context, M1M3SSPublisher* publisher, int32_t commandID,
                                       MTM1M3_command_turnPowerOnC* data) {
    _context = context;
    _publisher = publisher;
    this->commandID = commandID;
    _data.turnPowerNetworkAOn = data->turnPowerNetworkAOn;
    _data.turnPowerNetworkBOn = data->turnPowerNetworkBOn;
    _data.turnPowerNetworkCOn = data->turnPowerNetworkCOn;
    _data.turnPowerNetworkDOn = data->turnPowerNetworkDOn;
    _data.turnAuxPowerNetworkAOn = data->turnAuxPowerNetworkAOn;
    _data.turnAuxPowerNetworkBOn = data->turnAuxPowerNetworkBOn;
    _data.turnAuxPowerNetworkCOn = data->turnAuxPowerNetworkCOn;
    _data.turnAuxPowerNetworkDOn = data->turnAuxPowerNetworkDOn;
}

bool TurnPowerOnCommand::validate() {
    if (!(_data.turnPowerNetworkAOn || _data.turnPowerNetworkBOn ||
          _data.turnPowerNetworkCOn || _data.turnPowerNetworkDOn ||
          _data.turnAuxPowerNetworkAOn || _data.turnAuxPowerNetworkBOn ||
          _data.turnAuxPowerNetworkCOn || _data.turnAuxPowerNetworkDOn)) {
        _publisher->logCommandRejectionWarning("TurnPowerOn", "At least one field is not TRUE.");
    }
    return _data.turnPowerNetworkAOn || _data.turnPowerNetworkBOn ||
           _data.turnPowerNetworkCOn || _data.turnPowerNetworkDOn ||
           _data.turnAuxPowerNetworkAOn || _data.turnAuxPowerNetworkBOn ||
           _data.turnAuxPowerNetworkCOn || _data.turnAuxPowerNetworkDOn;
}

void TurnPowerOnCommand::execute() { _context->turnPowerOn(this); }

void TurnPowerOnCommand::ackInProgress() {
    _publisher->ackCommandturnPowerOn(this->commandID, ACK_INPROGRESS, "In-Progress");
}

void TurnPowerOnCommand::ackComplete() {
    _publisher->ackCommandturnPowerOn(this->commandID, ACK_COMPLETE, "Complete");
}

void TurnPowerOnCommand::ackFailed(std::string reason) {
    _publisher->ackCommandturnPowerOn(this->commandID, ACK_FAILED, "Failed: " + reason);
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
