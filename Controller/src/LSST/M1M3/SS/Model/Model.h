/*
 * Model.h
 *
 *  Created on: Sep 26, 2017
 *      Author: ccontaxis
 */

#ifndef MODEL_H_
#define MODEL_H_

#include <IModel.h>
#include <AirController.h>
#include <pthread.h>
#include <string>

namespace LSST {
namespace M1M3 {
namespace SS {

class ISettingReader;
class ForceCalculator;

class Model : public IModel {
private:
	ISettingReader* settingReader;
	IPublisher* publisher;
	IFPGA* fpga;
	IRS232* rs232;
	IILC* ilc;
	IAirController* airController;
	ForceCalculator* forceCalculator;

	pthread_mutex_t mutex;

public:
	Model(ISettingReader* settingReader, IPublisher* publisher, IFPGA* fpga);
	virtual ~Model();

	ISettingReader* getSettingReader() { return this->settingReader; }
	IPublisher* getPublisher() { return this->publisher; }
	IFPGA* getFPGA() { return this->fpga; }
	IRS232* getRS232() { return this->rs232; }
	IILC* getILC() { return this->ilc; }
	IAirController* getAirController() { return this->airController; }
	ForceCalculator* getForceCalculator() { return this->forceCalculator; }

	void loadSettings(std::string settingsToApply);

	void queryFPGAData();
	void publishFPGAData();

	void publishStateChange(States::Type newState);
	void publishRecommendedSettings();

	void shutdown();
	void waitForShutdown();
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* MODEL_H_ */
