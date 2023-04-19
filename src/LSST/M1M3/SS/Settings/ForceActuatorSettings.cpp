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

#include <spdlog/spdlog.h>

#include <rapidcsv.h>

#include <yaml-cpp/yaml.h>

#include <BoosterValveSettings.h>
#include <ForceActuatorSettings.h>
#include <M1M3SSPublisher.h>
#include <Model.h>
#include <SettingReader.h>
#include <TableLoader.h>

using namespace LSST::M1M3::SS;

ForceActuatorNeighbors::ForceActuatorNeighbors() {
    memset(NearZIDs, 0, sizeof(NearZIDs));
    memset(FarIDs, 0, sizeof(FarIDs));
}

ForceActuatorSettings::ForceActuatorSettings(token) {
    memset(primaryFollowingErrorWarningThreshold, 0, sizeof(primaryFollowingErrorWarningThreshold));
    memset(primaryFollowingErrorCountingFaultThreshold, 0,
           sizeof(primaryFollowingErrorCountingFaultThreshold));
    memset(primaryFollowingErrorImmediateFaultThreshold, 0,
           sizeof(primaryFollowingErrorImmediateFaultThreshold));

    memset(secondaryFollowingErrorWarningThreshold, 0, sizeof(secondaryFollowingErrorWarningThreshold));
    memset(secondaryFollowingErrorCountingFaultThreshold, 0,
           sizeof(secondaryFollowingErrorCountingFaultThreshold));
    memset(secondaryFollowingErrorImmediateFaultThreshold, 0,
           sizeof(secondaryFollowingErrorImmediateFaultThreshold));

    measuredWarningPercentage = 90;
}

