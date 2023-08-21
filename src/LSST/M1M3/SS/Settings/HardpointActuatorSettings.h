/*
 * This file is part of LSST M1M3 support system package.
 *
 * Developed for the Vera C. Rubin Telescope and Site System.
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

#include <string>
#include <vector>

#include <yaml-cpp/yaml.h>

#include <SAL_MTM1M3.h>

#include <cRIO/Singleton.h>

#include <DataTypes.h>

namespace LSST {
namespace M1M3 {
namespace SS {

class HardpointActuatorSettings : public MTM1M3_logevent_hardpointActuatorSettingsC,
                                  public cRIO::Singleton<HardpointActuatorSettings> {
public:
    HardpointActuatorSettings(token);

    void load(YAML::Node doc);

    void log();

    /**
     * Returns zero offset for given hardpoint.
     *
     * @param hp Hardpoint index (0-5)
     *
     * @return hardpoint zero offset (HP<HP>EncoderOffset value)
     */
    int32_t getEncoderOffset(int hp) { return encoderOffset[hp]; }

    std::vector<double> HardpointDisplacementToMirrorPosition;
    std::vector<double> MirrorPositionToHardpointDisplacement;
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* HARDPOINTACTUATORSETTINGS_H_ */
