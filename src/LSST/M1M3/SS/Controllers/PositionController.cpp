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

#include <algorithm>

#include <spdlog/spdlog.h>
#include <stdlib.h>

#include <SAL_MTM1M3C.h>

#include <HardpointActuatorSettings.h>
#include <HardpointActuatorWarning.h>
#include <M1M3SSPublisher.h>
#include <Model.h>
#include <PositionController.h>
#include <PositionControllerSettings.h>
#include <RaisingLoweringInfo.h>
#include <Range.h>
#include <TMA.h>

using namespace MTM1M3;
using namespace LSST::M1M3::SS;

/**
 * Can hardpoint see extra tension during raise operation?
 *
 * @param hp hardpoint (0-based) index
 *
 * @return true if the hardpoint can see excess tension as raised/lowered
 */
bool hp_can_see_tension(int hp) { return hp == 1 || hp == 4; }

/**
 * Can hardpoint see extra pression during raise?
 *
 * @param hp hardpoint (0-based) index
 *
 * @return true if the hardpoint compression excess shall be ignored till
 * enough of the mirror mass is lifted.
 */
bool hp_can_see_compression(int hp) { return hp == 0 || hp == 2 || hp == 3; }

PositionController::PositionController() {
    SPDLOG_DEBUG("PositionController: PositionController()");
    _safety_controller = Model::instance().getSafetyController();
    _position_controller_settings = &PositionControllerSettings::instance();
    _hardpoint_actuator_settings = &HardpointActuatorSettings::instance();
    _hardpoint_actuator_data = M1M3SSPublisher::instance().getHardpointActuatorData();
    _hardpointActuatorState = M1M3SSPublisher::instance().getEventHardpointActuatorState();
    _hardpointInfo = M1M3SSPublisher::instance().getEventHardpointActuatorInfo();
    _hardpointActuatorState->timestamp = M1M3SSPublisher::instance().getTimestamp();
    for (int i = 0; i < HP_COUNT; i++) {
        _hardpointActuatorState->motionState[i] = MTM1M3_shared_HardpointActuatorMotionState_Standby;
        _hardpoint_actuator_data->stepsQueued[i] = 0;
        _hardpoint_actuator_data->stepsCommanded[i] = 0;
        _scaledMaxStepsPerLoop[i] = _position_controller_settings->maxStepsPerLoop;
        _targetEncoderValues[i] = 0;
        _stableEncoderCount[i] = 0;
        _unstableEncoderCount[i] = 0;
    }
    _reset_wait_compression_tension();

    M1M3SSPublisher::instance().logHardpointActuatorState();
}

bool PositionController::enableChaseAll() {
    SPDLOG_INFO("PositionController: enableChaseAll()");
    if (_hardpointActuatorState->motionState[0] != MTM1M3_shared_HardpointActuatorMotionState_Standby ||
        _hardpointActuatorState->motionState[1] != MTM1M3_shared_HardpointActuatorMotionState_Standby ||
        _hardpointActuatorState->motionState[2] != MTM1M3_shared_HardpointActuatorMotionState_Standby ||
        _hardpointActuatorState->motionState[3] != MTM1M3_shared_HardpointActuatorMotionState_Standby ||
        _hardpointActuatorState->motionState[4] != MTM1M3_shared_HardpointActuatorMotionState_Standby ||
        _hardpointActuatorState->motionState[5] != MTM1M3_shared_HardpointActuatorMotionState_Standby) {
        return false;
    }
    _hardpointActuatorState->timestamp = M1M3SSPublisher::instance().getTimestamp();
    for (int i = 0; i < HP_COUNT; i++) {
        _hardpointActuatorState->motionState[i] = MTM1M3_shared_HardpointActuatorMotionState_Chasing;
        _last_encoder_count[i] = _hardpoint_actuator_data->encoder[i];
    }
    M1M3SSPublisher::instance().tryLogHardpointActuatorState();
    return true;
}

void PositionController::disableChaseAll() {
    SPDLOG_INFO("PositionController: disableChaseAll()");
    _hardpointActuatorState->timestamp = M1M3SSPublisher::instance().getTimestamp();
    for (int i = 0; i < HP_COUNT; i++) {
        _hardpointActuatorState->motionState[i] = MTM1M3_shared_HardpointActuatorMotionState_Standby;
    }
    M1M3SSPublisher::instance().tryLogHardpointActuatorState();
}

void PositionController::startRaise() {
    stopMotion();
    enableChaseAll();
    _reset_wait_compression_tension();
}

void PositionController::startLower() {
    stopMotion();
    _reset_wait_compression_tension();
}