void ForceActuatorSettings::load(const std::string &filename) {
    try {
        YAML::Node doc = YAML::LoadFile(filename);

        auto disabledIndices = doc["DisabledActuators"].as<std::vector<int>>();

        for (int i = 0; i < FA_COUNT; ++i) {
            int faId = SettingReader::instance().getForceActuatorApplicationSettings()->ZIndexToActuatorId(i);
            enabledActuators[i] =
                    std::find(disabledIndices.begin(), disabledIndices.end(), faId) == disabledIndices.end();
        }

        BoosterValveSettings::instance().load(doc["BoosterValveControl"]);

        TableLoader::loadTable(1, 3, &AccelerationXTable, doc["AccelerationXTablePath"].as<std::string>());
        TableLoader::loadTable(1, 3, &AccelerationYTable, doc["AccelerationYTablePath"].as<std::string>());
        TableLoader::loadTable(1, 3, &AccelerationZTable, doc["AccelerationZTablePath"].as<std::string>());
        TableLoader::loadTable(1, 6, &AzimuthXTable, doc["AzimuthXTablePath"].as<std::string>());
        TableLoader::loadTable(1, 6, &AzimuthYTable, doc["AzimuthYTablePath"].as<std::string>());
        TableLoader::loadTable(1, 6, &AzimuthZTable, doc["AzimuthZTablePath"].as<std::string>());
        TableLoader::loadTable(1, 6, &HardpointForceMomentTable,
                               doc["HardpointForceMomentTablePath"].as<std::string>());
        TableLoader::loadTable(1, 3, &ForceDistributionXTable,
                               doc["ForceDistributionXTablePath"].as<std::string>());
        TableLoader::loadTable(1, 3, &ForceDistributionYTable,
                               doc["ForceDistributionYTablePath"].as<std::string>());
        TableLoader::loadTable(1, 3, &ForceDistributionZTable,
                               doc["ForceDistributionZTablePath"].as<std::string>());
        TableLoader::loadTable(1, 3, &MomentDistributionXTable,
                               doc["MomentDistributionXTablePath"].as<std::string>());
        TableLoader::loadTable(1, 3, &MomentDistributionYTable,
                               doc["MomentDistributionYTablePath"].as<std::string>());
        TableLoader::loadTable(1, 3, &MomentDistributionZTable,
                               doc["MomentDistributionZTablePath"].as<std::string>());
        TableLoader::loadTable(1, 6, &ElevationXTable, doc["ElevationXTablePath"].as<std::string>());
        TableLoader::loadTable(1, 6, &ElevationYTable, doc["ElevationYTablePath"].as<std::string>());
        TableLoader::loadTable(1, 6, &ElevationZTable, doc["ElevationZTablePath"].as<std::string>());
        TableLoader::loadTable(1, 1, &StaticXTable, doc["StaticXTablePath"].as<std::string>());
        TableLoader::loadTable(1, 1, &StaticYTable, doc["StaticYTablePath"].as<std::string>());
        TableLoader::loadTable(1, 1, &StaticZTable, doc["StaticZTablePath"].as<std::string>());
        TableLoader::loadTable(1, 6, &ThermalXTable, doc["ThermalXTablePath"].as<std::string>());
        TableLoader::loadTable(1, 6, &ThermalYTable, doc["ThermalYTablePath"].as<std::string>());
        TableLoader::loadTable(1, 6, &ThermalZTable, doc["ThermalZTablePath"].as<std::string>());
        TableLoader::loadTable(1, 3, &VelocityXTable, doc["VelocityXTablePath"].as<std::string>());
        TableLoader::loadTable(1, 3, &VelocityYTable, doc["VelocityYTablePath"].as<std::string>());
        TableLoader::loadTable(1, 3, &VelocityZTable, doc["VelocityZTablePath"].as<std::string>());
        TableLoader::loadTable(1, 3, &VelocityXZTable, doc["VelocityXZTablePath"].as<std::string>());
        TableLoader::loadTable(1, 3, &VelocityYZTable, doc["VelocityYZTablePath"].as<std::string>());

        TableLoader::loadLimitTable(1, &AccelerationLimitXTable,
                                    doc["AccelerationLimitXTablePath"].as<std::string>());
        TableLoader::loadLimitTable(1, &AccelerationLimitYTable,
                                    doc["AccelerationLimitYTablePath"].as<std::string>());
        TableLoader::loadLimitTable(1, &AccelerationLimitZTable,
                                    doc["AccelerationLimitZTablePath"].as<std::string>());
        TableLoader::loadLimitTable(1, &ActiveOpticLimitZTable,
                                    doc["ActiveOpticLimitZTablePath"].as<std::string>());
        netActiveOpticForceTolerance = doc["NetActiveOpticForceTolerance"].as<float>();
        TableLoader::loadLimitTable(1, &AzimuthLimitXTable, doc["AzimuthLimitXTablePath"].as<std::string>());
        TableLoader::loadLimitTable(1, &AzimuthLimitYTable, doc["AzimuthLimitYTablePath"].as<std::string>());
        TableLoader::loadLimitTable(1, &AzimuthLimitZTable, doc["AzimuthLimitZTablePath"].as<std::string>());
        TableLoader::loadLimitTable(1, &BalanceLimitXTable, doc["BalanceLimitXTablePath"].as<std::string>());
        TableLoader::loadLimitTable(1, &BalanceLimitYTable, doc["BalanceLimitYTablePath"].as<std::string>());
        TableLoader::loadLimitTable(1, &BalanceLimitZTable, doc["BalanceLimitZTablePath"].as<std::string>());
        TableLoader::loadLimitTable(1, &ElevationLimitXTable,
                                    doc["ElevationLimitXTablePath"].as<std::string>());
        TableLoader::loadLimitTable(1, &ElevationLimitYTable,
                                    doc["ElevationLimitYTablePath"].as<std::string>());
        TableLoader::loadLimitTable(1, &ElevationLimitZTable,
                                    doc["ElevationLimitZTablePath"].as<std::string>());
        TableLoader::loadLimitTable(1, &ForceLimitXTable, doc["ForceLimitXTablePath"].as<std::string>());
        TableLoader::loadLimitTable(1, &ForceLimitYTable, doc["ForceLimitYTablePath"].as<std::string>());
        TableLoader::loadLimitTable(1, &ForceLimitZTable, doc["ForceLimitZTablePath"].as<std::string>());
        TableLoader::loadLimitTable(1, &StaticLimitXTable, doc["StaticLimitXTablePath"].as<std::string>());
        TableLoader::loadLimitTable(1, &StaticLimitYTable, doc["StaticLimitYTablePath"].as<std::string>());
        TableLoader::loadLimitTable(1, &StaticLimitZTable, doc["StaticLimitZTablePath"].as<std::string>());
        TableLoader::loadLimitTable(1, &OffsetLimitXTable, doc["OffsetLimitXTablePath"].as<std::string>());
        TableLoader::loadLimitTable(1, &OffsetLimitYTable, doc["OffsetLimitYTablePath"].as<std::string>());
        TableLoader::loadLimitTable(1, &OffsetLimitZTable, doc["OffsetLimitZTablePath"].as<std::string>());
        TableLoader::loadLimitTable(1, &ThermalLimitXTable, doc["ThermalLimitXTablePath"].as<std::string>());
        TableLoader::loadLimitTable(1, &ThermalLimitYTable, doc["ThermalLimitYTablePath"].as<std::string>());
        TableLoader::loadLimitTable(1, &ThermalLimitZTable, doc["ThermalLimitZTablePath"].as<std::string>());
        TableLoader::loadLimitTable(1, &VelocityLimitXTable,
                                    doc["VelocityLimitXTablePath"].as<std::string>());
        TableLoader::loadLimitTable(1, &VelocityLimitYTable,
                                    doc["VelocityLimitYTablePath"].as<std::string>());
        TableLoader::loadLimitTable(1, &VelocityLimitZTable,
                                    doc["VelocityLimitZTablePath"].as<std::string>());
        TableLoader::loadLimitTable(1, &CylinderLimitPrimaryTable,
                                    doc["CylinderLimitPrimaryTablePath"].as<std::string>());
        TableLoader::loadLimitTable(1, &CylinderLimitSecondaryTable,
                                    doc["CylinderLimitSecondaryTablePath"].as<std::string>());

        measuredWarningPercentage = doc["MeasuredWarningPercentage"].as<float>();

        TableLoader::loadCylinderLimitTable(
                1, primaryCylinderMeasuredForceLowLimit, primaryCylinderMeasuredForceHighLimit,
                secondaryCylinderMeasuredForceLowLimit, secondaryCylinderMeasuredForceHighLimit,
                doc["MeasuredCylinderLimitTablePath"].as<std::string>());
        _loadFollowingErrorTables(doc["FollowingErrorPrimaryCylinderLimitTablePath"].as<std::string>(),
                                  doc["FollowingErrorSecondaryCylinderLimitTablePath"].as<std::string>());

        _loadNearNeighborZTable(doc["ForceActuatorNearZNeighborsTablePath"].as<std::string>());
        _loadNeighborsTable(doc["ForceActuatorNeighborsTablePath"].as<std::string>());

        useInclinometer = doc["UseInclinometer"].as<bool>();
        mirrorXMoment = doc["MirrorXMoment"].as<float>();
        mirrorYMoment = doc["MirrorYMoment"].as<float>();
        mirrorZMoment = doc["MirrorZMoment"].as<float>();
        setpointXMomentLowLimitFactor = doc["SetpointXMomentLowLimitFactor"].as<float>();
        setpointXMomentHighLimitFactor = doc["SetpointXMomentHighLimitFactor"].as<float>();
        setpointYMomentLowLimitFactor = doc["SetpointYMomentLowLimitFactor"].as<float>();
        setpointYMomentHighLimitFactor = doc["SetpointYMomentHighLimitFactor"].as<float>();
        setpointZMomentLowLimitFactor = doc["SetpointZMomentLowLimitFactor"].as<float>();
        setpointZMomentHighLimitFactor = doc["SetpointZMomentHighLimitFactor"].as<float>();
        setpointNearNeighborLimitFactor = doc["SetpointNearNeighborLimitFactor"].as<float>();
        setpointMirrorWeightLimitFactor = doc["SetpointMirrorWeightLimitFactor"].as<float>();
        setpointFarNeighborLimitFactor = doc["SetpointFarNeighborLimitFactor"].as<float>();

        mirrorCenterOfGravityX = doc["MirrorCenterOfGravityX"].as<float>();
        mirrorCenterOfGravityY = doc["MirrorCenterOfGravityY"].as<float>();
        mirrorCenterOfGravityZ = doc["MirrorCenterOfGravityZ"].as<float>();

        raiseIncrementPercentage = doc["RaiseIncrementPercentage"].as<double>();
        lowerDecrementPercentage = doc["LowerDecrementPercentage"].as<double>();
        raiseLowerFollowingErrorLimit = doc["RaiseLowerFollowingErrorLimit"].as<float>();

        AccelerationComponentSettings.set(doc["AccelerationForceComponent"]);
        ActiveOpticComponentSettings.set(doc["ActiveOpticForceComponent"]);
        AzimuthComponentSettings.set(doc["AzimuthForceComponent"]);
        BalanceComponentSettings.set(doc["BalanceForceComponent"]);
        ElevationComponentSettings.set(doc["ElevationForceComponent"]);
        OffsetComponentSettings.set(doc["OffsetForceComponent"]);
        StaticComponentSettings.set(doc["StaticForceComponent"]);
        ThermalComponentSettings.set(doc["ThermalForceComponent"]);
        VelocityComponentSettings.set(doc["VelocityForceComponent"]);
        FinalComponentSettings.set(doc["FinalForceComponent"]);

        auto bumpTest = doc["BumpTest"];

        TestedTolerances.set(bumpTest["TestedTolerances"]);
        NonTestedTolerances.set(bumpTest["NonTestedTolerances"]);

        bumpTestSettleTime = bumpTest["SettleTime"].as<float>(3.0);
        bumpTestMeasurements = bumpTest["Measurements"].as<int>(10);
    } catch (YAML::Exception &ex) {
        throw std::runtime_error(fmt::format("YAML Loading {}: {}", filename, ex.what()));
    }

    log();
}

