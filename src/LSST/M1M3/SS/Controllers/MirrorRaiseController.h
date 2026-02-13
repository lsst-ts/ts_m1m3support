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

#ifndef MIRRORRAISECONTROLLER_H_
#define MIRRORRAISECONTROLLER_H_

#include <ForceController.h>
#include <PositionController.h>
#include <PowerController.h>
#include <SafetyController.h>

namespace LSST {
namespace M1M3 {
namespace SS {

/**
 * Keeps track of the HP in range after raising had finished. This is needed to
 * deliver mirror without significant HP force oscillations, causing failure
 * of balance force corrections executed right after mirror finishes raising.
 */
class HpInRangeCounter {
public:
    HpInRangeCounter();

    void reset();

    bool check(float force);

    bool timeouted();

private:
    float _current_middlepoint;
    int _counter;
    int _timeout_counter;
};

/**
 * Controls mirror raising operation. The operations are executed as command.
 * Command transition system to RaisingState or RaisingEngineeringState. The
 * newly set system state calls in a loop method the MirrorRaiseController to
 * perform operations
 * requested.
 *
 * Hardpoints (HPs) are put into Chasing mode as raises starts. In Chasing
 * state (controlled through PositionController), HP is following force seen on
 * its top end to remain close to 0 force.
 *
 * @see PositionController
 * @see MirrorLowerController
 */
class MirrorRaiseController {
public:
    /**
     * Construct mirror controller.
     *
     * @param positionController
     * @param forceController
     * @param safetyController
     * @param powerController
     */
    MirrorRaiseController(PositionController* positionController, ForceController* forceController,
                          SafetyController* safetyController, PowerController* powerController);

    void reset();

    /**
     * Starts mirror raising. Should be called once
     * @param bypassMoveToReference
     */
    void start(bool bypassMoveToReference);
    void runLoop();
    bool checkComplete();
    void complete();
    bool checkTimeout();
    void timeout();

    void pauseM1M3Raising();
    void resumeM1M3Raising();

private:
    bool _check_hp_ready();

    PositionController* _positionController;
    ForceController* _forceController;
    SafetyController* _safetyController;
    PowerController* _powerController;

    // time spend in paused state doesn't count to timeout
    double _cachedStartTime;
    double _remaininingTimedout;

    bool _bypassMoveToReference;

    bool _last_force_filled;
    bool _last_position_completed;
    bool _last_hp_forces_minimal;
    bool _airPressureWaitReported;
    bool _raisePauseReported;

    bool _raisingPaused;

    HpInRangeCounter _hp_in_range[HP_COUNT];
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* MIRRORRAISECONTROLLER_H_ */
