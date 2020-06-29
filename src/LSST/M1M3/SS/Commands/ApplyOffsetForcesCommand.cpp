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

#include <ApplyOffsetForcesCommand.h>
#include <Context.h>
#include <M1M3SSPublisher.h>

namespace LSST {
namespace M1M3 {
namespace SS {

ApplyOffsetForcesCommand::ApplyOffsetForcesCommand(Context* context, M1M3SSPublisher* publisher,
                                                   int32_t commandID,
                                                   MTM1M3_command_applyOffsetForcesC* data) {
    _context = context;
    _publisher = publisher;
    this->commandID = commandID;
    for (int i = 0; i < FA_COUNT; i++) {
        if (i < FA_X_COUNT) {
            _data.xForces[i] = data->xForces[i];
        }
        if (i < FA_Y_COUNT) {
            _data.yForces[i] = data->yForces[i];
        }
        _data.zForces[i] = data->zForces[i];
    }
}

bool ApplyOffsetForcesCommand::validate() { return true; }

void ApplyOffsetForcesCommand::execute() { _context->applyOffsetForces(this); }

void ApplyOffsetForcesCommand::ackInProgress() {
    _publisher->ackCommandapplyOffsetForces(this->commandID, ACK_INPROGRESS, "In-Progress");
}

void ApplyOffsetForcesCommand::ackComplete() {
    _publisher->ackCommandapplyOffsetForces(this->commandID, ACK_COMPLETE, "Complete");
}

void ApplyOffsetForcesCommand::ackFailed(std::string reason) {
    _publisher->ackCommandapplyOffsetForces(this->commandID, ACK_FAILED, "Failed: " + reason);
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
