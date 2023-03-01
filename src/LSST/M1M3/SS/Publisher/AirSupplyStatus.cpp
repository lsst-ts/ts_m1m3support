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

#include <M1M3SSPublisher.h>

#include "AirSupplyStatus.h"
#include <SettingReader.h>

using namespace LSST::M1M3::SS;

AirSupplyStatus::AirSupplyStatus(token) {
    _updated = false;  // send update only after DigitalInputOutput is read

    timestamp = 0;

    airCommandedOn = false;
    airCommandOutputOn = false;

    airValveClosed = false;
    airValveOpened = false;

    _airToggledTime = std::chrono::steady_clock::now();
}

void AirSupplyStatus::send() {
    if (_updated == false) {
        return;
    }

    M1M3SSPublisher::instance().logAirSupplyStatus(this);
    _updated = false;
}

void AirSupplyStatus::setAirCommanded(bool _airCommandedOn) {
    if (airCommandedOn != _airCommandedOn) {
        _updated = true;
        airCommandedOn = _airCommandedOn;
        _airToggledTime = std::chrono::steady_clock::now();
    }
}

bool AirSupplyStatus::setOutputs(double _timestamp, bool _airCommandOutputOn) {
    if (_airCommandOutputOn != airCommandOutputOn) {
        _updated = true;
        timestamp = _timestamp;
        airCommandOutputOn = _airCommandOutputOn;
    }
    return airCommandOutputOn != airCommandedOn;
}

bool AirSupplyStatus::setInputs(double _timestamp, bool _airValveClosed, bool _airValveOpened) {
    std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();

    if ((airValveClosed != _airValveClosed) || (airValveOpened != _airValveOpened)) {
        _updated = true;
        timestamp = _timestamp;
        airValveClosed = _airValveClosed;
        airValveOpened = _airValveOpened;
    }

    // grace period for valve opening/closing
    if ((now - _airToggledTime) < std::chrono::seconds(SettingReader::instance()
                                                               .getSafetyControllerSettings()
                                                               ->AirController.ValveTransitionTimeout)) {
        return false;
    }

    return (airCommandedOn == true && (airValveOpened == false || airValveClosed == true)) ||
           (airCommandedOn == false && (airValveOpened == true || airValveClosed == false));
}
