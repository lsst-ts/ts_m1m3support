/*
 * IPowerController.cpp
 *
 *  Created on: Dec 7, 2017
 *      Author: ccontaxis
 */

#include <IPowerController.h>

namespace LSST {
namespace M1M3 {
namespace SS {

IPowerController::~IPowerController() { }

void IPowerController::checkPowerStatus() { }

void IPowerController::setBothPowerNetworks(bool on) { }

void IPowerController::setAllPowerNetworks(bool on) { }
void IPowerController::setPowerNetworkA(bool on) { }
void IPowerController::setPowerNetworkB(bool on) { }
void IPowerController::setPowerNetworkC(bool on) { }
void IPowerController::setPowerNetworkD(bool on) { }

void IPowerController::setAllAuxPowerNetworks(bool on) { }
void IPowerController::setAuxPowerNetworkA(bool on) { }
void IPowerController::setAuxPowerNetworkB(bool on) { }
void IPowerController::setAuxPowerNetworkC(bool on) { }
void IPowerController::setAuxPowerNetworkD(bool on) { }

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
