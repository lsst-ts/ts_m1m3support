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

#ifndef EXITCONTROLCOMMAND_H_
#define EXITCONTROLCOMMAND_H_

#include <Command.h>
#include <SAL_MTM1M3C.h>
#include <DataTypes.h>

namespace LSST {
namespace M1M3 {
namespace SS {

/*!
 * This command is responsible for transitioning the state
 * machine from the standby state to the offline state.
 * This is an external command and can be issued via SAL.
 * Once this command has been executed the software will stop running.
 */
class ExitControlCommand : public Command {
public:
    ExitControlCommand(Context* context, M1M3SSPublisher* publisher, int32_t commandID,
                       MTM1M3_command_exitControlC*);

    void execute() override;
    void ackInProgress() override;
    void ackComplete() override;
    void ackFailed(std::string reason) override;

private:
    Context* _context;
    M1M3SSPublisher* _publisher;
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* EXITCONTROLCOMMAND_H_ */