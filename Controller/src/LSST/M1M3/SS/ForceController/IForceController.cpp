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

void IForceController::updateTMAAzimuthData(MTMount_AzC* tmaAzimuthData) { }
void IForceController::updateTMAElevationData(MTMount_AltC* tmaElevationData) { }

void IForceController::incSupportPercentage() { }
void IForceController::decSupportPercentage() { }
void IForceController::zeroSupportPercentage() { }
void IForceController::fillSupportPercentage() { }
bool IForceController::supportPercentageFilled() { return false; }
bool IForceController::supportPercentageZeroed() { return false; }

void IForceController::updateAppliedForces() { }
void IForceController::processAppliedForces() { }

void IForceController::applyStaticForces() { }
void IForceController::zeroStaticForces() { }

void IForceController::applyOffsetForces(double* x, double* y, double* z) { }
void IForceController::zeroOffsetForces() { }

void IForceController::applyAOSCorrectionByBendingModes(double* coefficients) { }
void IForceController::applyAOSCorrectionByForces(double* z) { }
void IForceController::zeroAOSCorrection() { }

void IForceController::applyAberrationByBendingModes(double* coefficients) { }
void IForceController::applyAberrationByForces(double* z) { }
void IForceController::zeroAberration() { }

void IForceController::applyElevationForces() { }
void IForceController::zeroElevationForces() { }

void IForceController::applyAzimuthForces() { }
void IForceController::zeroAzimuthForces() { }

void IForceController::applyTemperatureForces() { }
void IForceController::zeroTemperatureForces() { }

void IForceController::applyDynamicForces() { }
void IForceController::zeroDynamicForces() { }

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
