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

#ifndef HARDPOINTACTUATORSETTINGS_H_
#define HARDPOINTACTUATORSETTINGS_H_

#include <DataTypes.h>
#include <string>
#include <vector>

namespace LSST {
namespace M1M3 {
namespace SS {

class HardpointActuatorSettings {
public:
    std::vector<double> HardpointDisplacementToMirrorPosition;
    std::vector<double> MirrorPositionToHardpointDisplacement;
    double MicrometersPerStep;
    double MicrometersPerEncoder;
    int32_t HP1EncoderOffset;
    int32_t HP2EncoderOffset;
    int32_t HP3EncoderOffset;
    int32_t HP4EncoderOffset;
    int32_t HP5EncoderOffset;
    int32_t HP6EncoderOffset;
    float HardpointMeasuredForceFaultHigh;
    float HardpointMeasuredForceFaultLow;
    float HardpointMeasuredForceFSBWarningHigh;
    float HardpointMeasuredForceFSBWarningLow;
    float HardpointMeasuredForceWarningHigh;
    float HardpointMeasuredForceWarningLow;
    float AirPressureWarningHigh;
    float AirPressureWarningLow;

    void load(const std::string &filename);
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* HARDPOINTACTUATORSETTINGS_H_ */
