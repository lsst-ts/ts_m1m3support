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

#include <DisplacementSensorSettings.h>
#include <TableLoader.h>
#include <XMLDocLoad.h>
#include <boost/tokenizer.hpp>
#include <boost/lexical_cast.hpp>

namespace LSST {
namespace M1M3 {
namespace SS {

void DisplacementSensorSettings::load(const std::string &filename) {
    pugi::xml_document doc;
    XMLDocLoad(filename.c_str(), doc);
    TableLoader::loadTable(
            1, 1, 8, &this->ConverterMatrix,
            doc.select_node("//DisplacementSensorSettings/PositionTablePath").node().child_value());
    this->N1Port = boost::lexical_cast<int32_t>(
            doc.select_node("//DisplacementSensorSettings/N1Port").node().child_value());
    this->N2Port = boost::lexical_cast<int32_t>(
            doc.select_node("//DisplacementSensorSettings/N2Port").node().child_value());
    this->N3Port = boost::lexical_cast<int32_t>(
            doc.select_node("//DisplacementSensorSettings/N3Port").node().child_value());
    this->N4Port = boost::lexical_cast<int32_t>(
            doc.select_node("//DisplacementSensorSettings/N4Port").node().child_value());
    this->N5Port = boost::lexical_cast<int32_t>(
            doc.select_node("//DisplacementSensorSettings/N5Port").node().child_value());
    this->N6Port = boost::lexical_cast<int32_t>(
            doc.select_node("//DisplacementSensorSettings/N6Port").node().child_value());
    this->N7Port = boost::lexical_cast<int32_t>(
            doc.select_node("//DisplacementSensorSettings/N7Port").node().child_value());
    this->N8Port = boost::lexical_cast<int32_t>(
            doc.select_node("//DisplacementSensorSettings/N8Port").node().child_value());
    this->N1Offset = boost::lexical_cast<double>(
            doc.select_node("//DisplacementSensorSettings/N1Offset").node().child_value());
    this->N2Offset = boost::lexical_cast<double>(
            doc.select_node("//DisplacementSensorSettings/N2Offset").node().child_value());
    this->N3Offset = boost::lexical_cast<double>(
            doc.select_node("//DisplacementSensorSettings/N3Offset").node().child_value());
    this->N4Offset = boost::lexical_cast<double>(
            doc.select_node("//DisplacementSensorSettings/N4Offset").node().child_value());
    this->N5Offset = boost::lexical_cast<double>(
            doc.select_node("//DisplacementSensorSettings/N5Offset").node().child_value());
    this->N6Offset = boost::lexical_cast<double>(
            doc.select_node("//DisplacementSensorSettings/N6Offset").node().child_value());
    this->N7Offset = boost::lexical_cast<double>(
            doc.select_node("//DisplacementSensorSettings/N7Offset").node().child_value());
    this->N8Offset = boost::lexical_cast<double>(
            doc.select_node("//DisplacementSensorSettings/N8Offset").node().child_value());
    this->XPositionOffset = boost::lexical_cast<double>(
            doc.select_node("//DisplacementSensorSettings/XPositionOffset").node().child_value());
    this->YPositionOffset = boost::lexical_cast<double>(
            doc.select_node("//DisplacementSensorSettings/YPositionOffset").node().child_value());
    this->ZPositionOffset = boost::lexical_cast<double>(
            doc.select_node("//DisplacementSensorSettings/ZPositionOffset").node().child_value());
    this->XRotationOffset = boost::lexical_cast<double>(
            doc.select_node("//DisplacementSensorSettings/XRotationOffset").node().child_value());
    this->YRotationOffset = boost::lexical_cast<double>(
            doc.select_node("//DisplacementSensorSettings/YRotationOffset").node().child_value());
    this->ZRotationOffset = boost::lexical_cast<double>(
            doc.select_node("//DisplacementSensorSettings/ZRotationOffset").node().child_value());
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
