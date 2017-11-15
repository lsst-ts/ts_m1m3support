/*
 * ForceActuatorLimits.h
 *
 *  Created on: Nov 8, 2017
 *      Author: ccontaxis
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
	double AOSZLowLimit;
	double AOSZHighLimit;
	double AberrationZLowLimit;
	double AberrationZHighLimit;
	double OffsetXLowLimit;
	double OffsetXHighLimit;
	double OffsetYLowLimit;
	double OffsetYHighLimit;
	double OffsetZLowLimit;
	double OffsetZHighLimit;
	double StaticXLowLimit;
	double StaticXHighLimit;
	double StaticYLowLimit;
	double StaticYHighLimit;
	double StaticZLowLimit;
	double StaticZHighLimit;
};

#endif /* FORCEACTUATORLIMITS_H_ */
