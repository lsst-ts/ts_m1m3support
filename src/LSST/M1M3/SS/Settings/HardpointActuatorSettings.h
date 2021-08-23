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

#include <SAL_MTM1M3.h>

#include <DataTypes.h>
#include <string>
#include <vector>

namespace LSST {
namespace M1M3 {
namespace SS {

class HardpointActuatorSettings : public MTM1M3_logevent_hardpointActuatorSettingsC {
public:
    std::vector<double> HardpointDisplacementToMirrorPosition;
    std::vector<double> MirrorPositionToHardpointDisplacement;

    HardpointActuatorSettings();

    void load(const std::string &filename);

    void log();

    /**
     * Returns zero offset for given hardpoint.
     *
     * @param hp Hardpoint index (0-5)
     *
     * @return hardpoint zero offset (HP<HP>EncoderOffset value)
     */
    int32_t getEncoderOffset(int hp) { return _encoderOffset[hp]; }

private:
    int32_t _encoderOffset[HP_COUNT];
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* HARDPOINTACTUATORSETTINGS_H_ */
