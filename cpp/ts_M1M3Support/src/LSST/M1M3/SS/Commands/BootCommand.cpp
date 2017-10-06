/*
 * BootCommand.cpp
 *
 *  Created on: Sep 26, 2017
 *      Author: ccontaxis
 */

#include <BootCommand.h>
#include <IContext.h>

namespace LSST {
namespace M1M3 {
namespace SS {

BootCommand::BootCommand(IContext* context) {
	this->context = context;
}

void BootCommand::execute() {
	this->context->boot(this);
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
