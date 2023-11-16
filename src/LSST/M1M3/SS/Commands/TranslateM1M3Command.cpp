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

#include <Context.h>
#include <TranslateM1M3Command.h>
#include <M1M3SSPublisher.h>

using namespace LSST::cRIO::SAL;
using namespace LSST::M1M3::SS;

TranslateM1M3Command::TranslateM1M3Command(int32_t commandID, MTM1M3_command_translateM1M3C* data)
        : Command(commandID) {
    _data.xTranslation = data->xTranslation;
    _data.yTranslation = data->yTranslation;
    _data.zTranslation = data->zTranslation;
    _data.xRotation = data->xRotation;
    _data.yRotation = data->yRotation;
    _data.zRotation = data->zRotation;
}

void TranslateM1M3Command::execute() { Context::get().translateM1M3(this); }

void TranslateM1M3Command::ackInProgress(const char* description, double timeout) {
    M1M3SSPublisher::instance().ackCommandtranslateM1M3(getCommandID(), ACK_INPROGRESS, description, timeout);
}

void TranslateM1M3Command::ackComplete() {
    M1M3SSPublisher::instance().ackCommandtranslateM1M3(getCommandID(), ACK_COMPLETE, "Completed");
}

void TranslateM1M3Command::ackFailed(std::string reason) {
    M1M3SSPublisher::instance().ackCommandtranslateM1M3(getCommandID(), ACK_FAILED, "Failed: " + reason);
}
