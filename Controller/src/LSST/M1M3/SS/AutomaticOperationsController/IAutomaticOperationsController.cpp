/*
 * IAutomaticOperationsController.cpp
 *
 *  Created on: Dec 13, 2017
 *      Author: ccontaxis
 */

#include <IAutomaticOperationsController.h>

namespace LSST {
namespace M1M3 {
namespace SS {

IAutomaticOperationsController::~IAutomaticOperationsController() { }

void IAutomaticOperationsController::startRaiseOperation() { }
void IAutomaticOperationsController::tryIncrementingSupportPercentage() { }
bool IAutomaticOperationsController::checkRaiseOperationComplete() { return false; }
void IAutomaticOperationsController::completeRaiseOperation() { }
bool IAutomaticOperationsController::checkRaiseOperationTimeout() { return false; }
void IAutomaticOperationsController::timeoutRaiseOperation() { }

void IAutomaticOperationsController::startLowerOperation() { }
void IAutomaticOperationsController::tryDecrementSupportPercentage() { }
bool IAutomaticOperationsController::checkLowerOperationComplete() { return false; }
void IAutomaticOperationsController::completeLowerOperation() { }
bool IAutomaticOperationsController::checkLowerOperationTimeout() { return false; }
void IAutomaticOperationsController::timeoutLowerOperation() { }

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
