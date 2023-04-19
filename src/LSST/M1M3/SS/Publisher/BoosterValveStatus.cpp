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

#include <BoosterValveStatus.h>
#include <M1M3SSPublisher.h>

using namespace LSST::M1M3::SS;

BoosterValveStatus::BoosterValveStatus(token) {
    slewFlag = false;
    followingErrorTrigerred = false;
    accelerometerTrigerred = false;

    _shouldSend = true;
}

void BoosterValveStatus::setSlewFlag(bool newSlewFlag) {
    if (slewFlag != newSlewFlag) {
        slewFlag = newSlewFlag;
        log(true);
    }
}

void BoosterValveStatus::log(bool force) {
    if (_shouldSend == false && force == false) {
        return;
    }

    M1M3SSPublisher::instance().logBoosterValveStatus(this);

    _shouldSend = false;
}
