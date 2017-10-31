/*
 * IPositionController.cpp
 *
 *  Created on: Oct 30, 2017
 *      Author: ccontaxis
 */

#include <IPositionController.h>

namespace LSST {
namespace M1M3 {
namespace SS {

IPositionController::~IPositionController() { }

void IPositionController::enableChase(int32_t actuatorIndex) { }
void IPositionController::disableChase(int32_t actuatorIndex) { }
void IPositionController::enableChaseAll() { }
void IPositionController::disableChaseAll() { }

void IPositionController::move(int32_t* steps) { }
void IPositionController::translate(double x, double y, double z, double rX, double rY, double rZ) { }
void IPositionController::stopMotion() { }

void IPositionController::updateSteps() { }

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
