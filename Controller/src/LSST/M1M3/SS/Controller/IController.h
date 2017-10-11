/*
 * IController.h
 *
 *  Created on: Sep 27, 2017
 *      Author: ccontaxis
 */

#ifndef ICONTROLLER_H_
#define ICONTROLLER_H_

namespace LSST {
namespace M1M3 {
namespace SS {

class ICommand;

/*!
 * An interface for a controller that executes commands.
 */
class IController {
public:
	virtual ~IController();

	/*!
	 * Lock the controller to keep the command queue safe.
	 */
	virtual void lock();
	/*!
	 * Unlock the controller to stop blocking actions on the command queue.
	 */
	virtual void unlock();

	/*!
	 * Clear the command queue.
	 */
	virtual void clear();

	/*!
	 * Add a command to the command queue. Make sure to lock the controller before adding a command.
	 * @params[in] command The command to add to the command queue.
	 */
	virtual void enqueue(ICommand* command);
	/*!
	 * Removes a command from the command queue. Make sure to lock the controller before removing a command.
	 */
	virtual ICommand* dequeue();
	/*!
	 * Executes a command.
	 * @params[in] command The command to execute.
	 */
	virtual void execute(ICommand* command);
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* ICONTROLLER_H_ */
