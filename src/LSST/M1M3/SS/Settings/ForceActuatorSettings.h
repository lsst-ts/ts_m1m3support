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

#include <SAL_MTM1M3.h>

#include <DataTypes.h>
#include <ForceActuatorLimits.h>
#include <ForceComponentSettings.h>
#include <ForceActuatorBumpTestSettings.h>
#include <Limit.h>
#include <string>
#include <vector>

namespace LSST {
namespace M1M3 {
namespace SS {

struct ForceActuatorNeighbors {
    std::vector<int32_t> NearZIDs;
    std::vector<int32_t> FarIDs;
};

/**
 * Stores force actuator settings. Publish settings through SAL/DDS.
 */
class ForceActuatorSettings : public MTM1M3_logevent_forceActuatorSettingsC {
public:
    void load(const std::string &filename);

    /**
     * Returns true if actuator with given ID is disabled in configuration file.
     *
     * @param actId actuator ID (101..443)
     *
     * @return true if given actuator is disabled in configuration file.
     */
    bool isActuatorDisabled(int32_t actIndex) { return enabledActuators[actIndex] == false; }

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

    std::vector<ForceActuatorNeighbors> Neighbors;

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

private:
    void _loadNearNeighborZTable(const std::string &filename);
    void _loadNeighborsTable(const std::string &filename);
    void _loadFollowingErrorTables(const std::string &primaryFilename, const std::string &secondaryFilename);
};

}  // namespace SS
}  // namespace M1M3
}  // namespace LSST

#endif /* FORCEACTUATORSETTINGS_H_ */