class PositionLimitTrigger : public LimitTrigger<float, float, float, wait_hardpoint_t, wait_hardpoint_t> {
public:
    PositionLimitTrigger(int hp) {
        _hp = hp;
        _counter = 0;
    }

protected:
    bool trigger() override {
        _counter++;
        // problems are reported  when accumulated counter reaches those levels
        static int levels[4] = {1, 200, 500, 10000};
        for (int i = 0; i < 4; i++)
            if (levels[i] == _counter) return true;
        return ((_counter % levels[3]) == 0);
    }

    void execute(float low_limit, float high_limit, float measured, wait_hardpoint_t wait_tension,
                 wait_hardpoint_t wait_compression) override {
        if (wait_tension == WAITING || wait_compression == WAITING) {
            _counter = 0;
        } else {
            SPDLOG_WARN(
                    "Violated hardpoint {} measured force {} ({}th occurence), "
                    "limit {} to {}",
                    _hp, measured, _counter, low_limit, high_limit);
        }
    }

    void reset() override {
        if (_counter > 0) {
            SPDLOG_INFO("Hardpoint {} back nominal after {} failures", _hp, _counter);
            _counter = 0;
        }
    }

private:
    int _hp;
    int _counter;
};

bool PositionController::hpRaiseLowerForcesInTolerance(bool raise) {
    SPDLOG_TRACE("PositionController: forcesInTolerance({})", raise);
    bool ret = true;
    auto raiseLowerInfo = &RaisingLoweringInfo::instance();

    static PositionLimitTrigger triggers[6] = {PositionLimitTrigger(1), PositionLimitTrigger(2),
                                               PositionLimitTrigger(3), PositionLimitTrigger(4),
                                               PositionLimitTrigger(5), PositionLimitTrigger(6)};

    float base_low_limit = _hardpoint_actuator_settings->hardpointBreakawayFaultLow;
    float base_high_limit = _hardpoint_actuator_settings->hardpointBreakawayFaultHigh;
    if (raise) {
        base_low_limit = _position_controller_settings->raiseHPForceLimitLow;
        base_high_limit = _position_controller_settings->raiseHPForceLimitHigh;
    } else {
        base_low_limit = _position_controller_settings->lowerHPForceLimitLow;
        base_high_limit = _position_controller_settings->lowerHPForceLimitHigh;
    }

    for (int hp = 0; hp < HP_COUNT; hp++) {
        bool in_range = false;
        float high_limit = base_high_limit;
        float low_limit = base_low_limit;
        switch (_hardpointActuatorState->motionState[hp]) {
            // don't move into tension when the HP is already in tension
            case MTM1M3_shared_HardpointActuatorMotionState_WaitingTension:
                low_limit = base_low_limit / 2.0;
                break;
            // don't move into compression when the HP is already in compression
            case MTM1M3_shared_HardpointActuatorMotionState_WaitingCompression:
                high_limit = (base_low_limit + base_high_limit) / 2.0;
                break;
            default:
                break;
        }

        auto measured_force = _hardpoint_actuator_data->measuredForce[hp];

        in_range = Range::InRange(low_limit, high_limit, measured_force);

        raiseLowerInfo->setHPWait(hp, !in_range);

        // tread HP 2 and 5 (index 1 and 4) differently when raising/lowering below some elevation
        if ((TMA::instance().getElevation() <
             _hardpoint_actuator_settings->ignoreTensionRaisingLoweringElevation) &&
            hp_can_see_tension(hp)) {
            in_range = _hp_raise_lower_forces_in_tolerance_tension(hp, raise, in_range, measured_force,
                                                                   low_limit);
        } else if ((TMA::instance().getElevation() <
                    _hardpoint_actuator_settings->ignoreCompressionRaisingLoweringElevation) &&
                   hp_can_see_compression(hp)) {
            in_range = _hp_raise_lower_forces_in_tolerance_compression(hp, raise, in_range, measured_force,
                                                                       high_limit);
        }

        // report HP range violations
        if (in_range == false) {
            triggers[hp].check(low_limit, high_limit, measured_force, _wait_tension[hp],
                               _wait_compression[hp]);
        }

        ret &= in_range;

        checkLimits(hp);
    }

    M1M3SSPublisher::instance().tryLogHardpointActuatorState();

    return ret;
}

