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

#ifndef DISPLACEMENTSENSORSETTINGS_H_
#define DISPLACEMENTSENSORSETTINGS_H_

#include <DataTypes.h>
#include <string>
#include <vector>

namespace LSST {
namespace M1M3 {
namespace SS {

class DisplacementSensorSettings {
public:
    std::vector<double> ConverterMatrix;
    int32_t N1Port;
    int32_t N2Port;
    int32_t N3Port;
    int32_t N4Port;
    int32_t N5Port;
    int32_t N6Port;
    int32_t N7Port;
    int32_t N8Port;
    double N1Offset;
    double N2Offset;
    double N3Offset;
    double N4Offset;
    double N5Offset;
    double N6Offset;
    double N7Offset;
    double N8Offset;
    double XPositionOffset;
    double YPositionOffset;
    double ZPositionOffset;
    double XRotationOffset;
    double YRotationOffset;
    double ZRotationOffset;

    void load(const std::string &filename);
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* DISPLACEMENTSENSORSETTINGS_H_ */
