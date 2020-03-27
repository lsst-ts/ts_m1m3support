/*
 * Domain.h
 *
 *  Created on: Jul 10, 2018
 *      Author: ccontaxis
 */

#ifndef LSST_M1M3_SS_INCLUDE_APPLICATIONDOMAIN_H_
#define LSST_M1M3_SS_INCLUDE_APPLICATIONDOMAIN_H_

#define DCACCELEROMETER_1_X 0
#define DCACCELEROMETER_1_Y 1
#define DCACCELEROMETER_2_X 2
#define DCACCELEROMETER_2_Y 3
#define DCACCELEROMETER_3_X 4
#define DCACCELEROMETER_3_Y 5
#define DCACCELEROMETER_4_X 6
#define DCACCELEROMETER_4_Y 7
#define DCACCELEROMETER_COUNT 8

#define IMS_SENSOR_1
#define IMS_SENSOR_2
#define IMS_SENSOR_3
#define IMS_SENSOR_4
#define IMS_SENSOR_5
#define IMS_SENSOR_6
#define IMS_SENSOR_7
#define IMS_SENSOR_8
#define IMS_SENSOR_COUNT 8

#define X_INDEX 0
#define Y_INDEX 1
#define Z_INDEX 2
#define RX_INDEX 3
#define RY_INDEX 4
#define RZ_INDEX 5

#define FX_INDEX 0
#define FY_INDEX 1
#define FZ_INDEX 2
#define MX_INDEX 3
#define MY_INDEX 4
#define MZ_INDEX 5

#define AXIS_COUNT 3
#define DOF_COUNT 6

namespace LSST {
namespace M1M3 {
namespace SS {

struct DCAccelerometer {
	float VoltageInVolts;
	float AccelerationInMetersPerSecondSqrd;
};

struct MirrorCellAngularAcceleration {
	float AngularAcceleration[AXIS_COUNT];
};

struct MirrorCellAngularAccelerationSystem {
	DCAccelerometer Sensor[DCACCELEROMETER_COUNT];
	MirrorCellAngularAcceleration AngularAcceleration;
};

struct IMSSensor {
	float DisplacementInMillimeters;
	float DisplacementInMeters;
};

struct MirrorPosition {
	float Position[DOF_COUNT];
};

struct IMSSystem {
	IMSSensor Sensor[IMS_SENSOR_COUNT];
	MirrorPosition MirrorPosition;
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* LSST_M1M3_SS_INCLUDE_APPLICATIONDOMAIN_H_ */