bool PositionController::_hp_raise_lower_forces_in_tolerance_tension(int hp, bool raise, bool in_range,
                                                                     float measured_force, float low_limit) {
    switch (_hardpointActuatorState->motionState[hp]) {
        case MTM1M3_shared_HardpointActuatorMotionState_Chasing:
            if (_hardpoint_actuator_data->encoder[hp] >
                _hardpoint_actuator_settings->highProximityEncoder[hp]) {
                if (_wait_tension[hp] != CAN_WAIT) {
                    if (_wait_tension[hp] != ALREADY_WAITED) {
                        _wait_tension[hp] = ALREADY_WAITED;
                        SPDLOG_WARN(
                                "HP {} is at top end of its motion range. Continue with range checking, "
                                "reported force is {} N.",
                                hp + 1, measured_force);
                    }
                    return in_range;
                }

                if (measured_force <= low_limit) {
                    _hardpointActuatorState->motionState[hp] =
                            MTM1M3_shared_HardpointActuatorMotionState_WaitingTension;
                    _hardpoint_actuator_data->stepsCommanded[hp] = 0;
                    _wait_tension[hp] = WAITING;
                    M1M3SSPublisher::instance().logHardpointActuatorState();
                    if (raise) {
                        SPDLOG_INFO(
                                "HP {} waiting for tension force to clear as the "
                                "mirror will raise",
                                hp + 1);
                    } else {
                        SPDLOG_WARN("HP {} disable chasing as tension force is too high", hp + 1);
                    }
                    return true;
                }
            }
            break;
        case MTM1M3_shared_HardpointActuatorMotionState_WaitingTension:
            if (in_range) {
                _raising_lowering_in_range_samples[hp]++;
                if (_raising_lowering_in_range_samples[hp] <
                    _hardpoint_actuator_settings->inRangeReadoutsToChaseFromWaitingCompression) {
                    return false;
                }
                _hardpointActuatorState->motionState[hp] = MTM1M3_shared_HardpointActuatorMotionState_Chasing;
                _raising_lowering_in_range_samples[hp] = 0;
                M1M3SSPublisher::instance().logHardpointActuatorState();
                SPDLOG_INFO("HP {} back in safe tension range, continue chasing", hp + 1);
            } else {
                _raising_lowering_in_range_samples[hp] = 0;
                return true;
            }
            break;
        default:
            break;
    }
    return in_range;
}

bool PositionController::_hp_raise_lower_forces_in_tolerance_compression(int hp, bool raise, bool in_range,
                                                                         float measured_force,
                                                                         float high_limit) {
    switch (_hardpointActuatorState->motionState[hp]) {
        case MTM1M3_shared_HardpointActuatorMotionState_Chasing:
            if (_hardpoint_actuator_data->encoder[hp] <
                _hardpoint_actuator_settings->lowProximityEncoder[hp]) {
                if (_wait_compression[hp] != CAN_WAIT) {
                    if (_wait_compression[hp] != ALREADY_WAITED) {
                        _wait_compression[hp] = ALREADY_WAITED;
                        SPDLOG_WARN(
                                "HP {} is at bottom of its motion range. Continue with range checking, "
                                "reported force is {} N.",
                                hp + 1, measured_force);
                    }
                    return in_range;
                }

                if (measured_force >= high_limit) {
                    _hardpointActuatorState->motionState[hp] =
                            MTM1M3_shared_HardpointActuatorMotionState_WaitingCompression;
                    _hardpoint_actuator_data->stepsCommanded[hp] = 0;
                    _wait_compression[hp] = WAITING;
                    M1M3SSPublisher::instance().logHardpointActuatorState();
                    if (raise) {
                        SPDLOG_INFO(
                                "HP {} waiting for compression force to clear as the "
                                "mirror will raise",
                                hp + 1);
                    } else {
                        SPDLOG_WARN("HP {} disable chasing as compression force is too high", hp + 1);
                    }
                    return true;
                }
            }
            break;
        case MTM1M3_shared_HardpointActuatorMotionState_WaitingCompression:
            if (in_range) {
                _raising_lowering_in_range_samples[hp]++;
                if (_raising_lowering_in_range_samples[hp] <
                    _hardpoint_actuator_settings->inRangeReadoutsToChaseFromWaitingCompression) {
                    return false;
                }
                _hardpointActuatorState->motionState[hp] = MTM1M3_shared_HardpointActuatorMotionState_Chasing;
                _raising_lowering_in_range_samples[hp] = 0;
                M1M3SSPublisher::instance().logHardpointActuatorState();
                SPDLOG_INFO("HP {} back in safe compression range, continue chasing", hp + 1);
            } else {
                _raising_lowering_in_range_samples[hp] = 0;
                return true;
            }

            break;
        default:
            break;
    }
    return in_range;
}

bool PositionController::motionComplete() {
    SPDLOG_DEBUG("PositionController: motionComplete()");
    return _hardpointActuatorState->motionState[0] == MTM1M3_shared_HardpointActuatorMotionState_Standby &&
           _hardpointActuatorState->motionState[1] == MTM1M3_shared_HardpointActuatorMotionState_Standby &&
           _hardpointActuatorState->motionState[2] == MTM1M3_shared_HardpointActuatorMotionState_Standby &&
           _hardpointActuatorState->motionState[3] == MTM1M3_shared_HardpointActuatorMotionState_Standby &&
           _hardpointActuatorState->motionState[4] == MTM1M3_shared_HardpointActuatorMotionState_Standby &&
           _hardpointActuatorState->motionState[5] == MTM1M3_shared_HardpointActuatorMotionState_Standby;
}

