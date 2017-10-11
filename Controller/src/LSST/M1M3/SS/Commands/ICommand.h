/*
 * ICommand.h
 *
 *  Created on: Sep 26, 2017
 *      Author: ccontaxis
 */

#ifndef ICOMMAND_H_
#define ICOMMAND_H_

#include <string>

namespace LSST {
namespace M1M3 {
namespace SS {

class IContext;
class IPublisher;

/*!
 * An interface for a command.
 */
class ICommand {
public:
	virtual ~ICommand();

	/*!
	 * Validates the command.
	 */
	virtual bool validate();
	/*!
	 * Executes the command.
	 */
	virtual void execute();
	/*!
	 * Acknowledges the command is in progress.
	 */
	virtual void ackInProgress();
	/*!
	 * Acknowledges the command has completed successfully.
	 */
	virtual void ackComplete();
	/*!
	 * Acknowledges the command has failed.
	 * @param[in] reason The reason why the command has failed.
	 */
	virtual void ackFailed(std::string reason);
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* ICOMMAND_H_ */
