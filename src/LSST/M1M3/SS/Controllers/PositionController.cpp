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

#include <PositionController.h>
#include <PositionControllerSettings.h>
#include <HardpointActuatorSettings.h>
#include <HardpointActuatorWarning.h>
#include <M1M3SSPublisher.h>
#include <Model.h>
#include <HardpointActuatorMotionStates.h>
#include <Range.h>
#include <RaisingLoweringInfo.h>
#include <SAL_MTM1M3C.h>
#include <TMA.h>

#include <spdlog/spdlog.h>
#include <stdlib.h>

using namespace LSST::M1M3::SS;

PositionController::PositionController(PositionControllerSettings* positionControllerSettings,
                                       HardpointActuatorSettings* hardpointActuatorSettings) {
    SPDLOG_DEBUG("PositionController: PositionController()");
    _safetyController = Model::get().getSafetyController();
    _positionControllerSettings = positionControllerSettings;
    _hardpointActuatorSettings = hardpointActuatorSettings;
    _hardpointActuatorData = M1M3SSPublisher::instance().getHardpointActuatorData();
    _hardpointActuatorState = M1M3SSPublisher::instance().getEventHardpointActuatorState();
    _hardpointInfo = M1M3SSPublisher::instance().getEventHardpointActuatorInfo();
    _hardpointActuatorState->timestamp = M1M3SSPublisher::instance().getTimestamp();
    for (int i = 0; i < HP_COUNT; i++) {
        _hardpointActuatorState->motionState[i] = HardpointActuatorMotionStates::Standby;
        _hardpointActuatorData->stepsQueued[i] = 0;
        _hardpointActuatorData->stepsCommanded[i] = 0;
        _scaledMaxStepsPerLoop[i] = _positionControllerSettings->maxStepsPerLoop;
        _targetEncoderValues[i] = 0;
        _stableEncoderCount[i] = 0;
        _unstableEncoderCount[i] = 0;
    }
    M1M3SSPublisher::instance().logHardpointActuatorState();
}

bool PositionController::enableChaseAll() {
    SPDLOG_INFO("PositionController: enableChaseAll()");
    if (_hardpointActuatorState->motionState[0] != HardpointActuatorMotionStates::Standby ||
        _hardpointActuatorState->motionState[1] != HardpointActuatorMotionStates::Standby ||
        _hardpointActuatorState->motionState[2] != HardpointActuatorMotionStates::Standby ||
        _hardpointActuatorState->motionState[3] != HardpointActuatorMotionStates::Standby ||
        _hardpointActuatorState->motionState[4] != HardpointActuatorMotionStates::Standby ||
        _hardpointActuatorState->motionState[5] != HardpointActuatorMotionStates::Standby) {
        return false;
    }
    _hardpointActuatorState->timestamp = M1M3SSPublisher::instance().getTimestamp();
    for (int i = 0; i < HP_COUNT; i++) {
        _hardpointActuatorState->motionState[i] = HardpointActuatorMotionStates::Chasing;
        _lastEncoderCount[i] = _hardpointActuatorData->encoder[i];
    }
    M1M3SSPublisher::instance().tryLogHardpointActuatorState();
    return true;
}

void PositionController::disableChaseAll() {
    SPDLOG_INFO("PositionController: disableChaseAll()");
    _hardpointActuatorState->timestamp = M1M3SSPublisher::instance().getTimestamp();
    for (int i = 0; i < HP_COUNT; i++) {
        _hardpointActuatorState->motionState[i] = HardpointActuatorMotionStates::Standby;
    }
    M1M3SSPublisher::instance().tryLogHardpointActuatorState();
}

