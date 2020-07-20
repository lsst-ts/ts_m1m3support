/*
 * This file is part of LSST M1M3 support system package.
 *
 * Developed for the LSST Data Management System.
 * This product includes software developed by the LSST Project
 * (https://www.lsst.org).
 * See the COPYRIGHT file at the top-level directory of this distribution
 * for details of code ownership.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
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
