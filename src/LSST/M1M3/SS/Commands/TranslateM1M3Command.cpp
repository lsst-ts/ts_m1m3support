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

#include <TranslateM1M3Command.h>

namespace LSST {
namespace M1M3 {
namespace SS {

TranslateM1M3Command::TranslateM1M3Command(Context* context, M1M3SSPublisher* publisher, int32_t commandID,
                                           MTM1M3_command_translateM1M3C* data) {
    _context = context;
    _publisher = publisher;
    this->commandID = commandID;
    _data.xTranslation = data->xTranslation;
    _data.yTranslation = data->yTranslation;
    _data.zTranslation = data->zTranslation;
    _data.xRotation = data->xRotation;
    _data.yRotation = data->yRotation;
    _data.zRotation = data->zRotation;
}

void TranslateM1M3Command::execute() { _context->translateM1M3(this); }

void TranslateM1M3Command::ackInProgress() {
    _publisher->ackCommandtranslateM1M3(this->commandID, ACK_INPROGRESS, "In-Progress");
}

void TranslateM1M3Command::ackComplete() {
    _publisher->ackCommandtranslateM1M3(this->commandID, ACK_COMPLETE, "Completed");
}

void TranslateM1M3Command::ackFailed(std::string reason) {
    _publisher->ackCommandtranslateM1M3(this->commandID, ACK_FAILED, "Failed: " + reason);
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
