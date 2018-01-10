/*
 * PowerController.h
 *
 *  Created on: Dec 7, 2017
 *      Author: ccontaxis
 */

#ifndef POWERCONTROLLER_H_
#define POWERCONTROLLER_H_

#include <IPowerController.h>
#include <DataTypes.h>

struct m1m3_PowerSupplyDataC;

struct m1m3_logevent_PowerStatusC;
struct m1m3_logevent_PowerSupplyStatusC;
struct m1m3_logevent_PowerWarningC;

namespace LSST {
namespace M1M3 {
namespace SS {

class IPublisher;
class IFPGA;
class IExpansionFPGA;
class ISafetyController;

class PowerController: public IPowerController {
private:
	IPublisher* publisher;
	IFPGA* fpga;
	IExpansionFPGA* expansionFPGA;
	ISafetyController* safetyController;

	m1m3_PowerSupplyDataC* powerSupplyData;
	m1m3_logevent_PowerStatusC* powerStatus;
	m1m3_logevent_PowerSupplyStatusC* powerSupplyStatus;
	m1m3_logevent_PowerWarningC* powerWarning;
	uint16_t rxBuffer[16];
	double expansionSampleTime;
	uint32_t previousSlot2Sample;

public:
	PowerController(IPublisher* publisher, IFPGA* fpga, IExpansionFPGA* expansionFPGA, ISafetyController* safetyController);

	void checkPowerStatus();
	void samplePowerSupplyDataAndStatus();
	void publishPowerSupplyData();
	void publishPowerSupplyStatusIfRequired();

	void setBothPowerNetworks(bool on);

	void setAllPowerNetworks(bool on);
	void setPowerNetworkA(bool on);
	void setPowerNetworkB(bool on);
	void setPowerNetworkC(bool on);
	void setPowerNetworkD(bool on);

	void setAllAuxPowerNetworks(bool on);
	void setAuxPowerNetworkA(bool on);
	void setAuxPowerNetworkB(bool on);
	void setAuxPowerNetworkC(bool on);
	void setAuxPowerNetworkD(bool on);

private:
	void checkPowerNetworkAOutput();
	void checkPowerNetworkBOutput();
	void checkPowerNetworkCOutput();
	void checkPowerNetworkDOutput();

	void checkAuxPowerNetworkAOutput();
	void checkAuxPowerNetworkBOutput();
	void checkAuxPowerNetworkCOutput();
	void checkAuxPowerNetworkDOutput();

	bool isChangeInPowerNetworkAOutputMismatch();
	bool isChangeInPowerNetworkBOutputMismatch();
	bool isChangeInPowerNetworkCOutputMismatch();
	bool isChangeInPowerNetworkDOutputMismatch();

	bool isChangeInAuxPowerNetworkAOutputMismatch();
	bool isChangeInAuxPowerNetworkBOutputMismatch();
	bool isChangeInAuxPowerNetworkCOutputMismatch();
	bool isChangeInAuxPowerNetworkDOutputMismatch();

	void sendCommand(uint16_t* buffer, int32_t length);
	bool checkStatus(uint16_t address);

	void publishStatus();
	void publishWarning();
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* POWERCONTROLLER_H_ */
