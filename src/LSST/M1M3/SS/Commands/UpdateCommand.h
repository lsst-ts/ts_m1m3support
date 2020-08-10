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

#ifndef UPDATECOMMAND_H_
#define UPDATECOMMAND_H_

#include <Command.h>
#include <pthread.h>

namespace LSST {
namespace M1M3 {
namespace SS {

/*!
 * This command is responsible for running a periodic update.
 * This is an internal command only and cannot be issued via SAL.
 * The command is generated by the FPGA's outer loop clock.
 */
class UpdateCommand : public Command {
public:
    UpdateCommand(pthread_mutex_t* updateMutext);
    ~UpdateCommand();

    void execute() override;

private:
    pthread_mutex_t* _updateMutex;
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* UPDATECOMMAND_H_ */
