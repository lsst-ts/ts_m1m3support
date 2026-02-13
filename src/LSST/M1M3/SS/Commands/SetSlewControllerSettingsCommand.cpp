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

#include <spdlog/spdlog.h>

#include <SAL_MTM1M3C.h>

#include <cRIO/SAL/Command.h>

#include <Context.h>
#include <ForceControllerState.h>
#include <M1M3SSPublisher.h>
#include <SetSlewControllerSettingsCommand.h>

using namespace LSST::cRIO::SAL;
using namespace LSST::M1M3::SS;

SetSlewControllerSettingsCommand::SetSlewControllerSettingsCommand(
        int32_t commandID, MTM1M3_command_setSlewControllerSettingsC* data)
        : Command(commandID) {
    _data = *data;
}

bool SetSlewControllerSettingsCommand::validate() {
    if (_data.slewSettings < MTM1M3::setSlewControllerSettings_AccelerationForces ||
        _data.slewSettings > MTM1M3::setSlewControllerSettings_VelocityForces) {
        M1M3SSPublisher::instance().logCommandRejectionWarning(
                "SetSlewControllerSettings", fmt::format("The slewSettings field must be in range [{}, {}].",
                                                         MTM1M3::setSlewControllerSettings_AccelerationForces,
                                                         MTM1M3::setSlewControllerSettings_VelocityForces));
        return false;
    }
    if (ForceControllerState::instance().slewFlag == true) {
        M1M3SSPublisher::instance().logCommandRejectionWarning(
                "SetSlewControllerSettings",
                "Cannot modify slew controller settings while slew flag is active.");
        return false;
    }
    return true;
}

void SetSlewControllerSettingsCommand::execute() { Context::instance().setSlewControllerSettings(this); }

void SetSlewControllerSettingsCommand::ackInProgress(const char* description, double timeout) {
    M1M3SSPublisher::instance().ackCommandsetSlewControllerSettings(getCommandID(), ACK_INPROGRESS,
                                                                    description, timeout);
}

void SetSlewControllerSettingsCommand::ackComplete() {
    M1M3SSPublisher::instance().ackCommandsetSlewControllerSettings(getCommandID(), ACK_COMPLETE, "Complete");
}

void SetSlewControllerSettingsCommand::ackFailed(std::string reason) {
    M1M3SSPublisher::instance().ackCommandsetSlewControllerSettings(getCommandID(), ACK_FAILED,
                                                                    "Failed: " + reason);
}