bool PositionController::hpRaiseLowerForcesInTolerance(bool raise) {
    SPDLOG_TRACE("PositionController: forcesInTolerance({})", raise);
    bool ret = true;
    auto raiseLowerInfo = &RaisingLoweringInfo::instance();

    class PositionLimitTrigger : public LimitTrigger<float, float, float> {
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

        void execute(float lowLimit, float highLimit, float measured) override {
            SPDLOG_WARN("Violated hardpoint {} measured force {} ({}th occurence), limit {} to {}", _hp,
                        measured, _counter, lowLimit, highLimit);
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

    static PositionLimitTrigger triggers[6] = {PositionLimitTrigger(1), PositionLimitTrigger(2),
                                               PositionLimitTrigger(3), PositionLimitTrigger(4),
                                               PositionLimitTrigger(5), PositionLimitTrigger(6)};
    for (int i = 0; i < HP_COUNT; i++) {
        bool inRange;
        if (raise) {
            inRange = Range::InRangeTrigger((float)_positionControllerSettings->raiseHPForceLimitLow,
                                            (float)_positionControllerSettings->raiseHPForceLimitHigh,
                                            _hardpointActuatorData->measuredForce[i], triggers[i],
                                            (float)_positionControllerSettings->raiseHPForceLimitLow,
                                            (float)_positionControllerSettings->raiseHPForceLimitHigh,
                                            _hardpointActuatorData->measuredForce[i]);
        } else {
            inRange = Range::InRangeTrigger((float)_positionControllerSettings->lowerHPForceLimitLow,
                                            (float)_positionControllerSettings->lowerHPForceLimitHigh,
                                            _hardpointActuatorData->measuredForce[i], triggers[i],
                                            (float)_positionControllerSettings->lowerHPForceLimitLow,
                                            (float)_positionControllerSettings->lowerHPForceLimitHigh,
                                            _hardpointActuatorData->measuredForce[i]);
        }
        raiseLowerInfo->setHPWait(i, !inRange);

        ret = ret & inRange;
        checkLimits(i);
    }
    return ret;
}

bool PositionController::motionComplete() {
    SPDLOG_DEBUG("PositionController: motionComplete()");
    return _hardpointActuatorState->motionState[0] == HardpointActuatorMotionStates::Standby &&
           _hardpointActuatorState->motionState[1] == HardpointActuatorMotionStates::Standby &&
           _hardpointActuatorState->motionState[2] == HardpointActuatorMotionStates::Standby &&
           _hardpointActuatorState->motionState[3] == HardpointActuatorMotionStates::Standby &&
           _hardpointActuatorState->motionState[4] == HardpointActuatorMotionStates::Standby &&
           _hardpointActuatorState->motionState[5] == HardpointActuatorMotionStates::Standby;
}

bool PositionController::moveHardpoint(int32_t steps, int hpIndex) {
    SPDLOG_INFO("PositionController: moveHardpoint({}, {})", steps, hpIndex);
    if (steps == 0) {
        return false;
    }
    _hardpointActuatorData->stepsQueued[hpIndex] = steps;
    _hardpointActuatorState->motionState[hpIndex] = HardpointActuatorMotionStates::Stepping;

    double loopCycles[6];
    double maxLoopCycles = 0;
    for (int i = 0; i < HP_COUNT; i++) {
        loopCycles[i] = abs(_hardpointActuatorData->stepsQueued[i]) /
                        (double)_positionControllerSettings->maxStepsPerLoop;
        if (loopCycles[i] > maxLoopCycles) {
            maxLoopCycles = loopCycles[i];
        }
    }
    for (int i = 0; i < HP_COUNT; ++i) {
        _scaledMaxStepsPerLoop[i] =
                (int32_t)(_positionControllerSettings->maxStepsPerLoop / (maxLoopCycles / loopCycles[i]));
        if (_scaledMaxStepsPerLoop[i] == 0) {
            _scaledMaxStepsPerLoop[i] = 1;
        }
    }
    M1M3SSPublisher::instance().tryLogHardpointActuatorState();
    return true;
}

bool PositionController::move(int32_t* steps) {
    SPDLOG_INFO("PositionController: move({:d}, {:d}, {:d}, {:d}, {:d}, {:d})", steps[0], steps[1], steps[2],
                steps[3], steps[4], steps[5]);
    if ((_hardpointActuatorState->motionState[0] != HardpointActuatorMotionStates::Standby &&
         steps[0] != 0) ||
        (_hardpointActuatorState->motionState[1] != HardpointActuatorMotionStates::Standby &&
         steps[1] != 0) ||
        (_hardpointActuatorState->motionState[2] != HardpointActuatorMotionStates::Standby &&
         steps[2] != 0) ||
        (_hardpointActuatorState->motionState[3] != HardpointActuatorMotionStates::Standby &&
         steps[3] != 0) ||
        (_hardpointActuatorState->motionState[4] != HardpointActuatorMotionStates::Standby &&
         steps[4] != 0) ||
        (_hardpointActuatorState->motionState[5] != HardpointActuatorMotionStates::Standby &&
         steps[5] != 0)) {
        return false;
    }
    _hardpointActuatorState->timestamp = M1M3SSPublisher::instance().getTimestamp();
    double loopCycles[6];
    double maxLoopCycles = 0;
    for (int i = 0; i < HP_COUNT; i++) {
        _hardpointActuatorData->stepsQueued[i] = steps[i];
        _hardpointActuatorState->motionState[i] = steps[i] != 0 ? HardpointActuatorMotionStates::Stepping
                                                                : HardpointActuatorMotionStates::Standby;
        loopCycles[i] = abs(steps[i]) / (double)_positionControllerSettings->maxStepsPerLoop;
        if (loopCycles[i] > maxLoopCycles) {
            maxLoopCycles = loopCycles[i];
        }
    }
    for (int i = 0; i < HP_COUNT; ++i) {
        _scaledMaxStepsPerLoop[i] =
                (int32_t)(_positionControllerSettings->maxStepsPerLoop / (maxLoopCycles / loopCycles[i]));
        if (_scaledMaxStepsPerLoop[i] == 0) {
            _scaledMaxStepsPerLoop[i] = 1;
        }
    }
    M1M3SSPublisher::instance().tryLogHardpointActuatorState();
    return true;
}

bool PositionController::moveToEncoder(int32_t* encoderValues) {
    SPDLOG_INFO("PositionController: moveToEncoder({:d}, {:d}, {:d}, {:d}, {:d}, {:d})", encoderValues[0],
                encoderValues[1], encoderValues[2], encoderValues[3], encoderValues[4], encoderValues[5]);
    if ((_hardpointActuatorState->motionState[0] != HardpointActuatorMotionStates::Standby &&
         encoderValues[0] != _hardpointActuatorData->encoder[0]) ||
        (_hardpointActuatorState->motionState[1] != HardpointActuatorMotionStates::Standby &&
         encoderValues[1] != _hardpointActuatorData->encoder[1]) ||
        (_hardpointActuatorState->motionState[2] != HardpointActuatorMotionStates::Standby &&
         encoderValues[2] != _hardpointActuatorData->encoder[2]) ||
        (_hardpointActuatorState->motionState[3] != HardpointActuatorMotionStates::Standby &&
         encoderValues[3] != _hardpointActuatorData->encoder[3]) ||
        (_hardpointActuatorState->motionState[4] != HardpointActuatorMotionStates::Standby &&
         encoderValues[4] != _hardpointActuatorData->encoder[4]) ||
        (_hardpointActuatorState->motionState[5] != HardpointActuatorMotionStates::Standby &&
         encoderValues[5] != _hardpointActuatorData->encoder[5])) {
        return false;
    }
    _hardpointActuatorState->timestamp = M1M3SSPublisher::instance().getTimestamp();
    double loopCycles[6];
    double maxLoopCycles = 0;
    for (int i = 0; i < HP_COUNT; i++) {
        _targetEncoderValues[i] = encoderValues[i];
        _lastEncoderCount[i] = _hardpointActuatorData->encoder[i];
        _stableEncoderCount[i] = 0;
        _unstableEncoderCount[i] = 0;
        int deltaEncoder = _targetEncoderValues[i] - _hardpointActuatorData->encoder[i];
        // If we overshoot our target encoder value we have to clear what appears to be quite a bit of
        // backlash So lets not overshoot our target
        if (deltaEncoder > 0) {
            deltaEncoder -= 4;
            // We are already very close to our target so lets not do anything during the quick positioning
            // phase
            if (deltaEncoder < 0) {
                deltaEncoder = 0;
            }
        } else if (deltaEncoder < 0) {
            deltaEncoder += 4;
            if (deltaEncoder > 0) {
                deltaEncoder = 0;
            }
        }
        _hardpointActuatorData->stepsQueued[i] =
                deltaEncoder * _positionControllerSettings->encoderToStepsCoefficient;
        _hardpointActuatorState->motionState[i] = _hardpointActuatorData->stepsQueued[i] != 0
                                                          ? HardpointActuatorMotionStates::QuickPositioning
                                                          : HardpointActuatorMotionStates::Standby;
        loopCycles[i] = abs(_hardpointActuatorData->stepsQueued[i]) /
                        (double)_positionControllerSettings->maxStepsPerLoop;
        if (loopCycles[i] > maxLoopCycles) {
            maxLoopCycles = loopCycles[i];
        }
    }
    for (int i = 0; i < HP_COUNT; ++i) {
        _scaledMaxStepsPerLoop[i] =
                (int32_t)(_positionControllerSettings->maxStepsPerLoop / (maxLoopCycles / loopCycles[i]));
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
    _convertToSteps(steps, x, y, z, rX, rY, rZ);
    int32_t encoderValues[6];
    double stepsToEncoder = _hardpointActuatorSettings->micrometersPerStep /
                            _hardpointActuatorSettings->micrometersPerEncoder;
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
    double m_pos = _positionControllerSettings->lowerPositionOffset * MM2M;
    return moveToAbsolute(0, m_pos * TMA::instance().getElevationCos(),
                          m_pos * TMA::instance().getElevationSin(), 0, 0, 0);
}

bool PositionController::translate(double x, double y, double z, double rX, double rY, double rZ) {
    SPDLOG_INFO("PositionController: translate({:f}, {:f}, {:f}, {:f}, {:f}, {:f})", x, y, z, rX, rY, rZ);
    int32_t steps[6];
    _convertToSteps(steps, x, y, z, rX, rY, rZ);
    int32_t encoderValues[6];
    double stepsToEncoder = _hardpointActuatorSettings->micrometersPerStep /
                            _hardpointActuatorSettings->micrometersPerEncoder;
    for (int i = 0; i < HP_COUNT; ++i) {
        encoderValues[i] = _hardpointActuatorData->encoder[i] + steps[i] * stepsToEncoder;
    }
    return this->moveToEncoder(encoderValues);
}

void PositionController::stopMotion(int hardpointIndex) {
    SPDLOG_INFO("PositionController: stopMotion()");
    _hardpointActuatorState->timestamp = M1M3SSPublisher::instance().getTimestamp();
    if (hardpointIndex < 0) {
        for (int i = 0; i < HP_COUNT; i++) {
            _hardpointActuatorData->stepsQueued[i] = 0;
            _hardpointActuatorState->motionState[i] = HardpointActuatorMotionStates::Standby;
        }
    } else {
        _hardpointActuatorData->stepsQueued[hardpointIndex] = 0;
        _hardpointActuatorState->motionState[hardpointIndex] = HardpointActuatorMotionStates::Standby;
    }

    M1M3SSPublisher::instance().tryLogHardpointActuatorState();
}

void PositionController::updateSteps() {
    SPDLOG_TRACE("PositionController: updateSteps()");
    _hardpointActuatorState->timestamp = M1M3SSPublisher::instance().getTimestamp();
    bool publishState = false;
    for (int i = 0; i < HP_COUNT; i++) {
        switch (_hardpointActuatorState->motionState[i]) {
            case HardpointActuatorMotionStates::Standby:
                _hardpointActuatorData->stepsCommanded[i] = 0;
                _hardpointActuatorData->stepsQueued[i] = 0;
                break;
            case HardpointActuatorMotionStates::Chasing: {
                _checkFollowingError(i);
                float force = _hardpointActuatorData->measuredForce[i];
                int32_t chaseSteps = (int32_t)(force * _positionControllerSettings->forceToStepsCoefficient);
                chaseSteps = Range::CoerceIntoRange(-_positionControllerSettings->maxStepsPerLoop,
                                                    _positionControllerSettings->maxStepsPerLoop, chaseSteps);
                _hardpointActuatorData->stepsCommanded[i] = (int16_t)chaseSteps;
                break;
            }
            case HardpointActuatorMotionStates::Stepping: {
                _checkFollowingError(i);
                int32_t moveSteps =
                        Range::CoerceIntoRange(-_scaledMaxStepsPerLoop[i], _scaledMaxStepsPerLoop[i],
                                               _hardpointActuatorData->stepsQueued[i]);
                _hardpointActuatorData->stepsQueued[i] -= moveSteps;
                _hardpointActuatorData->stepsCommanded[i] = (int16_t)moveSteps;
                if (_hardpointActuatorData->stepsQueued[i] == 0 &&
                    _hardpointActuatorData->stepsCommanded[i] == 0) {
                    publishState = true;
                    _hardpointActuatorState->motionState[i] = HardpointActuatorMotionStates::Standby;
                }
                break;
            }
            case HardpointActuatorMotionStates::QuickPositioning: {
                _checkFollowingError(i);
                int32_t moveSteps =
                        Range::CoerceIntoRange(-_scaledMaxStepsPerLoop[i], _scaledMaxStepsPerLoop[i],
                                               _hardpointActuatorData->stepsQueued[i]);
                _hardpointActuatorData->stepsQueued[i] -= moveSteps;
                _hardpointActuatorData->stepsCommanded[i] = (int16_t)moveSteps;
                if (_hardpointActuatorData->stepsQueued[i] == 0 &&
                    _hardpointActuatorData->stepsCommanded[i] == 0) {
                    publishState = true;
                    _hardpointActuatorState->motionState[i] = HardpointActuatorMotionStates::FinePositioning;
                }
                break;
            }
            case HardpointActuatorMotionStates::FinePositioning: {
                _checkFollowingError(i);
                int32_t deltaEncoder = _targetEncoderValues[i] - _hardpointActuatorData->encoder[i];
                int32_t encoderSteps =
                        (int32_t)(deltaEncoder * _positionControllerSettings->encoderToStepsCoefficient);
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
                    _safetyController->positionControllerNotifyUnstable(i, _unstableEncoderCount[i],
                                                                        deltaEncoder);
                    _unstableEncoderCount[i]++;
                }
                _hardpointActuatorData->stepsCommanded[i] =
                        Range::CoerceIntoRange(-_positionControllerSettings->maxStepsPerLoop,
                                               _positionControllerSettings->maxStepsPerLoop, encoderSteps);
                if (deltaEncoder == 0 && _stableEncoderCount[i] >= 2) {
                    publishState = true;
                    _hardpointActuatorData->stepsCommanded[i] = 0;
                    _hardpointActuatorState->motionState[i] = HardpointActuatorMotionStates::Standby;
                }
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
    bool lowLimit = _hardpointActuatorData->stepsCommanded[hp] < 0 &&
                    HardpointActuatorWarning::instance().limitSwitch2Operated[hp] == true;
    bool highLimit = _hardpointActuatorData->stepsCommanded[hp] > 0 &&
                     HardpointActuatorWarning::instance().limitSwitch1Operated[hp] == true;
    if (lowLimit || highLimit) {
        _hardpointActuatorData->stepsCommanded[hp] = 0;
    }
    _safetyController->positionControllerNotifyLimitLow(hp, lowLimit);
    _safetyController->positionControllerNotifyLimitHigh(hp, highLimit);
}

void PositionController::_convertToSteps(int32_t* steps, double x, double y, double z, double rX, double rY,
                                         double rZ) {
    // The reason for defining HP 3 first (index 2) is due to the matrix. Review the
    // MirrorPositionToHardpointDisplacementTable for a description of the matrix.
    steps[2] = (_hardpointActuatorSettings->MirrorPositionToHardpointDisplacement[0] * x +
                _hardpointActuatorSettings->MirrorPositionToHardpointDisplacement[1] * y +
                _hardpointActuatorSettings->MirrorPositionToHardpointDisplacement[2] * z +
                _hardpointActuatorSettings->MirrorPositionToHardpointDisplacement[3] * rX +
                _hardpointActuatorSettings->MirrorPositionToHardpointDisplacement[4] * rY +
                _hardpointActuatorSettings->MirrorPositionToHardpointDisplacement[5] * rZ) *
               (MILLIMETERS_PER_METER * MICROMETERS_PER_MILLIMETER /
                _hardpointActuatorSettings->micrometersPerStep);
    steps[3] = (_hardpointActuatorSettings->MirrorPositionToHardpointDisplacement[6] * x +
                _hardpointActuatorSettings->MirrorPositionToHardpointDisplacement[7] * y +
                _hardpointActuatorSettings->MirrorPositionToHardpointDisplacement[8] * z +
                _hardpointActuatorSettings->MirrorPositionToHardpointDisplacement[9] * rX +
                _hardpointActuatorSettings->MirrorPositionToHardpointDisplacement[10] * rY +
                _hardpointActuatorSettings->MirrorPositionToHardpointDisplacement[11] * rZ) *
               (MILLIMETERS_PER_METER * MICROMETERS_PER_MILLIMETER /
                _hardpointActuatorSettings->micrometersPerStep);
    steps[4] = (_hardpointActuatorSettings->MirrorPositionToHardpointDisplacement[12] * x +
                _hardpointActuatorSettings->MirrorPositionToHardpointDisplacement[13] * y +
                _hardpointActuatorSettings->MirrorPositionToHardpointDisplacement[14] * z +
                _hardpointActuatorSettings->MirrorPositionToHardpointDisplacement[15] * rX +
                _hardpointActuatorSettings->MirrorPositionToHardpointDisplacement[16] * rY +
                _hardpointActuatorSettings->MirrorPositionToHardpointDisplacement[17] * rZ) *
               (MILLIMETERS_PER_METER * MICROMETERS_PER_MILLIMETER /
                _hardpointActuatorSettings->micrometersPerStep);
    steps[5] = (_hardpointActuatorSettings->MirrorPositionToHardpointDisplacement[18] * x +
                _hardpointActuatorSettings->MirrorPositionToHardpointDisplacement[19] * y +
                _hardpointActuatorSettings->MirrorPositionToHardpointDisplacement[20] * z +
                _hardpointActuatorSettings->MirrorPositionToHardpointDisplacement[21] * rX +
                _hardpointActuatorSettings->MirrorPositionToHardpointDisplacement[22] * rY +
                _hardpointActuatorSettings->MirrorPositionToHardpointDisplacement[23] * rZ) *
               (MILLIMETERS_PER_METER * MICROMETERS_PER_MILLIMETER /
                _hardpointActuatorSettings->micrometersPerStep);
    steps[0] = (_hardpointActuatorSettings->MirrorPositionToHardpointDisplacement[24] * x +
                _hardpointActuatorSettings->MirrorPositionToHardpointDisplacement[25] * y +
                _hardpointActuatorSettings->MirrorPositionToHardpointDisplacement[26] * z +
                _hardpointActuatorSettings->MirrorPositionToHardpointDisplacement[27] * rX +
                _hardpointActuatorSettings->MirrorPositionToHardpointDisplacement[28] * rY +
                _hardpointActuatorSettings->MirrorPositionToHardpointDisplacement[29] * rZ) *
               (MILLIMETERS_PER_METER * MICROMETERS_PER_MILLIMETER /
                _hardpointActuatorSettings->micrometersPerStep);
    steps[1] = (_hardpointActuatorSettings->MirrorPositionToHardpointDisplacement[30] * x +
                _hardpointActuatorSettings->MirrorPositionToHardpointDisplacement[31] * y +
                _hardpointActuatorSettings->MirrorPositionToHardpointDisplacement[32] * z +
                _hardpointActuatorSettings->MirrorPositionToHardpointDisplacement[33] * rX +
                _hardpointActuatorSettings->MirrorPositionToHardpointDisplacement[34] * rY +
                _hardpointActuatorSettings->MirrorPositionToHardpointDisplacement[35] * rZ) *
               (MILLIMETERS_PER_METER * MICROMETERS_PER_MILLIMETER /
                _hardpointActuatorSettings->micrometersPerStep);
}

void PositionController::_checkFollowingError(int hp) {
    // make sure small corrections, negligible relative to encoder step size,
    // aren't checked
    if (abs(_hardpointActuatorData->stepsCommanded[hp]) >
        _positionControllerSettings->encoderToStepsCoefficient) {
        double travelled = _hardpointActuatorData->encoder[hp] - _lastEncoderCount[hp];
        // due to condition above divider is > 1 or < -1, division by 0 cannot occur
        double shouldTravell = _hardpointActuatorData->stepsCommanded[hp] /
                               _positionControllerSettings->encoderToStepsCoefficient;

        double fePercent = 100 * (travelled / shouldTravell);
        _safetyController->hardpointActuatorFollowingError(hp, fePercent);
    }
    _lastEncoderCount[hp] = _hardpointActuatorData->encoder[hp];
}