ForcesAndMoments ForceActuatorSettings::calculateForcesAndMoments(
        ForceActuatorApplicationSettings *forceActuatorApplicationSettings, float *xForces, float *yForces,
        float *zForces) {
    ForcesAndMoments fm;
    fm.Fx = 0;
    fm.Fy = 0;
    fm.Fz = 0;
    fm.Mx = 0;
    fm.My = 0;
    fm.Mz = 0;
    fm.ForceMagnitude = 0;
    for (int zIndex = 0; zIndex < FA_COUNT; ++zIndex) {
        int xIndex = forceActuatorApplicationSettings->ZIndexToXIndex[zIndex];
        int yIndex = forceActuatorApplicationSettings->ZIndexToYIndex[zIndex];
        float rx = forceActuatorApplicationSettings->Table[zIndex].XPosition - mirrorCenterOfGravityX;
        float ry = forceActuatorApplicationSettings->Table[zIndex].YPosition - mirrorCenterOfGravityY;
        float rz = forceActuatorApplicationSettings->Table[zIndex].ZPosition - mirrorCenterOfGravityZ;
        float fx = 0;
        float fy = 0;
        float fz = zForces[zIndex];

        if (xIndex != -1) {
            fx = xForces[xIndex];
        }

        if (yIndex != -1) {
            fy = yForces[yIndex];
        }

        fm.Fx += fx;
        fm.Fy += fy;
        fm.Fz += fz;
        fm.Mx += (fz * ry) - (fy * rz);
        fm.My += (fx * rz) - (fz * rx);
        fm.Mz += (fy * rx) - (fx * ry);
    }
    fm.ForceMagnitude = sqrt(fm.Fx * fm.Fx + fm.Fy * fm.Fy + fm.Fz * fm.Fz);
    return fm;
}

