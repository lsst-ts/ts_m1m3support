/*
 * CommandFactory.h
 *
 *  Created on: Sep 29, 2017
 *      Author: ccontaxis
 */

#ifndef COMMANDFACTORY_H_
#define COMMANDFACTORY_H_

#include <ICommandFactory.h>

namespace LSST {
namespace M1M3 {
namespace SS {

class IPublisher;
class IContext;

/*!
 * A command factory that creates dynamically allocated command objects.
 * The command objects created by this factory must be destroyed or a
 * memory leak will occur.
 */
class CommandFactory: public ICommandFactory {
private:
	IPublisher* publisher;
	IContext* context;

public:
	/*!
	 * Instantiates the command factory.
	 * @param[in] publisher The publisher that will be provided to the commands created by this factory.
	 * @param[in] context The context that will be provided to the commands created by this factory.
	 */
	CommandFactory(IPublisher* publisher, IContext* context);

	/*!
	 * Creates a command.
	 * This command is dynamically allocated and must be destroyed.
	 * @param[in] commandType The type of command to create.
	 * @param[in] data The data for the command.
	 * @param[in] commandID The command ID (optional).
	 */
	ICommand* create(Commands::Type commandType, void* data = 0, int32_t commandID = 0);
	/*!
	 * Destroys a command created by this factory.
	 * Commands created by this factory are dynamically allocated and therefore must be cleaned up.
	 * @param[in] command The command to destroy.
	 */
	void destroy(ICommand* command);
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* COMMANDFACTORY_H_ */
