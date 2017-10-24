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

IRS232* IModel::getRS232() { return 0; }

IILC* IModel::getILC() { return 0; }

IAirController* IModel::getAirController() { return 0; }

void IModel::loadSettings(std::string settingsToApply) { }

void IModel::calculateForces() { }

void IModel::queryFPGAData() { }

void IModel::publishFPGAData() { }

void IModel::publishStateChange(States::Type newState) { }

void IModel::publishRecommendedSettings() { }

void IModel::shutdown() { }

void IModel::waitForShutdown() { }

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
