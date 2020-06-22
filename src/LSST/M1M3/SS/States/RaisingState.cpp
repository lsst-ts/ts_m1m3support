/*
 * This file is part of LSST M1M3 support system package.
 *
 * Developed for the LSST Data Management System.
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

#include <RaisingState.h>
#include <Model.h>
#include <SafetyController.h>
#include <AutomaticOperationsController.h>
#include <spdlog/spdlog.h>

namespace LSST {
namespace M1M3 {
namespace SS {

RaisingState::RaisingState(M1M3SSPublisher* publisher) : EnabledState(publisher, "RaisingState") {}

States::Type RaisingState::update(UpdateCommand* command, Model* model) {
    spdlog::trace("RaisingState: update()");
    this->startTimer();
    States::Type newState = States::NoStateTransition;
    model->getAutomaticOperationsController()->tryIncrementingSupportPercentage();
    EnabledState::update(command, model);
    if (model->getAutomaticOperationsController()->checkRaiseOperationComplete()) {
        model->getAutomaticOperationsController()->completeRaiseOperation();
        newState = States::ActiveState;
    } else if (model->getAutomaticOperationsController()->checkRaiseOperationTimeout()) {
        model->getAutomaticOperationsController()->timeoutRaiseOperation();
    }
    this->stopTimer();
    model->publishOuterLoop(this->getTimer());
    return model->getSafetyController()->checkSafety(newState);
}

States::Type RaisingState::abortRaiseM1M3(AbortRaiseM1M3Command* command, Model* model) {
    spdlog::info("RaisingState: abortRaiseM1M3()");
    States::Type newState = States::LoweringState;
    model->getAutomaticOperationsController()->abortRaiseM1M3();
    return model->getSafetyController()->checkSafety(newState);
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
