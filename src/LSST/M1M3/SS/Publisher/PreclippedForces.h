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
#ifndef __PreclippedForces__h_
#define __PreclippedForces__h_

#include <cmath>
#include <chrono>
#include <memory>

#include "ForceActuatorApplicationSettings.h"
#include "ForceActuatorSettings.h"

namespace LSST {
namespace M1M3 {
namespace SS {

/**
 * Template looking for changes in preclipped forces. It sends forces out only
 * at predefined intervals or when excessive changes were recorded, so the SAL
 * is not spoiled by many similar Events.
 *
 *
 * When implemented, send method shall be provided. This takes data as
 * parameter, and send those out.
 *
 */
template <class T>
class PreclippedForces : public T {
public:
    /**
     * Construct object for processing preclipped forces.
     *
     * @param ignore_changes When change from the last send forceMagnitude
     * value is above that value, the message will be send out, ignoring
     * max_delay parameter. Value is in N.
     *
     * @param max_delay Maximum delay between sucessive call to send. If
     * underlying values changes, but the forceMagnitude change is below the
     * limit passed in ignore_changes, and it is more than this time limit from
     * the last time the data were send out, the send method will be called.
     */
    PreclippedForces(std::function<void(T*)> send_function, float ignore_changes,
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

        this->_last_send_event = *this;
        this->_previous_event = *this;

        this->_send_function = send_function;
        this->_ignore_changes = ignore_changes;
        this->_max_delay = max_delay;
        this->_next_send = std::chrono::steady_clock::now() - _max_delay;

        this->_unsend_changes = false;
    }

    /**
     * Calculate total forces and moments. Sets f[xyz], m[xyz] and
     * forceMagnitude members to calculated total values.
     */
    void calculate_forces_and_moments() {
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

    /**
     * Checks for changes in data. Call send method if the data shall be
     * send out. Call this once all new data are processed.
     *
     * @return True if data changed from last check_changes call. That doesn't
     * necessary mean data were send out.
     */
    bool check_changes() {
        auto now = std::chrono::steady_clock::now();

        bool change_detected = false;
        for (int i = 0; i < FA_COUNT && !change_detected; ++i) {
            change_detected |= (i < FA_X_COUNT && this->xForces[i] != this->_previous_event.xForces[i]) ||
                               (i < FA_Y_COUNT && this->yForces[i] != this->_previous_event.yForces[i]) ||
                               (this->zForces[i] != this->_previous_event.zForces[i]);
        }
        if (((change_detected || this->_unsend_changes) && now >= this->_next_send) ||
            fabs(this->_last_send_event.forceMagnitude - this->forceMagnitude) > this->_ignore_changes) {
            this->_send_function(this);
            this->_last_send_event = *this;
            this->_next_send = now + _max_delay;
            this->_unsend_changes = false;
        } else if (change_detected) {
            this->_unsend_changes = true;
        }
        _previous_event = *this;
        return change_detected;
    }

protected:
    std::function<void(T*)> _send_function;

    T _last_send_event;
    T _previous_event;

    std::chrono::milliseconds _max_delay;
    std::chrono::time_point<std::chrono::steady_clock> _next_send;

    double _ignore_changes;
    bool _unsend_changes;
};

template <class T>
class PreclippedZForces : public T {
public:
    /**
     * Construct object for processing preclipped Z forces.
     *
     * @param ignore_changes When change from the last send fz
     * value is above that value, the message will be send out, ignoring
     * max_delay parameter. Value is in N.
     *
     * @param max_delay Maximum delay between sucessive call to send. If
     * underlying values changes, but the fz change is below the
     * limit passed in ignore_changes, and it is more than this time limit from
     * the last time the data were send out, the send method will be called.
     *
     * @see PreclippedForces
     */
    PreclippedZForces(std::function<void(T*)> send_function, float ignore_changes,
                      std::chrono::milliseconds max_delay) {
        for (int i = 0; i < FA_Z_COUNT; i++) {
            this->zForces[i] = NAN;
        }

        this->fz = NAN;
        this->my = NAN;
        this->mx = NAN;

        this->_last_send_event = *this;
        this->_previous_event = *this;

        this->_send_function = send_function;
        this->_ignore_changes = ignore_changes;
        this->_max_delay = max_delay;
        this->_next_send = std::chrono::steady_clock::now() - _max_delay;

        this->_unsend_changes = false;
    }

