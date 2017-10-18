/*
 * IILC.cpp
 *
 *  Created on: Oct 3, 2017
 *      Author: ccontaxis
 */

#include <IILC.h>

namespace LSST {
namespace M1M3 {
namespace SS {

IILC::~IILC() { }

void IILC::writeCalibrationDataBuffer() { }

void IILC::writeSetADCScanRateBuffer() { }

void IILC::writeSetBoostValveDCAGainBuffer() { }

void IILC::writeResetBuffer() { }

void IILC::writeReportServerIDBuffer() { }

void IILC::writeReportServerStatusBuffer() { }

void IILC::writeReportADCScanRateBuffer() { }

void IILC::writeReadCalibrationDataBuffer() { }

void IILC::writeReadBoostValveDCAGainBuffer() { }

void IILC::writeReportDCAIDBuffer() { }

void IILC::writeReportDCAStatusBuffer() { }

void IILC::writeSetModeStandbyBuffer() { }

void IILC::writeSetModeDisableBuffer() { }

void IILC::writeSetModeEnableBuffer() { }

void IILC::writeFreezeSensorListBuffer() { }

void IILC::triggerModbus() { }

void IILC::waitForSubnet(int32_t subnet, int32_t timeout) { }

void IILC::waitForAllSubnets(int32_t timeout) { }

void IILC::read(uint8_t subnet) { }

void IILC::readAll() { }

void IILC::verifyResponses() { }

void IILC::publishForceActuatorStatus() { }

void IILC::publishForceActuatorData() { }

void IILC::publishHardpointStatus() { }

void IILC::publishHardpointData() { }

void IILC::foo() { }

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
