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
#include <XMLDocLoad.h>
#include <boost/tokenizer.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>
#include <fstream>
#include <TableLoader.h>
#include <spdlog/spdlog.h>

namespace LSST {
namespace M1M3 {
namespace SS {

void ForceActuatorSettings::load(const std::string &filename) {
    pugi::xml_document doc;
    XMLDocLoad(filename.c_str(), doc);
    _loadDisabledActuators(doc.select_node("//ForceActuatorSettings/DisabledActuators").node().child_value());
    TableLoader::loadTable(
            1, 1, 3, &AccelerationXTable,
            doc.select_node("//ForceActuatorSettings/AccelerationXTablePath").node().child_value());
    TableLoader::loadTable(
            1, 1, 3, &AccelerationYTable,
            doc.select_node("//ForceActuatorSettings/AccelerationYTablePath").node().child_value());
    TableLoader::loadTable(
            1, 1, 3, &AccelerationZTable,
            doc.select_node("//ForceActuatorSettings/AccelerationZTablePath").node().child_value());
    TableLoader::loadTable(1, 1, 6, &AzimuthXTable,
                           doc.select_node("//ForceActuatorSettings/AzimuthXTablePath").node().child_value());
    TableLoader::loadTable(1, 1, 6, &AzimuthYTable,
                           doc.select_node("//ForceActuatorSettings/AzimuthYTablePath").node().child_value());
    TableLoader::loadTable(1, 1, 6, &AzimuthZTable,
                           doc.select_node("//ForceActuatorSettings/AzimuthZTablePath").node().child_value());
    TableLoader::loadTable(
            1, 1, 6, &HardpointForceMomentTable,
            doc.select_node("//ForceActuatorSettings/HardpointForceMomentTablePath").node().child_value());
    TableLoader::loadTable(
            1, 1, 3, &ForceDistributionXTable,
            doc.select_node("//ForceActuatorSettings/ForceDistributionXTablePath").node().child_value());
    TableLoader::loadTable(
            1, 1, 3, &ForceDistributionYTable,
            doc.select_node("//ForceActuatorSettings/ForceDistributionYTablePath").node().child_value());
    TableLoader::loadTable(
            1, 1, 3, &ForceDistributionZTable,
            doc.select_node("//ForceActuatorSettings/ForceDistributionZTablePath").node().child_value());
    TableLoader::loadTable(
            1, 1, 3, &MomentDistributionXTable,
            doc.select_node("//ForceActuatorSettings/MomentDistributionXTablePath").node().child_value());
    TableLoader::loadTable(
            1, 1, 3, &MomentDistributionYTable,
            doc.select_node("//ForceActuatorSettings/MomentDistributionYTablePath").node().child_value());
    TableLoader::loadTable(
            1, 1, 3, &MomentDistributionZTable,
            doc.select_node("//ForceActuatorSettings/MomentDistributionZTablePath").node().child_value());
    TableLoader::loadTable(
            1, 1, 6, &ElevationXTable,
            doc.select_node("//ForceActuatorSettings/ElevationXTablePath").node().child_value());
    TableLoader::loadTable(
            1, 1, 6, &ElevationYTable,
            doc.select_node("//ForceActuatorSettings/ElevationYTablePath").node().child_value());
    TableLoader::loadTable(
            1, 1, 6, &ElevationZTable,
            doc.select_node("//ForceActuatorSettings/ElevationZTablePath").node().child_value());
    TableLoader::loadTable(1, 1, 1, &StaticXTable,
                           doc.select_node("//ForceActuatorSettings/StaticXTablePath").node().child_value());
    TableLoader::loadTable(1, 1, 1, &StaticYTable,
                           doc.select_node("//ForceActuatorSettings/StaticYTablePath").node().child_value());
    TableLoader::loadTable(1, 1, 1, &StaticZTable,
                           doc.select_node("//ForceActuatorSettings/StaticZTablePath").node().child_value());
    TableLoader::loadTable(1, 1, 6, &ThermalXTable,
                           doc.select_node("//ForceActuatorSettings/ThermalXTablePath").node().child_value());
    TableLoader::loadTable(1, 1, 6, &ThermalYTable,
                           doc.select_node("//ForceActuatorSettings/ThermalYTablePath").node().child_value());
    TableLoader::loadTable(1, 1, 6, &ThermalZTable,
                           doc.select_node("//ForceActuatorSettings/ThermalZTablePath").node().child_value());
    TableLoader::loadTable(
            1, 1, 3, &VelocityXTable,
            doc.select_node("//ForceActuatorSettings/VelocityXTablePath").node().child_value());
    TableLoader::loadTable(
            1, 1, 3, &VelocityYTable,
            doc.select_node("//ForceActuatorSettings/VelocityYTablePath").node().child_value());
    TableLoader::loadTable(
            1, 1, 3, &VelocityZTable,
            doc.select_node("//ForceActuatorSettings/VelocityZTablePath").node().child_value());
    TableLoader::loadTable(
            1, 1, 3, &VelocityXZTable,
            doc.select_node("//ForceActuatorSettings/VelocityXZTablePath").node().child_value());
    TableLoader::loadTable(
            1, 1, 3, &VelocityYZTable,
            doc.select_node("//ForceActuatorSettings/VelocityYZTablePath").node().child_value());

    TableLoader::loadLimitTable(
            1, 1, &AberrationLimitZTable,
            doc.select_node("//ForceActuatorSettings/AberrationLimitZTablePath").node().child_value());
    NetAberrationForceTolerance = boost::lexical_cast<float>(
            doc.select_node("//ForceActuatorSettings/NetAberrationForceTolerance").node().child_value());
    TableLoader::loadLimitTable(
            1, 1, &AccelerationLimitXTable,
            doc.select_node("//ForceActuatorSettings/AccelerationLimitXTablePath").node().child_value());
    TableLoader::loadLimitTable(
            1, 1, &AccelerationLimitYTable,
            doc.select_node("//ForceActuatorSettings/AccelerationLimitYTablePath").node().child_value());
    TableLoader::loadLimitTable(
            1, 1, &AccelerationLimitZTable,
            doc.select_node("//ForceActuatorSettings/AccelerationLimitZTablePath").node().child_value());
    TableLoader::loadLimitTable(
            1, 1, &ActiveOpticLimitZTable,
            doc.select_node("//ForceActuatorSettings/ActiveOpticLimitZTablePath").node().child_value());
    NetActiveOpticForceTolerance = boost::lexical_cast<float>(
            doc.select_node("//ForceActuatorSettings/NetActiveOpticForceTolerance").node().child_value());
    TableLoader::loadLimitTable(
            1, 1, &AzimuthLimitXTable,
            doc.select_node("//ForceActuatorSettings/AzimuthLimitXTablePath").node().child_value());
    TableLoader::loadLimitTable(
            1, 1, &AzimuthLimitYTable,
            doc.select_node("//ForceActuatorSettings/AzimuthLimitYTablePath").node().child_value());
    TableLoader::loadLimitTable(
            1, 1, &AzimuthLimitZTable,
            doc.select_node("//ForceActuatorSettings/AzimuthLimitZTablePath").node().child_value());
    TableLoader::loadLimitTable(
            1, 1, &BalanceLimitXTable,
            doc.select_node("//ForceActuatorSettings/BalanceLimitXTablePath").node().child_value());
    TableLoader::loadLimitTable(
            1, 1, &BalanceLimitYTable,
            doc.select_node("//ForceActuatorSettings/BalanceLimitYTablePath").node().child_value());
    TableLoader::loadLimitTable(
            1, 1, &BalanceLimitZTable,
            doc.select_node("//ForceActuatorSettings/BalanceLimitZTablePath").node().child_value());
    TableLoader::loadLimitTable(
            1, 1, &ElevationLimitXTable,
            doc.select_node("//ForceActuatorSettings/ElevationLimitXTablePath").node().child_value());
    TableLoader::loadLimitTable(
            1, 1, &ElevationLimitYTable,
            doc.select_node("//ForceActuatorSettings/ElevationLimitYTablePath").node().child_value());
    TableLoader::loadLimitTable(
            1, 1, &ElevationLimitZTable,
            doc.select_node("//ForceActuatorSettings/ElevationLimitZTablePath").node().child_value());
    TableLoader::loadLimitTable(
            1, 1, &ForceLimitXTable,
            doc.select_node("//ForceActuatorSettings/ForceLimitXTablePath").node().child_value());
    TableLoader::loadLimitTable(
            1, 1, &ForceLimitYTable,
            doc.select_node("//ForceActuatorSettings/ForceLimitYTablePath").node().child_value());
    TableLoader::loadLimitTable(
            1, 1, &ForceLimitZTable,
            doc.select_node("//ForceActuatorSettings/ForceLimitZTablePath").node().child_value());
    TableLoader::loadLimitTable(
            1, 1, &StaticLimitXTable,
            doc.select_node("//ForceActuatorSettings/StaticLimitXTablePath").node().child_value());
    TableLoader::loadLimitTable(
            1, 1, &StaticLimitYTable,
            doc.select_node("//ForceActuatorSettings/StaticLimitYTablePath").node().child_value());
    TableLoader::loadLimitTable(
            1, 1, &StaticLimitZTable,
            doc.select_node("//ForceActuatorSettings/StaticLimitZTablePath").node().child_value());
    TableLoader::loadLimitTable(
            1, 1, &OffsetLimitXTable,
            doc.select_node("//ForceActuatorSettings/OffsetLimitXTablePath").node().child_value());
    TableLoader::loadLimitTable(
            1, 1, &OffsetLimitYTable,
            doc.select_node("//ForceActuatorSettings/OffsetLimitYTablePath").node().child_value());
    TableLoader::loadLimitTable(
            1, 1, &OffsetLimitZTable,
            doc.select_node("//ForceActuatorSettings/OffsetLimitZTablePath").node().child_value());
    TableLoader::loadLimitTable(
            1, 1, &ThermalLimitXTable,
            doc.select_node("//ForceActuatorSettings/ThermalLimitXTablePath").node().child_value());
    TableLoader::loadLimitTable(
            1, 1, &ThermalLimitYTable,
            doc.select_node("//ForceActuatorSettings/ThermalLimitYTablePath").node().child_value());
    TableLoader::loadLimitTable(
            1, 1, &ThermalLimitZTable,
            doc.select_node("//ForceActuatorSettings/ThermalLimitZTablePath").node().child_value());
    TableLoader::loadLimitTable(
            1, 1, &VelocityLimitXTable,
            doc.select_node("//ForceActuatorSettings/VelocityLimitXTablePath").node().child_value());
    TableLoader::loadLimitTable(
            1, 1, &VelocityLimitYTable,
            doc.select_node("//ForceActuatorSettings/VelocityLimitYTablePath").node().child_value());
    TableLoader::loadLimitTable(
            1, 1, &VelocityLimitZTable,
            doc.select_node("//ForceActuatorSettings/VelocityLimitZTablePath").node().child_value());
    TableLoader::loadLimitTable(
            1, 1, &CylinderLimitPrimaryTable,
            doc.select_node("//ForceActuatorSettings/CylinderLimitPrimaryTablePath").node().child_value());
    TableLoader::loadLimitTable(
            1, 1, &CylinderLimitSecondaryTable,
            doc.select_node("//ForceActuatorSettings/CylinderLimitSecondaryTablePath").node().child_value());

    TableLoader::loadLimitTable(
            1, 1, &MeasuredPrimaryCylinderLimitTable,
            doc.select_node("//ForceActuatorSettings/MeasuredPrimaryCylinderLimitTablePath")
                    .node()
                    .child_value());
    TableLoader::loadLimitTable(
            1, 1, &MeasuredSecondaryCylinderLimitTable,
            doc.select_node("//ForceActuatorSettings/MeasuredSecondaryCylinderLimitTablePath")
                    .node()
                    .child_value());
    TableLoader::loadLimitTable(
            1, 1, &FollowingErrorPrimaryCylinderLimitTable,
            doc.select_node("//ForceActuatorSettings/FollowingErrorPrimaryCylinderLimitTablePath")
                    .node()
                    .child_value());
    TableLoader::loadLimitTable(
            1, 1, &FollowingErrorSecondaryCylinderLimitTable,
            doc.select_node("//ForceActuatorSettings/FollowingErrorSecondaryCylinderLimitTablePath")
                    .node()
                    .child_value());

    Neighbors.clear();
    for (int i = 0; i < FA_COUNT; ++i) {
        ForceActuatorNeighbors neighbors;
        Neighbors.push_back(neighbors);
    }
    _loadNearNeighborZTable(doc.select_node("//ForceActuatorSettings/ForceActuatorNearZNeighborsTablePath")
                                    .node()
                                    .child_value());
    _loadNeighborsTable(
            doc.select_node("//ForceActuatorSettings/ForceActuatorNeighborsTablePath").node().child_value());

    UseInclinometer =
            boost::lexical_cast<int32_t>(
                    doc.select_node("//ForceActuatorSettings/UseInclinometer").node().child_value()) != 0;
    MirrorXMoment = boost::lexical_cast<float>(
            doc.select_node("//ForceActuatorSettings/MirrorXMoment").node().child_value());
    MirrorYMoment = boost::lexical_cast<float>(
            doc.select_node("//ForceActuatorSettings/MirrorYMoment").node().child_value());
    MirrorZMoment = boost::lexical_cast<float>(
            doc.select_node("//ForceActuatorSettings/MirrorZMoment").node().child_value());
    SetpointXMomentLowLimitPercentage = boost::lexical_cast<float>(
            doc.select_node("//ForceActuatorSettings/SetpointXMomentLowLimitPercentage")
                    .node()
                    .child_value());
    SetpointXMomentHighLimitPercentage = boost::lexical_cast<float>(
            doc.select_node("//ForceActuatorSettings/SetpointXMomentHighLimitPercentage")
                    .node()
                    .child_value());
    SetpointYMomentLowLimitPercentage = boost::lexical_cast<float>(
            doc.select_node("//ForceActuatorSettings/SetpointYMomentLowLimitPercentage")
                    .node()
                    .child_value());
    SetpointYMomentHighLimitPercentage = boost::lexical_cast<float>(
            doc.select_node("//ForceActuatorSettings/SetpointYMomentHighLimitPercentage")
                    .node()
                    .child_value());
    SetpointZMomentLowLimitPercentage = boost::lexical_cast<float>(
            doc.select_node("//ForceActuatorSettings/SetpointZMomentLowLimitPercentage")
                    .node()
                    .child_value());
    SetpointZMomentHighLimitPercentage = boost::lexical_cast<float>(
            doc.select_node("//ForceActuatorSettings/SetpointZMomentHighLimitPercentage")
                    .node()
                    .child_value());
    SetpointNearNeighborLimitPercentage = boost::lexical_cast<float>(
            doc.select_node("//ForceActuatorSettings/SetpointNearNeighborLimitPercentage")
                    .node()
                    .child_value());
    SetpointMirrorWeightLimitPercentage = boost::lexical_cast<float>(
            doc.select_node("//ForceActuatorSettings/SetpointMirrorWeightLimitPercentage")
                    .node()
                    .child_value());
    SetpointFarNeighborLimitPercentage = boost::lexical_cast<float>(
            doc.select_node("//ForceActuatorSettings/SetpointFarNeighborLimitPercentage")
                    .node()
                    .child_value());

    MirrorCenterOfGravityX = boost::lexical_cast<float>(
            doc.select_node("//ForceActuatorSettings/MirrorCenterOfGravityX").node().child_value());
    MirrorCenterOfGravityY = boost::lexical_cast<float>(
            doc.select_node("//ForceActuatorSettings/MirrorCenterOfGravityY").node().child_value());
    MirrorCenterOfGravityZ = boost::lexical_cast<float>(
            doc.select_node("//ForceActuatorSettings/MirrorCenterOfGravityZ").node().child_value());

    RaiseIncrementPercentage = boost::lexical_cast<double>(
            doc.select_node("//ForceActuatorSettings/RaiseIncrementPercentage").node().child_value());
    LowerDecrementPercentage = boost::lexical_cast<double>(
            doc.select_node("//ForceActuatorSettings/LowerDecrementPercentage").node().child_value());
    RaiseLowerFollowingErrorLimit = boost::lexical_cast<float>(
            doc.select_node("//ForceActuatorSettings/RaiseLowerFollowingErrorLimit").node().child_value());

    AberrationComponentSettings.MaxRateOfChange = boost::lexical_cast<float>(
            doc.select_node("//ForceActuatorSettings/AberrationForceComponent/MaxRateOfChange")
                    .node()
                    .child_value());
    AberrationComponentSettings.NearZeroValue = boost::lexical_cast<float>(
            doc.select_node("//ForceActuatorSettings/AberrationForceComponent/NearZeroValue")
                    .node()
                    .child_value());

    AccelerationComponentSettings.MaxRateOfChange = boost::lexical_cast<float>(
            doc.select_node("//ForceActuatorSettings/AccelerationForceComponent/MaxRateOfChange")
                    .node()
                    .child_value());
    AccelerationComponentSettings.NearZeroValue = boost::lexical_cast<float>(
            doc.select_node("//ForceActuatorSettings/AccelerationForceComponent/NearZeroValue")
                    .node()
                    .child_value());

    ActiveOpticComponentSettings.MaxRateOfChange = boost::lexical_cast<float>(
            doc.select_node("//ForceActuatorSettings/ActiveOpticForceComponent/MaxRateOfChange")
                    .node()
                    .child_value());
    ActiveOpticComponentSettings.NearZeroValue = boost::lexical_cast<float>(
            doc.select_node("//ForceActuatorSettings/ActiveOpticForceComponent/NearZeroValue")
                    .node()
                    .child_value());

    AzimuthComponentSettings.MaxRateOfChange = boost::lexical_cast<float>(
            doc.select_node("//ForceActuatorSettings/AzimuthForceComponent/MaxRateOfChange")
                    .node()
                    .child_value());
    AzimuthComponentSettings.NearZeroValue = boost::lexical_cast<float>(
            doc.select_node("//ForceActuatorSettings/AzimuthForceComponent/NearZeroValue")
                    .node()
                    .child_value());

    BalanceComponentSettings.MaxRateOfChange = boost::lexical_cast<float>(
            doc.select_node("//ForceActuatorSettings/BalanceForceComponent/MaxRateOfChange")
                    .node()
                    .child_value());
    BalanceComponentSettings.NearZeroValue = boost::lexical_cast<float>(
            doc.select_node("//ForceActuatorSettings/BalanceForceComponent/NearZeroValue")
                    .node()
                    .child_value());

    ElevationComponentSettings.MaxRateOfChange = boost::lexical_cast<float>(
            doc.select_node("//ForceActuatorSettings/ElevationForceComponent/MaxRateOfChange")
                    .node()
                    .child_value());
    ElevationComponentSettings.NearZeroValue = boost::lexical_cast<float>(
            doc.select_node("//ForceActuatorSettings/ElevationForceComponent/NearZeroValue")
                    .node()
                    .child_value());

    OffsetComponentSettings.MaxRateOfChange = boost::lexical_cast<float>(
            doc.select_node("//ForceActuatorSettings/OffsetForceComponent/MaxRateOfChange")
                    .node()
                    .child_value());
    OffsetComponentSettings.NearZeroValue = boost::lexical_cast<float>(
            doc.select_node("//ForceActuatorSettings/OffsetForceComponent/NearZeroValue")
                    .node()
                    .child_value());

    StaticComponentSettings.MaxRateOfChange = boost::lexical_cast<float>(
            doc.select_node("//ForceActuatorSettings/StaticForceComponent/MaxRateOfChange")
                    .node()
                    .child_value());
    StaticComponentSettings.NearZeroValue = boost::lexical_cast<float>(
            doc.select_node("//ForceActuatorSettings/StaticForceComponent/NearZeroValue")
                    .node()
                    .child_value());

    ThermalComponentSettings.MaxRateOfChange = boost::lexical_cast<float>(
            doc.select_node("//ForceActuatorSettings/ThermalForceComponent/MaxRateOfChange")
                    .node()
                    .child_value());
    ThermalComponentSettings.NearZeroValue = boost::lexical_cast<float>(
            doc.select_node("//ForceActuatorSettings/ThermalForceComponent/NearZeroValue")
                    .node()
                    .child_value());

    VelocityComponentSettings.MaxRateOfChange = boost::lexical_cast<float>(
            doc.select_node("//ForceActuatorSettings/VelocityForceComponent/MaxRateOfChange")
                    .node()
                    .child_value());
    VelocityComponentSettings.NearZeroValue = boost::lexical_cast<float>(
            doc.select_node("//ForceActuatorSettings/VelocityForceComponent/NearZeroValue")
                    .node()
                    .child_value());

    FinalComponentSettings.MaxRateOfChange = boost::lexical_cast<float>(
            doc.select_node("//ForceActuatorSettings/FinalForceComponent/MaxRateOfChange")
                    .node()
                    .child_value());
    FinalComponentSettings.NearZeroValue = boost::lexical_cast<float>(
            doc.select_node("//ForceActuatorSettings/FinalForceComponent/NearZeroValue")
                    .node()
                    .child_value());

    TestedTolerances.set(doc.select_node("//ForceActuatorSettings/BumpTest/TestedTolerances").node());
    NonTestedTolerances.set(doc.select_node("//ForceActuatorSettings/BumpTest/NonTestedTolerances").node());

    BumpTestSettleTime =
            doc.select_node("//ForceActuatorSettings/BumpTest/SettleTime").node().text().as_float(3.0);
    BumpTestMeasurements =
            doc.select_node("//ForceActuatorSettings/BumpTest/Measurements").node().text().as_int(10);
}

bool ForceActuatorSettings::IsActuatorDisabled(int32_t actId) {
    for (unsigned int i = 0; i < DisabledActuators.size(); ++i) {
        if (DisabledActuators[i] == actId) {
            return true;
        }
    }
    return false;
}

void ForceActuatorSettings::_loadDisabledActuators(const std::string line) {
    typedef boost::tokenizer<boost::escaped_list_separator<char> > tokenizer;
    tokenizer tok(line);
    tokenizer::iterator i = tok.begin();
    DisabledActuators.clear();
    while (i != tok.end()) {
        DisabledActuators.push_back(boost::lexical_cast<int32_t>(*i));
        SPDLOG_WARN("ForceActuatorSettings: Disabled Actuator {:d}",
                    DisabledActuators[DisabledActuators.size() - 1]);
        ++i;
    }
}

void ForceActuatorSettings::_loadNearNeighborZTable(const std::string &filename) {
    typedef boost::tokenizer<boost::escaped_list_separator<char> > tokenizer;
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
    typedef boost::tokenizer<boost::escaped_list_separator<char> > tokenizer;
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

}  // namespace SS
}  // namespace M1M3
}  // namespace LSST
