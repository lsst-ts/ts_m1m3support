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
#include <RaiseM1M3Command.h>
#include <M1M3SSPublisher.h>

#include <spdlog/spdlog.h>

namespace LSST {
namespace M1M3 {
namespace SS {

RaiseM1M3Command::RaiseM1M3Command(int32_t commandID, MTM1M3_command_raiseM1M3C* data) : Command(commandID) {
    _data.bypassReferencePosition = data->bypassReferencePosition;
}

bool RaiseM1M3Command::validate() {
    if (M1M3SSPublisher::get().getEventForceActuatorBumpTestStatus()->actuatorId >= 0) {
        M1M3SSPublisher::get().logCommandRejectionWarning(
                "RaiseM1M3",
                fmt::format("Cannot raise M1M3 as bump test for actuator {} is in progress.",
                            M1M3SSPublisher::get().getEventForceActuatorBumpTestStatus()->actuatorId));
        return false;
    }
    return Command::validate();
}

void RaiseM1M3Command::execute() { Context::get().raiseM1M3(this); }

void RaiseM1M3Command::ackInProgress() {
    M1M3SSPublisher::get().ackCommandraiseM1M3(getCommandID(), ACK_INPROGRESS, "In-Progress");
}

void RaiseM1M3Command::ackComplete() {
    M1M3SSPublisher::get().ackCommandraiseM1M3(getCommandID(), ACK_COMPLETE, "Complete");
}

void RaiseM1M3Command::ackFailed(std::string reason) {
    M1M3SSPublisher::get().ackCommandraiseM1M3(getCommandID(), ACK_FAILED, "Failed: " + reason);
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
