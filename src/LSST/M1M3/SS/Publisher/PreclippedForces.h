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
                     std::chrono::milliseconds max_delay);

    /**
     * Calculate total forces and moments. Sets f[xyz], m[xyz] and
     * forceMagnitude members to calculated total values.
     */
    void calculate_forces_and_moments();

    /**
     * Checks for changes in data. Call send method if the data shall be
     * send out. Call this once all new data are processed.
     *
     * @return True if data changed from last check_changes call. That doesn't
     * necessary mean data were send out.
     */
    bool check_changes();

protected:
    std::function<void(T*)> _send_function;

    T _last_sent_event;

    std::chrono::milliseconds _max_delay;
    std::chrono::time_point<std::chrono::steady_clock> _next_send;

    double _ignore_changes;
    bool _unsent_changes;
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
                      std::chrono::milliseconds max_delay);

    /**
     * Calculate total forces and moments. Sets fz and m[xy] members to
     * calculated total values.
     */
    void calculate_forces_and_moments();

    /**
     * Checks for changes in data. Call send method if the data shall be
     * send out. Call this once all new data are processed.
     *
     * @return True if data changed from last check_changes call. That doesn't
     * necessary mean data were send out.
     */
    bool check_changes();

protected:
    std::function<void(T*)> _send_function;

    T _last_sent_event;

    std::chrono::milliseconds _max_delay;
    std::chrono::time_point<std::chrono::steady_clock> _next_send;

    double _ignore_changes;
    bool _unsent_changes;
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
                             std::chrono::milliseconds max_delay);

    /**
     * Checks for changes in data. Call send method if the data shall be
     * send out. Call this once all new data are processed.
     *
     * @return True if data changed from last check_changes call. That doesn't
     * necessary mean data were send out.
     */
    bool check_changes();

protected:
    std::function<void(T*)> _send_function;

    T _last_sent_event;

    std::chrono::milliseconds _max_delay;
    std::chrono::time_point<std::chrono::steady_clock> _next_send;

    double _ignore_changes;
    bool _unsent_changes;
};

}  // namespace SS
}  // namespace M1M3
}  // namespace LSST

#endif  //! __PreclippedForces__h_
