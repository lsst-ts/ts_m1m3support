/*
 * Gyro.h
 *
 *  Created on: Jan 4, 2018
 *      Author: ccontaxis
 */

#ifndef GYRO_H_
#define GYRO_H_

#include <IGyro.h>
#include <DataTypes.h>
#include <SAL_m1m3C.h>
#include <string>

namespace LSST {
namespace M1M3 {
namespace SS {

class GyroSettings;
class IFPGA;
class IPublisher;
class ModbusBuffer;

class Gyro: public IGyro {
private:
	GyroSettings* gyroSettings;
	IFPGA* fpga;
	IPublisher* publisher;
	m1m3_GyroDataC* gyroData;
	m1m3_logevent_GyroWarningC* gyroWarning;
	m1m3_logevent_GyroWarningC previousGyroWarning;

	struct GyroBuffer {
		int32_t Size;
		uint16_t Buffer[156];
	};

	GyroBuffer enterConfigurationBuffer;
	GyroBuffer exitConfigurationBuffer;
	GyroBuffer resetBuffer;
	GyroBuffer rotationFormatRateBuffer;
	GyroBuffer rotationUnitsRadiansBuffer;
	GyroBuffer axesBuffer;
	GyroBuffer dataRateBuffer;

public:
	Gyro(GyroSettings* gyroSettings, IFPGA* fpga, IPublisher* publisher);

	void enterConfigurationMode();
	void exitConfigurationMode();

	void resetConfiguration();

	void setRotationFormatRate();
	void setRotationUnitsRadians();
	void setAxis();
	void setDataRate();

	void read();

	void publishGyroData();
	void publishGyroWarningIfRequired();

private:
	void setBuffer(GyroBuffer* buffer, std::string text);

	void writeCommand(GyroBuffer* buffer);
	void readCommand();

	void readShortBIT(ModbusBuffer* buffer);
	void readFirst6OfBIT(ModbusBuffer* buffer);
	void readLongBIT(ModbusBuffer* buffer);
	void readData(ModbusBuffer* buffer);

	bool checkGyroWarningForUpdates();
	void publishGyroWarning();
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* GYRO_H_ */
