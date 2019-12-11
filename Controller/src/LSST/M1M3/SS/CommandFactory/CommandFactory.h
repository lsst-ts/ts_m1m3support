/*
 * CommandFactory.h
 *
 *  Created on: Sep 29, 2017
 *      Author: ccontaxis
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
   private:
    M1M3SSPublisher* publisher;
    Context* context;

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
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* COMMANDFACTORY_H_ */
