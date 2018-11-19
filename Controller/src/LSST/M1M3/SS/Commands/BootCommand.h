/*
 * BootCommand.h
 *
 *  Created on: Sep 26, 2017
 *      Author: ccontaxis
 */

#ifndef BOOTCOMMAND_H_
#define BOOTCOMMAND_H_

#include <Command.h>

namespace LSST {
namespace M1M3 {
namespace SS {

/*!
 * This command is responsible for transitioning the state
 * machine from the offline state to the standby state.
 * This is an internal command only and cannot be issued via SAL.
 */
class BootCommand: public Command {
public:
	BootCommand(Context* context);

	void execute();
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* BOOTCOMMAND_H_ */
