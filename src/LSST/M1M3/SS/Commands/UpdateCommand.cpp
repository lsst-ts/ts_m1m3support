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

#include <UpdateCommand.h>
#include <Context.h>

namespace LSST {
namespace M1M3 {
namespace SS {

UpdateCommand::UpdateCommand(Context* context, pthread_mutex_t* updateMutex) {
    _context = context;
    _updateMutex = updateMutex;
    this->commandID = -1;

    // Lock the update mutex to prevent the outer loop clock
    // thread from executing.
    pthread_mutex_lock(_updateMutex);
}

UpdateCommand::~UpdateCommand() {
    // Release the update mutex to allow the outer loop clock
    // thread to continue executing.
    pthread_mutex_unlock(_updateMutex);
}

void UpdateCommand::execute() { _context->update(this); }

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