ForcesAndMoments ForceActuatorSettings::calculateForcesAndMoments(
        ForceActuatorApplicationSettings *forceActuatorApplicationSettings, float *zForces) {
    ForcesAndMoments fm;
    fm.Fx = 0;
    fm.Fy = 0;
    fm.Fz = 0;
    fm.Mx = 0;
    fm.My = 0;
    fm.Mz = 0;
    fm.ForceMagnitude = 0;
    for (int zIndex = 0; zIndex < FA_COUNT; ++zIndex) {
        float rx = forceActuatorApplicationSettings->Table[zIndex].XPosition - mirrorCenterOfGravityX;
        float ry = forceActuatorApplicationSettings->Table[zIndex].YPosition - mirrorCenterOfGravityY;
        float rz = forceActuatorApplicationSettings->Table[zIndex].ZPosition - mirrorCenterOfGravityZ;
        float fx = 0;
        float fy = 0;
        float fz = zForces[zIndex];

        fm.Fz += fz;
        fm.Mx += (fz * ry) - (fy * rz);
        fm.My += (fx * rz) - (fz * rx);
    }
    return fm;
}

DistributedForces ForceActuatorSettings::calculateForceFromAngularAcceleration(float angularAccelerationX,
                                                                               float angularAccelerationY,
                                                                               float angularAccelerationZ) {
    DistributedForces forces;
    for (int zIndex = 0; zIndex < FA_COUNT; ++zIndex) {
        int mIndex = zIndex * 3;

        forces.XForces[zIndex] = (AccelerationXTable[mIndex + 0] * angularAccelerationX +
                                  AccelerationYTable[mIndex + 0] * angularAccelerationY +
                                  AccelerationZTable[mIndex + 0] * angularAccelerationZ) /
                                 1000.0;

        forces.YForces[zIndex] = (AccelerationXTable[mIndex + 1] * angularAccelerationX +
                                  AccelerationYTable[mIndex + 1] * angularAccelerationY +
                                  AccelerationZTable[mIndex + 1] * angularAccelerationZ) /
                                 1000.0;

        forces.ZForces[zIndex] = (AccelerationXTable[mIndex + 2] * angularAccelerationX +
                                  AccelerationYTable[mIndex + 2] * angularAccelerationY +
                                  AccelerationZTable[mIndex + 2] * angularAccelerationZ) /
                                 1000.0;
    }

    return forces;
}

