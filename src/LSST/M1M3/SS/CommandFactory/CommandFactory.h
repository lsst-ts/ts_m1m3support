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

namespace LSST {
namespace M1M3 {
namespace SS {

class M1M3SSPublisher;
class Context;
class Command;

/*!
 * A command factory that creates dynamically allocated command objects.
 * The command objects created by this factory must be destroyed or a
 * memory leak will occur.
 */
class CommandFactory {
public:
    /*!
     * Instantiates the command factory.
     * @param[in] publisher The publisher that will be provided to the commands created by this factory.
     * @param[in] context The context that will be provided to the commands created by this factory.
     */
    CommandFactory(M1M3SSPublisher* publisher, Context* context);

    /*!
     * Creates a command.
     * This command is dynamically allocated and must be destroyed.
     * @param[in] commandType The type of command to create.
     * @param[in] data The data for the command.
     * @param[in] commandID The command ID (optional).
     */
    Command* create(Commands::Type commandType, void* data = 0, int32_t commandID = 0);
    /*!
     * Destroys a command created by this factory.
     * Commands created by this factory are dynamically allocated and therefore must be cleaned up.
     * @param[in] command The command to destroy.
     */
    void destroy(Command* command);

private:
    M1M3SSPublisher* _publisher;
    Context* _context;
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* COMMANDFACTORY_H_ */
