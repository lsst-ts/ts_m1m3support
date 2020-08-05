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

#include <AbortRaiseM1M3Command.h>
#include <M1M3SSPublisher.h>

namespace LSST {
namespace M1M3 {
namespace SS {

AbortRaiseM1M3Command::AbortRaiseM1M3Command(Context* context, int32_t commandID,
                                             MTM1M3_command_abortRaiseM1M3C*) {
    _context = context;
    this->commandID = commandID;
}

void AbortRaiseM1M3Command::execute() { _context->abortRaiseM1M3(this); }

void AbortRaiseM1M3Command::ackInProgress() {
    M1M3SSPublisher::get().ackCommandabortRaiseM1M3(this->commandID, ACK_INPROGRESS, "In-Progress");
}

void AbortRaiseM1M3Command::ackComplete() {
    M1M3SSPublisher::get().ackCommandabortRaiseM1M3(this->commandID, ACK_COMPLETE, "Completed");
}

void AbortRaiseM1M3Command::ackFailed(std::string reason) {
    M1M3SSPublisher::get().ackCommandabortRaiseM1M3(this->commandID, ACK_FAILED, "Failed: " + reason);
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
