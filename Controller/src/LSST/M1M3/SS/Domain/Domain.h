/*
 * Domain.h
 *
 *  Created on: Nov 2, 2018
 *      Author: ccontaxis
 */

#ifndef LSST_M1M3_SS_DOMAIN_DOMAIN_H_
#define LSST_M1M3_SS_DOMAIN_DOMAIN_H_

#define LIMIT_NONE                                             0
#define LIMIT_LOW_FAULT                                        1
#define LIMIT_LOW_WARNING                                      2
#define LIMIT_OK                                               3
#define LIMIT_HIGH_WARNING                                     4
#define LIMIT_HIGH_FAULT                                       5

struct Limit {
	enum Type {
		None = 0,
		Ok = 0,
		Warning = 1,
		LowWarning = 1,
		HighWarning = 1,
		Fault = 2,
		LowFault = 2,
		HighFault = 2
	};
};

template <class a> class LimitDefinition {
public:
	virtual ~LimitDefinition();

	virtual Limit check(a value) { return Limit::None; }
};

template <class a> class LimitEvaluator {
private:
	LimitDefinition<a> initialStrategy;
	LimitDefinition<a> currentStrategy;
public:
	LimitEvaluator(LimitDefinition<a> initialStrategy) {
		this->initialStrategy = initialStrategy;
		this->reset();
	}

	void reset() { this->currentStrategy = this->initialStrategy; }
	void changeStrategy(LimitDefinition<a> newStrategy) { this->currentStrategy = newStrategy; }
	Limit check(a value) { return this->currentStrategy.check(value); }
};

template <class a> class ValueCorrection {
public:
	virtual ~ValueCorrection();

	virtual a Process(a value) { return value; }
};

#define DC_ACCELEROMETER_COUNT                                 8
#define DC_ACCELEROMETER_ANGULAR_ACCELERATION_AXIS_COUNT       3
#define DC_ACCELEROMETER_1X                                    0
#define DC_ACCELEROMETER_1Y                                    1
#define DC_ACCELEROMETER_2X                                    2
#define DC_ACCELEROMETER_2Y                                    3
#define DC_ACCELEROMETER_3X                                    4
#define DC_ACCELEROMETER_3Y                                    5
#define DC_ACCELEROMETER_4X                                    6
#define DC_ACCELEROMETER_4Y                                    7
#define DC_ACCELEROMETER_ANGULAR_ACCELERATION_X                0
#define DC_ACCELEROMETER_ANGULAR_ACCELERATION_Y                1
#define DC_ACCELEROMETER_ANGULAR_ACCELERATION_Z                2

struct DCAccelerometerSettings {
	LimitEvaluator<float> RawAccelerometerLimits[DC_ACCELEROMETER_COUNT];
	LimitEvaluator<float> AccelerometerLimits[DC_ACCELEROMETER_COUNT];
	LimitEvaluator<float> AngularAccelerationLimits[DC_ACCELEROMETER_ANGULAR_ACCELERATION_AXIS_COUNT];
	ValueCorrection<float> RawAccelerometerCorrections[DC_ACCELEROMETER_COUNT];
	ValueCorrection<float> AccelerometerCorrections[DC_ACCELEROMETER_COUNT];
	ValueCorrection<float> AngularAccelerationCorrections[DC_ACCELEROMETER_COUNT];
};

struct DCAccelerometerData {
	double SampleTimestamp;
	float RawAccelerometerValues[DC_ACCELEROMETER_COUNT];
	float AccelerometerValues[DC_ACCELEROMETER_COUNT];
	float AngularAccelerationValues[DC_ACCELEROMETER_ANGULAR_ACCELERATION_AXIS_COUNT];
};

#define DI_MASK_INTERLOCK_AUX_POWER_NETWORKS_OFF               0x0001
#define DI_MASK_INTERLOCK_THERMAL_EQUIPMENT_OFF                0x0002
#define DI_MASK_INTERLOCK_AIR_SUPPLY_OFF                       0x0008
#define DI_MASK_INTERLOCK_CABINET_DOOR_OPEN                    0x0010
#define DI_MASK_INTERLOCK_TMA_MOTION_STOP                      0x0040
#define DI_MASK_INTERLOCK_GIS_HEARTBEAT_LOST                   0x0080
#define DI_MASK_AIR_VALVE_OPENED                               0x0100
#define DI_MASK_AIR_VALVE_CLOSED                               0x0200
#define DI_MASK_CELL_LIGHTS_ON                                 0x0400


struct DISettings {
	LimitEvaluator<int> InterlockAuxPowerNetworkOffLimit;
	LimitEvaluator<int> InterlockAirSupplyOffLimit;
	LimitEvaluator<int> InterlockCabinetDoorOpenLimit;
	LimitEvaluator<int> InterlockTMAMotionStopLimit;
	LimitEvaluator<int> InterlockGISHeartbeatLostLimit;
	LimitEvaluator<int> AirValveOpenedLimit;
	LimitEvaluator<int> AirValveClosedLimit;
	LimitEvaluator<int> CellLightsOnLimit;
};

struct DIData {
	double SampleTimestamp;
	int DIState;
};

#define DO_MASK_HEARTBEAT_OUTPUT_ON                            0x01
#define DO_MASK_AIR_OUTPUT_ON                                  0x10
#define DO_MASK_CELL_LIGHTS_OUTPUT_ON                          0x20

struct DOSettings {
	LimitEvaluator<int> HeartbeatOutputLimit;
	LimitEvaluator<int> AirOutputOnLimit;
	LimitEvaluator<int> CellLightsOnLimit;
};

struct DOData {
	double SampleTimestamp;
	int DOState;
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

struct GyroSettings {
	LimitEvaluator<float> AngularVelocityLimits[GYRO_AXIS_COUNT];
	LimitEvaluator<float> TemperatureLimits;
	LimitEvaluator<int> AxisStatusFlagLimit;
	LimitEvaluator<int> BITValueLimits[GYRO_BIT_WORD_COUNT];
	ValueCorrection<float> AngularVelocityCorrections[GYRO_AXIS_COUNT];
};

struct GyroData {
	double SampleTimestamp;
	float AngularVelocityValues[GYRO_AXIS_COUNT];
	int SequenceNumber;
	int Temperature;
	int StatusFlag;
	double BITTimestamp;
	int BITValues[GYRO_BIT_WORD_COUNT];
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

struct IMSSettings {
	LimitDefinition<float> RawDisplacementLimits[IMS_SENSOR_COUNT];
	LimitDefinition<float> MirrorPositionLimits[IMS_AXIS_COUNT];
	ValueCorrection<float> RawDisplacementCorrections[IMS_SENSOR_COUNT];
	ValueCorrection<float> MirrorPositionCorrections[IMS_AXIS_COUNT];
};

struct IMSData {
	double SampleTimestamp;
	float RawDisplacementValues[IMS_SENSOR_COUNT];
	float MirrorPositionValues[IMS_AXIS_COUNT];
	int ErrorFlags;
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

struct InclinometerSettings {
	LimitDefinition<float> InclinometerLimit;
	ValueCorrection<float> InclinometerCorrection;
};

struct InclinometerData {
	double SampleTimestamp;
	float InclinometerAngle;
	int ErrorFlags;
};

#endif /* LSST_M1M3_SS_DOMAIN_DOMAIN_H_ */
