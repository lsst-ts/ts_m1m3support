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

#include <HardpointActuatorSettings.h>
#include <TableLoader.h>
#include <XMLDocLoad.h>
#include <cstring>
#include <boost/lexical_cast.hpp>

namespace LSST {
namespace M1M3 {
namespace SS {

HardpointActuatorSettings::HardpointActuatorSettings() { memset(_encoderOffset, 0, sizeof(_encoderOffset)); }

void HardpointActuatorSettings::load(const std::string &filename) {
    pugi::xml_document doc;
    XMLDocLoad(filename.c_str(), doc);
    TableLoader::loadTable(
            1, 1, 6, &this->HardpointDisplacementToMirrorPosition,
            doc.select_node("//HardpointActuatorSettings/HardpointDisplacementToMirrorPositionTablePath")
                    .node()
                    .child_value());
    TableLoader::loadTable(
            1, 1, 6, &this->MirrorPositionToHardpointDisplacement,
            doc.select_node("//HardpointActuatorSettings/MirrorPositionToHardpointDisplacementTablePath")
                    .node()
                    .child_value());
    this->MicrometersPerStep = boost::lexical_cast<double>(
            doc.select_node("//HardpointActuatorSettings/MicrometersPerStep").node().child_value());
    this->MicrometersPerEncoder = boost::lexical_cast<double>(
            doc.select_node("//HardpointActuatorSettings/MicrometersPerEncoder").node().child_value());
    for (int hp = 0; hp < 6; hp++) {
        _encoderOffset[hp] = boost::lexical_cast<int32_t>(
                doc.select_node(("//HardpointActuatorSettings/HP" + std::to_string(hp + 1) + "EncoderOffset").c_str())
                        .node()
                        .child_value());
    }
    this->HardpointMeasuredForceFaultHigh = boost::lexical_cast<float>(
            doc.select_node("//HardpointActuatorSettings/HardpointMeasuredForceFaultHigh")
                    .node()
                    .child_value());
    this->HardpointMeasuredForceFaultLow = boost::lexical_cast<float>(
            doc.select_node("//HardpointActuatorSettings/HardpointMeasuredForceFaultLow")
                    .node()
                    .child_value());
    this->HardpointMeasuredForceFSBWarningHigh = boost::lexical_cast<float>(
            doc.select_node("//HardpointActuatorSettings/HardpointMeasuredForceFSBWarningHigh")
                    .node()
                    .child_value());
    this->HardpointMeasuredForceFSBWarningLow = boost::lexical_cast<float>(
            doc.select_node("//HardpointActuatorSettings/HardpointMeasuredForceFSBWarningLow")
                    .node()
                    .child_value());
    this->HardpointMeasuredForceWarningHigh = boost::lexical_cast<float>(
            doc.select_node("//HardpointActuatorSettings/HardpointMeasuredForceWarningHigh")
                    .node()
                    .child_value());
    this->HardpointMeasuredForceWarningLow = boost::lexical_cast<float>(
            doc.select_node("//HardpointActuatorSettings/HardpointMeasuredForceWarningLow")
                    .node()
                    .child_value());
    this->AirPressureWarningHigh = boost::lexical_cast<float>(
            doc.select_node("//HardpointActuatorSettings/AirPressureWarningHigh").node().child_value());
    this->AirPressureWarningLow = boost::lexical_cast<float>(
            doc.select_node("//HardpointActuatorSettings/AirPressureWarningLow").node().child_value());
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
