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

#include <ForceActuatorSettings.h>
#include <SettingReader.h>
#include <M1M3SSPublisher.h>
#include <yaml-cpp/yaml.h>
#include <TableLoader.h>
#include <spdlog/spdlog.h>
#include <algorithm>

using namespace LSST::M1M3::SS;

void ForceActuatorSettings::load(const std::string &filename) {
    try {
        YAML::Node doc = YAML::LoadFile(filename);

        auto disabledIndices = doc["DisabledActuators"].as<std::vector<int>>();

        for (int i = 0; i < FA_COUNT; ++i) {
            enabledActuators[i] =
                    std::find(disabledIndices.begin(), disabledIndices.end(),
                              SettingReader::get().getForceActuatorApplicationSettings()->ZIndexToActuatorId(
                                      i)) == disabledIndices.end();
        }

        TableLoader::loadTable(1, 1, 3, &AccelerationXTable, doc["AccelerationXTablePath"].as<std::string>());
        TableLoader::loadTable(1, 1, 3, &AccelerationYTable, doc["AccelerationYTablePath"].as<std::string>());
        TableLoader::loadTable(1, 1, 3, &AccelerationZTable, doc["AccelerationZTablePath"].as<std::string>());
        TableLoader::loadTable(1, 1, 6, &AzimuthXTable, doc["AzimuthXTablePath"].as<std::string>());
        TableLoader::loadTable(1, 1, 6, &AzimuthYTable, doc["AzimuthYTablePath"].as<std::string>());
        TableLoader::loadTable(1, 1, 6, &AzimuthZTable, doc["AzimuthZTablePath"].as<std::string>());
        TableLoader::loadTable(1, 1, 6, &HardpointForceMomentTable,
                               doc["HardpointForceMomentTablePath"].as<std::string>());
        TableLoader::loadTable(1, 1, 3, &ForceDistributionXTable,
                               doc["ForceDistributionXTablePath"].as<std::string>());
        TableLoader::loadTable(1, 1, 3, &ForceDistributionYTable,
                               doc["ForceDistributionYTablePath"].as<std::string>());
        TableLoader::loadTable(1, 1, 3, &ForceDistributionZTable,
                               doc["ForceDistributionZTablePath"].as<std::string>());
        TableLoader::loadTable(1, 1, 3, &MomentDistributionXTable,
                               doc["MomentDistributionXTablePath"].as<std::string>());
        TableLoader::loadTable(1, 1, 3, &MomentDistributionYTable,
                               doc["MomentDistributionYTablePath"].as<std::string>());
        TableLoader::loadTable(1, 1, 3, &MomentDistributionZTable,
                               doc["MomentDistributionZTablePath"].as<std::string>());
        TableLoader::loadTable(1, 1, 6, &ElevationXTable, doc["ElevationXTablePath"].as<std::string>());
        TableLoader::loadTable(1, 1, 6, &ElevationYTable, doc["ElevationYTablePath"].as<std::string>());
        TableLoader::loadTable(1, 1, 6, &ElevationZTable, doc["ElevationZTablePath"].as<std::string>());
        TableLoader::loadTable(1, 1, 1, &StaticXTable, doc["StaticXTablePath"].as<std::string>());
        TableLoader::loadTable(1, 1, 1, &StaticYTable, doc["StaticYTablePath"].as<std::string>());
        TableLoader::loadTable(1, 1, 1, &StaticZTable, doc["StaticZTablePath"].as<std::string>());
        TableLoader::loadTable(1, 1, 6, &ThermalXTable, doc["ThermalXTablePath"].as<std::string>());
        TableLoader::loadTable(1, 1, 6, &ThermalYTable, doc["ThermalYTablePath"].as<std::string>());
        TableLoader::loadTable(1, 1, 6, &ThermalZTable, doc["ThermalZTablePath"].as<std::string>());
        TableLoader::loadTable(1, 1, 3, &VelocityXTable, doc["VelocityXTablePath"].as<std::string>());
        TableLoader::loadTable(1, 1, 3, &VelocityYTable, doc["VelocityYTablePath"].as<std::string>());
        TableLoader::loadTable(1, 1, 3, &VelocityZTable, doc["VelocityZTablePath"].as<std::string>());
        TableLoader::loadTable(1, 1, 3, &VelocityXZTable, doc["VelocityXZTablePath"].as<std::string>());
        TableLoader::loadTable(1, 1, 3, &VelocityYZTable, doc["VelocityYZTablePath"].as<std::string>());

        TableLoader::loadLimitTable(1, 1, &AberrationLimitZTable,
                                    doc["AberrationLimitZTablePath"].as<std::string>());
        NetAberrationForceTolerance =
                boost::lexical_cast<float>(doc["NetAberrationForceTolerance"].as<std::string>());
        TableLoader::loadLimitTable(1, 1, &AccelerationLimitXTable,
                                    doc["AccelerationLimitXTablePath"].as<std::string>());
        TableLoader::loadLimitTable(1, 1, &AccelerationLimitYTable,
                                    doc["AccelerationLimitYTablePath"].as<std::string>());
        TableLoader::loadLimitTable(1, 1, &AccelerationLimitZTable,
                                    doc["AccelerationLimitZTablePath"].as<std::string>());
        TableLoader::loadLimitTable(1, 1, &ActiveOpticLimitZTable,
                                    doc["ActiveOpticLimitZTablePath"].as<std::string>());
        NetActiveOpticForceTolerance =
                boost::lexical_cast<float>(doc["NetActiveOpticForceTolerance"].as<std::string>());
        TableLoader::loadLimitTable(1, 1, &AzimuthLimitXTable,
                                    doc["AzimuthLimitXTablePath"].as<std::string>());
        TableLoader::loadLimitTable(1, 1, &AzimuthLimitYTable,
                                    doc["AzimuthLimitYTablePath"].as<std::string>());
        TableLoader::loadLimitTable(1, 1, &AzimuthLimitZTable,
                                    doc["AzimuthLimitZTablePath"].as<std::string>());
        TableLoader::loadLimitTable(1, 1, &BalanceLimitXTable,
                                    doc["BalanceLimitXTablePath"].as<std::string>());
        TableLoader::loadLimitTable(1, 1, &BalanceLimitYTable,
                                    doc["BalanceLimitYTablePath"].as<std::string>());
        TableLoader::loadLimitTable(1, 1, &BalanceLimitZTable,
                                    doc["BalanceLimitZTablePath"].as<std::string>());
        TableLoader::loadLimitTable(1, 1, &ElevationLimitXTable,
                                    doc["ElevationLimitXTablePath"].as<std::string>());
        TableLoader::loadLimitTable(1, 1, &ElevationLimitYTable,
                                    doc["ElevationLimitYTablePath"].as<std::string>());
        TableLoader::loadLimitTable(1, 1, &ElevationLimitZTable,
                                    doc["ElevationLimitZTablePath"].as<std::string>());
        TableLoader::loadLimitTable(1, 1, &ForceLimitXTable, doc["ForceLimitXTablePath"].as<std::string>());
        TableLoader::loadLimitTable(1, 1, &ForceLimitYTable, doc["ForceLimitYTablePath"].as<std::string>());
        TableLoader::loadLimitTable(1, 1, &ForceLimitZTable, doc["ForceLimitZTablePath"].as<std::string>());
        TableLoader::loadLimitTable(1, 1, &StaticLimitXTable, doc["StaticLimitXTablePath"].as<std::string>());
        TableLoader::loadLimitTable(1, 1, &StaticLimitYTable, doc["StaticLimitYTablePath"].as<std::string>());
        TableLoader::loadLimitTable(1, 1, &StaticLimitZTable, doc["StaticLimitZTablePath"].as<std::string>());
        TableLoader::loadLimitTable(1, 1, &OffsetLimitXTable, doc["OffsetLimitXTablePath"].as<std::string>());
        TableLoader::loadLimitTable(1, 1, &OffsetLimitYTable, doc["OffsetLimitYTablePath"].as<std::string>());
        TableLoader::loadLimitTable(1, 1, &OffsetLimitZTable, doc["OffsetLimitZTablePath"].as<std::string>());
        TableLoader::loadLimitTable(1, 1, &ThermalLimitXTable,
                                    doc["ThermalLimitXTablePath"].as<std::string>());
        TableLoader::loadLimitTable(1, 1, &ThermalLimitYTable,
                                    doc["ThermalLimitYTablePath"].as<std::string>());
        TableLoader::loadLimitTable(1, 1, &ThermalLimitZTable,
                                    doc["ThermalLimitZTablePath"].as<std::string>());
        TableLoader::loadLimitTable(1, 1, &VelocityLimitXTable,
                                    doc["VelocityLimitXTablePath"].as<std::string>());
        TableLoader::loadLimitTable(1, 1, &VelocityLimitYTable,
                                    doc["VelocityLimitYTablePath"].as<std::string>());
        TableLoader::loadLimitTable(1, 1, &VelocityLimitZTable,
                                    doc["VelocityLimitZTablePath"].as<std::string>());
        TableLoader::loadLimitTable(1, 1, &CylinderLimitPrimaryTable,
                                    doc["CylinderLimitPrimaryTablePath"].as<std::string>());
        TableLoader::loadLimitTable(1, 1, &CylinderLimitSecondaryTable,
                                    doc["CylinderLimitSecondaryTablePath"].as<std::string>());

        TableLoader::loadLimitTable(1, 1, &MeasuredPrimaryCylinderLimitTable,
                                    doc["MeasuredPrimaryCylinderLimitTablePath"].as<std::string>());
        TableLoader::loadLimitTable(1, 1, &MeasuredSecondaryCylinderLimitTable,
                                    doc["MeasuredSecondaryCylinderLimitTablePath"].as<std::string>());
        TableLoader::loadLimitTable(1, 1, &FollowingErrorPrimaryCylinderLimitTable,
                                    doc["FollowingErrorPrimaryCylinderLimitTablePath"].as<std::string>());
        TableLoader::loadLimitTable(1, 1, &FollowingErrorSecondaryCylinderLimitTable,
                                    doc["FollowingErrorSecondaryCylinderLimitTablePath"].as<std::string>());

        Neighbors.clear();
        for (int i = 0; i < FA_COUNT; ++i) {
            ForceActuatorNeighbors neighbors;
            Neighbors.push_back(neighbors);
        }
        _loadNearNeighborZTable(doc["ForceActuatorNearZNeighborsTablePath"].as<std::string>());
        _loadNeighborsTable(doc["ForceActuatorNeighborsTablePath"].as<std::string>());

        useInclinometer = doc["UseInclinometer"].as<bool>();
        mirrorXMoment = doc["MirrorXMoment"].as<float>();
        mirrorYMoment = doc["MirrorYMoment"].as<float>();
        mirrorZMoment = doc["MirrorZMoment"].as<float>();
        setpointXMomentLowLimitPercentage = doc["SetpointXMomentLowLimitPercentage"].as<float>();
        setpointXMomentHighLimitPercentage = doc["SetpointXMomentHighLimitPercentage"].as<float>();
        setpointYMomentLowLimitPercentage = doc["SetpointYMomentLowLimitPercentage"].as<float>();
        setpointYMomentHighLimitPercentage = doc["SetpointYMomentHighLimitPercentage"].as<float>();
        setpointZMomentLowLimitPercentage = doc["SetpointZMomentLowLimitPercentage"].as<float>();
        setpointZMomentHighLimitPercentage = doc["SetpointZMomentHighLimitPercentage"].as<float>();
        setpointNearNeighborLimitPercentage = doc["SetpointNearNeighborLimitPercentage"].as<float>();
        setpointMirrorWeightLimitPercentage = doc["SetpointMirrorWeightLimitPercentage"].as<float>();
        setpointFarNeighborLimitPercentage = doc["SetpointFarNeighborLimitPercentage"].as<float>();

        mirrorCenterOfGravityX = doc["MirrorCenterOfGravityX"].as<float>();
        mirrorCenterOfGravityY = doc["MirrorCenterOfGravityY"].as<float>();
        mirrorCenterOfGravityZ = doc["MirrorCenterOfGravityZ"].as<float>();

        raiseIncrementPercentage = doc["RaiseIncrementPercentage"].as<double>();
        lowerDecrementPercentage = doc["LowerDecrementPercentage"].as<double>();
        raiseLowerFollowingErrorLimit = doc["RaiseLowerFollowingErrorLimit"].as<float>();

        AberrationComponentSettings.set(doc["AberrationForceComponent"]);
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

void ForceActuatorSettings::log() { M1M3SSPublisher::get().logForceActuatorSettings(this); }

void ForceActuatorSettings::_loadNearNeighborZTable(const std::string &filename) {
    typedef boost::tokenizer<boost::escaped_list_separator<char>> tokenizer;
    std::string fullname = SettingReader::get().getFilePath(filename);
    std::ifstream inputStream(fullname.c_str());
    if (!inputStream.is_open()) {
        throw std::runtime_error("Cannot read " + fullname + ": " + strerror(errno));
    }

    std::string lineText;
    int32_t lineNumber = 0;
    while (std::getline(inputStream, lineText)) {
        boost::trim_right(lineText);
        // Skip the first ROW (Header)
        if (lineNumber >= 1 && !lineText.empty()) {
            // Line Format:
            //     ActuatorID,Neighbor1,...,Neighbor7
            tokenizer tok(lineText);
            ForceActuatorNeighbors neighbors;
            tokenizer::iterator i = tok.begin();
            // Skip the first COLUMN (Row ID)
            ++i;
            for (; i != tok.end(); ++i) {
                int32_t id = boost::lexical_cast<int32_t>(*i);
                if (id != 0) {
                    Neighbors[lineNumber - 1].NearZIDs.push_back(id);
                }
            }
        }
        lineNumber++;
    }
    inputStream.close();
}

void ForceActuatorSettings::_loadNeighborsTable(const std::string &filename) {
    typedef boost::tokenizer<boost::escaped_list_separator<char>> tokenizer;
    std::string fullname = SettingReader::get().getFilePath(filename);
    std::ifstream inputStream(fullname.c_str());
    if (!inputStream.is_open()) {
        throw std::runtime_error("Cannot read " + fullname + ": " + strerror(errno));
    }

    std::string lineText;
    int32_t lineNumber = 0;
    while (std::getline(inputStream, lineText)) {
        boost::trim_right(lineText);
        if (lineNumber >= 1 && !lineText.empty()) {
            // Line Format:
            //     ActuatorID,Neighbor1,...,Neighbor12
            tokenizer tok(lineText);
            tokenizer::iterator i = tok.begin();
            ForceActuatorNeighbors neighbors;
            for (int j = 0; j < 12; j++) {
                ++i;
                int32_t id = boost::lexical_cast<int32_t>(*i);
                if (id != 0) {
                    Neighbors[lineNumber - 1].FarIDs.push_back(id);
                }
            }
        }
        lineNumber++;
    }
    inputStream.close();
}