bool PositionController::moveHardpoint(int32_t steps, int hpIndex) {
    SPDLOG_INFO("PositionController: moveHardpoint({}, {})", steps, hpIndex);
    if (steps == 0) {
        return false;
    }
    _hardpoint_actuator_data->stepsQueued[hpIndex] = steps;
    _hardpointActuatorState->motionState[hpIndex] = MTM1M3_shared_HardpointActuatorMotionState_Stepping;

    double loopCycles[6];
    double maxLoopCycles = 0;
    for (int i = 0; i < HP_COUNT; i++) {
        loopCycles[i] = abs(_hardpoint_actuator_data->stepsQueued[i]) /
                        (double)_position_controller_settings->maxStepsPerLoop;
        if (loopCycles[i] > maxLoopCycles) {
            maxLoopCycles = loopCycles[i];
        }
    }
    for (int i = 0; i < HP_COUNT; ++i) {
        _scaledMaxStepsPerLoop[i] =
                (int32_t)(_position_controller_settings->maxStepsPerLoop / (maxLoopCycles / loopCycles[i]));
        if (_scaledMaxStepsPerLoop[i] == 0) {
            _scaledMaxStepsPerLoop[i] = 1;
        }
    }
    M1M3SSPublisher::instance().tryLogHardpointActuatorState();
    return true;
}

bool PositionController::move(const std::vector<int>& steps) {
    SPDLOG_INFO("PositionController: move({:d}, {:d}, {:d}, {:d}, {:d}, {:d})", steps[0], steps[1], steps[2],
                steps[3], steps[4], steps[5]);
    if ((_hardpointActuatorState->motionState[0] != MTM1M3_shared_HardpointActuatorMotionState_Standby &&
         steps[0] != 0) ||
        (_hardpointActuatorState->motionState[1] != MTM1M3_shared_HardpointActuatorMotionState_Standby &&
         steps[1] != 0) ||
        (_hardpointActuatorState->motionState[2] != MTM1M3_shared_HardpointActuatorMotionState_Standby &&
         steps[2] != 0) ||
        (_hardpointActuatorState->motionState[3] != MTM1M3_shared_HardpointActuatorMotionState_Standby &&
         steps[3] != 0) ||
        (_hardpointActuatorState->motionState[4] != MTM1M3_shared_HardpointActuatorMotionState_Standby &&
         steps[4] != 0) ||
        (_hardpointActuatorState->motionState[5] != MTM1M3_shared_HardpointActuatorMotionState_Standby &&
         steps[5] != 0)) {
        return false;
    }
    _hardpointActuatorState->timestamp = M1M3SSPublisher::instance().getTimestamp();
    double loopCycles[6];
    double maxLoopCycles = 0;
    for (int i = 0; i < HP_COUNT; i++) {
        _hardpoint_actuator_data->stepsQueued[i] = steps[i];
        _hardpointActuatorState->motionState[i] =
                steps[i] != 0 ? MTM1M3_shared_HardpointActuatorMotionState_Stepping
                              : MTM1M3_shared_HardpointActuatorMotionState_Standby;
        loopCycles[i] = abs(steps[i]) / (double)_position_controller_settings->maxStepsPerLoop;
        if (loopCycles[i] > maxLoopCycles) {
            maxLoopCycles = loopCycles[i];
        }
    }
    for (int i = 0; i < HP_COUNT; ++i) {
        _scaledMaxStepsPerLoop[i] =
                (int32_t)(_position_controller_settings->maxStepsPerLoop / (maxLoopCycles / loopCycles[i]));
        if (_scaledMaxStepsPerLoop[i] == 0) {
            _scaledMaxStepsPerLoop[i] = 1;
        }
    }
    M1M3SSPublisher::instance().tryLogHardpointActuatorState();
    return true;
}

