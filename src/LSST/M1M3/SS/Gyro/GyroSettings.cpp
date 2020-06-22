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

#include <GyroSettings.h>
#include <XMLDocLoad.h>
#include <boost/lexical_cast.hpp>
#include <boost/tokenizer.hpp>

namespace LSST {
namespace M1M3 {
namespace SS {

void GyroSettings::load(const std::string &filename) {
    pugi::xml_document doc;
    XMLDocLoad(filename.c_str(), doc);
    this->DataRate =
            boost::lexical_cast<int>(doc.select_node("//GyroSettings/DataRate").node().child_value());
    this->AngularVelocityXOffset = boost::lexical_cast<float>(
            doc.select_node("//GyroSettings/AngularVelocityXOffset").node().child_value());
    this->AngularVelocityYOffset = boost::lexical_cast<float>(
            doc.select_node("//GyroSettings/AngularVelocityYOffset").node().child_value());
    this->AngularVelocityZOffset = boost::lexical_cast<float>(
            doc.select_node("//GyroSettings/AngularVelocityZOffset").node().child_value());
    std::string axesMatrix = doc.select_node("//GyroSettings/AxesMatrix").node().child_value();
    typedef boost::tokenizer<boost::escaped_list_separator<char> > tokenizer;
    tokenizer tokenize(axesMatrix);
    this->AxesMatrix.clear();
    for (tokenizer::iterator token = tokenize.begin(); token != tokenize.end(); ++token) {
        this->AxesMatrix.push_back(boost::lexical_cast<double>(*token));
    }
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
