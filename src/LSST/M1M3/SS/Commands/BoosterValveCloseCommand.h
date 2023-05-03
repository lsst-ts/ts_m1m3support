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

#ifndef BOOSTERVALVECLOSECOMMAND_H_
#define BOOSTERVALVECLOSECOMMAND_H_

#include <SAL_MTM1M3C.h>

#include <Command.h>
#include <DataTypes.h>

namespace LSST {
namespace M1M3 {
namespace SS {

/**
 * Sets slew flag for force actuators. Should be used for testing booster
 * valves - slew flag operates booster valves.
 */
class BoosterValveCloseCommand : public Command {
public:
    BoosterValveCloseCommand(int32_t commandID, MTM1M3_command_boosterValveCloseC*);

    void execute() override;
    void ackInProgress() override;
    void ackComplete() override;
    void ackFailed(std::string reason) override;
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif  // !BOOSTERVALVECLOSECOMMAND_H_