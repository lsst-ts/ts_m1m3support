/*
 * Gyro.h
 *
 *  Created on: Jan 4, 2018
 *      Author: ccontaxis
 */

#ifndef GYRO_H_
#define GYRO_H_

#include <DataTypes.h>
#include <SAL_m1m3C.h>
#include <string>

namespace LSST {
namespace M1M3 {
namespace SS {

class GyroSettings;
class FPGA;
class M1M3SSPublisher;
class ModbusBuffer;

class Gyro {
private:
	GyroSettings* gyroSettings;
	FPGA* fpga;
	M1M3SSPublisher* publisher;
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
	GyroBuffer bitBuffer;

public:
	Gyro(GyroSettings* gyroSettings, FPGA* fpga, M1M3SSPublisher* publisher);

	void bit();

	void enterConfigurationMode();
	void exitConfigurationMode();

	void enableIgnore();
	void disableIgnore();

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

	void readShortBIT(ModbusBuffer* buffer);
	void readFirst6OfBIT(ModbusBuffer* buffer);
	void readLongBIT(ModbusBuffer* buffer);
	void readData(ModbusBuffer* buffer);
	void readToEndOfFrame(ModbusBuffer* buffer);

	bool checkGyroWarningForUpdates();
	void publishGyroWarning();
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* GYRO_H_ */
