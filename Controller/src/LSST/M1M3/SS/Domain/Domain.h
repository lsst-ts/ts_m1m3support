/*
 * Domain.h
 *
 *  Created on: Nov 2, 2018
 *      Author: ccontaxis
 */

#include <queue>
#include <Functions.h>
#include <LimitDefinitions.h>

#ifndef LSST_M1M3_SS_DOMAIN_DOMAIN_H_
#define LSST_M1M3_SS_DOMAIN_DOMAIN_H_

using namespace LSST::M1M3::SS;

#define ACCELEROMETER_COUNT                                 8
#define ACCELEROMETER_ANGULAR_ACCELERATION_AXIS_COUNT       3
#define ACCELEROMETER_1X                                    0
#define ACCELEROMETER_1Y                                    1
#define ACCELEROMETER_2X                                    2
#define ACCELEROMETER_2Y                                    3
#define ACCELEROMETER_3X                                    4
#define ACCELEROMETER_3Y                                    5
#define ACCELEROMETER_4X                                    6
#define ACCELEROMETER_4Y                                    7
#define ACCELEROMETER_ANGULAR_ACCELERATION_X                0
#define ACCELEROMETER_ANGULAR_ACCELERATION_Y                1
#define ACCELEROMETER_ANGULAR_ACCELERATION_Z                2

struct AccelerometerSettings {
	LinearFunction<float> RawVoltageToCorrectedVoltage[ACCELEROMETER_COUNT];
	LinearFunction<float> CorrectedVoltageToLinearAcceleration[ACCELEROMETER_COUNT];
	AngularAccelerationFunction LinearAccelerationToAngularAcceleration[ACCELEROMETER_ANGULAR_ACCELERATION_AXIS_COUNT];
	NotInRangeLimitDefinition<float> RawVoltageLimit[ACCELEROMETER_COUNT];
	NotInRangeLimitDefinition<float> AngularAccelerationLimit[ACCELEROMETER_ANGULAR_ACCELERATION_AXIS_COUNT];
};

struct AccelerometerData {
	double SampleTimestamp;
	float RawVoltage[ACCELEROMETER_COUNT];
	float CorrectedVoltage[ACCELEROMETER_COUNT];
	float LinearAcceleration[ACCELEROMETER_COUNT];
	float AngularAcceleration[ACCELEROMETER_ANGULAR_ACCELERATION_AXIS_COUNT];
	Limit RawVoltageLimit[ACCELEROMETER_COUNT];
	Limit AngularAccelerationLimit[ACCELEROMETER_ANGULAR_ACCELERATION_AXIS_COUNT];
};

struct AirSystemSettings {
	BitSetFunction AirValveOpenedSensor;
	BitSetFunction AirValveClosedSensor;
	NotEqualLimitDefinition<bool> AirCommandedOpenAirValveOpenedSensorLimit;
	NotEqualLimitDefinition<bool> AirCommandedOpenAirValveClosedSensorLimit;
	NotEqualLimitDefinition<bool> AirCommandedClosedAirValveOpenedSensorLimit;
	NotEqualLimitDefinition<bool> AirCommandedClosedAirValveClosedSensorLimit;
	TimedLimitDefinition AirCommandedOpenAirValveOpenedSensorLimitTimed;
	TimedLimitDefinition AirCommandedOpenAirValveClosedSensorLimitTimed;
	TimedLimitDefinition AirCommandedClosedAirValveOpenedSensorLimitTimed;
	TimedLimitDefinition AirCommandedClosedAirValveClosedSensorLimitTimed;
	int AirValveCommandBit;
};

struct AirSystemData {
	double SampleTimestamp;
	unsigned long RawSample;
	bool AirValveOpenedSensor;
	bool AirValveClosedSensor;
	Limit AirValveOpenedSensorLimit;
	Limit AirValveClosedSensorLimit;
	bool AirValveCommandedOpen;
};

