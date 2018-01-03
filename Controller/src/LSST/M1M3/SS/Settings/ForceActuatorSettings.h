/*
 * ForceActuatorSettings.h
 *
 *  Created on: Oct 25, 2017
 *      Author: ccontaxis
 */

#ifndef FORCEACTUATORSETTINGS_H_
#define FORCEACTUATORSETTINGS_H_

#include <DataTypes.h>
#include <ForceActuatorNeighbors.h>
#include <ForceActuatorLimits.h>
#include <string>
#include <vector>

namespace LSST {
namespace M1M3 {
namespace SS {

class ForceActuatorSettings {
public:
	std::vector<double> StaticXForces;
	std::vector<double> StaticYForces;
	std::vector<double> StaticZForces;
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
	std::vector<ForceActuatorLimits> Limits;
	std::vector<ForceActuatorNeighbors> Neighbors;
	bool UseInclinometer;
	double MirrorWeight;
	double MirrorXMoment;
	double MirrorYMoment;
	double MirrorZMoment;
	double SetpointXMomentLowLimitPercentage;
	double SetpointXMomentHighLimitPercentage;
	double SetpointYMomentLowLimitPercentage;
	double SetpointYMomentHighLimitPercentage;
	double SetpointZMomentLowLimitPercentage;
	double SetpointZMomentHighLimitPercentage;
	double SetpointNearNeighborLimitPercentage;
	double SetpointMirrorWeightLimitPercentage;
	double SetpointFarNeighborLimitPercentage;
	double NetAOSForceTolerance;
	double NetAberrationForceTolerance;
	double RaiseIncrementPercentage;
	double LowerDecrementPercentage;

	void load(const std::string &filename);

private:
	void loadStaticForceTable(const std::string &filename);
	void loadLimitTable(const std::string &filename);
	void loadNeighborsTable(const std::string &filename);
	void loadTable(int rowsToSkip, int columnsToSkip, int columnsToKeep, std::vector<double>* data, const std::string &filename);
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* FORCEACTUATORSETTINGS_H_ */