    /**
     * Calculate total forces and moments. Sets fz and m[xy] members to
     * calculated total values.
     */
    void calculate_forces_and_moments() {
        auto f_m = ForceActuatorSettings::instance().calculateForcesAndMoments(this->zForces);

        this->fz = f_m.Fz;
        this->mx = f_m.Mx;
        this->my = f_m.My;
    }

    /**
     * Checks for changes in data. Call send method if the data shall be
     * send out. Call this once all new data are processed.
     *
     * @return True if data changed from last check_changes call. That doesn't
     * necessary mean data were send out.
     */
    bool check_changes() {
        auto now = std::chrono::steady_clock::now();

        bool change_detected = false;
        for (int i = 0; i < FA_COUNT && !change_detected; ++i) {
            change_detected |= this->zForces[i] != this->_previous_event.zForces[i];
        }
        if (((change_detected || this->_unsend_changes) && now >= this->_next_send) ||
            fabs(this->_last_send_event.fz - this->fz) > this->_ignore_changes) {
            this->_send_function(this);
            this->_last_send_event = *this;
            this->_next_send = now + _max_delay;
            this->_unsend_changes = false;
        } else if (change_detected) {
            this->_unsend_changes = true;
        }
        _previous_event = *this;
        return change_detected;
    }

protected:
    std::function<void(T*)> _send_function;

    T _last_send_event;
    T _previous_event;

    std::chrono::milliseconds _max_delay;
    std::chrono::time_point<std::chrono::steady_clock> _next_send;

    double _ignore_changes;
    bool _unsend_changes;
};

/**
 * Template looking for changes in preclipped cylinder forces. It sends forces
 * out only at predefined intervals or when excessive changes were recorded, so
 * the SAL is not spoiled by many similar Events.
 *
 * When implemented, send method shall be provided. This takes data as
 * parameter, and send those out.
 *
 */
template <typename T>
class PreclippedCylinderForces : public T {
public:
    /**
     * Construct object for processing preclipped cylinder forces.
     *
     * @param ignore_changes When change from the last send forceMagnitude
     * value is above that value, the message will be send out, ignoring
     * max_delay parameter. Value is in N.
     *
     * @param max_delay Maximum delay between sucessive call to send. If
     * underlying values changes, but the forceMagnitude change is below the
     * limit passed in ignore_changes, and it is more than this time limit from
     * the last time the data were send out, the send method will be called.
     */
    PreclippedCylinderForces(std::function<void(T*)> send_function, float ignore_changes,
                             std::chrono::milliseconds max_delay) {
        this->primaryCylinderForces = std::vector<int>(FA_COUNT, 0);
        this->secondaryCylinderForces = std::vector<int>(FA_S_COUNT, 0);

        this->_last_send_event = *this;
        this->_previous_event = *this;

        this->_send_function = send_function;
        this->_ignore_changes = ignore_changes;
        this->_max_delay = max_delay;
        this->_next_send = std::chrono::steady_clock::now() - _max_delay;

        this->_unsend_changes = false;
    }

    /**
     * Checks for changes in data. Call send method if the data shall be
     * send out. Call this once all new data are processed.
     *
     * @return True if data changed from last check_changes call. That doesn't
     * necessary mean data were send out.
     */
    bool check_changes() {
        auto now = std::chrono::steady_clock::now();

        bool change_detected = false;
        for (int i = 0; i < FA_COUNT && !change_detected; ++i) {
            change_detected |=
                    (i < FA_S_COUNT &&
                     this->secondaryCylinderForces[i] != this->_previous_event.secondaryCylinderForces[i]) ||
                    (this->primaryCylinderForces[i] != this->_previous_event.primaryCylinderForces[i]);
        }
        if ((change_detected || this->_unsend_changes) && now >= this->_next_send) {
            this->_send_function(this);
            this->_last_send_event = *this;
            this->_next_send = now + _max_delay;
            this->_unsend_changes = false;
        } else if (change_detected) {
            this->_unsend_changes = true;
        }
        _previous_event = *this;
        return change_detected;
    }

protected:
    std::function<void(T*)> _send_function;

    T _last_send_event;
    T _previous_event;

    std::chrono::milliseconds _max_delay;
    std::chrono::time_point<std::chrono::steady_clock> _next_send;

    double _ignore_changes;
    bool _unsend_changes;
};

}  // namespace SS
}  // namespace M1M3
}  // namespace LSST

#endif  //! __PreclippedForces__h_