#define GYRO_AXIS_COUNT                                        3
#define GYRO_BIT_WORD_COUNT                                    8
#define GYRO_BIT_0                                             0
#define GYRO_BIT_1                                             1
#define GYRO_BIT_2                                             2
#define GYRO_BIT_3                                             3
#define GYRO_BIT_4                                             4
#define GYRO_BIT_5                                             5
#define GYRO_BIT_6                                             6
#define GYRO_BIT_7                                             7
#define GYRO_ANGULAR_VELOCITY_X                                0
#define GYRO_ANGULAR_VELOCITY_Y                                1
#define GYRO_ANGULAR_VELOCITY_Z                                2
#define GYRO_ERROR_NONE                                        0
#define GYRO_ERROR_INVALID_HEADER                              1
#define GYRO_ERROR_CRC_MISMATCH                                2
#define GYRO_ERROR_INCOMPLETE_FRAME                            3

struct GyroSystemSettings {
	LinearFunction<float> RawAngularVelocityToAngularVelocity[GYRO_AXIS_COUNT];
	NotInRangeLimitDefinition<float> RawAngularVelocityLimit[GYRO_AXIS_COUNT];
	NotInRangeLimitDefinition<float> AngularVelocityLimit[GYRO_AXIS_COUNT];
	NotInRangeLimitDefinition<int> TemperatureLimit;
	NotEqualLimitDefinition<int> StatusFlagLimit;
	NotEqualLimitDefinition<int> BITFlagLimit[GYRO_BIT_WORD_COUNT];
};

struct GyroSystemData {
	double SampleTimestamp;
	float RawAngularVelocity[GYRO_AXIS_COUNT];
	float AngularVelocity[GYRO_AXIS_COUNT];
	int SequenceNumber;
	int Temperature;
	int StatusFlag;
	double BITTimestamp;
	int BITFlag[GYRO_BIT_WORD_COUNT];
	Limit RawAngularVelocityLimit[GYRO_AXIS_COUNT];
	Limit AngularVelocityLimit[GYRO_AXIS_COUNT];
	Limit TemperatureLimit;
	Limit StatusFlagLimit;
	Limit BITFlagLimit[GYRO_BIT_WORD_COUNT];
};

struct InterlockSystemSettings {
	NotBitSetFunction AuxPowerNetworkOff;
	NotBitSetFunction ThermalEquipmentOff;
	NotBitSetFunction AirSupplyOff;
	NotBitSetFunction CabinetDoorOpen;
	NotBitSetFunction TMAMotionStop;
	NotBitSetFunction GISHeartbeatLost;
	NotBitSetLimitDefinition AuxPowerNetworkOffLimit;
	NotBitSetLimitDefinition ThermalEquipmentOffLimit;
	NotBitSetLimitDefinition AirSupplyOffLimit;
	NotBitSetLimitDefinition CabinetDoorOpenLimit;
	NotBitSetLimitDefinition TMAMotionStopLimit;
	NotBitSetLimitDefinition GISHeartbeatLostLimit;
	int HeartbeatCommandBit;
};

struct InterlockSystemData {
	double SampleTimestamp;
	unsigned long RawSample;
	bool AuxPowerNetworkOff;
	bool ThermalEquipmentOff;
	bool AirSupplyOff;
	bool CabinetDoorOpen;
	bool TMAMotionStop;
	bool GISHeartbeatLost;
	Limit AuxPowerNetworkOffLimit;
	Limit ThermalEquipmentOffLimit;
	Limit AirSupplyOffLimit;
	Limit CabinetDoorOpenLimit;
	Limit TMAMotionStopLimit;
	Limit GISHeartbeatLostLimit;
	bool HeartbeatCommandedHigh;
};

struct LightSystemSettings {
	NotEqualLimitDefinition<bool> LightOnLightSensorLimit;
	NotEqualLimitDefinition<bool> LightOffLightSensorLimit;
	TimedLimitDefinition LightOnLightSensorLimitTimed;
	TimedLimitDefinition LightOffLightSensorLimitTimed;
	int LightCommandBit;
};

struct LightSystemData {
	double SampleTimestamp;
	unsigned long RawSample;
	Limit LightSensorLimit;
	bool LightCommandedOn;
};

