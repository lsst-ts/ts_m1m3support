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

#include <spdlog/spdlog.h>
#include <fmt/ranges.h>

#include "AirSupplyStatus.h"
#include "ForceController.h"
#include "HardpointActuatorWarning.h"
#include "M1M3SSPublisher.h"
#include "MirrorRaiseController.h"
#include "PositionController.h"
#include "PowerController.h"
#include "RaisingLoweringInfo.h"
#include "SafetyController.h"

using namespace LSST::M1M3::SS;

HpInRangeCounter::HpInRangeCounter() { reset(); }

void HpInRangeCounter::reset() {
    _current_middlepoint = NAN;
    _counter = -1;
    _timeout_counter = -1;
}

bool HpInRangeCounter::check(float force) {
    auto& hp_actuator_settings = HardpointActuatorSettings::instance();
    if (_counter < 0) {
        _counter = 0;
        _timeout_counter = 0;
        _current_middlepoint = force;
        return false;
    }

    if (_timeout_counter > hp_actuator_settings.inRangeAfterRaiseTimeoutLoops) {
        return true;
    }
    _timeout_counter++;

    if (abs(force - _current_middlepoint) > hp_actuator_settings.inRangeAfterRaiseForce) {
        _counter = 0;
        _current_middlepoint = (force + _current_middlepoint) / 2.0;
        return false;
    }
    if (_counter > hp_actuator_settings.inRangeAfterRaiseLoops) {
        return true;
    }
    _counter++;
    return false;
}

bool HpInRangeCounter::timeouted() {
    return _timeout_counter > HardpointActuatorSettings::instance().inRangeAfterRaiseTimeoutLoops;
}

MirrorRaiseController::MirrorRaiseController(PositionController* positionController,
                                             ForceController* forceController,
                                             SafetyController* safetyController,
                                             PowerController* powerController) {
    SPDLOG_DEBUG("MirrorRaiseController: MirrorRaiseController()");
    _positionController = positionController;
    _forceController = forceController;
    _safetyController = safetyController;
    _powerController = powerController;
    _cachedStartTime = 0;
    _remaininingTimedout = 0;
    RaisingLoweringInfo::instance().setTimeTimeout(0);
    _bypassMoveToReference = false;

    reset();
}

void MirrorRaiseController::reset() {
    _last_force_filled = false;
    _last_position_completed = false;
    _last_hp_forces_minimal = false;
    _raisePauseReported = false;
    _raisingPaused = false;
}

void MirrorRaiseController::start(bool bypassMoveToReference) {
    SPDLOG_INFO("MirrorRaiseController: start({})", bypassMoveToReference);
    _bypassMoveToReference = bypassMoveToReference;

    reset();
    for (int i = 0; i < HP_COUNT; i++) {
        _hp_in_range[i].reset();
    }

    _safetyController->raiseOperationTimeout(false);
    _positionController->startRaise();
    _forceController->zeroAccelerationForces();
    _forceController->zeroActiveOpticForces();
    _forceController->zeroAzimuthForces();
    _forceController->zeroBalanceForces();
    _forceController->applyElevationForces();
    _forceController->zeroOffsetForces();
    _forceController->zeroStaticForces();
    _forceController->zeroThermalForces();
    _forceController->zeroVelocityForces();
    RaisingLoweringInfo::instance().zeroSupportPercentage();
    _cachedStartTime = M1M3SSPublisher::instance().getTimestamp();
    _remaininingTimedout = _positionController->getRaiseTimeout();
    RaisingLoweringInfo::instance().setTimeTimeout(_cachedStartTime + _remaininingTimedout);

    RaisingLoweringInfo::instance().setWaitAirPressure(false);

    if (AirSupplyStatus::instance().airValveClosed == true) {
        SPDLOG_WARN(
                "Air valve is closed and the mirror was commanded to raise. "
                "Please check that the compressed "
                "air is provided to the M1M3 support system");
    }
}

