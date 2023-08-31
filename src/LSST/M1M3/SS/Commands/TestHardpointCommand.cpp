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
#include <M1M3SSPublisher.h>
#include <Model.h>
#include <TestHardpointCommand.h>

namespace LSST {
namespace M1M3 {
namespace SS {

TestHardpointCommand::TestHardpointCommand(int32_t commandID, MTM1M3_command_testHardpointC* data)
        : Command(commandID) {
    _data.hardpointActuator = data->hardpointActuator;
}

bool TestHardpointCommand::validate() {
    if (!(_data.hardpointActuator >= 1 && _data.hardpointActuator <= 6)) {
        M1M3SSPublisher::instance().logCommandRejectionWarning(
                "TestHardpoint", "The field HardpointActuator must be in range [1, 6].");
    } else if (Model::instance().getHardpointTestController()->isTested(_data.hardpointActuator - 1)) {
        M1M3SSPublisher::instance().logCommandRejectionWarning("TestHardpoint",
                                                               "Hardpoint is already being tested.");
    } else {
        return true;
    }
    return false;
}

void TestHardpointCommand::execute() { Context::get().testHardpoint(this); }

void TestHardpointCommand::ackInProgress(const char* description, double timeout) {
    M1M3SSPublisher::instance().ackCommandtestHardpoint(getCommandID(), ACK_INPROGRESS, description, timeout);
}

void TestHardpointCommand::ackComplete() {
    M1M3SSPublisher::instance().ackCommandtestHardpoint(getCommandID(), ACK_COMPLETE, "Completed");
}

void TestHardpointCommand::ackFailed(std::string reason) {
    M1M3SSPublisher::instance().ackCommandtestHardpoint(getCommandID(), ACK_FAILED, "Failed: " + reason);
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