bool PositionController::moveToEncoder(const std::vector<int>& encoderValues) {
    SPDLOG_INFO("PositionController: moveToEncoder({:d}, {:d}, {:d}, {:d}, {:d}, {:d})", encoderValues[0],
                encoderValues[1], encoderValues[2], encoderValues[3], encoderValues[4], encoderValues[5]);
    if ((_hardpointActuatorState->motionState[0] != MTM1M3_shared_HardpointActuatorMotionState_Standby &&
         encoderValues[0] != _hardpoint_actuator_data->encoder[0]) ||
        (_hardpointActuatorState->motionState[1] != MTM1M3_shared_HardpointActuatorMotionState_Standby &&
         encoderValues[1] != _hardpoint_actuator_data->encoder[1]) ||
        (_hardpointActuatorState->motionState[2] != MTM1M3_shared_HardpointActuatorMotionState_Standby &&
         encoderValues[2] != _hardpoint_actuator_data->encoder[2]) ||
        (_hardpointActuatorState->motionState[3] != MTM1M3_shared_HardpointActuatorMotionState_Standby &&
         encoderValues[3] != _hardpoint_actuator_data->encoder[3]) ||
        (_hardpointActuatorState->motionState[4] != MTM1M3_shared_HardpointActuatorMotionState_Standby &&
         encoderValues[4] != _hardpoint_actuator_data->encoder[4]) ||
        (_hardpointActuatorState->motionState[5] != MTM1M3_shared_HardpointActuatorMotionState_Standby &&
         encoderValues[5] != _hardpoint_actuator_data->encoder[5])) {
        return false;
    }
    _hardpointActuatorState->timestamp = M1M3SSPublisher::instance().getTimestamp();
    double loopCycles[6];
    double maxLoopCycles = 0;
    for (int i = 0; i < HP_COUNT; i++) {
        _targetEncoderValues[i] = encoderValues[i];
        _last_encoder_count[i] = _hardpoint_actuator_data->encoder[i];
        _stableEncoderCount[i] = 0;
        _unstableEncoderCount[i] = 0;
        int deltaEncoder = _targetEncoderValues[i] - _hardpoint_actuator_data->encoder[i];
        // If we overshoot our target encoder value we have to clear what appears to
        // be quite a bit of backlash So lets not overshoot our target
        if (deltaEncoder > 0) {
            deltaEncoder -= 4;
            // We are already very close to our target so lets not do anything during
            // the quick positioning phase
            if (deltaEncoder < 0) {
                deltaEncoder = 0;
            }
        } else if (deltaEncoder < 0) {
            deltaEncoder += 4;
            if (deltaEncoder > 0) {
                deltaEncoder = 0;
            }
        }
        _hardpoint_actuator_data->stepsQueued[i] =
                deltaEncoder * _position_controller_settings->encoderToStepsCoefficient;
        _hardpointActuatorState->motionState[i] =
                _hardpoint_actuator_data->stepsQueued[i] != 0
                        ? MTM1M3_shared_HardpointActuatorMotionState_QuickPositioning
                        : MTM1M3_shared_HardpointActuatorMotionState_Standby;
        loopCycles[i] = abs(_hardpoint_actuator_data->stepsQueued[i]) /
                        (double)_position_controller_settings->maxStepsPerLoop;
        if (loopCycles[i] > maxLoopCycles) {
            maxLoopCycles = loopCycles[i];
        }
    }
    for (int i = 0; i < HP_COUNT; ++i) {
        _scaledMaxStepsPerLoop[i] =
                (int32_t)(_position_controller_settings->maxStepsPerLoop / (maxLoopCycles / loopCycles[i]));
        if (_scaledMaxStepsPerLoop[i] == 0) {
            _scaledMaxStepsPerLoop[i] = 1;
        }
    }
    M1M3SSPublisher::instance().tryLogHardpointActuatorState();
    return true;
}

bool PositionController::moveToAbsolute(double x, double y, double z, double rX, double rY, double rZ) {
    SPDLOG_INFO("PositionController: moveToAbsolute({:f}, {:f}, {:f}, {:f}, {:f}, {:f})", x, y, z, rX, rY,
                rZ);
    int32_t steps[6];
    _convert_to_steps(steps, x, y, z, rX, rY, rZ);
    std::vector<int> encoderValues(6, 0);
    double stepsToEncoder = _hardpoint_actuator_settings->micrometersPerStep /
                            _hardpoint_actuator_settings->micrometersPerEncoder;
    for (int i = 0; i < HP_COUNT; ++i) {
        encoderValues[i] = _hardpointInfo->referencePosition[i] + steps[i] * stepsToEncoder;
    }
    return this->moveToEncoder(encoderValues);
}

bool PositionController::moveToReferencePosition() {
    SPDLOG_INFO("PositionController: moveToReferencePosition()");
    return this->moveToEncoder(_hardpointInfo->referencePosition);
}

bool PositionController::moveToLowerPosition() {
    double m_pos = _position_controller_settings->lowerPositionOffset * MM2M;
    return moveToAbsolute(0, m_pos * TMA::instance().getElevationCos(),
                          m_pos * TMA::instance().getElevationSin(), 0, 0, 0);
}

bool PositionController::translate(double x, double y, double z, double rX, double rY, double rZ) {
    SPDLOG_INFO("PositionController: translate({:f}, {:f}, {:f}, {:f}, {:f}, {:f})", x, y, z, rX, rY, rZ);
    int32_t steps[6];
    _convert_to_steps(steps, x, y, z, rX, rY, rZ);
    std::vector<int> encoderValues(6, 0);
    double stepsToEncoder = _hardpoint_actuator_settings->micrometersPerStep /
                            _hardpoint_actuator_settings->micrometersPerEncoder;
    for (int i = 0; i < HP_COUNT; ++i) {
        encoderValues[i] = _hardpoint_actuator_data->encoder[i] + steps[i] * stepsToEncoder;
    }
    return this->moveToEncoder(encoderValues);
}