DistributedForces ForceActuatorSettings::calculateForceFromAngularVelocity(float angularVelocityX,
                                                                           float angularVelocityY,
                                                                           float angularVelocityZ) {
    float angularVelocityXX = angularVelocityX * angularVelocityX;
    float angularVelocityYY = angularVelocityY * angularVelocityY;
    float angularVelocityZZ = angularVelocityZ * angularVelocityZ;
    float angularVelocityXZ = angularVelocityX * angularVelocityZ;
    float angularVelocityYZ = angularVelocityY * angularVelocityZ;
    DistributedForces forces;
    for (int zIndex = 0; zIndex < FA_COUNT; ++zIndex) {
        int mIndex = zIndex * 3;

        forces.XForces[zIndex] = (VelocityXTable[mIndex + 0] * angularVelocityXX +
                                  VelocityYTable[mIndex + 0] * angularVelocityYY +
                                  VelocityZTable[mIndex + 0] * angularVelocityZZ +
                                  VelocityXZTable[mIndex + 0] * angularVelocityXZ +
                                  VelocityYZTable[mIndex + 0] * angularVelocityYZ) /
                                 1000.0;

        forces.YForces[zIndex] = (VelocityXTable[mIndex + 1] * angularVelocityXX +
                                  VelocityYTable[mIndex + 1] * angularVelocityYY +
                                  VelocityZTable[mIndex + 1] * angularVelocityZZ +
                                  VelocityXZTable[mIndex + 1] * angularVelocityXZ +
                                  VelocityYZTable[mIndex + 1] * angularVelocityYZ) /
                                 1000.0;

        forces.ZForces[zIndex] = (VelocityXTable[mIndex + 2] * angularVelocityXX +
                                  VelocityYTable[mIndex + 2] * angularVelocityYY +
                                  VelocityZTable[mIndex + 2] * angularVelocityZZ +
                                  VelocityXZTable[mIndex + 2] * angularVelocityXZ +
                                  VelocityYZTable[mIndex + 2] * angularVelocityYZ) /
                                 1000.0;
    }

    return forces;
}

DistributedForces ForceActuatorSettings::calculateForceFromAzimuthAngle(float azimuthAngle) {
    float azimuthMatrix[] = {std::pow(azimuthAngle, 5.0f),
                             std::pow(azimuthAngle, 4.0f),
                             std::pow(azimuthAngle, 3.0f),
                             std::pow(azimuthAngle, 2.0f),
                             azimuthAngle,
                             1.0f};
    DistributedForces forces;
    for (int zIndex = 0; zIndex < FA_COUNT; ++zIndex) {
        int mIndex = zIndex * 6;
        forces.XForces[zIndex] =
                AzimuthXTable[mIndex + 0] * azimuthMatrix[0] + AzimuthXTable[mIndex + 1] * azimuthMatrix[1] +
                AzimuthXTable[mIndex + 2] * azimuthMatrix[2] + AzimuthXTable[mIndex + 3] * azimuthMatrix[3] +
                AzimuthXTable[mIndex + 4] * azimuthMatrix[4] + AzimuthXTable[mIndex + 5];

        forces.YForces[zIndex] =
                AzimuthYTable[mIndex + 0] * azimuthMatrix[0] + AzimuthYTable[mIndex + 1] * azimuthMatrix[1] +
                AzimuthYTable[mIndex + 2] * azimuthMatrix[2] + AzimuthYTable[mIndex + 3] * azimuthMatrix[3] +
                AzimuthYTable[mIndex + 4] * azimuthMatrix[4] + AzimuthYTable[mIndex + 5];

        forces.ZForces[zIndex] =
                AzimuthZTable[mIndex + 0] * azimuthMatrix[0] + AzimuthZTable[mIndex + 1] * azimuthMatrix[1] +
                AzimuthZTable[mIndex + 2] * azimuthMatrix[2] + AzimuthZTable[mIndex + 3] * azimuthMatrix[3] +
                AzimuthZTable[mIndex + 4] * azimuthMatrix[4] + AzimuthZTable[mIndex + 5];
    }
    return forces;
}

