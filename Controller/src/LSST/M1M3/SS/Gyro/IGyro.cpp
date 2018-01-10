/*
 * IGyro.cpp
 *
 *  Created on: Jan 4, 2018
 *      Author: ccontaxis
 */

#include <IGyro.h>

namespace LSST {
namespace M1M3 {
namespace SS {

IGyro::~IGyro() { }

void IGyro::enterConfigurationMode() { }
void IGyro::exitConfigurationMode() { }

void IGyro::resetConfiguration() { }

void IGyro::setRotationFormatRate() { }
void IGyro::setRotationUnitsRadians() { }
void IGyro::setAxis() { }
void IGyro::setDataRate() { }

void IGyro::read() { }

void IGyro::publishGyroData() { }
void IGyro::publishGyroWarningIfRequired() { }

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