#define IMS_SENSOR_COUNT                                       8
#define IMS_AXIS_COUNT                                         6
#define IMS_DISPLACEMENT_1                                     0
#define IMS_DISPLACEMENT_2                                     1
#define IMS_DISPLACEMENT_3                                     2
#define IMS_DISPLACEMENT_4                                     3
#define IMS_DISPLACEMENT_5                                     4
#define IMS_DISPLACEMENT_6                                     5
#define IMS_DISPLACEMENT_7                                     6
#define IMS_DISPLACEMENT_8                                     7
#define IMS_POSITION_X                                         0
#define IMS_POSITION_Y                                         1
#define IMS_POSITION_Z                                         2
#define IMS_ROTATION_X                                         3
#define IMS_ROTATION_Y                                         4
#define IMS_ROTATION_Z                                         5
#define IMS_ERROR_NONE                                         0
#define IMS_ERROR_UNKNOWN_PROBLEM                              1
#define IMS_ERROR_INVALID_RESPONSE                             2
#define IMS_ERROR_RESPONSE_TIMEOUT                             3
#define IMS_ERROR_SENSOR_REPORTS_INVALID_COMMAND               4
#define IMS_ERROR_SENSOR_REPORTS_COMMUNICATION_TIMEOUT_ERROR   5
#define IMS_ERROR_SENSOR_REPORTS_DATA_LENGTH_ERROR             6
#define IMS_ERROR_SENSOR_REPORTS_NUMBER_OF_PARAMETERS_ERROR    7
#define IMS_ERROR_SENSOR_REPORTS_PARAMETER_ERROR               8
#define IMS_ERROR_SENSOR_REPORTS_COMMUNICATION_ERROR           9
#define IMS_ERROR_SENSOR_REPORTS_ID_NUMBER_ERROR               10
#define IMS_ERROR_SENSOR_REPORTS_EXPANSION_LINE_ERROR          11
#define IMS_ERROR_SENSOR_REPORTS_WRITE_CONTROL_ERROR           12

struct IMSSystemSettings {
	LinearFunction<float> RawDisplacementToDisplacement[IMS_SENSOR_COUNT];
	Matrix1x8Function DisplacementToRawPosition[IMS_AXIS_COUNT];
	LinearFunction<float> RawPositionToPosition[IMS_AXIS_COUNT];
	NotInRangeLimitDefinition<int> RawDisplacementLimit[IMS_SENSOR_COUNT];
	NotInRangeLimitDefinition<float> PositionLimit[IMS_AXIS_COUNT];
	NotEqualLimitDefinition<int> ErrorCodeLimit;
};

struct IMSSystemData {
	double SampleTimestamp;
	int RawDisplacement[IMS_SENSOR_COUNT];
	float Displacement[IMS_SENSOR_COUNT];
	float RawPosition[IMS_AXIS_COUNT];
	float Position[IMS_AXIS_COUNT];
	double ErrorTimestamp;
	int ErrorCode;
	Limit RawDisplacementLimit[IMS_SENSOR_COUNT];
	Limit PositionLimit[IMS_AXIS_COUNT];
	Limit ErrorCodeLimit;
};

#define INCLINOMETER_ERROR_NONE                                0
#define INCLINOMETER_ERROR_UNKNOWN_ADDRESS                     1
#define INCLINOMETER_ERROR_UNKNOWN_FUNCTION                    2
#define INCLINOMETER_ERROR_INVALID_LENGTH                      3
#define INCLINOMETER_ERROR_INVALID_CRC                         4
#define INCLINOMETER_ERROR_UNKNOWN_PROBLEM                     5
#define INCLINOMETER_ERROR_RESPONSE_TIMEOUT                    6
#define INCLINOMETER_ERROR_SENSOR_REPORTS_ILLEGAL_FUNCTION     7
#define INCLINOMETER_ERROR_SENSOR_REPORTS_ILLEGAL_DATA_ADDRESS 8

struct InclinometerSystemSettings {
	LinearFunction<float> RawInclinomterToInclinometer;
	NotInRangeLimitDefinition<float> RawInclinometerLimit;
	NotInRangeLimitDefinition<float> InclinometerLimit;
	NotEqualLimitDefinition<int> ErrorCodeLimit;
};

struct InclinometerSystemData {
	double SampleTimestamp;
	float RawInclinometerAngle;
	float InclinometerAngle;
	int ErrorFlags;
	Limit RawInclinometerLimit;
	Limit InclinometerLimit;
	Limit ErrorCodeLimit;
};

#endif /* LSST_M1M3_SS_DOMAIN_DOMAIN_H_ */
