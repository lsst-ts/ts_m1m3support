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
#include <string>

struct m1m3_GyroDataC;

namespace LSST {
namespace M1M3 {
namespace SS {

class GyroSettings;
class IFPGA;
class IPublisher;

class Gyro: public IGyro {
private:
	GyroSettings* gyroSettings;
	IFPGA* fpga;
	IPublisher* publisher;
	m1m3_GyroDataC* gyroData;

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

private:
	void setBuffer(GyroBuffer* buffer, std::string text);

	void publishGyroData();

	void writeCommand(GyroBuffer* buffer);
	void readCommand();
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* GYRO_H_ */