void PositionController::stopMotion(int hardpointIndex) {
    SPDLOG_INFO("PositionController: stopMotion()");
    _hardpointActuatorState->timestamp = M1M3SSPublisher::instance().getTimestamp();
    if (hardpointIndex < 0) {
        for (int i = 0; i < HP_COUNT; i++) {
            _hardpoint_actuator_data->stepsQueued[i] = 0;
            _hardpointActuatorState->motionState[i] = MTM1M3_shared_HardpointActuatorMotionState_Standby;
        }
    } else {
        _hardpoint_actuator_data->stepsQueued[hardpointIndex] = 0;
        _hardpointActuatorState->motionState[hardpointIndex] =
                MTM1M3_shared_HardpointActuatorMotionState_Standby;
    }

    M1M3SSPublisher::instance().tryLogHardpointActuatorState();
}

void PositionController::updateSteps() {
    SPDLOG_TRACE("PositionController: updateSteps()");
    _hardpointActuatorState->timestamp = M1M3SSPublisher::instance().getTimestamp();
    bool publishState = false;
    for (int i = 0; i < HP_COUNT; i++) {
        switch (_hardpointActuatorState->motionState[i]) {
            case MTM1M3_shared_HardpointActuatorMotionState_Standby:
                _hardpoint_actuator_data->stepsCommanded[i] = 0;
                _hardpoint_actuator_data->stepsQueued[i] = 0;
                break;
            case MTM1M3_shared_HardpointActuatorMotionState_Chasing: {
                _check_following_error(i);
                float force = _hardpoint_actuator_data->measuredForce[i];
                int32_t chaseSteps =
                        (int32_t)(force * _position_controller_settings->forceToStepsCoefficient);
                chaseSteps =
                        Range::CoerceIntoRange(-_position_controller_settings->maxStepsPerLoop,
                                               _position_controller_settings->maxStepsPerLoop, chaseSteps);
                _hardpoint_actuator_data->stepsCommanded[i] = (int16_t)chaseSteps;
                break;
            }
            case MTM1M3_shared_HardpointActuatorMotionState_Stepping: {
                _check_following_error(i);
                int32_t moveSteps =
                        Range::CoerceIntoRange<int32_t>(-_scaledMaxStepsPerLoop[i], _scaledMaxStepsPerLoop[i],
                                                        _hardpoint_actuator_data->stepsQueued[i]);
                _hardpoint_actuator_data->stepsQueued[i] -= moveSteps;
                _hardpoint_actuator_data->stepsCommanded[i] = (int16_t)moveSteps;
                if (_hardpoint_actuator_data->stepsQueued[i] == 0 &&
                    _hardpoint_actuator_data->stepsCommanded[i] == 0) {
                    publishState = true;
                    _hardpointActuatorState->motionState[i] =
                            MTM1M3_shared_HardpointActuatorMotionState_Standby;
                }
                break;
            }
            case MTM1M3_shared_HardpointActuatorMotionState_QuickPositioning: {
                _check_following_error(i);
                int32_t moveSteps =
                        Range::CoerceIntoRange<int32_t>(-_scaledMaxStepsPerLoop[i], _scaledMaxStepsPerLoop[i],
                                                        _hardpoint_actuator_data->stepsQueued[i]);
                _hardpoint_actuator_data->stepsQueued[i] -= moveSteps;
                _hardpoint_actuator_data->stepsCommanded[i] = (int16_t)moveSteps;
                if (_hardpoint_actuator_data->stepsQueued[i] == 0 &&
                    _hardpoint_actuator_data->stepsCommanded[i] == 0) {
                    publishState = true;
                    _hardpointActuatorState->motionState[i] =
                            MTM1M3_shared_HardpointActuatorMotionState_FinePositioning;
                }
                break;
            }
            case MTM1M3_shared_HardpointActuatorMotionState_FinePositioning: {
                _check_following_error(i);
                int32_t deltaEncoder = _targetEncoderValues[i] - _hardpoint_actuator_data->encoder[i];
                int32_t encoderSteps =
                        (int32_t)(deltaEncoder * _position_controller_settings->encoderToStepsCoefficient);
                if (fabs(deltaEncoder) <= 2) {
                    if (deltaEncoder < 0) {
                        encoderSteps = -1;
                        _stableEncoderCount[i] = 0;
                    } else if (deltaEncoder > 0) {
                        encoderSteps = 1;
                        _stableEncoderCount[i] = 0;
                    } else {
                        encoderSteps = 0;
                        _stableEncoderCount[i]++;
                    }
                    _unstableEncoderCount[i] = 0;
                } else {
                    _safety_controller->positionControllerNotifyUnstable(i, _unstableEncoderCount[i],
                                                                         deltaEncoder);
                    _unstableEncoderCount[i]++;
                }
                _hardpoint_actuator_data->stepsCommanded[i] =
                        Range::CoerceIntoRange(-_position_controller_settings->maxStepsPerLoop,
                                               _position_controller_settings->maxStepsPerLoop, encoderSteps);
                if (deltaEncoder == 0 && _stableEncoderCount[i] >= 2) {
                    publishState = true;
                    _hardpoint_actuator_data->stepsCommanded[i] = 0;
                    _hardpointActuatorState->motionState[i] =
                            MTM1M3_shared_HardpointActuatorMotionState_Standby;
                }
                break;
            }
            case MTM1M3_shared_HardpointActuatorMotionState_WaitingTension: {
                _check_following_error(i);
                float force = _hardpoint_actuator_data->measuredForce[i];
                int32_t chaseSteps =
                        (int32_t)(force * _position_controller_settings->forceToStepsCoefficient);
                chaseSteps =
                        Range::CoerceIntoRange(-_position_controller_settings->maxStepsPerLoop,
                                               _position_controller_settings->maxStepsPerLoop, chaseSteps);
                // only allow downward movement
                _hardpoint_actuator_data->stepsCommanded[i] = std::min(0, chaseSteps);

                auto raiseLowerInfo = &RaisingLoweringInfo::instance();
                _safety_controller->positionControllerHighTension(i, raiseLowerInfo->weightSupportedPercent);
                break;
            }
            case MTM1M3_shared_HardpointActuatorMotionState_WaitingCompression: {
                _check_following_error(i);
                float force = _hardpoint_actuator_data->measuredForce[i];
                int32_t chaseSteps =
                        (int32_t)(force * _position_controller_settings->forceToStepsCoefficient);
                chaseSteps =
                        Range::CoerceIntoRange(-_position_controller_settings->maxStepsPerLoop,
                                               _position_controller_settings->maxStepsPerLoop, chaseSteps);
                // only allow upward movement
                _hardpoint_actuator_data->stepsCommanded[i] = std::max(0, chaseSteps);

                auto raiseLowerInfo = &RaisingLoweringInfo::instance();
                _safety_controller->positionControllerHighCompression(i,
                                                                      raiseLowerInfo->weightSupportedPercent);
                break;
            }
        }
        checkLimits(i);
    }
    if (publishState) {
        M1M3SSPublisher::instance().tryLogHardpointActuatorState();
    }
}

