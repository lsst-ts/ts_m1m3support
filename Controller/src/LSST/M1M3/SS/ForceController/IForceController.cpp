/*
 * IForceController.cpp
 *
 *  Created on: Oct 25, 2017
 *      Author: ccontaxis
 */

#include <IForceController.h>

namespace LSST {
namespace M1M3 {
namespace SS {

IForceController::~IForceController() { }

void IForceController::updateAppliedForces() { }
void IForceController::processAppliedForces() { }

void IForceController::applyStaticForces() { }
void IForceController::zeroStaticForces() { }

void IForceController::applyOffsetForces(double* x, double* y, double* z) { }
void IForceController::zeroOffsetForces() { }

void IForceController::applyElevationForces() { }
void IForceController::zeroElevationForces() { }

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
