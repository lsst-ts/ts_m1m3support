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

#include <SAL_MTM1M3C.h>

#include "PreclippedForces.h"

using namespace LSST::M1M3::SS;

template <class T>
PreclippedForces<T>::PreclippedForces(std::function<void(T*)> send_function, float ignore_changes,
                                      std::chrono::milliseconds max_delay) {
    for (int i = 0; i < FA_Z_COUNT; i++) {
        this->zForces[i] = NAN;
        if (i < FA_Y_COUNT) {
            this->yForces[i] = NAN;
        }
        if (i < FA_X_COUNT) {
            this->xForces[i] = NAN;
        }
    }

    this->fz = NAN;
    this->fy = NAN;
    this->fx = NAN;
    this->mz = NAN;
    this->my = NAN;
    this->mx = NAN;
    this->forceMagnitude = NAN;

    _last_sent_event = *this;

    _send_function = send_function;
    _ignore_changes = ignore_changes;
    _max_delay = max_delay;
    _next_send = std::chrono::steady_clock::now() - _max_delay;

    _unsent_changes = false;
}

template <class T>
void PreclippedForces<T>::calculate_forces_and_moments() {
    auto f_m = ForceActuatorSettings::instance().calculateForcesAndMoments(this->xForces, this->yForces,
                                                                           this->zForces);

    this->fx = f_m.Fx;
    this->fy = f_m.Fy;
    this->fz = f_m.Fz;
    this->mx = f_m.Mx;
    this->my = f_m.My;
    this->mz = f_m.Mz;
    this->forceMagnitude = f_m.ForceMagnitude;
}

template <class T>
bool PreclippedForces<T>::check_changes() {
    auto now = std::chrono::steady_clock::now();

    bool change_detected = false;
    for (int i = 0; i < FA_COUNT && !change_detected; ++i) {
        change_detected |= (i < FA_X_COUNT && this->xForces[i] != this->_last_sent_event.xForces[i]) ||
                           (i < FA_Y_COUNT && this->yForces[i] != this->_last_sent_event.yForces[i]) ||
                           (this->zForces[i] != _last_sent_event.zForces[i]);
    }
    if (((change_detected || _unsent_changes) && now >= _next_send) ||
        fabs(_last_sent_event.forceMagnitude - this->forceMagnitude) > _ignore_changes) {
        _send_function(this);
        _last_sent_event = *this;
        _next_send = now + _max_delay;
        _unsent_changes = false;
    } else if (change_detected) {
        _unsent_changes = true;
    }
    return change_detected;
}

template class PreclippedForces<MTM1M3_logevent_preclippedAccelerationForcesC>;
template class PreclippedForces<MTM1M3_logevent_preclippedAzimuthForcesC>;
template class PreclippedForces<MTM1M3_logevent_preclippedBalanceForcesC>;
template class PreclippedForces<MTM1M3_logevent_preclippedElevationForcesC>;
template class PreclippedForces<MTM1M3_logevent_preclippedForcesC>;
template class PreclippedForces<MTM1M3_logevent_preclippedOffsetForcesC>;
template class PreclippedForces<MTM1M3_logevent_preclippedStaticForcesC>;
template class PreclippedForces<MTM1M3_logevent_preclippedThermalForcesC>;
template class PreclippedForces<MTM1M3_logevent_preclippedVelocityForcesC>;

template <class T>
PreclippedZForces<T>::PreclippedZForces(std::function<void(T*)> send_function, float ignore_changes,
                                        std::chrono::milliseconds max_delay) {
    for (int i = 0; i < FA_Z_COUNT; i++) {
        this->zForces[i] = NAN;
    }

    this->fz = NAN;
    this->my = NAN;
    this->mx = NAN;

    this->_last_sent_event = *this;

    this->_send_function = send_function;
    this->_ignore_changes = ignore_changes;
    this->_max_delay = max_delay;
    this->_next_send = std::chrono::steady_clock::now() - _max_delay;

    this->_unsent_changes = false;
}

template <class T>
void PreclippedZForces<T>::calculate_forces_and_moments() {
    auto f_m = ForceActuatorSettings::instance().calculateForcesAndMoments(this->zForces);

    this->fz = f_m.Fz;
    this->mx = f_m.Mx;
    this->my = f_m.My;
}

template <class T>
bool PreclippedZForces<T>::check_changes() {
    auto now = std::chrono::steady_clock::now();

    bool change_detected = false;
    for (int i = 0; i < FA_COUNT && !change_detected; ++i) {
        change_detected |= this->zForces[i] != this->_last_sent_event.zForces[i];
    }
    if (((change_detected || this->_unsent_changes) && now >= this->_next_send) ||
        fabs(this->_last_sent_event.fz - this->fz) > this->_ignore_changes) {
        this->_send_function(this);
        this->_last_sent_event = *this;
        this->_next_send = now + _max_delay;
        this->_unsent_changes = false;
    } else if (change_detected) {
        this->_unsent_changes = true;
    }
    return change_detected;
}

template class PreclippedZForces<MTM1M3_logevent_preclippedActiveOpticForcesC>;

template <class T>
PreclippedCylinderForces<T>::PreclippedCylinderForces(std::function<void(T*)> send_function,
                                                      float ignore_changes,
                                                      std::chrono::milliseconds max_delay) {
    this->primaryCylinderForces = std::vector<int>(FA_COUNT, 0);
    this->secondaryCylinderForces = std::vector<int>(FA_S_COUNT, 0);

    this->_last_sent_event = *this;

    this->_send_function = send_function;
    this->_ignore_changes = ignore_changes;
    this->_max_delay = max_delay;
    this->_next_send = std::chrono::steady_clock::now() - _max_delay;

    this->_unsent_changes = false;
}

template <class T>
bool PreclippedCylinderForces<T>::check_changes() {
    auto now = std::chrono::steady_clock::now();

    bool change_detected = false;
    for (int i = 0; i < FA_COUNT && !change_detected; ++i) {
        change_detected |=
                (i < FA_S_COUNT &&
                 this->secondaryCylinderForces[i] != this->_last_sent_event.secondaryCylinderForces[i]) ||
                (this->primaryCylinderForces[i] != this->_last_sent_event.primaryCylinderForces[i]);
    }
    if ((change_detected || this->_unsent_changes) && now >= this->_next_send) {
        this->_send_function(this);
        this->_last_sent_event = *this;
        this->_next_send = now + _max_delay;
        this->_unsent_changes = false;
    } else if (change_detected) {
        this->_unsent_changes = true;
    }
    return change_detected;
}

template class PreclippedCylinderForces<MTM1M3_logevent_preclippedCylinderForcesC>;
