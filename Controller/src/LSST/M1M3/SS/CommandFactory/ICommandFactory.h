/*
 * ICommandFactory.h
 *
 *  Created on: Sep 29, 2017
 *      Author: ccontaxis
 */

#ifndef ICOMMANDFACTORY_H_
#define ICOMMANDFACTORY_H_

#include <CommandTypes.h>
#include <DataTypes.h>

namespace LSST {
namespace M1M3 {
namespace SS {

class ICommand;
/*!
 *  An interface for creating command objects.
 */
class ICommandFactory {
public:
	virtual ~ICommandFactory();

	/*!
	 * Creates the requested command type.
	 * @param[in] command The command to instantiate.
	 * @param[in] data The data for the command to act on.
	 * @param[in] commandID The commandID of the command.
	 */
	virtual ICommand* create(Commands::Type commandType, void* data = 0, int32_t commandID = 0);
	/*!
	 * Cleans up the command object.
	 * @param[in] command The command to delete.
	 */
	virtual void destroy(ICommand* command);
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* ICOMMANDFACTORY_H_ */