void PositionController::checkLimits(int hp) {
    SPDLOG_TRACE("PositionController: checkLimits({})", hp);
    bool low_limit = _hardpoint_actuator_data->stepsCommanded[hp] < 0 &&
                     HardpointActuatorWarning::instance().limitSwitch2Operated[hp] == true;
    bool high_limit = _hardpoint_actuator_data->stepsCommanded[hp] > 0 &&
                      HardpointActuatorWarning::instance().limitSwitch1Operated[hp] == true;
    if (low_limit || high_limit) {
        _hardpoint_actuator_data->stepsCommanded[hp] = 0;
    }
    _safety_controller->positionControllerNotifyLimitLow(hp, low_limit);
    _safety_controller->positionControllerNotifyLimitHigh(hp, high_limit);
}

void PositionController::_convert_to_steps(int32_t* steps, double x, double y, double z, double rX, double rY,
                                           double rZ) {
    // The reason for defining HP 3 first (index 2) is due to the matrix. Review
    // the MirrorPositionToHardpointDisplacementTable for a description of the
    // matrix.
    steps[2] = (_hardpoint_actuator_settings->MirrorPositionToHardpointDisplacement[0] * x +
                _hardpoint_actuator_settings->MirrorPositionToHardpointDisplacement[1] * y +
                _hardpoint_actuator_settings->MirrorPositionToHardpointDisplacement[2] * z +
                _hardpoint_actuator_settings->MirrorPositionToHardpointDisplacement[3] * rX +
                _hardpoint_actuator_settings->MirrorPositionToHardpointDisplacement[4] * rY +
                _hardpoint_actuator_settings->MirrorPositionToHardpointDisplacement[5] * rZ) *
               (MILLIMETERS_PER_METER * MICROMETERS_PER_MILLIMETER /
                _hardpoint_actuator_settings->micrometersPerStep);
    steps[3] = (_hardpoint_actuator_settings->MirrorPositionToHardpointDisplacement[6] * x +
                _hardpoint_actuator_settings->MirrorPositionToHardpointDisplacement[7] * y +
                _hardpoint_actuator_settings->MirrorPositionToHardpointDisplacement[8] * z +
                _hardpoint_actuator_settings->MirrorPositionToHardpointDisplacement[9] * rX +
                _hardpoint_actuator_settings->MirrorPositionToHardpointDisplacement[10] * rY +
                _hardpoint_actuator_settings->MirrorPositionToHardpointDisplacement[11] * rZ) *
               (MILLIMETERS_PER_METER * MICROMETERS_PER_MILLIMETER /
                _hardpoint_actuator_settings->micrometersPerStep);
    steps[4] = (_hardpoint_actuator_settings->MirrorPositionToHardpointDisplacement[12] * x +
                _hardpoint_actuator_settings->MirrorPositionToHardpointDisplacement[13] * y +
                _hardpoint_actuator_settings->MirrorPositionToHardpointDisplacement[14] * z +
                _hardpoint_actuator_settings->MirrorPositionToHardpointDisplacement[15] * rX +
                _hardpoint_actuator_settings->MirrorPositionToHardpointDisplacement[16] * rY +
                _hardpoint_actuator_settings->MirrorPositionToHardpointDisplacement[17] * rZ) *
               (MILLIMETERS_PER_METER * MICROMETERS_PER_MILLIMETER /
                _hardpoint_actuator_settings->micrometersPerStep);
    steps[5] = (_hardpoint_actuator_settings->MirrorPositionToHardpointDisplacement[18] * x +
                _hardpoint_actuator_settings->MirrorPositionToHardpointDisplacement[19] * y +
                _hardpoint_actuator_settings->MirrorPositionToHardpointDisplacement[20] * z +
                _hardpoint_actuator_settings->MirrorPositionToHardpointDisplacement[21] * rX +
                _hardpoint_actuator_settings->MirrorPositionToHardpointDisplacement[22] * rY +
                _hardpoint_actuator_settings->MirrorPositionToHardpointDisplacement[23] * rZ) *
               (MILLIMETERS_PER_METER * MICROMETERS_PER_MILLIMETER /
                _hardpoint_actuator_settings->micrometersPerStep);
    steps[0] = (_hardpoint_actuator_settings->MirrorPositionToHardpointDisplacement[24] * x +
                _hardpoint_actuator_settings->MirrorPositionToHardpointDisplacement[25] * y +
                _hardpoint_actuator_settings->MirrorPositionToHardpointDisplacement[26] * z +
                _hardpoint_actuator_settings->MirrorPositionToHardpointDisplacement[27] * rX +
                _hardpoint_actuator_settings->MirrorPositionToHardpointDisplacement[28] * rY +
                _hardpoint_actuator_settings->MirrorPositionToHardpointDisplacement[29] * rZ) *
               (MILLIMETERS_PER_METER * MICROMETERS_PER_MILLIMETER /
                _hardpoint_actuator_settings->micrometersPerStep);
    steps[1] = (_hardpoint_actuator_settings->MirrorPositionToHardpointDisplacement[30] * x +
                _hardpoint_actuator_settings->MirrorPositionToHardpointDisplacement[31] * y +
                _hardpoint_actuator_settings->MirrorPositionToHardpointDisplacement[32] * z +
                _hardpoint_actuator_settings->MirrorPositionToHardpointDisplacement[33] * rX +
                _hardpoint_actuator_settings->MirrorPositionToHardpointDisplacement[34] * rY +
                _hardpoint_actuator_settings->MirrorPositionToHardpointDisplacement[35] * rZ) *
               (MILLIMETERS_PER_METER * MICROMETERS_PER_MILLIMETER /
                _hardpoint_actuator_settings->micrometersPerStep);
}

void PositionController::_check_following_error(int hp) {
    // make sure small corrections, negligible relative to encoder step size,
    // aren't checked
    if (abs(_hardpoint_actuator_data->stepsCommanded[hp]) >
        _position_controller_settings->encoderToStepsCoefficient) {
        double traveled = _hardpoint_actuator_data->encoder[hp] - _last_encoder_count[hp];
        // due to condition above divider is > 1 or < -1, division by 0 cannot occur
        double shouldTravel = _hardpoint_actuator_data->stepsCommanded[hp] /
                              _position_controller_settings->encoderToStepsCoefficient;

        double fePercent = 100 * (traveled / shouldTravel);
        _safety_controller->hardpointActuatorFollowingError(hp, fePercent);
    }
    _last_encoder_count[hp] = _hardpoint_actuator_data->encoder[hp];
}

void PositionController::_reset_wait_compression_tension() {
    for (int hp = 0; hp < HP_COUNT; hp++) {
        _wait_tension[hp] = hp_can_see_tension(hp) ? CAN_WAIT : NO_WAIT;
        _wait_compression[hp] = hp_can_see_compression(hp) ? CAN_WAIT : NO_WAIT;
        _raising_lowering_in_range_samples[hp] = 0;
    }
}