void MirrorRaiseController::runLoop() {
    if (_raisingPaused == true) {
        return;
    }
    SPDLOG_TRACE("MirrorRaiseController: runLoop() {}",
                 RaisingLoweringInfo::instance().weightSupportedPercent);
    auto raiseInfo = &RaisingLoweringInfo::instance();
    if (!raiseInfo->supportPercentageFilled()) {
        auto hpWarning = &HardpointActuatorWarning::instance();
        // Wait for pressure to raise after valve opening
        if (raiseInfo->supportPercentageZeroed() && hpWarning->anyLowAirPressureFault) {
            raiseInfo->setWaitAirPressure(true);
            // hpRaiseLowerForcesInTolerance needs to be called to stop encoder
            // movement if on tension limit
            _positionController->hpRaiseLowerForcesInTolerance(true);
            return;
        }
        raiseInfo->setWaitAirPressure(false);
        // We are still in the process of transferring the support force from the
        // static supports to the force actuators
        if (_positionController->hpRaiseLowerForcesInTolerance(true) &&
            _forceController->faRaiseFollowingErrorInTolerance()) {
            // The forces on the hardpoints are within tolerance and
            // the force actuators are following their setpoints, we can continue to
            // transfer the support force from the static supports to the force
            // actuators
            RaisingLoweringInfo::instance().incSupportPercentage();
            if (_raisePauseReported == true) {
                _raisePauseReported = false;
                SPDLOG_INFO("Raising resumed");
            }
            if (RaisingLoweringInfo::instance().weightSupportedPercent >
                ForceActuatorSettings::instance().enableStaticForcesSupportedPercentage) {
                _forceController->applyStaticForces();
            }
            if (RaisingLoweringInfo::instance().supportPercentageFilled()) {
                // All of the support force has been transfered from the static supports
                // to the force actuators, stop the hardpoints from chasing and start
                // moving to the reference position
                _positionController->disableChaseAll();
                if (!_bypassMoveToReference) {
                    _positionController->moveToReferencePosition();
                }
            }
        } else {
            if (_raisePauseReported == false) {
                _raisePauseReported = true;
                SPDLOG_WARN("Raising paused - waiting for hardpoints movements");
            }
        }
        raiseInfo->sendUpdates();
    }
}

bool MirrorRaiseController::checkComplete() {
    bool force_filled = RaisingLoweringInfo::instance().supportPercentageFilled();
    bool position_completed = _positionController->motionComplete();

    bool hp_force_minimal = false;
    if (position_completed == true) {
        hp_force_minimal = _check_hp_ready();
        if (_last_hp_forces_minimal != hp_force_minimal) {
            SPDLOG_INFO("HP Forces {} within limit for activating mirror balance forces.",
                        hp_force_minimal ? "are" : "are not");
        }
        _last_hp_forces_minimal = hp_force_minimal;
    }

    if (_last_force_filled != force_filled) {
        SPDLOG_INFO(
                "MirrorRaiseController::checkComplete force controller support "
                "percentage {}",
                force_filled ? "filled" : "not filled");
    }
    if (_last_position_completed != position_completed) {
        SPDLOG_INFO("MirrorRaiseController::checkComplete position controller moves {}",
                    position_completed ? "completed" : "not completed");
    }
    _last_force_filled = force_filled;
    _last_position_completed = position_completed;

    return force_filled && position_completed && hp_force_minimal;
}

void MirrorRaiseController::complete() {
    SPDLOG_INFO("MirrorRaiseController: complete()");
    // Transition to the end state (active or active engineering) if all of the
    // support force has been transfered from the static supports to the force
    // actuators and all hardpoints have completed their commanded motions
    _forceController->zeroAccelerationForces();
    _forceController->zeroActiveOpticForces();
    _forceController->applyAzimuthForces();
    _forceController->zeroBalanceForces();
    _forceController->applyElevationForces();
    _forceController->zeroOffsetForces();
    _forceController->applyStaticForces();
    _forceController->applyThermalForces();
    _forceController->zeroVelocityForces();
    if (ForceActuatorSettings::instance().hardpointBalanceForcesOnInActiveState) {
        std::vector<int> hp_timeouted;
        for (int i = 0; i < HP_COUNT; i++) {
            if (_hp_in_range[i].timeouted()) {
                hp_timeouted.push_back(i + 1);
            }
        }
        if (hp_timeouted.size() > 0) {
            SPDLOG_ERROR("HP(s) {} timeouted after raising, balance corrections not enabled.",
                         fmt::join(hp_timeouted, " "));
        } else {
            _forceController->applyBalanceForces();
        }
    }
    RaisingLoweringInfo::instance().fillSupportPercentage();
}

bool MirrorRaiseController::checkTimeout() {
    if (isnan(RaisingLoweringInfo::instance().timeTimeout)) {
        return false;
    }
    return M1M3SSPublisher::instance().getTimestamp() >= RaisingLoweringInfo::instance().timeTimeout;
}

void MirrorRaiseController::timeout() {
    SPDLOG_ERROR("MirrorRaiseController: timeout()");
    _safetyController->raiseOperationTimeout(true);
}

void MirrorRaiseController::pauseM1M3Raising() {
    _raisingPaused = true;
    _remaininingTimedout -= (M1M3SSPublisher::instance().getTimestamp() - _cachedStartTime);
    RaisingLoweringInfo::instance().setTimeTimeout(NAN);
}

void MirrorRaiseController::resumeM1M3Raising() {
    _raisingPaused = false;
    RaisingLoweringInfo::instance().setTimeTimeout(M1M3SSPublisher::instance().getTimestamp() +
                                                   _remaininingTimedout);
}

bool MirrorRaiseController::_check_hp_ready() {
    auto _hardpoint_actuator_data = M1M3SSPublisher::instance().getHardpointActuatorData();
    for (int i = 0; i < HP_COUNT; i++) {
        if (_hp_in_range[i].check(_hardpoint_actuator_data->measuredForce[i]) == false) {
            return false;
        }
    }
    return true;
}
