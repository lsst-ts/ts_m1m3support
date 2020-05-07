/*
 * SupportFPGAData.h
 *
 *  Created on: Mar 29, 2018
 *      Author: ccontaxis
 */

#ifndef SUPPORTFPGADATA_H_
#define SUPPORTFPGADATA_H_

#include <DataTypes.h>

namespace LSST {
namespace M1M3 {
namespace SS {

struct SupportFPGAData {
	uint64_t Reserved;
	uint64_t InclinometerTxBytes;
	uint64_t InclinometerRxBytes;
	uint64_t InclinometerTxFrames;
	uint64_t InclinometerRxFrames;
	uint64_t InclinometerErrorTimestamp;
	uint8_t InclinometerErrorCode;
	uint64_t InclinometerSampleTimestamp;
	int32_t InclinometerAngleRaw;
	uint64_t DisplacementTxBytes;
	uint64_t DisplacementRxBytes;
	uint64_t DisplacementTxFrames;
	uint64_t DisplacementRxFrames;
	uint64_t DisplacementErrorTimestamp;
	uint8_t DisplacementErrorCode;
	uint64_t DisplacementSampleTimestamp;
	int32_t DisplacementRaw1;
	int32_t DisplacementRaw2;
	int32_t DisplacementRaw3;
	int32_t DisplacementRaw4;
	int32_t DisplacementRaw5;
	int32_t DisplacementRaw6;
	int32_t DisplacementRaw7;
	int32_t DisplacementRaw8;
	uint64_t AccelerometerSampleCount;
	uint64_t AccelerometerSampleTimestamp;
	float AccelerometerRaw1;
	float AccelerometerRaw2;
	float AccelerometerRaw3;
	float AccelerometerRaw4;
	float AccelerometerRaw5;
	float AccelerometerRaw6;
	float AccelerometerRaw7;
	float AccelerometerRaw8;
	uint64_t GyroTxBytes;
	uint64_t GyroRxBytes;
	uint64_t GyroTxFrames;
	uint64_t GyroRxFrames;
	uint64_t GyroErrorTimestamp;
	uint8_t GyroErrorCode;
	uint64_t GyroSampleTimestamp;
	float GyroRawX;
	float GyroRawY;
	float GyroRawZ;
	uint8_t GyroStatus;
	uint8_t GyroSequenceNumber;
	int16_t GyroTemperature;
	uint64_t GyroBITTimestamp;
	uint8_t GyroBIT0;
	uint8_t GyroBIT1;
	uint8_t GyroBIT2;
	uint8_t GyroBIT3;
	uint8_t GyroBIT4;
	uint8_t GyroBIT5;
	uint8_t GyroBIT6;
	uint8_t GyroBIT7;
	uint64_t DigitalInputSampleCount;
	uint64_t DigitalInputTimestamp;
	uint16_t DigitalInputStates;
	uint64_t DigitalOutputSampleCount;
	uint64_t DigitalOutputTimestamp;
	uint8_t DigitalOutputStates;
	uint64_t PowerSupplySampleCount;
	uint64_t PowerSupplyTimestamp;
	uint8_t PowerSupplyStates;
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* SUPPORTFPGADATA_H_ */
