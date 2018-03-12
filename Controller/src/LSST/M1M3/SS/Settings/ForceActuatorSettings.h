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
#include <Limit.h>
#include <string>
#include <vector>

namespace LSST {
namespace M1M3 {
namespace SS {

class ForceActuatorSettings {
public:
	std::vector<float> AccelerationXTable;
	std::vector<float> AccelerationYTable;
	std::vector<float> AccelerationZTable;
	std::vector<float> BendingModeTable;
	std::vector<float> AzimuthXTable;
	std::vector<float> AzimuthYTable;
	std::vector<float> AzimuthZTable;
	std::vector<float> HardpointForceMomentTable;
	std::vector<float> ForceDistributionXTable;
	std::vector<float> ForceDistributionYTable;
	std::vector<float> ForceDistributionZTable;
	std::vector<float> MomentDistributionXTable;
	std::vector<float> MomentDistributionYTable;
	std::vector<float> MomentDistributionZTable;
	std::vector<float> ElevationXTable;
	std::vector<float> ElevationYTable;
	std::vector<float> ElevationZTable;
	std::vector<float> StaticXTable;
	std::vector<float> StaticYTable;
	std::vector<float> StaticZTable;
	std::vector<float> ThermalXTable;
	std::vector<float> ThermalYTable;
	std::vector<float> ThermalZTable;
	std::vector<float> VelocityXTable;
	std::vector<float> VelocityYTable;
	std::vector<float> VelocityZTable;
	std::vector<float> VelocityXZTable;
	std::vector<float> VelocityYZTable;

	std::vector<Limit> AberrationLimitZTable;
	float NetAberrationForceTolerance;
	std::vector<Limit> AccelerationLimitXTable;
	std::vector<Limit> AccelerationLimitYTable;
	std::vector<Limit> AccelerationLimitZTable;
	std::vector<Limit> ActiveOpticLimitZTable;
	float NetActiveOpticForceTolerance;
	std::vector<Limit> AzimuthLimitXTable;
	std::vector<Limit> AzimuthLimitYTable;
	std::vector<Limit> AzimuthLimitZTable;
	std::vector<Limit> BalanceLimitXTable;
	std::vector<Limit> BalanceLimitYTable;
	std::vector<Limit> BalanceLimitZTable;
	std::vector<Limit> ElevationLimitXTable;
	std::vector<Limit> ElevationLimitYTable;
	std::vector<Limit> ElevationLimitZTable;
	std::vector<Limit> ForceLimitXTable;
	std::vector<Limit> ForceLimitYTable;
	std::vector<Limit> ForceLimitZTable;
	std::vector<Limit> OffsetLimitXTable;
	std::vector<Limit> OffsetLimitYTable;
	std::vector<Limit> OffsetLimitZTable;
	std::vector<Limit> StaticLimitXTable;
	std::vector<Limit> StaticLimitYTable;
	std::vector<Limit> StaticLimitZTable;
	std::vector<Limit> ThermalLimitXTable;
	std::vector<Limit> ThermalLimitYTable;
	std::vector<Limit> ThermalLimitZTable;
	std::vector<Limit> VelocityLimitXTable;
	std::vector<Limit> VelocityLimitYTable;
	std::vector<Limit> VelocityLimitZTable;
	std::vector<Limit> CylinderLimitPrimaryTable;
	std::vector<Limit> CylinderLimitSecondaryTable;

	std::vector<Limit> MeasuredPrimaryCylinderLimitTable;
	std::vector<Limit> MeasuredSecondaryCylinderLimitTable;
	std::vector<Limit> FollowingErrorPrimaryCylinderLimitTable;
	std::vector<Limit> FollowingErrorSecondaryCylinderLimitTable;

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

	double RaiseIncrementPercentage;
	double LowerDecrementPercentage;

	void load(const std::string &filename);

private:
	void loadNeighborsTable(const std::string &filename);
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* FORCEACTUATORSETTINGS_H_ */
