/*
 * Model.h
 *
 *  Created on: Sep 26, 2017
 *      Author: ccontaxis
 */

#ifndef MODEL_H_
#define MODEL_H_

#include <IModel.h>
#include <pthread.h>
#include <string>

namespace LSST {
namespace M1M3 {
namespace SS {

class ISettingReader;

class Model : public IModel {
private:
	ISettingReader* settingReader;
	IPublisher* publisher;
	IFPGA* fpga;
	IRS232* rs232;
	IILC* ilc;

	pthread_mutex_t mutex;

public:
	Model(ISettingReader* settingReader, IPublisher* publisher, IFPGA* fpga);
	virtual ~Model();

	virtual ISettingReader* getSettingReader();
	virtual IPublisher* getPublisher();
	virtual IFPGA* getFPGA();
	virtual IRS232* getRS232();
	virtual IILC* getILC();

	virtual void loadSettings(std::string settingsToApply);

	virtual void queryFPGAData();
	virtual void publishFPGAData();

	virtual void publishStateChange(States::Type newState);
	virtual void publishRecommendedSettings();

	virtual void shutdown();
	virtual void waitForShutdown();
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* MODEL_H_ */
