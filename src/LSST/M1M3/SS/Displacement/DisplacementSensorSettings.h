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

#ifndef DISPLACEMENTSENSORSETTINGS_H_
#define DISPLACEMENTSENSORSETTINGS_H_

#include <string>
#include <vector>

#include <yaml-cpp/yaml.h>

#include <SAL_MTM1M3.h>

#include <cRIO/Singleton.h>

#include <cRIO/DataTypes.h>
#include <M1M3SSPublisher.h>

namespace LSST {
namespace M1M3 {
namespace SS {

class DisplacementSensorSettings : public MTM1M3_logevent_displacementSensorSettingsC,
                                   public cRIO::Singleton<DisplacementSensorSettings> {
public:
    std::vector<double> ConverterMatrix;
    std::vector<int32_t> NPorts;
    std::vector<double> NOffsets;

    DisplacementSensorSettings(token) : NPorts(8), NOffsets(8) {}

    void load(YAML::Node doc);

    void log() { M1M3SSPublisher::instance().logDisplacementSensorSettings(this); }
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* DISPLACEMENTSENSORSETTINGS_H_ */
