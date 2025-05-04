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

#ifndef FORCEACTUATORSETTINGS_H_
#define FORCEACTUATORSETTINGS_H_

#include <string>
#include <vector>

#include <yaml-cpp/yaml.h>

#include <SAL_MTM1M3.h>

#include <cRIO/Singleton.h>

#include <DistributedForces.h>
#include <ForceActuatorBumpTestSettings.h>
#include <ForceComponentSettings.h>
#include <ForcesAndMoments.h>
#include <Limit.h>
#include <cRIO/DataTypes.h>

namespace LSST {
namespace M1M3 {
namespace SS {

struct ForceActuatorNeighbors {
    ForceActuatorNeighbors();
    int NearZIDs[FA_MAX_NEAR_COUNT];
    int FarIDs[FA_FAR_COUNT];
};

/**
 * Stores force actuator settings. Publish settings through SAL/DDS.
 */
class ForceActuatorSettings : public MTM1M3_logevent_forceActuatorSettingsC,
                              public cRIO::Singleton<ForceActuatorSettings> {
public:
    ForceActuatorSettings(token);

    void load(YAML::Node doc);

    /**
     * Returns true if actuator with given ID is disabled in configuration file.
     *
     * @param actId actuator ID (101..443)
     *
     * @return true if given actuator is disabled in configuration file.
     */
    bool isActuatorDisabled(int32_t actIndex) { return enabledActuators[actIndex] == false; }

    ForcesAndMoments calculateForcesAndMoments(const std::vector<float> &xForces,
                                               const std::vector<float> &yForces,
                                               const std::vector<float> &zForces);

    /** Calculates
     */
    ForcesAndMoments calculateForcesAndMoments(const std::vector<float> &zForces);
    DistributedForces calculateForceFromAngularAcceleration(float angularAccelerationX,
                                                            float angularAccelerationY,
                                                            float angularAccelerationZ);

    DistributedForces calculateForceFromAngularVelocity(float angularVelocityX, float angularVelocityY,
                                                        float angularVelocityZ);

    DistributedForces calculateForceFromAzimuthAngle(float azimuthAngle);

    DistributedForces calculateForceFromElevationAngle(float elevationAngle);

    DistributedForces calculateForceFromTemperature(float temperature);

    DistributedForces calculateForceDistribution(float xForce, float yForce, float zForce, float xMoment,
                                                 float yMoment, float zMoment);

    /**
     * Sends updates through SAL/DDS.
     */
    void log();

    std::vector<float> AccelerationXTable;
    std::vector<float> AccelerationYTable;
    std::vector<float> AccelerationZTable;
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
    std::vector<float> VelocityXYTable;

    std::vector<Limit> AberrationLimitZTable;
    std::vector<Limit> AccelerationLimitXTable;
    std::vector<Limit> AccelerationLimitYTable;
    std::vector<Limit> AccelerationLimitZTable;
    std::vector<Limit> ActiveOpticLimitZTable;
    std::vector<Limit> AzimuthLimitXTable;
    std::vector<Limit> AzimuthLimitYTable;
    std::vector<Limit> AzimuthLimitZTable;
    std::vector<Limit> BalanceLimitXTable;
    std::vector<Limit> BalanceLimitYTable;
    std::vector<Limit> BalanceLimitZTable;
    std::vector<Limit> ElevationLimitXTable;
    std::vector<Limit> ElevationLimitYTable;
    std::vector<Limit> ElevationLimitZTable;
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

    ForceActuatorNeighbors Neighbors[FA_COUNT];

    ForceComponentSettings AberrationComponentSettings;
    ForceComponentSettings AccelerationComponentSettings;
    ForceComponentSettings ActiveOpticComponentSettings;
    ForceComponentSettings AzimuthComponentSettings;
    ForceComponentSettings BalanceComponentSettings;
    ForceComponentSettings ElevationComponentSettings;
    ForceComponentSettings OffsetComponentSettings;
    ForceComponentSettings StaticComponentSettings;
    ForceComponentSettings ThermalComponentSettings;
    ForceComponentSettings VelocityComponentSettings;
    ForceComponentSettings FinalComponentSettings;

    /**
     * Tolerances for actuators being tested.
     */
    ForceActuatorBumpTestSettings TestedTolerances;

    /**
     * Tolerances for actuators not tested.
     */
    ForceActuatorBumpTestSettings NonTestedTolerances;

    /// TODO temporary workaround, not needed once
    /// hardpointBalanceForcesOnInActiveState is in XML

private:
    void _loadNearNeighborZTable(const std::string &filename);
    void _loadNeighborsTable(const std::string &filename);
    void _loadFollowingErrorTables(const std::string &primaryFilename, const std::string &secondaryFilename);

    float _measuredForceWarningRatio;
};

}  // namespace SS
}  // namespace M1M3
}  // namespace LSST

#endif /* FORCEACTUATORSETTINGS_H_ */
