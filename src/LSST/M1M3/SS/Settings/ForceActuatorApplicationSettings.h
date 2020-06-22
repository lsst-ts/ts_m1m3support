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

#ifndef FORCEACTUATORAPPLICATIONSETTINGS_H_
#define FORCEACTUATORAPPLICATIONSETTINGS_H_

#include <ForceActuatorTableRow.h>
#include <string>
#include <vector>

namespace LSST {
namespace M1M3 {
namespace SS {

class ForceActuatorApplicationSettings {
public:
    std::vector<int32_t> XIndexToZIndex;
    std::vector<int32_t> YIndexToZIndex;
    std::vector<int32_t> SecondaryCylinderIndexToZIndex;
    std::vector<int32_t> ZIndexToXIndex;
    std::vector<int32_t> ZIndexToYIndex;
    std::vector<int32_t> ZIndexToSecondaryCylinderIndex;
    std::vector<ForceActuatorTableRow> Table;

    void load(const std::string &filename);

private:
    void loadForceActuatorTable(const std::string &filename);
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* FORCEACTUATORAPPLICATIONSETTINGS_H_ */
