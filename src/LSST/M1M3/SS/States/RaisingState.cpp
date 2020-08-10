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
#include <ModelPublisher.h>
#include <spdlog/spdlog.h>

namespace LSST {
namespace M1M3 {
namespace SS {

RaisingState::RaisingState() : EnabledState("RaisingState") {}

States::Type RaisingState::update(UpdateCommand* command) {
    ModelPublisher publishIt();
    spdlog::trace("RaisingState: update()");
    Model::get().getAutomaticOperationsController()->tryIncrementingSupportPercentage();
    runLoop();
    return Model::get().getSafetyController()->checkSafety(raiseCompleted() ? States::ActiveState
                                                                            : States::NoStateTransition);
}

States::Type RaisingState::abortRaiseM1M3(AbortRaiseM1M3Command* command) {
    spdlog::info("RaisingState: abortRaiseM1M3()");
    Model::get().getAutomaticOperationsController()->abortRaiseM1M3();
    return Model::get().getSafetyController()->checkSafety(States::LoweringState);
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
