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

#include <RecommendedApplicationSettings.h>
#include <XMLDocLoad.h>

using namespace pugi;

namespace LSST {
namespace M1M3 {
namespace SS {

void RecommendedApplicationSettings::load(const std::string &filename) {
    xml_document doc;
    XMLDocLoad(filename.c_str(), doc);
    xpath_node_set nodes =
            doc.select_nodes("//RecommendedApplicationSettings/RecommendedSettings/RecommendedSetting");
    this->RecommendedSettings.clear();
    for (xpath_node_set::const_iterator node = nodes.begin(); node != nodes.end(); ++node) {
        RecommendedSettings.push_back(node->node().child_value());
    }
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
