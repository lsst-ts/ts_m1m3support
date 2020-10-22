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
#include <ForceActuatorBumpTestCommand.h>
#include <M1M3SSPublisher.h>

namespace LSST {
namespace M1M3 {
namespace SS {

ForceActuatorBumpTestCommand::ForceActuatorBumpTestCommand(int32_t commandID,
                                                           MTM1M3_command_forceActuatorBumpTestC* data) {
    this->commandID = commandID;
    memcpy(&_data, data, sizeof(MTM1M3_command_forceActuatorBumpTestC));
}

bool ForceActuatorBumpTestCommand::validate() {
    if (_data.actuatorId < 100 || _data.actuatorId > 443) {
        M1M3SSPublisher::get().logCommandRejectionWarning("ForceActuatorBumpTest", "Invalid actuatorId.");
        return false;
    }
    return true;
}

void ForceActuatorBumpTestCommand::execute() { Context::get().forceActuatorBumpTest(this); }

void ForceActuatorBumpTestCommand::ackInProgress() {
    M1M3SSPublisher::get().ackCommandforceActuatorBumpTest(this->commandID, ACK_INPROGRESS, "In-Progress");
}

void ForceActuatorBumpTestCommand::ackComplete() {
    M1M3SSPublisher::get().ackCommandforceActuatorBumpTest(this->commandID, ACK_COMPLETE, "Completed");
}

void ForceActuatorBumpTestCommand::ackFailed(std::string reason) {
    M1M3SSPublisher::get().ackCommandforceActuatorBumpTest(this->commandID, ACK_FAILED, "Failed: " + reason);
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
