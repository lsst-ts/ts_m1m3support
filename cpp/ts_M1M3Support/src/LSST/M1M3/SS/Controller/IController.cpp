/*
 * IController.cpp
 *
 *  Created on: Sep 27, 2017
 *      Author: ccontaxis
 */

#include <IController.h>

namespace LSST {
namespace M1M3 {
namespace SS {

IController::~IController() { }

void IController::lock() { }

void IController::unlock() { }

void IController::clear() { }

void IController::enqueue(ICommand* command) { }

ICommand* IController::dequeue() { return 0; }

void IController::execute(ICommand* command) { }

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
