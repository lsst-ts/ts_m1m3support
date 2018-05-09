/*
 * ILCMessageFactory.h
 *
 *  Created on: Oct 19, 2017
 *      Author: ccontaxis
 */

#ifndef ILCMESSAGEFACTORY_H_
#define ILCMESSAGEFACTORY_H_

#include <DataTypes.h>

namespace LSST {
namespace M1M3 {
namespace SS {

class ModbusBuffer;
struct ILCApplicationSettings;

class ILCMessageFactory {
private:
	ILCApplicationSettings* ilcApplicationSettings;

public:
	ILCMessageFactory(ILCApplicationSettings* ilcApplicationSettings);

	void reportServerID(ModbusBuffer* buffer, uint8_t address);
	void reportServerStatus(ModbusBuffer* buffer, uint8_t address);
	void changeILCMode(ModbusBuffer* buffer, uint8_t address, uint16_t mode);
	void reportILCMode(ModbusBuffer* buffer, uint8_t address);
	void broadcastStepMotor(ModbusBuffer* buffer, uint8_t broadcastCounter, int8_t* steps);
	void unicastStepMotor(ModbusBuffer* buffer, uint8_t address, int8_t steps);
	void electromechanicalForceAndStatus(ModbusBuffer* buffer, uint8_t address);
	void broadcastElectromechanicalFreezeSensorValues(ModbusBuffer* buffer, uint8_t broadcastCounter);
	void broadcastPneumaticFreezeSensorValues(ModbusBuffer* buffer, uint8_t broadcastCounter);
	void setBoostValveDCAGains(ModbusBuffer* buffer, uint8_t address, float primaryGain, float secondaryGain);
	void readBoostValveDCAGains(ModbusBuffer* buffer, uint8_t address);
	void broadcastForceDemand(ModbusBuffer* buffer, uint8_t broadcastCounter, bool slewFlag, int32_t* saaPrimarySetpoint, int32_t* daaPrimarySetpoint, int32_t* daaSecondarySetpoint);
	void unicastForceDemand(ModbusBuffer* buffer, uint8_t address, bool slewFlag, int32_t primarySetpoint, int32_t secondarySetpoint);
	void unicastSingleAxisForceDemand(ModbusBuffer* buffer, uint8_t address, bool slewFlag, int32_t primarySetpoint);
	void unicastDualAxisForceDemand(ModbusBuffer* buffer, uint8_t address, bool slewFlag, int32_t primarySetpoint, int32_t secondarySetpoint);
	void pneumaticForceStatus(ModbusBuffer* buffer, uint8_t address);
	void setADCScanRate(ModbusBuffer* buffer, uint8_t address, uint8_t rate);
	void reportADCScanRate(ModbusBuffer* buffer, uint8_t address);
	void setADCChannelOffsetAndSensitivity(ModbusBuffer* buffer, uint8_t address, uint8_t channel, float offset, float sensitivity);
	void reset(ModbusBuffer* buffer, uint8_t address);
	void readCalibration(ModbusBuffer* buffer, uint8_t address);
	void reportDCAPressure(ModbusBuffer* buffer, uint8_t address);
	void reportDCAID(ModbusBuffer* buffer, uint8_t address);
	void reportDCAStatus(ModbusBuffer* buffer, uint8_t address);
	void reportLVDT(ModbusBuffer* buffer, uint8_t address);
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* ILCMESSAGEFACTORY_H_ */
