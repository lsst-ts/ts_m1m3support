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

#include <RunMirrorForceProfileCommand.h>
#include <M1M3SSPublisher.h>

namespace LSST {
namespace M1M3 {
namespace SS {

RunMirrorForceProfileCommand::RunMirrorForceProfileCommand(Context* context, int32_t commandID,
                                                           MTM1M3_command_runMirrorForceProfileC* data) {
    _context = context;
    this->commandID = commandID;
    for (int i = 0; i < 1000; ++i) {
        _data.xForce[i] = data->xForce[i];
        _data.yForce[i] = data->yForce[i];
        _data.zForce[i] = data->zForce[i];
        _data.xMoment[i] = data->xMoment[i];
        _data.yMoment[i] = data->yMoment[i];
        _data.zMoment[i] = data->zMoment[i];
    }
}

bool RunMirrorForceProfileCommand::validate() { return true; }

void RunMirrorForceProfileCommand::execute() { _context->runMirrorForceProfile(this); }

void RunMirrorForceProfileCommand::ackInProgress() {
    M1M3SSPublisher::get().ackCommandrunMirrorForceProfile(this->commandID, ACK_INPROGRESS, "In-Progress");
}

void RunMirrorForceProfileCommand::ackComplete() {
    M1M3SSPublisher::get().ackCommandrunMirrorForceProfile(this->commandID, ACK_COMPLETE, "Complete");
}

void RunMirrorForceProfileCommand::ackFailed(std::string reason) {
    M1M3SSPublisher::get().ackCommandrunMirrorForceProfile(this->commandID, ACK_FAILED, "Failed: " + reason);
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
