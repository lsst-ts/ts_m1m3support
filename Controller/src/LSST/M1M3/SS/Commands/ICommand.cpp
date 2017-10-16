/*
 * ICommand.cpp
 *
 *  Created on: Sep 26, 2017
 *      Author: ccontaxis
 */

#include <ICommand.h>

namespace LSST {
namespace M1M3 {
namespace SS {

ICommand::~ICommand() { }

bool ICommand::validate() { return true; }

void ICommand::execute() { }

void ICommand::ackInProgress() { }

void ICommand::ackComplete() { }

void ICommand::ackFailed(std::string reason) { }

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
