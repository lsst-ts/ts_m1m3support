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

#ifndef GYROSETTINGS_H_
#define GYROSETTINGS_H_

#include <SAL_MTM1M3.h>
#include <M1M3SSPublisher.h>

#include <string>
#include <vector>

namespace LSST {
namespace M1M3 {
namespace SS {

struct GyroSettings : public MTM1M3_logevent_gyroSettingsC {
    std::vector<double> AxesMatrix;

    void load(const std::string &filename);

    void log() { M1M3SSPublisher::get().logGyroSettings(this); }
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* GYROSETTINGS_H_ */
