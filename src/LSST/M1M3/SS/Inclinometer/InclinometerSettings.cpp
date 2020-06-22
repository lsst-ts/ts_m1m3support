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

#include <InclinometerSettings.h>
#include <XMLDocLoad.h>
#include <boost/lexical_cast.hpp>

namespace LSST {
namespace M1M3 {
namespace SS {

void InclinometerSettings::load(const std::string &filename) {
    pugi::xml_document doc;
    XMLDocLoad(filename.c_str(), doc);
    this->Offset =
            boost::lexical_cast<float>(doc.select_node("//InclinometerSettings/Offset").node().child_value());
    this->FaultLow = boost::lexical_cast<float>(
            doc.select_node("//InclinometerSettings/Limits/FaultLow").node().child_value());
    this->WarningLow = boost::lexical_cast<float>(
            doc.select_node("//InclinometerSettings/Limits/WarningLow").node().child_value());
    this->WarningHigh = boost::lexical_cast<float>(
            doc.select_node("//InclinometerSettings/Limits/WarningHigh").node().child_value());
    this->FaultHigh = boost::lexical_cast<float>(
            doc.select_node("//InclinometerSettings/Limits/FaultHigh").node().child_value());
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
