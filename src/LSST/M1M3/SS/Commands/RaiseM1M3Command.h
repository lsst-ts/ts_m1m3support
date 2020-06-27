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

#ifndef RAISEM1M3COMMAND_H_
#define RAISEM1M3COMMAND_H_

#include <Command.h>
#include <SAL_MTM1M3C.h>
#include <DataTypes.h>

namespace LSST {
namespace M1M3 {
namespace SS {

class RaiseM1M3Command : public Command {
public:
    RaiseM1M3Command(Context* context, M1M3SSPublisher* publisher, int32_t commandID,
                     MTM1M3_command_raiseM1M3C* data);

    MTM1M3_command_raiseM1M3C* getData() { return &_data; }

    bool validate() override;
    void execute() override;
    void ackInProgress() override;
    void ackComplete() override;
    void ackFailed(std::string reason) override;

private:
    Context* _context;
    M1M3SSPublisher* _publisher;
    MTM1M3_command_raiseM1M3C _data;
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* RAISEM1M3COMMAND_H_ */
