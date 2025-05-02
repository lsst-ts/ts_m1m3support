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

#include <cRIO/SAL/Command.h>

#include <ApplyActiveOpticForcesCommand.h>
#include <Context.h>
#include <M1M3SSPublisher.h>

#include <ForceActuatorSettings.h>

using namespace LSST::cRIO::SAL;
using namespace LSST::M1M3::SS;

ApplyActiveOpticForcesCommand::ApplyActiveOpticForcesCommand(int32_t commandID,
                                                             MTM1M3_command_applyActiveOpticForcesC *data)
        : Command(commandID) {
    for (int i = 0; i < FA_COUNT; i++) {
        _data.zForces[i] = data->zForces[i];
    }
}

bool ApplyActiveOpticForcesCommand::validate() {
    for (int zIndex = 0; zIndex < FA_COUNT; zIndex++) {
        float zLowFault = ForceActuatorSettings::instance().ActiveOpticLimitZTable[zIndex].LowFault;
        float zHighFault = ForceActuatorSettings::instance().ActiveOpticLimitZTable[zIndex].HighFault;

        if (_data.zForces[zIndex] <= zLowFault) {
            M1M3SSPublisher::instance().logCommandRejectionWarning(
                    "ApplyActiveOpticForces",
                    fmt::format("Applied Z force for FA {} is below limit: {:.2f} N <= {:.2f} N", zIndex,
                                _data.zForces[zIndex], zLowFault));
            return false;
        }

        if (_data.zForces[zIndex] >= zHighFault) {
            M1M3SSPublisher::instance().logCommandRejectionWarning(
                    "ApplyActiveOpticForces",
                    fmt::format("Applied Z force for FA {} is above limit: {:.2f} N >= {:.2f} N", zIndex,
                                _data.zForces[zIndex], zHighFault));
            return false;
        }
    }

    return true;
}

void ApplyActiveOpticForcesCommand::execute() { Context::instance().applyActiveOpticForces(this); }

void ApplyActiveOpticForcesCommand::ackInProgress(const char *description, double timeout) {
    M1M3SSPublisher::instance().ackCommandapplyActiveOpticForces(getCommandID(), ACK_INPROGRESS, description,
                                                                 timeout);
}

void ApplyActiveOpticForcesCommand::ackComplete() {
    M1M3SSPublisher::instance().ackCommandapplyActiveOpticForces(getCommandID(), ACK_COMPLETE, "Complete");
}

void ApplyActiveOpticForcesCommand::ackFailed(std::string reason) {
    M1M3SSPublisher::instance().ackCommandapplyActiveOpticForces(getCommandID(), ACK_FAILED,
                                                                 "Failed: " + reason);
}
