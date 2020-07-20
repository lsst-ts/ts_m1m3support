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

#include <TestForceActuatorCommand.h>

namespace LSST {
namespace M1M3 {
namespace SS {

TestForceActuatorCommand::TestForceActuatorCommand(Context* context, M1M3SSPublisher* publisher,
                                                   int32_t commandID,
                                                   MTM1M3_command_testForceActuatorC* data) {
    _context = context;
    _publisher = publisher;
    this->commandID = commandID;
    _data.forceActuator = data->forceActuator;
}

bool TestForceActuatorCommand::validate() {
    if (!((_data.forceActuator >= 101 && _data.forceActuator <= 143) ||
          (_data.forceActuator >= 207 && _data.forceActuator <= 243) ||
          (_data.forceActuator >= 301 && _data.forceActuator <= 343) ||
          (_data.forceActuator >= 407 && _data.forceActuator <= 443))) {
        _publisher->logCommandRejectionWarning("TestForceActuator",
                                               "The field ForceActuator must be in range [101, 143] or "
                                               "[207, 243] or [301, 343] or [407, 443].");
    }
    return (_data.forceActuator >= 101 && _data.forceActuator <= 143) ||
           (_data.forceActuator >= 207 && _data.forceActuator <= 243) ||
           (_data.forceActuator >= 301 && _data.forceActuator <= 343) ||
           (_data.forceActuator >= 407 && _data.forceActuator <= 443);
}

void TestForceActuatorCommand::execute() { _context->testForceActuator(this); }

void TestForceActuatorCommand::ackInProgress() {
    _publisher->ackCommandtestForceActuator(this->commandID, ACK_INPROGRESS, "In-Progress");
}

void TestForceActuatorCommand::ackComplete() {
    _publisher->ackCommandtestForceActuator(this->commandID, ACK_COMPLETE, "Completed");
}

void TestForceActuatorCommand::ackFailed(std::string reason) {
    _publisher->ackCommandtestForceActuator(this->commandID, ACK_FAILED, "Failed: " + reason);
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
