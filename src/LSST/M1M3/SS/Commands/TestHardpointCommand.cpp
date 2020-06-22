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

#include <TestHardpointCommand.h>
#include <Context.h>
#include <M1M3SSPublisher.h>

namespace LSST {
namespace M1M3 {
namespace SS {

TestHardpointCommand::TestHardpointCommand(Context* context, M1M3SSPublisher* publisher, int32_t commandID,
                                           MTM1M3_command_testHardpointC* data) {
    this->context = context;
    this->publisher = publisher;
    this->commandID = commandID;
    this->data.hardpointActuator = data->hardpointActuator;
}

bool TestHardpointCommand::validate() {
    if (!(this->data.hardpointActuator >= 1 && this->data.hardpointActuator <= 6)) {
        this->publisher->logCommandRejectionWarning("TestHardpoint",
                                                    "The field HardpointActuator must be in range [1, 6].");
    }
    return this->data.hardpointActuator >= 1 && this->data.hardpointActuator <= 6;
}

void TestHardpointCommand::execute() { this->context->testHardpoint(this); }

void TestHardpointCommand::ackInProgress() {
    this->publisher->ackCommandtestHardpoint(this->commandID, ACK_INPROGRESS, "In-Progress");
}

void TestHardpointCommand::ackComplete() {
    this->publisher->ackCommandtestHardpoint(this->commandID, ACK_COMPLETE, "Completed");
}

void TestHardpointCommand::ackFailed(std::string reason) {
    this->publisher->ackCommandtestHardpoint(this->commandID, ACK_FAILED, "Failed: " + reason);
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
