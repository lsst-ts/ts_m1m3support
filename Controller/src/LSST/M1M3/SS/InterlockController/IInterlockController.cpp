/*
 * IInterlockController.cpp
 *
 *  Created on: Nov 1, 2017
 *      Author: ccontaxis
 */

#include <IInterlockController.h>

namespace LSST {
namespace M1M3 {
namespace SS {

IInterlockController::~IInterlockController() { }

void IInterlockController::setHeartbeat(bool state) { }
void IInterlockController::setCriticalFault(bool state) { }
void IInterlockController::setMirrorLoweringRaising(bool state) { }
void IInterlockController::setMirrorParked(bool state) { }

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
