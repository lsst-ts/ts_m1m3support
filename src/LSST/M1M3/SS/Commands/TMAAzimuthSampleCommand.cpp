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

#include <Context.h>
#include <LimitLog.h>
#include <SettingReader.h>
#include <TMAAzimuthSampleCommand.h>
#include <cstring>

using namespace LSST::M1M3::SS;

TMAAzimuthSampleCommand::TMAAzimuthSampleCommand(MTMount_azimuthC *data) : Command(-1) {
    memcpy(&_data, data, sizeof(MTMount_azimuthC));
}

void TMAAzimuthSampleCommand::execute() {
    double diff = _data.timestamp - M1M3SSPublisher::instance().getTimestamp();
    double limit = SettingReader::instance().getSafetyControllerSettings()->TMA.AzimuthTimeout;
    if (limit > 0 && fabs(diff) > limit) {
        using namespace std::chrono_literals;
        TG_LOG_ERROR(2s,
                     "Received azimuth timestamp ({2:.4f}) deviates by more than "
                     "{0:.3f}s: {1:.3f}",
                     limit, diff, _data.timestamp);
        return;
    }
    Context::get().storeTMAAzimuthSample(this);
}
