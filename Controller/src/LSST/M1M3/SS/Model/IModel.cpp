/*
 * IModel.cpp
 *
 *  Created on: Sep 26, 2017
 *      Author: ccontaxis
 */

#include <IModel.h>

namespace LSST {
namespace M1M3 {
namespace SS {

IModel::~IModel() { }

ISettingReader* IModel::getSettingReader() { return 0; }
IPublisher* IModel::getPublisher() { return 0; }
IFPGA* IModel::getFPGA() { return 0; }
IDisplacement* IModel::getDisplacement() { return 0; }
IInclinometer* IModel::getInclinometer() { return 0; }
IILC* IModel::getILC() { return 0; }
IAirController* IModel::getAirController() { return 0; }
IForceController* IModel::getForceController() { return 0; }
ISafetyController* IModel::getSafetyController() { return 0; }
IPositionController* IModel::getPositionController() { return 0; }
IInterlockController* IModel::getInterlockController() { return 0; }
IAccelerometer* IModel::getAccelerometer() { return 0; }

void IModel::loadSettings(std::string settingsToApply) { }

void IModel::queryFPGAData() { }

void IModel::publishFPGAData() { }

void IModel::publishStateChange(States::Type newState) { }

void IModel::publishRecommendedSettings() { }

void IModel::shutdown() { }

void IModel::waitForShutdown() { }

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