DistributedForces ForceActuatorSettings::calculateForceFromElevationAngle(float elevationAngle) {
    float elevationMatrix[] = {std::pow(elevationAngle, 5.0f),
                               std::pow(elevationAngle, 4.0f),
                               std::pow(elevationAngle, 3.0f),
                               std::pow(elevationAngle, 2.0f),
                               elevationAngle,
                               1.0f};
    DistributedForces forces;
    for (int zIndex = 0; zIndex < FA_COUNT; ++zIndex) {
        int mIndex = zIndex * 6;
        forces.XForces[zIndex] = ElevationXTable[mIndex + 0] * elevationMatrix[0] +
                                 ElevationXTable[mIndex + 1] * elevationMatrix[1] +
                                 ElevationXTable[mIndex + 2] * elevationMatrix[2] +
                                 ElevationXTable[mIndex + 3] * elevationMatrix[3] +
                                 ElevationXTable[mIndex + 4] * elevationMatrix[4] +
                                 ElevationXTable[mIndex + 5];

        forces.YForces[zIndex] = ElevationYTable[mIndex + 0] * elevationMatrix[0] +
                                 ElevationYTable[mIndex + 1] * elevationMatrix[1] +
                                 ElevationYTable[mIndex + 2] * elevationMatrix[2] +
                                 ElevationYTable[mIndex + 3] * elevationMatrix[3] +
                                 ElevationYTable[mIndex + 4] * elevationMatrix[4] +
                                 ElevationYTable[mIndex + 5];

        forces.ZForces[zIndex] = ElevationZTable[mIndex + 0] * elevationMatrix[0] +
                                 ElevationZTable[mIndex + 1] * elevationMatrix[1] +
                                 ElevationZTable[mIndex + 2] * elevationMatrix[2] +
                                 ElevationZTable[mIndex + 3] * elevationMatrix[3] +
                                 ElevationZTable[mIndex + 4] * elevationMatrix[4] +
                                 ElevationZTable[mIndex + 5];
    }
    return forces;
}

DistributedForces ForceActuatorSettings::calculateForceFromTemperature(float temperature) {
    float temperatureMatrix[] = {std::pow(temperature, 5.0f),
                                 std::pow(temperature, 4.0f),
                                 std::pow(temperature, 3.0f),
                                 std::pow(temperature, 2.0f),
                                 temperature,
                                 1.0f};
    DistributedForces forces;
    for (int zIndex = 0; zIndex < FA_COUNT; ++zIndex) {
        int mIndex = zIndex * 6;
        forces.XForces[zIndex] = ThermalXTable[mIndex + 0] * temperatureMatrix[0] +
                                 ThermalXTable[mIndex + 1] * temperatureMatrix[1] +
                                 ThermalXTable[mIndex + 2] * temperatureMatrix[2] +
                                 ThermalXTable[mIndex + 3] * temperatureMatrix[3] +
                                 ThermalXTable[mIndex + 4] * temperatureMatrix[4] + ThermalXTable[mIndex + 5];

        forces.YForces[zIndex] = ThermalYTable[mIndex + 0] * temperatureMatrix[0] +
                                 ThermalYTable[mIndex + 1] * temperatureMatrix[1] +
                                 ThermalYTable[mIndex + 2] * temperatureMatrix[2] +
                                 ThermalYTable[mIndex + 3] * temperatureMatrix[3] +
                                 ThermalYTable[mIndex + 4] * temperatureMatrix[4] + ThermalYTable[mIndex + 5];

        forces.ZForces[zIndex] = ThermalZTable[mIndex + 0] * temperatureMatrix[0] +
                                 ThermalZTable[mIndex + 1] * temperatureMatrix[1] +
                                 ThermalZTable[mIndex + 2] * temperatureMatrix[2] +
                                 ThermalZTable[mIndex + 3] * temperatureMatrix[3] +
                                 ThermalZTable[mIndex + 4] * temperatureMatrix[4] + ThermalZTable[mIndex + 5];
    }
    return forces;
}

