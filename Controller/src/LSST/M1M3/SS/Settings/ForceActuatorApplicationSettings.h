/*
 * ForceActuatorApplicationSettings.h
 *
 *  Created on: Sep 29, 2017
 *      Author: ccontaxis
 */

#ifndef FORCEACTUATORAPPLICATIONSETTINGS_H_
#define FORCEACTUATORAPPLICATIONSETTINGS_H_

#include <DataTypes.h>
#include <string>
#include <vector>

namespace LSST {
namespace M1M3 {
namespace SS {

struct ForceActuatorTypes {
	enum Type {
		SAA = 0,
		DAA = 1
	};
};

struct ForceActuatorOrientations {
	enum Type {
		NA = 0,
		PositiveY = 1,
		NegativeY = 2,
		PositiveX = 3,
		NegativeX = 4
	};
};

struct ForceActuatorTableRow {
	int32_t Index;
	int32_t ActuatorID;
	double XPosition;
	double YPosition;
	double ZPosition;
	ForceActuatorTypes::Type Type;
	uint8_t Subnet;
	uint8_t Address;
	ForceActuatorOrientations::Type Orientation;
	float PrimaryAxisSensorOffset;
	float PrimaryAxisSensorSensitivity;
	float SecondaryAxisSensorOffset;
	float SecondaryAxisSensorSensitivity;
};

class ForceActuatorApplicationSettings {
public:
	ForceActuatorTableRow Table[156];
	std::vector<double> StaticForces;
	std::vector<double> ElevationXAxisCoefficients;
	std::vector<double> ElevationYAxisCoefficients;
	std::vector<double> ElevationZAxisCoefficients;
	std::vector<double> AzimuthXAxisCoefficients;
	std::vector<double> AzimuthYAxisCoefficients;
	std::vector<double> AzimuthZAxisCoefficients;
	std::vector<double> TemperatureXAxisCoefficients;
	std::vector<double> TemperatureYAxisCoefficients;
	std::vector<double> TemperatureZAxisCoefficients;
	std::vector<double> BendingModeMatrix;
	std::vector<double> HardpointForceMomentMatrix;
	std::vector<double> DynamicAccelerationXMatrix;
	std::vector<double> DynamicAccelerationYMatrix;
	std::vector<double> DynamicAccelerationZMatrix;
	std::vector<double> DynamicVelocityXMatrix;
	std::vector<double> DynamicVelocityYMatrix;
	std::vector<double> DynamicVelocityZMatrix;
	std::vector<double> DynamicVelocityXZMatrix;
	std::vector<double> DynamicVelocityYZMatrix;
	std::vector<double> ForceDistributionXMatrix;
	std::vector<double> ForceDistributionYMatrix;
	std::vector<double> ForceDistributionZMatrix;
	std::vector<double> MomentDistributionXMatrix;
	std::vector<double> MomentDistributionYMatrix;
	std::vector<double> MomentDistributionZMatrix;

	void load(const std::string &filename);

private:
	void loadForceActuatorTable(const std::string &filename);
	void loadTable(int rowsToSkip, int columnsToSkip, int columnsToKeep, std::vector<double>* data, const std::string &filename);
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* FORCEACTUATORAPPLICATIONSETTINGS_H_ */
