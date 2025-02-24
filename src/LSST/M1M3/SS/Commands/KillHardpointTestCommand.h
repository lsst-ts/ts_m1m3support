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

#ifndef KILLHARDPOINTTESTCOMMAND_H_
#define KILLHARDPOINTTESTCOMMAND_H_

#include <Command.h>
#include <SAL_MTM1M3C.h>
#include <cRIO/DataTypes.h>

namespace LSST {
namespace M1M3 {
namespace SS {

/**
 * Abort any hardpoint test in progress,
 */
class KillHardpointTestCommand : public Command {
public:
    KillHardpointTestCommand(int32_t commandID, MTM1M3_command_killHardpointTestC *data);

    MTM1M3_command_killHardpointTestC *getData() { return &_data; }

    bool validate() override;
    void execute() override;
    void ackInProgress(const char *description, double timeout) override;
    void ackComplete() override;
    void ackFailed(std::string reason) override;

private:
    MTM1M3_command_killHardpointTestC _data;
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* KILLHARDPOINTTESTCOMMAND_H_ */
