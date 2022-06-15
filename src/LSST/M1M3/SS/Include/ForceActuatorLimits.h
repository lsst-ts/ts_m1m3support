/*
 * This file is part of LSST M1M3 support system package.
 *
 * Developed for the Vera C. Rubin Telescope and Site System.
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

#ifndef FORCEACTUATORLIMITS_H_
#define FORCEACTUATORLIMITS_H_

#include <DataTypes.h>

struct ForceActuatorLimits {
    int32_t Id;
    int32_t PrimaryAxisTotalLowLimit;
    int32_t PrimaryAxisTotalHighLimit;
    int32_t SecondaryAxisTotalLowLimit;
    int32_t SecondaryAxisTotalHighLimit;
    double ElevationXLowLimit;
    double ElevationXHighLimit;
    double ElevationYLowLimit;
    double ElevationYHighLimit;
    double ElevationZLowLimit;
    double ElevationZHighLimit;
    double AzimuthXLowLimit;
    double AzimuthXHighLimit;
    double AzimuthYLowLimit;
    double AzimuthYHighLimit;
    double AzimuthZLowLimit;
    double AzimuthZHighLimit;
    double TemperatureXLowLimit;
    double TemperatureXHighLimit;
    double TemperatureYLowLimit;
    double TemperatureYHighLimit;
    double TemperatureZLowLimit;
    double TemperatureZHighLimit;
    double HardpointOffloadingXLowLimit;
    double HardpointOffloadingXHighLimit;
    double HardpointOffloadingYLowLimit;
    double HardpointOffloadingYHighLimit;
    double HardpointOffloadingZLowLimit;
    double HardpointOffloadingZHighLimit;
    double DynamicXLowLimit;
    double DynamicXHighLimit;
    double DynamicYLowLimit;
    double DynamicYHighLimit;
    double DynamicZLowLimit;
    double DynamicZHighLimit;
    double MeasuredPrimaryAxisLowLimit;
    double MeasuredPrimaryAxisHighLimit;
    double MeasuredSecondaryAxisLowLimit;
    double MeasuredSecondaryAxisHighLimit;
    double FollowingErrorPrimaryAxisLimit;
    double FollowingErrorSecondaryAxisLimit;
};

#endif /* FORCEACTUATORLIMITS_H_ */