DistributedForces ForceActuatorSettings::calculateForceDistribution(float xForce, float yForce, float zForce,
                                                                    float xMoment, float yMoment,
                                                                    float zMoment) {
    DistributedForces forces;
    for (int i = 0; i < FA_COUNT; ++i) {
        forces.XForces[i] = 0;
        forces.YForces[i] = 0;
        forces.ZForces[i] = 0;
        forces.XForces[i] += ForceDistributionXTable[i * 3 + 0] * xForce;
        forces.YForces[i] += ForceDistributionXTable[i * 3 + 1] * xForce;
        forces.ZForces[i] += ForceDistributionXTable[i * 3 + 2] * xForce;
        forces.XForces[i] += ForceDistributionYTable[i * 3 + 0] * yForce;
        forces.YForces[i] += ForceDistributionYTable[i * 3 + 1] * yForce;
        forces.ZForces[i] += ForceDistributionYTable[i * 3 + 2] * yForce;
        forces.XForces[i] += ForceDistributionZTable[i * 3 + 0] * zForce;
        forces.YForces[i] += ForceDistributionZTable[i * 3 + 1] * zForce;
        forces.ZForces[i] += ForceDistributionZTable[i * 3 + 2] * zForce;

        forces.XForces[i] += MomentDistributionXTable[i * 3 + 0] * xMoment;
        forces.YForces[i] += MomentDistributionXTable[i * 3 + 1] * xMoment;
        forces.ZForces[i] += MomentDistributionXTable[i * 3 + 2] * xMoment;
        forces.XForces[i] += MomentDistributionYTable[i * 3 + 0] * yMoment;
        forces.YForces[i] += MomentDistributionYTable[i * 3 + 1] * yMoment;
        forces.ZForces[i] += MomentDistributionYTable[i * 3 + 2] * yMoment;
        forces.XForces[i] += MomentDistributionZTable[i * 3 + 0] * zMoment;
        forces.YForces[i] += MomentDistributionZTable[i * 3 + 1] * zMoment;
        forces.ZForces[i] += MomentDistributionZTable[i * 3 + 2] * zMoment;
    }
    return forces;
}

void ForceActuatorSettings::log() { M1M3SSPublisher::instance().logForceActuatorSettings(this); }

void ForceActuatorSettings::_loadNearNeighborZTable(const std::string &filename) {
    std::string fullPath = SettingReader::instance().getFilePath(filename);
    try {
        rapidcsv::Document nearTable(fullPath);
        if (nearTable.GetColumnCount() != FA_MAX_NEAR_COUNT + 1) {
            throw std::runtime_error(fmt::format("Near neighbors CSV {} has {} columns, expected {}",
                                                 fullPath, nearTable.GetColumnCount(),
                                                 FA_MAX_NEAR_COUNT + 1));
        }
        if (nearTable.GetRowCount() != FA_COUNT) {
            throw std::runtime_error(fmt::format("Near neighbors CSV {} has {} rows, expected {}", fullPath,
                                                 nearTable.GetRowCount(), FA_COUNT));
        }
        for (size_t row = 0; row < FA_COUNT; row++) {
            size_t neighIdx = 0;
            try {
                auto tableID = nearTable.GetCell<unsigned>(0, row);
                unsigned expectedID =
                        SettingReader::instance().getForceActuatorApplicationSettings()->ZIndexToActuatorId(
                                row);
                if (tableID != expectedID) {
                    throw std::runtime_error(
                            fmt::format("{}:{} expected ID {}, read {}", fullPath, row, expectedID, tableID));
                }
                for (neighIdx = 0; neighIdx < FA_MAX_NEAR_COUNT; neighIdx++) {
                    Neighbors[row].NearZIDs[neighIdx] = nearTable.GetCell<unsigned>(neighIdx + 1, row);
                }
            } catch (std::logic_error &er) {
                throw std::runtime_error(
                        fmt::format("{}:{}: cannot parse row (\"{}\") (column {}): {}", fullPath, row,
                                    rowToStr(nearTable.GetRow<std::string>(row)), neighIdx, er.what()));
            }
        }
    } catch (std::ios_base::failure &er) {
        throw std::runtime_error(fmt::format("Cannot read near neighbors CSV {}: {}", fullPath, er.what()));
    }
}

void ForceActuatorSettings::_loadNeighborsTable(const std::string &filename) {
    std::string fullPath = SettingReader::instance().getFilePath(filename);
    try {
        rapidcsv::Document farTable(fullPath);
        if (farTable.GetColumnCount() != FA_FAR_COUNT + 1) {
            throw std::runtime_error(fmt::format("Far neighbor CSV {} has {} columns, expected {}", fullPath,
                                                 farTable.GetColumnCount(), FA_FAR_COUNT + 1));
        }
        if (farTable.GetRowCount() != FA_COUNT) {
            throw std::runtime_error(fmt::format("Far neighbor CSV {} has {} rows, expected {}", fullPath,
                                                 farTable.GetRowCount(), FA_COUNT));
        }
        for (size_t row = 0; row < FA_COUNT; row++) {
            size_t neighIdx = 0;
            try {
                auto tableID = farTable.GetCell<unsigned>(0, row);
                unsigned expectedID =
                        SettingReader::instance().getForceActuatorApplicationSettings()->ZIndexToActuatorId(
                                row);
                if (tableID != expectedID) {
                    throw std::runtime_error(
                            fmt::format("{}:{} expected ID {}, read {}", fullPath, row, expectedID, tableID));
                }
                for (neighIdx = 0; neighIdx < FA_FAR_COUNT; neighIdx++) {
                    Neighbors[row].FarIDs[neighIdx] = farTable.GetCell<unsigned>(neighIdx + 1, row);
                }
            } catch (std::logic_error &er) {
                throw std::runtime_error(
                        fmt::format("{}:{}: cannot parse row (\"{}\") (column {}): {}", fullPath, row,
                                    rowToStr(farTable.GetRow<std::string>(row)), neighIdx, er.what()));
            }
        }
    } catch (std::ios_base::failure &er) {
        throw std::runtime_error(fmt::format("Cannot read CSV {}: {}", fullPath, er.what()));
    }
}

