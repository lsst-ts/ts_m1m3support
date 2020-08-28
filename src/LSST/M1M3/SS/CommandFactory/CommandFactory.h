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

#ifndef COMMANDFACTORY_H_
#define COMMANDFACTORY_H_

#include <DataTypes.h>
#include <CommandTypes.h>
#include <Command.h>

namespace LSST {
namespace M1M3 {
namespace SS {

/**
 * @brief Creates dynamically allocated Command objects.
 */
class CommandFactory {
public:
    /**
     * Creates a command.
     *
     * @param[in] commandType The type of command to create.
     * @param[in] data The data for the command.
     * @param[in] commandID The command ID (optional).
     *
     * @return Pointer to the created Command object. The returned command is
     * dynamically allocated and must be deleted.
     */
    static Command* create(Commands::Type commandType, void* data = 0, int32_t commandID = 0);
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* COMMANDFACTORY_H_ */
