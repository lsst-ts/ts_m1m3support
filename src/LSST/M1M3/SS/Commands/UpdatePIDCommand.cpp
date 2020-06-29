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

#include <UpdatePIDCommand.h>
#include <Context.h>
#include <M1M3SSPublisher.h>

namespace LSST {
namespace M1M3 {
namespace SS {

UpdatePIDCommand::UpdatePIDCommand(Context* context, M1M3SSPublisher* publisher, int32_t commandID,
                                   MTM1M3_command_updatePIDC* data) {
    _context = context;
    _publisher = publisher;
    this->commandID = commandID;
    _data.pid = data->pid;
    _data.timestep = data->timestep;
    _data.p = data->p;
    _data.i = data->i;
    _data.d = data->d;
    _data.n = data->n;
}

bool UpdatePIDCommand::validate() {
    if (_data.pid < 1 || _data.pid > 6) {
        _publisher->logCommandRejectionWarning("UpdatePID", "The field PID must be in range [1, 6].");
    }
    return _data.pid >= 1 && _data.pid <= 6;
}

void UpdatePIDCommand::execute() { _context->updatePID(this); }

void UpdatePIDCommand::ackInProgress() {
    _publisher->ackCommandupdatePID(this->commandID, ACK_INPROGRESS, "In-Progress");
}

void UpdatePIDCommand::ackComplete() {
    _publisher->ackCommandupdatePID(this->commandID, ACK_COMPLETE, "Complete");
}

void UpdatePIDCommand::ackFailed(std::string reason) {
    _publisher->ackCommandupdatePID(this->commandID, ACK_FAILED, "Failed: " + reason);
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