void ForceActuatorSettings::_loadFollowingErrorTables(const std::string &primaryFilename,
                                                      const std::string &secondaryFilename) {
    auto primaryFullPath = SettingReader::instance().getFilePath(primaryFilename);
    try {
        rapidcsv::Document primary(primaryFullPath);
        auto ID = primary.GetColumn<int>("ID");
        if (ID.size() != FA_COUNT) {
            throw std::runtime_error(
                    fmt::format("PrimaryFollowingErrorTable {} doesn't contain all {} rows - {} read",
                                primaryFullPath, FA_COUNT, ID.size()));
        }
        auto WarningThreshold = primary.GetColumn<float>("WarningThreshold");
        auto CountingFaultThreshold = primary.GetColumn<float>("CountingFaultThreshold");
        auto ImmediateFaultThreshold = primary.GetColumn<float>("ImmediateFaultThreshold");
        for (int i = 0; i < FA_COUNT; i++) {
            if (WarningThreshold[i] > ImmediateFaultThreshold[i]) {
                throw std::runtime_error(fmt::format(
                        "Primary FA ID {} warning threshold is larger than immediate fault: {} > {}", ID[i],
                        WarningThreshold[i], ImmediateFaultThreshold[i]));
            }
            if (CountingFaultThreshold[i] > ImmediateFaultThreshold[i]) {
                throw std::runtime_error(fmt::format(
                        "Primary FA ID {} counting threshold is larger than immediate fault: {} > {}", ID[i],
                        CountingFaultThreshold[i], ImmediateFaultThreshold[i]));
            }

            primaryFollowingErrorWarningThreshold[i] = WarningThreshold[i];
            primaryFollowingErrorCountingFaultThreshold[i] = CountingFaultThreshold[i];
            primaryFollowingErrorImmediateFaultThreshold[i] = ImmediateFaultThreshold[i];
        }
    } catch (std::ios_base::failure &er) {
        throw std::runtime_error(fmt::format("Cannot read {}: {}", primaryFilename, er.what()));
    }

    auto secondaryFullPath = SettingReader::instance().getFilePath(secondaryFilename);
    try {
        rapidcsv::Document secondary(secondaryFullPath);
        auto ID = secondary.GetColumn<int>("ID");
        if (ID.size() != FA_S_COUNT) {
            throw std::runtime_error(
                    fmt::format("SecondaryFollowingErrorTable {} doesn't contain all {} rows - {} read",
                                secondaryFullPath, FA_S_COUNT, ID.size()));
        }
        auto WarningThreshold = secondary.GetColumn<float>("WarningThreshold");
        auto CountingFaultThreshold = secondary.GetColumn<float>("CountingFaultThreshold");
        auto ImmediateFaultThreshold = secondary.GetColumn<float>("ImmediateFaultThreshold");
        for (int i = 0; i < FA_S_COUNT; i++) {
            if (WarningThreshold[i] > ImmediateFaultThreshold[i]) {
                throw std::runtime_error(fmt::format(
                        "Secondary FA ID {} warning threshold is larger than immediate fault: {} > {}", ID[i],
                        WarningThreshold[i], ImmediateFaultThreshold[i]));
            }
            if (CountingFaultThreshold[i] > ImmediateFaultThreshold[i]) {
                throw std::runtime_error(fmt::format(
                        "Secondary FA ID {} counting threshold is larger than immediate fault: {} > {}",
                        ID[i], CountingFaultThreshold[i], ImmediateFaultThreshold[i]));
            }

            secondaryFollowingErrorWarningThreshold[i] = WarningThreshold[i];
            secondaryFollowingErrorCountingFaultThreshold[i] = CountingFaultThreshold[i];
            secondaryFollowingErrorImmediateFaultThreshold[i] = ImmediateFaultThreshold[i];
        }
    } catch (std::ios_base::failure &er) {
        throw std::runtime_error(fmt::format("Cannot read {}: {}", secondaryFilename, er.what()));
    }
}
