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
#include <ClearSlewFlagCommand.h>
#include <M1M3SSPublisher.h>

namespace LSST {
namespace M1M3 {
namespace SS {

ClearSlewFlagCommand::ClearSlewFlagCommand(int32_t commandID) : Command(commandID) {}

void ClearSlewFlagCommand::execute() { Context::get().clearSlewFlag(this); }

void ClearSlewFlagCommand::ackInProgress() {
    M1M3SSPublisher::instance().ackCommandclearSlewFlag(getCommandID(), ACK_INPROGRESS, "In-Progress");
}

void ClearSlewFlagCommand::ackComplete() {
    M1M3SSPublisher::instance().ackCommandclearSlewFlag(getCommandID(), ACK_COMPLETE, "Complete");
}

void ClearSlewFlagCommand::ackFailed(std::string reason) {
    M1M3SSPublisher::instance().ackCommandclearSlewFlag(getCommandID(), ACK_FAILED, "Failed: " + reason);
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */